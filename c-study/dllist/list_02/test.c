#include <stdio.h>
#include <strings.h>
#include "list.h"

int main(int argc,char *argv[])
{
    LIST_HEAD(list);        //定义存放文件内容的链表
    FILE *fp;

    struct file_store {
        char c;
        struct list_head node;
    } *pstore;

    if(argc<2){
        printf("usage: pfile <filename> <[r]>\n");
        return -1;
    }
    if(!(fp=fopen(argv[1],"rb"))){
        printf("fopen(%s) error\n",argv[1]);
            return -1;
    }

    /* 读文件到链表 */
    while(1){
        if(!(pstore=(struct file_store *)malloc(sizeof(struct file_store))))
            break;
        pstore->c=fgetc(fp);
        if(feof(fp)){
            free(pstore);
            break;
        }
        list_add_tail(&pstore->node,&list);         //将本字符插入链表中
    }
    fclose(fp);

    /* 遍历链表，输出链表中的节点个数，即文件字符数 */
    int count=0;
    struct list_head *p;
    list_for_each(p,&list){
        count++;
    }
    printf("%s has altogether %d character(s)\n",argv[1],count);

    /* 根据命令行参数正向/反向遍历链表，输出链表中存放的字符，同时释放各节点 */
    if(argc>2 && !strcasecmp(argv[2],"r")){
        struct list_head *p;
        list_for_each_entry_reverse(pstore,&list,node){     //反向遍历，没有保护
            p=pstore->node.next;
            list_del(&pstore->node);
            putchar(pstore->c);
            free(pstore);
            /* 如果没有这一句，将报segmentation fault */
pstore=list_entry(p,struct file_store,node);    //取数据项
        }
    }else{
        struct file_store *p;
        list_for_each_entry_safe(pstore,p,&list,node){      //正向遍历，有保护
            list_del(&pstore->node);
            putchar(pstore->c);
            free(pstore);
        }
    }

    return 0;
}
