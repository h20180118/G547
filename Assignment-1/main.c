# include <linux/init.h>
# include <linux/module.h>
# include <linux/moduleparam.h>
# include <linux/version.h>
# include <linux/types.h>
# include <linux/kdev_t.h>
# include <linux/fs.h>
# include <linux/device.h>
# include <linux/cdev.h>
# include <linux/uaccess.h>
# include <linux/random.h>

static dev_t first;
static struct cdev c_dev;
static struct class *cls;
static char s[3] ={'x','y','z'};
 char value[100];
int i,k,q,m;






// call back function

static int my_open(struct inode *i, struct file *f)

{
printk(KERN_INFO "open\n");
return 0;
}


static int my_close(struct inode *i, struct file *f)

{
printk(KERN_INFO "close\n");
return 0;
}


static ssize_t my_read(struct file *f, char __user *buf, size_t len, loff_t *off)

{unsigned long r;

unsigned int j=0;
unsigned int out=0;
get_random_bytes(&j,sizeof(int));

printk(KERN_INFO "value of out is %u\n",out);
printk(KERN_INFO " read()\n");
for(m=0;m<10;m++)
{
out = j % 10;
j=j/10;
value[m]=out;
}
r=copy_to_user(buf,value,len);
return r;

}

//file operation structure
static struct file_operations fops =

{

.owner= THIS_MODULE,
.open= my_open,
.release= my_close,
.read= my_read


};


// init function

 static int __init my_init(void)

{
printk(KERN_ALERT "HELLO LINUX  \n");

// major & minor number


if (alloc_chrdev_region (&first ,0,1 , "ADXL") <0)
{
return -1;
}




// class creation


if ((cls=class_create (THIS_MODULE, "ADXL_Driver"))==NULL)

{
unregister_chrdev_region(first, 1);
return -1;
}
//device file
for(i=0;i<3;i++){
    if (device_create(cls, NULL, MKDEV(MAJOR(first),i), NULL, "ADXL_%c",s[i]) == NULL)
  {
    class_destroy(cls);
    unregister_chrdev_region(first, 1);
    return -1;
  }
}
//cdev
for(k=1;k<4;k++){
    cdev_init(&c_dev, &fops);
    if (cdev_add(&c_dev, MKDEV(MAJOR(first),(k-1)), k) == -1)
  {
    device_destroy(cls, MKDEV(MAJOR(first),(k-1)));
    class_destroy(cls);
    unregister_chrdev_region(first, k);
    return -1;
  
  }
}
  return 0;
}




// exit function

static void __exit my_exit(void)

{

printk(KERN_ALERT "BYE LINUX \n");

unregister_chrdev_region(first,1);

cdev_del(&c_dev);
for(q=0;q<3;q++){
  device_destroy(cls, MKDEV(MAJOR(first),q));
}
class_destroy(cls);



}

MODULE_DESCRIPTION ("assignment 1");

MODULE_AUTHOR ("MALAIKA <malaikaafra gmail.com>");

MODULE_LICENSE ("GPL");

MODULE_INFO (SupportedChips,"PCA");



module_init(my_init);
module_exit(my_exit);
