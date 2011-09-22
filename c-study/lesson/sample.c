#ifndef __KERNEL__
#define __KERNEL__
#endif
#ifndef MODULE
#define MODULE
#endif
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/ioport.h>
#include <linux/sched.h>
#include <linux/vmalloc.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include <asm/delay.h>

#include <gmodule.h> /* Must be included first */

#include <linux-bde.h>

#include <appl/diag/sysconf.h>
#include <appl/diag/system.h>
#include <appl/diag/cmdlist.h>

#include <bcm-core.h>
#include <bcm/field.h>
#include <bcm/l2.h>
#include <bcm/vlan.h>
#include <bcm/trunk.h>
#include <bcm/error.h>

#include "sample.h"
extern cmd_result_t cmd_robo_reg_set(int unit, args_t *a);
extern cmd_result_t cmd_robo_reg_get_(int unit, args_t *a);
extern uint16_t sr_subc(uint8_t device_id,  uint8_t sub_mode, uint16_t address);
extern uint16_t sw_subc(uint8_t device_id, uint8_t sub_mode, uint16_t address, uint16_t value);
#undef printk
#define printk printk

int group_used[ G_GROUP_NUM ];

static void
port_info_init(int unit, int port, bcm_port_info_t *info, uint32 actions)
{
    info->action_mask = actions;

    /* We generally need to get link state and autoneg and adverts */
    info->action_mask |= BCM_PORT_ATTR_LINKSTAT_MASK;
    info->action_mask |= BCM_PORT_ATTR_LOCAL_ADVERT_MASK;
    info->action_mask |= BCM_PORT_ATTR_REMOTE_ADVERT_MASK;
    info->action_mask |= BCM_PORT_ATTR_AUTONEG_MASK;

    
    if (unit >= 0 && SOC_IS_XGS12_FABRIC(unit)) {
        info->action_mask |= BCM_PORT_ATTR_ENCAP_MASK;
    }

    /* Clear rate for HG ports */
    if (unit >= 0 && IS_HG_PORT(unit, port)) {
        info->action_mask &= ~(BCM_PORT_ATTR_RATE_MCAST_MASK |
                               BCM_PORT_ATTR_RATE_BCAST_MASK |
                               BCM_PORT_ATTR_RATE_DLFBC_MASK);
    }
}

int
port_parse_port_info_set(uint32 flags,
			 bcm_port_info_t *src,
                         bcm_port_info_t *dest)
{
    int info_speed_adj;

    if (flags & BCM_PORT_ATTR_AUTONEG_MASK) {
        dest->autoneg = src->autoneg;
    }

    if (flags & BCM_PORT_ATTR_ENABLE_MASK) {
        dest->enable = src->enable;
    }

    if (flags & BCM_PORT_ATTR_STP_STATE_MASK) {
        dest->stp_state = src->stp_state;
    }

    /*
     * info_speed_adj is the same as src->speed except a speed of 0
     * is replaced by the maximum speed supported by the port.
     */

    info_speed_adj = src->speed;

    if ((flags & BCM_PORT_ATTR_SPEED_MASK) && (info_speed_adj == 0)) {
        info_speed_adj = src->speed_max;
    }

    /*
     * If local_advert was parsed, use it.  Otherwise, calculate a
     * reasonable local advertisement from the given values and current
     * values of speed/duplex.
     */

    if ((flags & BCM_PORT_ATTR_LOCAL_ADVERT_MASK) != 0) {
	dest->local_advert = src->local_advert;
    }

    if (dest->autoneg &&
	(flags & BCM_PORT_ATTR_LOCAL_ADVERT_MASK) == 0) {
        int                 cur_speed, cur_duplex;
        int                 cur_pause_tx, cur_pause_rx;
        int                 new_speed, new_duplex;
        int                 new_pause_tx, new_pause_rx;
        bcm_port_abil_t     mode;

        /*
         * Update link advertisements for speed/duplex/pause.  All
         * speeds less than or equal to the requested speed are
         * advertised.
         */

        mode = dest->local_advert;

        cur_speed = BCM_PORT_ABIL_SPD_MAX(mode);
        cur_duplex = ((mode & BCM_PORT_ABIL_FD) ?
                      SOC_PORT_DUPLEX_FULL : SOC_PORT_DUPLEX_HALF);
        cur_pause_tx = (mode & BCM_PORT_ABIL_PAUSE_TX) != 0;
        cur_pause_rx = (mode & BCM_PORT_ABIL_PAUSE_RX) != 0;

        new_speed = (flags & BCM_PORT_ATTR_SPEED_MASK ?
                     info_speed_adj : cur_speed);
        new_duplex = (flags & BCM_PORT_ATTR_DUPLEX_MASK ?
                      src->duplex : cur_duplex);
        new_pause_tx = (flags & BCM_PORT_ATTR_PAUSE_TX_MASK ?
                        src->pause_tx : cur_pause_tx);
        new_pause_rx = (flags & BCM_PORT_ATTR_PAUSE_RX_MASK ?
                        src->pause_rx : cur_pause_rx);

        /* Start with maximum ability and cut down */

        mode = src->ability;

        if (new_duplex == SOC_PORT_DUPLEX_HALF) {
            mode &= ~BCM_PORT_ABIL_FD;
        }

        if (new_speed < 1000) {
            mode &= ~BCM_PORT_ABIL_1000MB;
        }

        if (new_speed < 100) {
            mode &= ~BCM_PORT_ABIL_100MB;
        }

        if (!(mode & BCM_PORT_ABIL_PAUSE_ASYMM) &&
            (new_pause_tx != new_pause_rx)) {
            printk("port parse: Error: Asymmetrical pause not available\n");
            return -1;
        }

        if (!new_pause_tx) {
            mode &= ~BCM_PORT_ABIL_PAUSE_TX;
        }

        if (!new_pause_rx) {
            mode &= ~BCM_PORT_ABIL_PAUSE_RX;
        }

        dest->local_advert = mode;
    } else {
        /* Update forced values for speed/duplex/pause */

        if (flags & BCM_PORT_ATTR_SPEED_MASK) {
            dest->speed = info_speed_adj;
        }

        if (flags & BCM_PORT_ATTR_DUPLEX_MASK) {
            dest->duplex = src->duplex;
        }

        if (flags & BCM_PORT_ATTR_PAUSE_TX_MASK) {
            dest->pause_tx = src->pause_tx;
        }

        if (flags & BCM_PORT_ATTR_PAUSE_RX_MASK) {
            dest->pause_rx = src->pause_rx;
        }
    }

    if (flags & BCM_PORT_ATTR_PAUSE_MAC_MASK) {
        sal_memcpy(dest->pause_mac, src->pause_mac, sizeof (mac_addr_t));
    }

    if (flags & BCM_PORT_ATTR_LINKSCAN_MASK) {
        dest->linkscan = src->linkscan;
    }

    if (flags & BCM_PORT_ATTR_LEARN_MASK) {
        dest->learn = src->learn;
    }

    if (flags & BCM_PORT_ATTR_DISCARD_MASK) {
        dest->discard = src->discard;
    }

    if (flags & BCM_PORT_ATTR_VLANFILTER_MASK) {
        dest->vlanfilter = src->vlanfilter;
    }

    if (flags & BCM_PORT_ATTR_UNTAG_PRI_MASK) {
        dest->untagged_priority = src->untagged_priority;
    }

    if (flags & BCM_PORT_ATTR_PFM_MASK) {
        dest->pfm = src->pfm;
    }

    if (flags & BCM_PORT_ATTR_PHY_MASTER_MASK) {
        dest->phy_master = src->phy_master;
    }

    if (flags & BCM_PORT_ATTR_INTERFACE_MASK) {
        dest->interface = src->interface;
    }

    if (flags & BCM_PORT_ATTR_LOOPBACK_MASK) {
        dest->loopback = src->loopback;
    }

    if (flags & BCM_PORT_ATTR_ENCAP_MASK) {
        dest->encap_mode = src->encap_mode;
    }

    if (flags & BCM_PORT_ATTR_FRAME_MAX_MASK) {
        dest->frame_max = src->frame_max;
    }

    if (flags & BCM_PORT_ATTR_MDIX_MASK) {
        dest->mdix = src->mdix;
    }

    return 0;
}

