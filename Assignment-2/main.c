#include <linux/types.h>
#include <linux/string.h>
#include <linux/vmalloc.h>
#include <linux/string.h>
#include <linux/errno.h>

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <linux/spinlock.h>
#include <linux/genhd.h> 
#include <linux/blkdev.h> 
#include <linux/hdreg.h> 



#define ARRAY_SIZE(a) (sizeof(a) / sizeof(*a))

#define SECTOR_SIZE 512
#define MBR_SIZE SECTOR_SIZE
#define MBR_DISK_SIGNATURE_OFFSET 440
#define MBR_DISK_SIGNATURE_SIZE 4
#define PARTITION_TABLE_OFFSET 446
#define PARTITION_ENTRY_SIZE 16 
#define PARTITION_TABLE_SIZE 64 
#define MBR_SIGNATURE_OFFSET 510
#define MBR_SIGNATURE_SIZE 2
#define MBR_SIGNATURE 0xAA55
#define BR_SIZE SECTOR_SIZE
#define BR_SIGNATURE_OFFSET 510
#define BR_SIGNATURE_SIZE 2
#define BR_SIGNATURE 0xAA55

#define RB_DEVICE_SIZE 1024

#define RB_FIRST_MINOR 0
#define RB_MINOR_CNT 16
#define RB_SECTOR_SIZE 512

typedef struct
{
	unsigned char boot_type; 
	unsigned char start_head;
	unsigned char start_sec:6;
	unsigned char start_cyl_hi:2;
	unsigned char start_cyl;
	unsigned char part_type;
	unsigned char end_head;
	unsigned char end_sec:6;
	unsigned char end_cyl_hi:2;
	unsigned char end_cyl;
	unsigned int abs_start_sec;
	unsigned int sec_in_part;
} PartEntry;

typedef PartEntry PartTable[4];

static PartTable def_part_table =
{
	{
		boot_type: 0x00,
		start_head: 0x00,
		start_sec: 0x2,
		start_cyl: 0x00,
		part_type: 0x83,
		end_head: 0x00,
		end_sec: 0x20,
		end_cyl: 0x09,
		abs_start_sec: 0x00000001,
		sec_in_part: 0x0000013F
	},
	{
		boot_type: 0x00,
		start_head: 0x00,
		start_sec: 0x1,
		start_cyl: 0x14,
		part_type: 0x83,
		end_head: 0x00,
		end_sec: 0x20,
		end_cyl: 0x1F,
		abs_start_sec: 0x00000280,
		sec_in_part: 0x00000180
	}
};

static void copy_mbr(u8 *disk)
{
	memset(disk, 0x0, MBR_SIZE);
	*(unsigned long *)(disk + MBR_DISK_SIGNATURE_OFFSET) = 0x36E5756D;
	memcpy(disk + PARTITION_TABLE_OFFSET, &def_part_table, PARTITION_TABLE_SIZE);
	*(unsigned short *)(disk + MBR_SIGNATURE_OFFSET) = MBR_SIGNATURE;
}

extern void copy_mbr_n_br(u8 *disk)
{
	copy_mbr(disk);

}

static u8 *block_device;

extern int device_init(void)
{
	block_device = vmalloc(RB_DEVICE_SIZE * RB_SECTOR_SIZE);
	if (block_device == NULL)
		return -ENOMEM;
	
	copy_mbr_n_br(block_device);
	return RB_DEVICE_SIZE;
}

extern void device_cleanup(void)
{
	vfree(block_device);
}

extern void device_write(sector_t sector_off, u8 *buffer, unsigned int sectors)
{
	memcpy(block_device + sector_off * RB_SECTOR_SIZE, buffer,sectors * RB_SECTOR_SIZE);
}
extern void device_read(sector_t sector_off, u8 *buffer, unsigned int sectors)
{
	memcpy(buffer, block_device + sector_off * RB_SECTOR_SIZE,sectors * RB_SECTOR_SIZE);
}


static u_int block_major_no = 0;


static struct my_struct
{
	
	unsigned int size;
	
	spinlock_t my_lock;
	
	struct request_queue *my_queue;
	
	struct gendisk *my_gendisk;
} s;

static int my_open(struct block_device *bdev, fmode_t mode)
{
	unsigned unit = iminor(bdev->bd_inode);

	printk(KERN_INFO "Block Device is opened\n");
	printk(KERN_INFO " Inode number is %d\n", unit);

	if (unit > RB_MINOR_CNT)
		return -ENODEV;
	return 0;
}

#if (LINUX_VERSION_CODE < KERNEL_VERSION(3,10,0))
static int my_close(struct gendisk *disk, fmode_t mode)
{
	printk(KERN_INFO " Block Device is closed\n");
	return 0;
}
#else
static void my_close(struct gendisk *disk, fmode_t mode)
{
	printk(KERN_INFO "Block Device is closed\n");
}
#endif


