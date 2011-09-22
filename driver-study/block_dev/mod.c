  /*
   * Sample disk driver, from the beginning.
   */
  
  //#include <linux/config.h>
  #include <linux/module.h>
  #include <linux/moduleparam.h>
  #include <linux/init.h>
  
  #include <linux/sched.h>
  #include <linux/kernel.h>    /* printk() */
  #include <linux/slab.h>        /* kmalloc() */
  #include <linux/fs.h>        /* everything... */
  #include <linux/errno.h>    /* error codes */
  #include <linux/timer.h>
  #include <linux/types.h>    /* size_t */
  #include <linux/fcntl.h>    /* O_ACCMODE */
  #include <linux/hdreg.h>    /* HDIO_GETGEO */
  #include <linux/kdev_t.h>
  #include <linux/vmalloc.h>
  #include <linux/genhd.h>
  #include <linux/blkdev.h>
  #include <linux/buffer_head.h>    /* invalidate_bdev */
  #include <linux/bio.h>
  
  MODULE_LICENSE("Dual BSD/GPL");
  
  static int sbull_major = 0;
  module_param(sbull_major, int, 0);
  static int hardsect_size = 512;
  module_param(hardsect_size, int, 0);
  static int nsectors = 1024;    /* How big the drive is */
  module_param(nsectors, int, 0);
  static int ndevices = 4;
  module_param(ndevices, int, 0);
  
  /*
   * The different "request modes" we can use.
   */
  enum {
      RM_SIMPLE  = 0,    /* The extra-simple request function */
      RM_FULL    = 1,    /* The full-blown version */
      RM_NOQUEUE = 2,    /* Use make_request */
  };
  static int request_mode = RM_SIMPLE;
  module_param(request_mode, int, 0);
  
  /*
   * Minor number and partition management.
   */
  #define SBULL_MINORS    16
  #define MINOR_SHIFT    4
  #define DEVNUM(kdevnum)    (MINOR(kdev_t_to_nr(kdevnum)) >> MINOR_SHIFT
  
  /*
   * We can tweak our hardware sector size, but the kernel talks to us
   * in terms of small sectors, always.
   */
  #define KERNEL_SECTOR_SIZE    512
  
  /*
   * After this much idle time, the driver will simulate a media change.
   */
  #define INVALIDATE_DELAY    30*HZ
  
  /*
   * The internal representation of our device.
   */
  struct sbull_dev {  //描述sbull设备的结构体
          int size;                       /* Device size in sectors */
          u8 *data;                       /* The data array */
          short users;                    /* How many users */
          short media_change;             /* Flag a media change? */
          spinlock_t lock;                /* For mutual exclusion */
          struct request_queue *queue;    /* The device request queue */
          struct gendisk *gd;             /* The gendisk structure */
          struct timer_list timer;        /* For simulated media changes */
  };
  
  static struct sbull_dev *Devices = NULL;
  
  /*
   * Handle an I/O request.
   */
  static void sbull_transfer(struct sbull_dev *dev, unsigned long sector,
          unsigned long nsect, char *buffer, int write)
  {
      unsigned long offset = sector*KERNEL_SECTOR_SIZE;
      unsigned long nbytes = nsect*KERNEL_SECTOR_SIZE;
  
      if ((offset + nbytes) > dev->size) {
          printk (KERN_NOTICE "Beyond-end write (%ld %ld)\n", offset, nbytes);
          return;
      }
      if (write)
          memcpy(dev->data + offset, buffer, nbytes);
      else
          memcpy(buffer, dev->data + offset, nbytes);
  }
 
 /*
  * The simple form of the request function.
  * 块设备的驱动程序的核心是它的请求函数，至少如设备的启动都是在这个函数里面完成的...
  * 编写一个简单的request如下，但是驱动程序如果要在很高的层析上控制非常复杂的硬件，它将不再这么简单
  *
  * 内核需要驱动程序处理读取写入以及其他的设备操作时，就会调用该函数
  * request是在一个原子上下文运行的，该锁是由内核控制的，拥有锁时防止内核为设备安排其他请求
  * 
  */
 static void sbull_request(request_queue_t *q)  
 {        // 每一个设备都需要一个请求队列，因为对磁盘的数据实际的传入和传出的时间 与内核请求的时间相差很大
          // 驱动程序所需要知道的任何信息都敖汉在通过请求队列传给我们的结构中
          // 请求队列保存了表述设备所能处理的请求的参数：最大尺寸，在同一个请求所能包含的独立段的数目、硬件扇区大小、对齐需求等等
          // 请求队列还实现了插件接口，使多规格IO调度器的使用称为可能         
     struct request *req;
 
     while ((req = elv_next_request(q)) != NULL) {
         // elv_next_request 返回队列中第一个未完成的请求，如果没有请求返回null
         // 这个函数并不从队列中删除请求，如果不加以干涉而两次调用该函数，则两次放回相同的结构
         struct sbull_dev *dev = req->rq_disk->private_data;
         if (! blk_fs_request(req)) {
         // blk_fs_request告诉用户该请求是否是一个文件系统请求——移动块设备数据请求
             printk (KERN_NOTICE "Skip non-fs request\n");
             end_request(req, 0);
             continue;
         }
     //        printk (KERN_NOTICE "Req dev %d dir %ld sec %ld, nr %d f %lx\n",
     //                dev - Devices, 
     //                rq_data_dir(req),传输方向，0表述从设备读，非0表示写
     //                req->sector,开始扇区的索引号
     //                req->current_nr_sectors, 需要传输的扇区数
     //                req->flags);
     //              sbull_transfer 传输的数据的缓冲区指针
     //              实现数据的真正移动
         sbull_transfer(dev, req->sector, req->current_nr_sectors,
                 req->buffer, rq_data_dir(req));
         end_request(req, 1);
     }
 }
 
 
 /*
  * Transfer a single BIO.
  * 该函数遍历了bio结构中的每个段，获得内核虚拟地址以访问缓冲区，然后调用前面介绍过的sbull_transfer函数，以完成数据的拷贝.
  */
 static int sbull_xfer_bio(struct sbull_dev *dev, struct bio *bio)
 {
     int i;
     struct bio_vec *bvec;
     sector_t sector = bio->bi_sector;
 
     /* Do each segment independently. */
     bio_for_each_segment(bvec, bio, i) {
         char *buffer = __bio_kmap_atomic(bio, i, KM_USER0);
         sbull_transfer(dev, sector, bio_cur_sectors(bio),
                 buffer, bio_data_dir(bio) == WRITE);
         sector += bio_cur_sectors(bio);
         __bio_kunmap_atomic(bio, KM_USER0);
     }
     return 0; /* Always "succeed" */
 }
 
 /*
  * Transfer a full request.
  * 实际执行请求的函数
  */
 static int sbull_xfer_request(struct sbull_dev *dev, struct request *req)
 {
     struct bio *bio;
     int nsect = 0;
     
     rq_for_each_bio(bio, req) {  // rq_for_each_bio宏 只是遍历了请求中的每个biio结构，并提供可可以传递给sbull_xfer_bio函数用于传输的指针
         sbull_xfer_bio(dev, bio);
         nsect += bio->bi_size/KERNEL_SECTOR_SIZE;
     }
     return nsect;
 }
 
 
 
 /*
  * Smarter request function that "handles clustering".
  * 这个函数将注册一个bio请求函数
  */
 static void sbull_full_request(request_queue_t *q)
 {
     struct request *req;
     int sectors_xferred;
     struct sbull_dev *dev = q->queuedata;
 
     while ((req = elv_next_request(q)) != NULL) {
         if (! blk_fs_request(req)) {
             printk (KERN_NOTICE "Skip non-fs request\n");
             end_request(req, 0);
             continue;
         }
         sectors_xferred = sbull_xfer_request(dev, req);
         if (! end_that_request_first(req, 1, sectors_xferred)) {
             blkdev_dequeue_request(req);
             end_that_request_last(req, 0); 
         }
     }
 }
 
 
 
 /*
  * The direct make request version.
  */
 static int sbull_make_request(request_queue_t *q, struct bio *bio)
 {
     struct sbull_dev *dev = q->queuedata;
     int status;
 
     status = sbull_xfer_bio(dev, bio);
     bio_endio(bio, bio->bi_size, status);
     return 0;
 }
 
 
 /*
  * Open and close.
  * // sbull必须知道最后一个用户何时关闭了设备
  */
 
 static int sbull_open(struct inode *inode, struct file *filp)
 {
     struct sbull_dev *dev = inode->i_bdev->bd_disk->private_data;
 
     del_timer_sync(&dev->timer);
     filp->private_data = dev;
     spin_lock(&dev->lock);
     if (! dev->users) 
         check_disk_change(inode->i_bdev);
     dev->users++;
     spin_unlock(&dev->lock);
     return 0;
 }
 /*
  * 减少用户基数，并启用介质移除定时器
  */
 static int sbull_release(struct inode *inode, struct file *filp)
 {
     struct sbull_dev *dev = inode->i_bdev->bd_disk->private_data;
 
     spin_lock(&dev->lock);
     dev->users--;
 
     if (!dev->users) {
         dev->timer.expires = jiffies + INVALIDATE_DELAY; //30秒的定时器，如果这个时段内设备没有被打开则移除设备
         add_timer(&dev->timer);
     }
     spin_unlock(&dev->lock);
 
     return 0;
 }
 
 /*
  * Look for a (simulated) media change.
  */
 int sbull_media_changed(struct gendisk *gd)
 {
     struct sbull_dev *dev = gd->private_data;
     
     return dev->media_change;
 }
 
 /*
  * Revalidate.  WE DO NOT TAKE THE LOCK HERE, for fear of deadlocking
  * with open.  That needs to be reevaluated.
  * 调用此函数内核将试着重新读取分区表，在这里这个函数只是简单地重置了media_change的标志位，并清除内存空间以模拟插入一张磁盘
  */
 int sbull_revalidate(struct gendisk *gd)
 {
     struct sbull_dev *dev = gd->private_data;
     
     if (dev->media_change) {
         dev->media_change = 0;
         memset (dev->data, 0, dev->size);
     }
     return 0;
 }
 
 /*
  * The "invalidate" function runs out of the device timer; it sets
  * a flag to simulate the removal of the media.
  */
 void sbull_invalidate(unsigned long ldev)
 {
     struct sbull_dev *dev = (struct sbull_dev *) ldev;
 
     spin_lock(&dev->lock);
     if (dev->users || !dev->data) 
         printk (KERN_WARNING "sbull: timer sanity check failed\n");
     else
         dev->media_change = 1;
     spin_unlock(&dev->lock);
 }
 
 /*
  * The ioctl() implementation
  * ioctl函数在这里只处理了一个命令，对设备物理信息的查询请求
  * 这里由于是虚拟设备，因此只提供了一些虚拟信息
  */
 
 int sbull_ioctl (struct inode *inode, struct file *filp,
                  unsigned int cmd, unsigned long arg)
 {
     long size;
     struct hd_geometry geo;
     struct sbull_dev *dev = filp->private_data;
 
     switch(cmd) {
         case HDIO_GETGEO:
             /*
          * Get geometry: since we are a virtual device, we have to make
          * up something plausible.  So we claim 16 sectors, four heads,
          * and calculate the corresponding number of cylinders.  We set the
          * start of data at sector four.
          */
         size = dev->size*(hardsect_size/KERNEL_SECTOR_SIZE);
         geo.cylinders = (size & ~0x3f) >> 6;
         geo.heads = 4;
         geo.sectors = 16;
         geo.start = 4;
         if (copy_to_user((void __user *) arg, &geo, sizeof(geo)))
             return -EFAULT;
         return 0;
     }
 
     return -ENOTTY; /* unknown command */
 }
 
 
 
 /*
  * The device operations structure.
  */
 static struct block_device_operations sbull_ops = {
     .owner           = THIS_MODULE,
     .open              = sbull_open,
     .release      = NULL,//sbull_release,
     .media_changed   = sbull_media_changed,
     .revalidate_disk = sbull_revalidate,
     .ioctl             = sbull_ioctl
 };
 
 
 /*
  * Set up our internal device.初始化sbull_dev结构
  */
 static void setup_device(struct sbull_dev *dev, int which)
 {
     /*
      * Get some memory.
      */
     memset (dev, 0, sizeof (struct sbull_dev));
     dev->size = nsectors*hardsect_size;
     dev->data = vmalloc(dev->size);
     if (dev->data == NULL) {
         printk (KERN_NOTICE "vmalloc failure.\n");
         return;
     }
     spin_lock_init(&dev->lock);
     
     /*
      * The timer which "invalidates" the device.
      */
     init_timer(&dev->timer);
     dev->timer.data = (unsigned long) dev;
     dev->timer.function = sbull_invalidate;
     
     /*
      * The I/O queue, depending on whether we are using our own
      * make_request function or not.
      */
     switch (request_mode) {
         case RM_NOQUEUE:
         dev->queue = blk_alloc_queue(GFP_KERNEL);
         if (dev->queue == NULL)
             goto out_vfree;
         blk_queue_make_request(dev->queue, sbull_make_request);
         break;
 
         case RM_FULL:
         dev->queue = blk_init_queue(sbull_full_request, &dev->lock);//创建iqngqiu队列
         if (dev->queue == NULL)
             goto out_vfree;
         break;
 
         default:
         printk(KERN_NOTICE "Bad request mode %d, using simple\n", request_mode);
             /* fall into.. */
     
         case RM_SIMPLE:
         dev->queue = blk_init_queue(sbull_request, &dev->lock);
         //sbull_request是请求函数，负责执行块设备的读写请求。
         if (dev->queue == NULL)
             goto out_vfree;
         break;
     }
     blk_queue_hardsect_size(dev->queue, hardsect_size); //设置扇区大小
     dev->queue->queuedata = dev;
     /*
      * And the gendisk structure.
      * 拥有了设备内存和请求队列，就可以分配、初始化及安装相应的gendisk结构了.
      */
     dev->gd = alloc_disk(SBULL_MINORS);//SBULL_MINORS是每个sbull设备所支持的次设备号的数量
     if (! dev->gd) {
         printk (KERN_NOTICE "alloc_disk failure\n");
         goto out_vfree;
     }
     dev->gd->major = sbull_major;
     dev->gd->first_minor = which*SBULL_MINORS;
     dev->gd->fops = &sbull_ops;
     dev->gd->queue = dev->queue;
     dev->gd->private_data = dev;
     snprintf (dev->gd->disk_name, 32, "sbull%c", which + 'a');
     set_capacity(dev->gd, nsectors*(hardsect_size/KERNEL_SECTOR_SIZE));//每个请求的大小都是扇区大小的整数倍，内核总是认为扇区大小是512字节，因此必须进行转换
     //KERNEL_SECTOR_SIZE是本地定义的一个常量，使用该常量进行内核512自己上去到实际扇区大小的转换。
     
     add_disk(dev->gd);//结束设置过程
     return;
 
   out_vfree:
     if (dev->data)
         vfree(dev->data);
 }
 
 
 
 static int __init sbull_init(void)
 {
     int i;
     /*
      * Get registered.
      */
     sbull_major = register_blkdev(sbull_major, "sbull"); //动态分配主设备号
     if (sbull_major <= 0) {
         printk(KERN_WARNING "sbull: unable to get major number\n");
         return -EBUSY;
     }
     /*
      * Allocate the device array, and initialize each one.
      */
     Devices = kmalloc(ndevices*sizeof (struct sbull_dev), GFP_KERNEL);
     if (Devices == NULL)
         goto out_unregister;
     for (i = 0; i < ndevices; i++) 
         setup_device(Devices + i, i);//初始化sbull_dev结构
     
     return 0;
 
   out_unregister:
     unregister_blkdev(sbull_major, "sbd");
     return -ENOMEM;
 }
 
 static void sbull_exit(void)
 {
     int i;
 
     for (i = 0; i < ndevices; i++) {
         struct sbull_dev *dev = Devices + i;
 
         del_timer_sync(&dev->timer);
         if (dev->gd) {
             del_gendisk(dev->gd);
             put_disk(dev->gd);
         }
         if (dev->queue) {
             if (request_mode == RM_NOQUEUE)
                 blk_put_queue(dev->queue);
             else
                 blk_cleanup_queue(dev->queue);// 删除请求队列
         }
         if (dev->data)
             vfree(dev->data);
     }
     unregister_blkdev(sbull_major, "sbull");
     kfree(Devices);
 }
     
 module_init(sbull_init);
 module_exit(sbull_exit);