static void
get_counter(int unit, soc_port_t port, soc_reg_t ctr_reg,
                int ar_idx, int flags, xy_counter_t ** cntr, int * cntr_num)
{
    uint64		val,/* prev_val, diff,*/ rate;
//    int			changed;
    int                 is_ed_cntr;
/*    int			tabwidth = soc_property_get(unit, "diag_tabs", 8);
    int			commachr = soc_property_get(unit, "diag_comma", ',');
*/
    is_ed_cntr = SOC_REG_INFO(unit, ctr_reg).flags & SOC_REG_FLAG_ED_CNTR;

    if (!(SOC_REG_INFO(unit, ctr_reg).flags & SOC_REG_FLAG_ARRAY)) {
        ar_idx = 0;
    } else {
        if (ar_idx < 0) { /* Set all elts of array */
            for (ar_idx = 0; ar_idx < SOC_REG_INFO(unit, ctr_reg).numels;
		 ar_idx++) {
                get_counter(unit, port, ctr_reg, ar_idx, flags, cntr,cntr_num);
            }
	    return;
        }
    }
    soc_counter_get(unit, port, ctr_reg, ar_idx, &val);

    soc_counter_get_rate(unit, port, ctr_reg, ar_idx, &rate);
    //printk("unit %d, port %x, reg %d[%10s], idx %x, val %d\n",unit, port, ctr_reg, soc_reg_name[ctr_reg], ar_idx, val);

    sal_memcpy((*cntr)->name, soc_reg_name[ctr_reg], 10);
    (*cntr)->total=val;
    (*cntr)->rate=rate;
    (*cntr)++;
    (*cntr_num)++;
    return;
}

int get_counters(int unit, pbmp_t pbmp, xy_counter_t * cntr, int * cntr_num)
{
//    soc_control_t	*soc = SOC_CONTROL(unit);
    soc_port_t		port;
    int			i;
    soc_cmap_t          *cmap;
    soc_reg_t           reg;
    int                 numregs;
    int flags = (SHOW_CTR_CHANGED | SHOW_CTR_SAME |
                          SHOW_CTR_Z | SHOW_CTR_NZ);
    xy_counter_t ** pcntr = &cntr;
    PBMP_ITER(pbmp, port) {
        cmap = soc_port_cmap_get(unit, port);
        numregs = cmap->cmap_size;
        for (i = 0; i < numregs; i++) {
            reg = cmap->cmap_base[i].reg;
            if (reg != INVALIDr) {
                get_counter(unit, port, reg, -1, flags, pcntr, cntr_num);
            }
        }
    }
    return 0;
}
int clear_counters(int unit, pbmp_t pbmp)
{
	if(soc_counter_set32_by_port(unit, pbmp, 0)<0)
		return 1;
	return 0;
}
int get_port_status(int unit, 
					  int port, 
					  int *link,
					  int *speed,
					  int *duplex,
					  int *autoneg,
					  int *pause,
					  int *max_frame)
{
	int r;
    //int i;

    bcm_port_info_t *info_all;
    if (unit == 0)
        info_all = sal_alloc(sizeof(bcm_port_info_t),"if_port_stat");
    else 
        info_all = sal_alloc(sizeof(bcm_port_info_t),"if_robo_port_stat");
        
    if (info_all == NULL){
        printk("Insufficient memory.\n");
        return 1;
    }    
    port_info_init(unit,port, info_all, BCM_PORT_ATTR_ALL_MASK);

    if ((r = bcm_port_selective_get(unit,port, info_all)) < 0) {
        sal_free(info_all);
	    return 1;
    }

	*link = info_all->linkstatus;              /* link or not*/
	*autoneg = info_all->autoneg;	          /* AN */
	*speed = info_all->speed;             /*  Initial speed*/
	*duplex = info_all->duplex;               /* Dpx bit*/
	*pause = info_all->pause_rx;              /* Flow control */
    sal_free(info_all);
#if 0
    bcm_port_info_t *info_all;
    for ( i = 0; i < 10; i++)
    {
	    port_info_init(unit,port, &info_all, BCM_PORT_ATTR_ALL_MASK);
	    if ((r = bcm_port_selective_get(unit,port , &info_all)) < 0) {
		    return 1;
        }
    }    
	*link = info_all.linkstatus;              /* link or not*/
	*autoneg = info_all.autoneg;	          /* AN */
	*speed = info_all.speed;             /*  Initial speed*/
	*duplex = info_all.duplex;               /* Dpx bit*/
	*pause = info_all.pause_rx;              /* Flow control */
#endif

#if 0
	bcm_port_info_t info_all;
	port_info_init(unit,port, &info_all, BCM_PORT_ATTR_ALL_MASK);
	if ((r = bcm_port_selective_get(unit,port , &info_all)) < 0) {
		return 1;
    }
	*link = info_all.linkstatus;              /* link or not*/
	*autoneg = info_all.autoneg;	          /* AN */
	*speed = info_all.speed;             /*  Initial speed*/
	*duplex = info_all.duplex;               /* Dpx bit*/
	*pause = info_all.pause_rx;              /* Flow control */
#endif
    return 0;
}