static int my_transfer(struct request *req)
{
	struct my_struct *dev = (struct my_struct *)(req->rq_disk->private_data);

	int dir = rq_data_dir(req);
	sector_t start_sector = blk_rq_pos(req);
	unsigned int sector_cnt = blk_rq_sectors(req);

#if (LINUX_VERSION_CODE < KERNEL_VERSION(3,14,0))
#define BV_PAGE(bv) ((bv)->bv_page)
#define BV_OFFSET(bv) ((bv)->bv_offset)
#define BV_LEN(bv) ((bv)->bv_len)
	struct bio_vec *bv;
#else
#define BV_PAGE(bv) ((bv).bv_page)
#define BV_OFFSET(bv) ((bv).bv_offset)
#define BV_LEN(bv) ((bv).bv_len)
	struct bio_vec bv;
#endif
	struct req_iterator iter;

	sector_t sector_offset;
	unsigned int sectors;
	u8 *buffer;

	int ret = 0;

	

	sector_offset = 0;
        
	rq_for_each_segment(bv, req, iter)
	{
		buffer = page_address(BV_PAGE(bv)) + BV_OFFSET(bv);
		if (BV_LEN(bv) % RB_SECTOR_SIZE != 0)
		{
			printk(KERN_ERR "rb: Should never happen: "
				"bio size (%d) is not a multiple of RB_SECTOR_SIZE (%d).\n"
				"This may lead to data truncation.\n",
				BV_LEN(bv), RB_SECTOR_SIZE);
			ret = -EIO;
		}
		sectors = BV_LEN(bv) / RB_SECTOR_SIZE;
		printk(KERN_DEBUG "rb: Start Sector: %llu, Sector Offset: %llu; Buffer: %p; Length: %u sectors\n",
			(unsigned long long)(start_sector), (unsigned long long)(sector_offset), buffer, sectors);
		if (dir == WRITE) 
		{    
			device_write(start_sector + sector_offset, buffer, sectors);
                        
                          
		}
		else 
		{
			device_read(start_sector + sector_offset, buffer, sectors);
                          
		}
		sector_offset += sectors;
	}
	if (sector_offset != sector_cnt)
	{
		printk(KERN_ERR "bio info doesn't match with the request info");
		ret = -EIO;
	}

	return ret;
}
	

static void my_request(struct request_queue *q)
{
	struct request *req;
	int ret;

	
	while ((req = blk_fetch_request(q)) != NULL)
	{

		ret = my_transfer(req);
		__blk_end_request_all(req, ret);
		
	}
}


static struct block_device_operations my_fops =
{
	.owner = THIS_MODULE,
	.open = my_open,
	.release = my_close,
	
};
	

static int __init my_init(void)
{
	int ret;

	
	if ((ret = device_init()) < 0)
	{
		return ret;
	}
	s.size = ret;

	
	block_major_no = register_blkdev(block_major_no, "disk_on_ram");
	if (block_major_no <= 0)
	{
		printk(KERN_ERR "Major Number error\n");
		device_cleanup();
		return -EBUSY;
	}

	
	spin_lock_init(&s.my_lock);
	s.my_queue = blk_init_queue(my_request, &s.my_lock);
	if (s.my_queue == NULL)
	{
		printk(KERN_ERR "queue failure\n");
		unregister_blkdev(block_major_no, "disk_on_ram");
	        device_cleanup();
		return -ENOMEM;
	}
	
	
	s.my_gendisk = alloc_disk(RB_MINOR_CNT);
	if (!s.my_gendisk)
	{
		printk(KERN_ERR " disk allocation failed\n");
		blk_cleanup_queue(s.my_queue);
		unregister_blkdev(block_major_no, "disk_on_ram");
		device_cleanup();
		return -ENOMEM;
	}

 	
	s.my_gendisk->major = block_major_no;
  	
	s.my_gendisk->first_minor = RB_FIRST_MINOR;
 	
	s.my_gendisk->fops = &my_fops;
 	
	s.my_gendisk->private_data = &s;
	s.my_gendisk->queue = s.my_queue;
	
	
	sprintf(s.my_gendisk->disk_name, "disk_on_ram");
	
	set_capacity(s.my_gendisk, s.size);

	
	add_disk(s.my_gendisk);
	
	printk(KERN_INFO "Block device driver registered (%d sectors; %d bytes)\n",
		s.size, s.size * RB_SECTOR_SIZE);

	return 0;
}

static void __exit my_exit(void)
{
	del_gendisk(s.my_gendisk);
	put_disk(s.my_gendisk);
	blk_cleanup_queue(s.my_queue);
	unregister_blkdev(block_major_no, "disk_on_ram");
        device_cleanup();
	printk(KERN_INFO "Block device driver unregistered \n");
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Malaika Afra <h20180118@goa.bits-pilani.ac.in>");
MODULE_DESCRIPTION(" My Block Device Driver");
MODULE_ALIAS_BLOCKDEV_MAJOR(block_major_no);