int set_port_autoneg(int unit, int port,  int autoneg)
{
	int r;
    	bcm_port_info_t given, info_all;
	uint32 parsed;

	parsed=BCM_PORT_ATTR_AUTONEG_MASK;

	port_info_init(unit, port, &info_all, parsed);

	if ((r = bcm_port_selective_get(unit, port, &info_all)) < 0) {
		return 1;
	}

        if ((r = bcm_port_speed_max(unit, port, &given.speed_max)) < 0) {
		return 1;
        }

        if ((r = bcm_port_ability_get(unit, port, &given.ability)) < 0) {
		return 1;
        }


	given.action_mask=parsed;
	given.autoneg=autoneg;
	
        if ((r = port_parse_port_info_set(parsed, &given, &info_all)) < 0) {
		return 1;
        }


        /* If AN is on, do not set speed, duplex, pause */
        if (info_all.autoneg) {
            info_all.action_mask &= ~BCM_PORT_AN_ATTRS;
        }


        if ((r = bcm_port_selective_set(unit, port, &info_all)) < 0) {
		return 1;
        }

	return 0;
}

int set_port_pause(int unit, int port, int pause)
{
	int r;
    	bcm_port_info_t given, info_all;
	uint32 parsed;

	parsed=BCM_PORT_ATTR_PAUSE_MASK;

	port_info_init(unit, port, &info_all, parsed);

	if ((r = bcm_port_selective_get(unit, port, &info_all)) < 0) {
		return 1;
	}

        if ((r = bcm_port_speed_max(unit, port, &given.speed_max)) < 0) {
		return 1;
        }

        if ((r = bcm_port_ability_get(unit, port, &given.ability)) < 0) {
		return 1;
        }


	given.action_mask=parsed;
	given.pause_rx=pause;
	given.pause_tx=pause;
	
        if ((r = port_parse_port_info_set(parsed, &given, &info_all)) < 0) {
		return 1;
        }


        /* If AN is on, do not set speed, duplex, pause */
        if (info_all.autoneg) {
            info_all.action_mask &= ~BCM_PORT_AN_ATTRS;
        }


        if ((r = bcm_port_selective_set(unit, port, &info_all)) < 0) {
		return 1;
        }

	return 0;
}

int set_port_speed(int unit, int port, int speed, int duplex)
{
	int r;
    	bcm_port_info_t given, info_all;
	uint32 parsed;

	parsed=BCM_PORT_ATTR_SPEED_MASK|BCM_PORT_ATTR_DUPLEX_MASK;

	port_info_init(unit, port, &info_all, parsed);

	if ((r = bcm_port_selective_get(unit, port, &info_all)) < 0) {
		return 1;
	}

        if ((r = bcm_port_speed_max(unit, port, &given.speed_max)) < 0) {
		return 1;
        }

        if ((r = bcm_port_ability_get(unit, port, &given.ability)) < 0) {
		return 1;
        }


	given.action_mask=parsed;
	given.speed=speed;
	given.duplex=duplex;
	
        if ((r = port_parse_port_info_set(parsed, &given, &info_all)) < 0) {
		return 1;
        }


        /* If AN is on, do not set speed, duplex, pause */
        if (info_all.autoneg) {
            info_all.action_mask &= ~BCM_PORT_AN_ATTRS;
        }


        if ((r = bcm_port_selective_set(unit, port, &info_all)) < 0) {
		return 1;
        }

	return 0;
}

int set_port_learn(int unit, int port,int enable)
{
	int r;
	uint32 flag;
	
	if ((r = bcm_port_learn_get(unit, port, &flag)) < 0) {
		return 1;
	}

	if (enable) {
		flag |= BCM_PORT_LEARN_ARL;
	}else{
		flag &= ~BCM_PORT_LEARN_ARL;
	}

	if ((r = bcm_port_learn_set(unit, port, flag)) < 0) {
		return 1;
	}
	return 0;
}

int init_vlan( void )
{
	int r_v;
	
	r_v = bcm_vlan_init( 0 );
	if (r_v != 0)
		return -1;
	return 0;
}
int create_vlan(int vid,unsigned int port_map,unsigned int untag_map)
{
	pbmp_t pbmp,ubmp;

	SOC_PBMP_CLEAR(pbmp);
	SOC_PBMP_CLEAR(ubmp);

	SOC_PBMP_WORD_SET(pbmp,0,port_map);
	SOC_PBMP_WORD_SET(ubmp,0,untag_map);

	if(bcm_vlan_create(0,vid))return -1;

	if(bcm_vlan_port_add(0,vid,pbmp,ubmp))return -1;

	return 0;
}

int add_vlan(int vid,unsigned int port_map,unsigned int untag_map)
{
	pbmp_t pbmp,ubmp;

	SOC_PBMP_CLEAR(pbmp);
	SOC_PBMP_CLEAR(ubmp);

	SOC_PBMP_WORD_SET(pbmp,0,port_map);
	SOC_PBMP_WORD_SET(ubmp,0,untag_map);

	if(bcm_vlan_port_add(0,vid,pbmp,ubmp))return -1;

	return 0;
}

int remove_vlan(int vid,unsigned int port_map)
{
	pbmp_t pbmp;

	SOC_PBMP_CLEAR(pbmp);
	SOC_PBMP_WORD_SET(pbmp,0,port_map);

	if(bcm_vlan_port_remove(0,vid,pbmp))return -1;

	return 0;
}

int destroy_vlan(int vid)
{

	if(bcm_vlan_destroy(0,vid))return -1;

	return 0;
}

int set_port_pvlan(int port_map,int vid)
{
	pbmp_t pbmp;
	soc_port_t port;

	SOC_PBMP_CLEAR(pbmp);
	SOC_PBMP_WORD_SET(pbmp,0,port_map);

	PBMP_ITER(pbmp, port){
		bcm_port_untagged_vlan_set(0,port,vid);
	}
	return 0;
}

int set_xe_phy_register(uint32 port,uint8 devad,uint16 reg,uint16 mask,uint16 value)
{
	uint32 tmp;

	if(mask!=0xffff)
	{	if(bcm_port_phy_get(0,port,BCM_PORT_PHY_CLAUSE45,(devad<<16)|reg,&tmp)<0) return -1;
		tmp &= mask;
		tmp |= value;
	}else{
		tmp = value;
	}

	if(bcm_port_phy_set(0,port,BCM_PORT_PHY_CLAUSE45,(devad<<16)|reg,tmp)<0) return -1;

	return 0;
}

int get_xe_phy_register(uint32 port,uint8 devad,uint16 reg,uint16 mask,uint16 *value)
{
	uint32 tmp;


	if(bcm_port_phy_get(0,port,BCM_PORT_PHY_CLAUSE45,(devad<<16)|reg,&tmp)<0) return -1;
	*value=tmp&0xffff;


	return 0;
}

int set_port_phy_register(uint32 port, uint16 reg,uint16 mask,uint16 value)
{
	uint32 tmp;

	if(mask!=0xffff)
	{	if(bcm_port_phy_get(0,port,BCM_PORT_PHY_INTERNAL, reg,&tmp)<0) return -1;
		tmp &= mask;
		tmp |= value;
	}else{
		tmp = value;
	}

	if(bcm_port_phy_set(0,port,BCM_PORT_PHY_INTERNAL, reg,tmp)<0) return -1;

	return 0;
}

int get_port_phy_register(uint32 port,uint16 reg,uint16 mask,uint16 *value)
{
	uint32 tmp;


	if(bcm_port_phy_get(0,port,BCM_PORT_PHY_INTERNAL, reg,&tmp)<0) return -1;
	*value=tmp&0xffff;


	return 0;
}

int trunk_add_l2_cache(int tid)
{
    int rv;
    int cidx;
    unsigned char mac[6];
    bcm_l2_cache_addr_t l2caddr;

    bcm_l2_cache_addr_t_init(&l2caddr);

    l2caddr.vlan = 0;
    l2caddr.vlan_mask = 0;

    memset(mac,0,sizeof(mac));
    ENET_COPY_MACADDR(mac, l2caddr.mac);
    ENET_COPY_MACADDR(mac, l2caddr.mac_mask);

    l2caddr.lookup_class = 0;
    l2caddr.dest_modid = 0;
    l2caddr.dest_trunk = tid;
    l2caddr.flags |= BCM_L2_CACHE_TRUNK;

    rv = bcm_l2_cache_set(0, 0, &l2caddr, &cidx);

    return rv;

}

int trunk_delete_l2_cache(int tid)
{
    int idx, idx_max, rv;
    bcm_l2_cache_addr_t l2caddr;

    if ((rv = bcm_l2_cache_size_get(0, &idx_max)) < 0) {
        return 0;
    }

    for (idx = 0; idx <= idx_max; idx++) {
	if (bcm_l2_cache_get(0, idx, &l2caddr) == BCM_E_NONE) {
	    if( (l2caddr.flags & BCM_L2_CACHE_TRUNK) && (l2caddr.dest_trunk==tid) ) 
		bcm_l2_cache_delete(0, idx);
	}
    }
    return 0;
}

int trunk_add(ioctl_bcm_trunk_t *fp)
{

    pbmp_t pbmp;
    bcm_trunk_t tid = -1;
    int r;
    int psc;
    int i, j;
    bcm_trunk_add_info_t t_add_info;

    tid = fp->tid;
    psc = fp->rtag;
    SOC_PBMP_CLEAR(pbmp);
    SOC_PBMP_WORD_SET(pbmp,0,fp->pbmp);
    
    sal_memset(&t_add_info, 0, sizeof (t_add_info));

    r = bcm_trunk_get(0, tid, &t_add_info);

    if(r>=0) {  /* not first creat */
        j=t_add_info.num_ports;
        SOC_PBMP_ITER(pbmp, i) {
        t_add_info.tp[j] = i;
        j++;
        };
        t_add_info.num_ports = j;
    }  
    else {/* first creat */
        j = 0;
        SOC_PBMP_ITER(pbmp, i) { 
            t_add_info.tp[j++] = i;
        };
    
        t_add_info.num_ports = j;   
        t_add_info.psc = psc;     	
	t_add_info.dlf_index = BCM_TRUNK_UNSPEC_INDEX;
	t_add_info.mc_index = BCM_TRUNK_UNSPEC_INDEX;
	t_add_info.ipmc_index = BCM_TRUNK_UNSPEC_INDEX;
        bcm_trunk_create_with_tid(0, tid);        	
    }    
                
    r = bcm_trunk_set(0, tid, &t_add_info);     

    trunk_add_l2_cache(tid);

    return r;
}

int trunk_remove(ioctl_bcm_trunk_t *fp)
{
    pbmp_t pbmp,rbmp;
    bcm_trunk_t tid = -1;
    int r;
    int i, j;
    bcm_trunk_add_info_t t_add_info;

    tid = fp->tid;
    sal_memset(&t_add_info, 0, sizeof (t_add_info));
    r = bcm_trunk_get(0, tid, &t_add_info);

    if (r >= 0) {
        SOC_PBMP_CLEAR(pbmp);
        for (i = 0; i < t_add_info.num_ports; i++) { 
            SOC_PBMP_PORT_ADD(pbmp, t_add_info.tp[i]);
    	}
       
        SOC_PBMP_CLEAR(rbmp);
        SOC_PBMP_WORD_SET(rbmp,0,fp->pbmp);
        SOC_PBMP_REMOVE(pbmp, rbmp);

        j = 0;
        SOC_PBMP_ITER(pbmp, i) {
            t_add_info.tm[j] = 0;	
            t_add_info.tp[j++] = i;
        }
        t_add_info.num_ports = j;
     
        if (j) {
            r = bcm_trunk_set(0, tid, &t_add_info);
        } else {
            r = bcm_trunk_destroy(0, tid);
            trunk_delete_l2_cache(tid);
        }
    }

    return r;
}



int set_field_init(void)
{
	int d;
	int i;
	
	d=bde->pci_conf_read(0, 0x0);
	
	if(bcm_field_init(0))
		return -1;
	for(i = 0; i < G_GROUP_NUM; i++)
		group_used[ i ] = 0;
	
	return 0;
}

int get_field_group_number(int *group_number)
{
    *group_number = G_GROUP_NUM;
	return 0;
}

int set_field_udf_set(int udf_id,int type,int offset)
{
	bcm_field_udf_spec_t udf_spec;
	int flag_mask=0x0;
	int i;


	for(i=0;i<96;i++)udf_spec.offset[i] =offset;

	for(i=0;i<96&&type;i++) 	
	{
		{
			flag_mask = BCM_UDF_VLAN_MASK;
			/* tag bits in udf_index bits [6:5] */
			if ((i & flag_mask) == BCM_UDF_VLAN_ONETAG) { 
				udf_spec.offset[i] += 1; /* 2'b01 */
			}
			else if ((i & flag_mask) == BCM_UDF_VLAN_TWOTAG) { 
				udf_spec.offset[i] += 2; /* 2'b10 */
			}

			/* L2 bits in udf_index bits [4:3] */
			flag_mask = BCM_UDF_L2_MASK;
			if ((i & flag_mask) == BCM_UDF_L2_ETHERNET2 ) { /* Ethernet II */
				udf_spec.offset[i] +=  4; /* 2'b00 */
			}
			else if ((i & flag_mask) == BCM_UDF_L2_SNAP) { /* 802.3 SNAP  */
				udf_spec.offset[i] +=  6; /* 2'b01 */
			}
			else if ((i & flag_mask) == BCM_UDF_L2_LLC )  { /* 802.3 LLC   */
				udf_spec.offset[i] +=  4; /* 2'b10 */
			}
		}		

		if(type==2)
		{
			/* IP bits in udf_index bits [2:0] */
			flag_mask = BCM_UDF_IP_MASK;
			if ((i & flag_mask) == BCM_UDF_IP4_HDR_ONLY) {
				udf_spec.offset[i] +=  5; /* 3'b000 */
				udf_spec.offset[i] |=  0x8000; 
			}
			else if ((i  & flag_mask) == BCM_UDF_IP6_HDR_ONLY) {
				udf_spec.offset[i] +=  10; /* 3'b001 */
			}
			else if ((i & flag_mask) == BCM_UDF_IP4_OVER_IP4) {
				udf_spec.offset[i] +=  5;
				udf_spec.offset[i] |=  0x8000; 
			}
			else if ((i & flag_mask) == BCM_UDF_IP6_OVER_IP4) {
				udf_spec.offset[i] +=  5; /* 3'b011 */
			}
			else if ((i & flag_mask) == BCM_UDF_IP_NOTUSED) {/* No L3 Hdr */
				udf_spec.offset[i] +=  0; /* 3'b100 */
			}
		}

	}

	for(i=0;i<96;i++)udf_spec.offset[i] &=0x8000001f;
	
	if(bcm_field_udf_create_id(0, &udf_spec, udf_id)) return -1;
	
	return 0;
}

int set_field_remove_entry(bcm_fp_entry_t *fp)
{
	bcm_field_group_status_t status;

	if(bcm_field_entry_remove(0, fp->eid)<0) 
		return -1;
	if(bcm_field_entry_destroy(0, fp->eid)<0) 
		return -1;
	if(bcm_field_group_status_get(0, fp->gid, &status))
	    return -1;
	
	if(status.entries_free==status.entries_total) 
    {
	    if(bcm_field_group_destroy(0, fp->gid)) 
            return -1;
		group_used[ fp->gid - 1] = 0;
	}

	return 0;
}

int set_field_add_action(bcm_fp_entry_action_t *fp)
{
	int action;

	switch(fp->action){
		case 0: 
			action=bcmFieldActionDrop; 
			break;
		case 1: 
			action=bcmFieldActionEgressMask; 
			break;
		case 2: 
			action=bcmFieldActionEgressMask; 
			break;
		default:
			return -1;
	}
	bcm_field_action_remove(0, fp->eid, bcmFieldActionEgressMask );
	if(bcm_field_action_add(0,fp->eid,action,fp->param0,fp->param1))
		return -1;
	if(bcm_field_entry_install(0, fp->eid)<0) 
		return -1;

	return 0;
}

static int _get_fp_key_mode(unsigned int key_mask)
{
	if( (key_mask&KEY_MASK_ALL) &&((key_mask&0x30000)==0) ) 
		return KEY_MODE_TYPICAL;
	if( key_mask == KEY_MASK_SIPV6) 
		return KEY_MODE_IPV6_SRC; 
	if( key_mask == KEY_MASK_DIPV6) 
		return KEY_MODE_IPV6_DST; 

	return -1;
}

static bcm_field_qset_t _get_fp_qset(int key_mode)
{
	bcm_field_qset_t qset;
	
	BCM_FIELD_QSET_INIT(qset);
	switch(key_mode)
	{
		case KEY_MODE_TYPICAL:
				BCM_FIELD_QSET_ADD(qset, bcmFieldQualifySrcIp);
				BCM_FIELD_QSET_ADD(qset, bcmFieldQualifyDstIp);
				BCM_FIELD_QSET_ADD(qset, bcmFieldQualifyL4DstPort);
				BCM_FIELD_QSET_ADD(qset, bcmFieldQualifyL4SrcPort);
				BCM_FIELD_QSET_ADD(qset, bcmFieldQualifyTcpControl);
				BCM_FIELD_QSET_ADD(qset, bcmFieldQualifyIpFlags);
				BCM_FIELD_QSET_ADD(qset, bcmFieldQualifyIpProtocol);
				BCM_FIELD_QSET_ADD(qset, bcmFieldQualifyInPorts);
                BCM_FIELD_QSET_ADD(qset, bcmFieldQualifyOuterVlan);
				break;
		case KEY_MODE_IPV6_SRC:
				BCM_FIELD_QSET_ADD(qset, bcmFieldQualifySrcIp6);
				BCM_FIELD_QSET_ADD(qset, bcmFieldQualifyInPorts);
				break;
		case KEY_MODE_IPV6_DST:
				BCM_FIELD_QSET_ADD(qset, bcmFieldQualifyDstIp6);
				BCM_FIELD_QSET_ADD(qset, bcmFieldQualifyInPorts);
				break;
		case KEY_MODE_RESERVE:
				BCM_FIELD_QSET_ADD(qset, bcmFieldQualifyInPorts);
				BCM_FIELD_QSET_ADD(qset, bcmFieldQualifyDstPort);
				break;
		default:
				break;
	}
	return qset;
	
}

int _add_fp_entry_field(int eid,bcm_fp_entry_t *fp)
{

	if(fp->key_mask&KEY_MASK_PROTOCOL)
	{
		if(bcm_field_qualify_IpProtocol(0,eid,fp->protocol,0xff))return -1;
	}

	if(fp->key_mask&KEY_MASK_TCP_FLAG)
	{
		uint8 mask=(fp->key_mask&KEY_MASK_TCP_FLAG)>>6;

		if(bcm_field_qualify_TcpControl(0,eid,fp->tcp_flag,mask))return -1;

	}

	if(fp->key_mask&KEY_MASK_SIP)
	{
		if(bcm_field_qualify_SrcIp(0,eid,fp->sip,fp->sip_mask))return -1;
	}

	if(fp->key_mask&KEY_MASK_DIP)
	{
		if(bcm_field_qualify_DstIp(0,eid,fp->dip,fp->dip_mask))return -1;
	}
    
    if( fp->key_mask&KEY_MASK_OUTER_VLAN)
    {
        if(bcm_field_qualify_OuterVlan(0,eid,fp->outer_vlan,fp->outer_vlan_mask)) return -1;
    }

	if(fp->key_mask&KEY_MASK_SPORT)
	{
		
		if(fp->min_sport==fp->max_sport)
		{
			if(bcm_field_qualify_L4SrcPort(0,eid,fp->min_sport,fp->sport_mask))return -1;
		}
	}

	if(fp->key_mask&KEY_MASK_DPORT)
	{
		if(fp->min_dport==fp->max_dport)
		{
			if(bcm_field_qualify_L4DstPort(0,eid,fp->min_dport,fp->dport_mask))return -1;
		}
	}

	if(fp->key_mask&KEY_MASK_IPFRAGMENT)
	{
		if(bcm_field_qualify_IpFlags(0,eid,0,0x2))return -1;
	}

	if(fp->key_mask&KEY_MASK_IPVERSION)
	{
		int fmt;

		fmt=(fp->ipversion==0x0) ? 0x10 : 0x20;

		if(bcm_field_qualify_PacketFormat(0,eid,fmt,0x3f))return -1;
	}

	if((fp->key_mask&KEY_MASK_INTERFACE)||(fp->key_mask&KEY_MASK_INPORTS))
	{

		pbmp_t data,mask;

		SOC_PBMP_CLEAR(data);
		SOC_PBMP_CLEAR(mask);
		SOC_PBMP_WORD_SET(data,0,fp->inports);
		SOC_PBMP_WORD_SET(mask,0,0x1fffffff);
		if(bcm_field_qualify_InPorts(0,eid,data,mask))return -1;

	}

	if(fp->key_mask&KEY_MASK_DSTPORTTGID)
	{
		if(bcm_field_qualify_DstPort(0,eid,0,0,fp->dstporttgid,0x1f))return -1;
	}

	return 0;
}

static int get_gid_prio(int group_id)
{
    int rtv_prio = 0;

    if(group_id < 1 || group_id > G_GROUP_NUM)
        return -1;
	
    rtv_prio = G_GROUP_NUM - group_id;
    return rtv_prio;
}


static int create_fp_group_id(int group_id,int key_mode)
{
	int prio;
	bcm_field_qset_t qset;
 
    prio = get_gid_prio(group_id);
	if(prio == -1 )
	{
		printk("get group prio failed.\n");
		return -1;
	}
	qset=_get_fp_qset(key_mode);
    if(group_used [group_id - 1 ] == 0)
    {
    	if( bcm_field_group_create_id(0, qset, prio, group_id) ) 
    	{
        	printk("group create id failed.\n");
        	return -1;
    	}
		group_used [group_id - 1] = 1;
		//printk("group create id %d over.\n",group_id);
    }
	
	return group_id;
}

static int get_rule_entry_prio(int rule_id)
{
    int entry_prio;
    
    if(rule_id < 0 || rule_id > G_RULE_NUM)
        return -1;

    entry_prio = (65 - rule_id) << 4;

    return entry_prio;
}

int set_field_add_entry(bcm_fp_entry_t *fp)
{
	int eid,gid;
	int key_mode;
	int entry_prio;

	if (fp->ruleid == G_RULE_NUM + 1)
	{
		entry_prio = 0;
		fp->gid = G_GROUP_NUM;
	}
	else
		entry_prio = get_rule_entry_prio(fp->ruleid);
	
	if( (key_mode = _get_fp_key_mode(fp->key_mask))<0) 
		return -1;

	if( (gid = create_fp_group_id(fp->gid, key_mode))<0 ) 
		return -1;

	if(bcm_field_entry_create(0, gid, &eid)<0) 
		return -1;

	if(bcm_field_entry_prio_set(0, eid,entry_prio)<0) 
		return -1;

	if(_add_fp_entry_field( eid, fp)<0) 
		return -1;
    
	if(fp->gid)
		if(bcm_field_action_add(0,eid,bcmFieldActionAddClassTag,fp->gid,0))
			return -1;

	if(bcm_field_entry_install(0, eid))return -1;
    
	fp->eid=eid;
	fp->gid=gid;

	return 0;
}

static struct file_operations fops;

static int
_init(void)
{
	int result;
	result = register_chrdev(120, "sample", &fops);	

	if(result<0){
		printk("Can not get major 120\n");
		return -1;
	}

    return 0;
}

/*
 * Function: _cleanup
 *
 * Purpose:
 *    Module cleanup function
 * Parameters:
 *    None
 * Returns:
 *    Always 0
 * Notes:
 *    The BCM diag thread will be destroyed to avoid page faults.
 */
static int
_cleanup(void)
{
    unregister_chrdev(120,"sample");

    return 0;
}	
static int
_open(struct inode *inode, struct file *file)
{
	return 0;
}
static int 
_close(struct inode *inode, struct file *file)
{
	return 0;
}
static int
_ioctl(struct inode *inode, struct file *file, unsigned int cmd, unsigned long arg)
{
	pbmp_t pbmp_get_cntr;
	pbmp_t pbmp_clr_cntr;
	int i;
	
	switch(cmd)
	{
		case IOCTL_BCM_GET_COUNTERS:
		{
			ioctl_bcm_get_counters_t * cmd_arg_get_cntr = (ioctl_bcm_get_counters_t *)arg;
			BCM_PBMP_PORT_SET(pbmp_get_cntr,cmd_arg_get_cntr->port);
			get_counters(cmd_arg_get_cntr->unit, pbmp_get_cntr, cmd_arg_get_cntr->cntr, &cmd_arg_get_cntr->cntr_num);
			break;
		}
		case IOCTL_BCM_CLEAR_COUNTERS:
		{
			ioctl_bcm_clear_counters_t * cmd_arg_clr_cntr = (ioctl_bcm_clear_counters_t *)arg;
			BCM_PBMP_PORT_SET(pbmp_clr_cntr,cmd_arg_clr_cntr->port);
			clear_counters(cmd_arg_clr_cntr->unit, pbmp_clr_cntr);
			break;
		}
		case IOCTL_BCM_GET_PORT_STATUS:
		{
			ioctl_bcm_get_port_status_t * cmd_arg_gps = (ioctl_bcm_get_port_status_t *)arg;
            //int link, speed, duplex, autoneg, pause, max_frame;
			//get_port_status(cmd_arg_gps->unit, cmd_arg_gps->port, &link,&speed,&duplex,&autoneg,&pause,&max_frame);
			get_port_status(cmd_arg_gps->unit, cmd_arg_gps->port, &cmd_arg_gps->link,&cmd_arg_gps->speed,&cmd_arg_gps->duplex,&cmd_arg_gps->autoneg,&cmd_arg_gps->pause,&cmd_arg_gps->max_frame);
			break;
		}
		case IOCTL_BCM_SET_PORT_AUTONEG:
		{
			ioctl_bcm_set_port_config_t * cmd_arg = (ioctl_bcm_set_port_config_t *)arg;
			set_port_autoneg(cmd_arg->unit, cmd_arg->port, cmd_arg->autoneg);
			break;
		}
		case IOCTL_BCM_SET_PORT_SPEED:
		{
			ioctl_bcm_set_port_config_t * cmd_arg = (ioctl_bcm_set_port_config_t *)arg;
			set_port_speed(cmd_arg->unit, cmd_arg->port, cmd_arg->speed, cmd_arg->duplex);
			break;
		}
		case IOCTL_BCM_SET_PORT_PAUSE:
		{
			ioctl_bcm_set_port_config_t * cmd_arg = (ioctl_bcm_set_port_config_t *)arg;
			set_port_pause(cmd_arg->unit, cmd_arg->port, cmd_arg->pause);
			break;
		}
		case IOCTL_BCM_FP_INIT:
		{
			set_field_init();
			break;
		}
		case IOCTL_BCM_FP_ENTRY_ADD:
		{
			bcm_fp_entry_t *fp=(bcm_fp_entry_t *)arg;

			if(set_field_add_entry(fp)) return -1;
			
			break;
		}
		case IOCTL_BCM_FP_UDF_SET:
		{
			bcm_fp_udf_t *fp=(bcm_fp_udf_t *)arg;

			if(set_field_udf_set(fp->udf_id,fp->type,fp->offset)) return -1;
			
			break;
		}
		case IOCTL_BCM_VLAN_INIT:
		{
			if(init_vlan()) return -1;
			break;
		}
		case IOCTL_BCM_VLAN_CREATE:
		{
			bcm_ext_vlan_t *p=(bcm_ext_vlan_t *)arg;

			if(create_vlan(p->vid,p->port_map,p->untag_map)) return -1;
			
			break;
		}
		case IOCTL_BCM_VLAN_ADD:
		{
			bcm_ext_vlan_t *p=(bcm_ext_vlan_t *)arg;

			if(add_vlan(p->vid,p->port_map,p->untag_map)) return -1;
			
			break;
		}
		case IOCTL_BCM_VLAN_REMOVE:
		{
			bcm_ext_vlan_t *p=(bcm_ext_vlan_t *)arg;

			if(remove_vlan(p->vid,p->port_map)) return -1;
			
			break;
		}
		case IOCTL_BCM_VLAN_DESTROY:
		{
			bcm_ext_vlan_t *p=(bcm_ext_vlan_t *)arg;

			if(destroy_vlan(p->vid)) return -1;
			
			break;
		}
		case IOCTL_BCM_FP_ENTRY_REMOVE:
		{
			bcm_fp_entry_t *fp=(bcm_fp_entry_t *)arg;

			if(set_field_remove_entry(fp)) return -1;
			
			break;
		}
		case IOCTL_BCM_PVLAN_SET:
		{
			bcm_pvlan_t *fp=(bcm_pvlan_t *)arg;

			if(set_port_pvlan(fp->port_map,fp->vid))return -1;

			break;
		}
		case IOCTL_BCM_GROUP_NUMBER_GET:
		{
			int *number=(int *)arg;

			if(get_field_group_number(number))return -1;

			break;
		}
		case IOCTL_BCM_XGE_PHY_SET:
		{
			bcm_phy_t *fp=(bcm_phy_t *)arg;

			if(set_xe_phy_register(fp->port, fp->devad, fp->reg, fp->mask, fp->value))return -1;

			break;
		}
		case IOCTL_BCM_XGE_PHY_GET:
		{
			bcm_phy_t *fp=(bcm_phy_t *)arg;

			if(get_xe_phy_register(fp->port, fp->devad, fp->reg, fp->mask, &fp->value))return -1;

			break;
		}
		case IOCTL_BCM_SET_PORT_LEARN:
		{
			ioctl_bcm_set_port_learn_t * cmd_arg = (ioctl_bcm_set_port_learn_t *)arg;
			set_port_learn(cmd_arg->unit, cmd_arg->port, cmd_arg->enable);
			break;
		}
		case IOCTL_BCM_GE_PHY_SET:
		{
			bcm_phy_t *fp=(bcm_phy_t *)arg;

			if(set_port_phy_register(fp->port, fp->reg, fp->mask, fp->value))return -1;

			break;
		}
		case IOCTL_BCM_GE_PHY_GET:
		{
			bcm_phy_t *fp=(bcm_phy_t *)arg;

			if(get_port_phy_register(fp->port, fp->reg, fp->mask, &fp->value))return -1;

			break;
		}
		case IOCTL_BCM_FP_ENTRY_ACTION:
		{
			bcm_fp_entry_action_t *fp=(bcm_fp_entry_action_t *)arg;

			if(set_field_add_action(fp))return -1;

			break;
		}
		case IOCTL_BCM_TRUNK_ADD:
		{
			ioctl_bcm_trunk_t *cmd_arg=(ioctl_bcm_trunk_t *)arg;

			if(trunk_add(cmd_arg))return -1;

			break;
		}
		case IOCTL_BCM_TRUNK_REMOVE:
		{
			ioctl_bcm_trunk_t *cmd_arg=(ioctl_bcm_trunk_t *)arg;

			if(trunk_remove(cmd_arg))return -1;

			break;
		}
		case IOCTL_BCM_REG_GET:
		{
			args_t temp_arg_rg;
			bcm_reg_args_t *in_argc_rg;
		
			in_argc_rg = (bcm_reg_args_t *)arg;
			memset(&temp_arg_rg, 0, sizeof(args_t));
			temp_arg_rg.a_cmd = in_argc_rg->a_cmd;
			for ( i = 0; i < 16; i++ )
				temp_arg_rg.a_argv[i] = in_argc_rg->a_argv[i];
			strncpy(temp_arg_rg.a_buffer,in_argc_rg->a_buffer,64);
			temp_arg_rg.a_argc = in_argc_rg->a_argc;
			temp_arg_rg.a_arg = in_argc_rg->a_arg;
			if(cmd_robo_reg_get_(in_argc_rg->unit, &temp_arg_rg))
				return -1;
			strncpy(in_argc_rg->a_buffer,temp_arg_rg.a_buffer,64);
			break;
		}
		case IOCTL_BCM_REG_SET:
		{
			args_t temp_arg;
			bcm_reg_args_t *in_argc;
			
			in_argc = (bcm_reg_args_t *)arg;
			memset(&temp_arg, 0, sizeof(args_t));
			temp_arg.a_cmd = in_argc->a_cmd;
			for ( i = 0; i < 16; i++ )
				temp_arg.a_argv[i] = in_argc->a_argv[i];
			strncpy(temp_arg.a_buffer,in_argc->a_buffer,64);
			temp_arg.a_argc = in_argc->a_argc;
			temp_arg.a_arg = in_argc->a_arg;
			if(cmd_robo_reg_set(in_argc->unit, &temp_arg))
				return -1;
			break;
		}
		case IOCTL_BCM_SLB_READ:
		{
			bcm_slb_args_t *cmd_arg	= (bcm_slb_args_t*)arg;
			cmd_arg->value = sr_subc(cmd_arg->device_id,
			                         cmd_arg->sub_mode,
			                         cmd_arg->address);
			break;
		}
		case IOCTL_BCM_SLB_WRITE:
		{
			bcm_slb_args_t *cmd_arg	= (bcm_slb_args_t*)arg;
			sw_subc(cmd_arg->device_id,
			            cmd_arg->sub_mode,
			            cmd_arg->address,
			            cmd_arg->value);
			break;
		}
		default:
			break;
	}
	return 0;
}

int init_module(void)
{
	return _init();
}
void cleanup_module(void)
{
    _cleanup();
}

/* Module vectors */
static struct file_operations fops = {
    read: NULL,
    write: NULL,
    open: _open, 
    release: _close, 
    ioctl: _ioctl,
}; 

