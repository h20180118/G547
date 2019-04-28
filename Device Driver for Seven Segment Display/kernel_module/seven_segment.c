#include <linux/init.h>
#include <linux/module.h>
#include <asm/io.h>
#include <linux/timer.h>
#include <linux/device.h>
#include <linux/err.h>
#include <linux/mm.h>
#include <linux/module.h>
#include <linux/version.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/moduleparam.h>
#include <linux/random.h>
#include <linux/kernel.h>
#define PERIPH_BASE 0x3f000000
#define GPIO_BASE (PERIPH_BASE + 0x200000)  // mapping raspberry pi physical address to kernel virtual memory address


struct gpio_registers 
{
    uint32_t GPFSEL[6];
    uint32_t Reserved1;
    uint32_t GPSET[2];
    uint32_t Reserved2;
    uint32_t GPCLR[2];
    
};


struct gpio_registers *s_gpio_registers; 
static dev_t first; // Global variable for the first device number 
static struct cdev c_dev; // Global variable for the character device structure
static struct class *cl; // Global variable for the device class
static struct device *s_pDeviceObject;
static int input_number = 1000;
static const int gpio_pin18 = 18; // gpio pin initialization
static const int gpio_pin13 = 13; 
static const int gpio_pin12 = 12;   
static const int gpio_pin16 = 16;   
static const int gpio_pin20 = 20;   
static const int gpio_pin21 = 21;   
static const int gpio_pin19 = 19;   





int i,p,m;
char tx_buffer[2];

static void gpio_direction(int GPIO, int functionCode)   //function to set gpio pins as input or output
{
    
    
    s_gpio_registers->GPFSEL[1] = 0x9040242;
    s_gpio_registers->GPFSEL[2] = 0x9;
   
}


static void gpio_output_value(int GPIO, bool outputValue)  //function to set or clear 
{

    if (outputValue){
        s_gpio_registers->GPSET[GPIO / 32] = (1 << (GPIO % 32));
        printk("setis %x\n", s_gpio_registers->GPSET[GPIO / 32]);
		}

    else{
        s_gpio_registers->GPCLR[GPIO / 32] = (1 << (GPIO % 32));
		printk("clearis %x\n", s_gpio_registers->GPCLR[GPIO / 32]);
		}
}
static int my_open(struct inode *i, struct file *f) //callback function for device open
{
  printk(KERN_INFO "Driver: open()\n");
  return 0;
}
  static int my_close(struct inode *i, struct file *f)   //callback function for device close
{
  printk(KERN_INFO "Driver: close()\n");
  return 0;
}
  


  
static ssize_t my_write(struct file *f, const char __user *buf,   size_t len, loff_t *off)   //callback function to write to the device
{
  printk(KERN_INFO "Driver: write()\n");
 int retval = copy_from_user(tx_buffer,buf,len);
 printk("copy_from_user returned (%d) we read [%s]\n",retval , tx_buffer);
 int l=tx_buffer[0]-'0';
printk("the value of l is %d \n",l);
gpio_direction(gpio_pin18, 0b001);
    gpio_direction(gpio_pin13, 0b001);  
    gpio_direction(gpio_pin12, 0b001);  
    gpio_direction(gpio_pin16, 0b001);
    gpio_direction(gpio_pin20, 0b001);
    gpio_direction(gpio_pin21, 0b001);
    gpio_direction(gpio_pin19, 0b001);
    
    	
    if(l==0)
    {
    
    gpio_output_value(gpio_pin18, 0b000);
    gpio_output_value(gpio_pin13, 0b000);  
    gpio_output_value(gpio_pin12, 0b000);  
    gpio_output_value(gpio_pin16, 0b000);
    gpio_output_value(gpio_pin20, 0b000);
    gpio_output_value(gpio_pin21, 0b001);
    gpio_output_value(gpio_pin19, 0b000);
    }
    
    else if(l==1){
    
    gpio_output_value(gpio_pin18, 0b000);
    gpio_output_value(gpio_pin13, 0b001);  
    gpio_output_value(gpio_pin12, 0b001);  
    gpio_output_value(gpio_pin16, 0b000);
    gpio_output_value(gpio_pin20, 0b001);
    gpio_output_value(gpio_pin21, 0b001);
    gpio_output_value(gpio_pin19, 0b001);
   
    }
    

     else if(l==2){

    gpio_output_value(gpio_pin18, 0b000);
    gpio_output_value(gpio_pin13, 0b000);  
    gpio_output_value(gpio_pin12, 0b000);  
    gpio_output_value(gpio_pin16, 0b001);
    gpio_output_value(gpio_pin20, 0b001);
    gpio_output_value(gpio_pin21, 0b000);
    gpio_output_value(gpio_pin19, 0b000);
   }
    
    else if(l==3){

    gpio_output_value(gpio_pin18, 0b000);
    gpio_output_value(gpio_pin13, 0b000);  
    gpio_output_value(gpio_pin12, 0b001);  
    gpio_output_value(gpio_pin16, 0b000);
    gpio_output_value(gpio_pin20, 0b001);
    gpio_output_value(gpio_pin21, 0b000);
    gpio_output_value(gpio_pin19, 0b000);
    }
    
    else if(l==4){
    
    gpio_output_value(gpio_pin18, 0b000);
    gpio_output_value(gpio_pin13, 0b001);  
    gpio_output_value(gpio_pin12, 0b001);  
    gpio_output_value(gpio_pin16, 0b000);
    gpio_output_value(gpio_pin20, 0b000);
    gpio_output_value(gpio_pin21, 0b000);
    gpio_output_value(gpio_pin19, 0b001);
    }
    
    else if(l==5){

    gpio_output_value(gpio_pin18, 0b001);
    gpio_output_value(gpio_pin13, 0b000);  
    gpio_output_value(gpio_pin12, 0b001);  
    gpio_output_value(gpio_pin16, 0b000);
    gpio_output_value(gpio_pin20, 0b000);
    gpio_output_value(gpio_pin21, 0b000);
    gpio_output_value(gpio_pin19, 0b000);
    }
    
    else if(l==6){
  
    gpio_output_value(gpio_pin18, 0b001);
    gpio_output_value(gpio_pin13, 0b000);  
    gpio_output_value(gpio_pin12, 0b000);  
    gpio_output_value(gpio_pin16, 0b000);
    gpio_output_value(gpio_pin20, 0b000);
    gpio_output_value(gpio_pin21, 0b000);
    gpio_output_value(gpio_pin19, 0b000);
    }
    
    else if(l==7){
    
    gpio_output_value(gpio_pin18, 0b000);
    gpio_output_value(gpio_pin13, 0b001);  
    gpio_output_value(gpio_pin12, 0b001);  
    gpio_output_value(gpio_pin16, 0b000);
    gpio_output_value(gpio_pin20, 0b001);
    gpio_output_value(gpio_pin21, 0b001);
    gpio_output_value(gpio_pin19, 0b000);
    }
        
    else if(l==8){
    
    gpio_output_value(gpio_pin18, 0b000);
    gpio_output_value(gpio_pin13, 0b000);  
    gpio_output_value(gpio_pin12, 0b000);  
    gpio_output_value(gpio_pin16, 0b000);
    gpio_output_value(gpio_pin20, 0b000);
    gpio_output_value(gpio_pin21, 0b000);
    gpio_output_value(gpio_pin19, 0b000);
    }
    
    else if(l==9){
    gpio_output_value(gpio_pin18, 0b000);
    gpio_output_value(gpio_pin13, 0b000);  
    gpio_output_value(gpio_pin12, 0b001);  
    gpio_output_value(gpio_pin16, 0b000);
    gpio_output_value(gpio_pin20, 0b000);
    gpio_output_value(gpio_pin21, 0b000);
    gpio_output_value(gpio_pin19, 0b000);
    }
    else
    {
    gpio_output_value(gpio_pin18, 0b001);
    gpio_output_value(gpio_pin13, 0b001);  
    gpio_output_value(gpio_pin12, 0b001);  
    gpio_output_value(gpio_pin16, 0b001);
    gpio_output_value(gpio_pin20, 0b001);
    gpio_output_value(gpio_pin21, 0b001);
    gpio_output_value(gpio_pin19, 0b001);
        }


  return len;
}
  static struct file_operations pugs_fops =
{
  .owner = THIS_MODULE,
  .open = my_open,
  .release = my_close,
  .write = my_write
};



static ssize_t set_period_callback(struct device* dev, struct device_attribute* attr, const char* buf, size_t count)  //callback function when write operation using sysfs interface is done
{
    long period_value = 0;
    if (kstrtol(buf, 10, &period_value) < 0)   //converting string value to long 
        return -EINVAL;
         
    

    input_number = period_value;
    gpio_direction(gpio_pin18, 0b001);
    gpio_direction(gpio_pin13, 0b001);  
    gpio_direction(gpio_pin12, 0b001);  
    gpio_direction(gpio_pin16, 0b001);
    gpio_direction(gpio_pin20, 0b001);
    gpio_direction(gpio_pin21, 0b001);
    gpio_direction(gpio_pin19, 0b001);
    
    if(input_number==0)
    {
    
    gpio_output_value(gpio_pin18, 0b000);
    gpio_output_value(gpio_pin13, 0b000);  
    gpio_output_value(gpio_pin12, 0b000);  
    gpio_output_value(gpio_pin16, 0b000);
    gpio_output_value(gpio_pin20, 0b000);
    gpio_output_value(gpio_pin21, 0b001);
    gpio_output_value(gpio_pin19, 0b000);
    }
    
    else if(input_number==1){
    
    gpio_output_value(gpio_pin18, 0b000);
    gpio_output_value(gpio_pin13, 0b001);  
    gpio_output_value(gpio_pin12, 0b001);  
    gpio_output_value(gpio_pin16, 0b000);
    gpio_output_value(gpio_pin20, 0b001);
    gpio_output_value(gpio_pin21, 0b001);
    gpio_output_value(gpio_pin19, 0b001);
   
    }
    

     else if(input_number==2){

    gpio_output_value(gpio_pin18, 0b000);
    gpio_output_value(gpio_pin13, 0b000);  
    gpio_output_value(gpio_pin12, 0b000);  
    gpio_output_value(gpio_pin16, 0b001);
    gpio_output_value(gpio_pin20, 0b001);
    gpio_output_value(gpio_pin21, 0b000);
    gpio_output_value(gpio_pin19, 0b000);
   }
    
    else if(input_number==3){

    gpio_output_value(gpio_pin18, 0b000);
    gpio_output_value(gpio_pin13, 0b000);  
    gpio_output_value(gpio_pin12, 0b001);  
    gpio_output_value(gpio_pin16, 0b000);
    gpio_output_value(gpio_pin20, 0b001);
    gpio_output_value(gpio_pin21, 0b000);
    gpio_output_value(gpio_pin19, 0b000);
    }
    
    else if(input_number==4){
    
    gpio_output_value(gpio_pin18, 0b000);
    gpio_output_value(gpio_pin13, 0b001);  
    gpio_output_value(gpio_pin12, 0b001);  
    gpio_output_value(gpio_pin16, 0b000);
    gpio_output_value(gpio_pin20, 0b000);
    gpio_output_value(gpio_pin21, 0b000);
    gpio_output_value(gpio_pin19, 0b001);
    }
    
    else if(input_number==5){

    gpio_output_value(gpio_pin18, 0b001);
    gpio_output_value(gpio_pin13, 0b000);  
    gpio_output_value(gpio_pin12, 0b001);  
    gpio_output_value(gpio_pin16, 0b000);
    gpio_output_value(gpio_pin20, 0b000);
    gpio_output_value(gpio_pin21, 0b000);
    gpio_output_value(gpio_pin19, 0b000);
    }
    
    else if(input_number==6){
  
    gpio_output_value(gpio_pin18, 0b001);
    gpio_output_value(gpio_pin13, 0b000);  
    gpio_output_value(gpio_pin12, 0b000);  
    gpio_output_value(gpio_pin16, 0b000);
    gpio_output_value(gpio_pin20, 0b000);
    gpio_output_value(gpio_pin21, 0b000);
    gpio_output_value(gpio_pin19, 0b000);
    }
    
    else if(input_number==7){
    
    gpio_output_value(gpio_pin18, 0b000);
    gpio_output_value(gpio_pin13, 0b001);  
    gpio_output_value(gpio_pin12, 0b001);  
    gpio_output_value(gpio_pin16, 0b000);
    gpio_output_value(gpio_pin20, 0b001);
    gpio_output_value(gpio_pin21, 0b001);
    gpio_output_value(gpio_pin19, 0b000);
    }
        
    else if(input_number==8){
    
    gpio_output_value(gpio_pin18, 0b000);
    gpio_output_value(gpio_pin13, 0b000);  
    gpio_output_value(gpio_pin12, 0b000);  
    gpio_output_value(gpio_pin16, 0b000);
    gpio_output_value(gpio_pin20, 0b000);
    gpio_output_value(gpio_pin21, 0b000);
    gpio_output_value(gpio_pin19, 0b000);
    }
    
    else if(input_number==9){
    gpio_output_value(gpio_pin18, 0b000);
    gpio_output_value(gpio_pin13, 0b000);  
    gpio_output_value(gpio_pin12, 0b001);  
    gpio_output_value(gpio_pin16, 0b000);
    gpio_output_value(gpio_pin20, 0b000);
    gpio_output_value(gpio_pin21, 0b000);
    gpio_output_value(gpio_pin19, 0b000);
    }
    else
    {
    gpio_output_value(gpio_pin18, 0b001);
    gpio_output_value(gpio_pin13, 0b001);  
    gpio_output_value(gpio_pin12, 0b001);  
    gpio_output_value(gpio_pin16, 0b001);
    gpio_output_value(gpio_pin20, 0b001);
    gpio_output_value(gpio_pin21, 0b001);
    gpio_output_value(gpio_pin19, 0b001);
        }
    return count;
}





static DEVICE_ATTR(period, S_IRWXU | S_IRWXG, NULL, set_period_callback);


static int __init my_init(void)
{
    int result;
    s_gpio_registers = (struct gpio_registers *)ioremap(GPIO_BASE, sizeof(struct gpio_registers));
    gpio_direction(gpio_pin18, 0b001);
    gpio_direction(gpio_pin13, 0b001);  
    gpio_direction(gpio_pin12, 0b001);  
    gpio_direction(gpio_pin16, 0b001);
    gpio_direction(gpio_pin20, 0b001);
    gpio_direction(gpio_pin21, 0b001);
    gpio_direction(gpio_pin19, 0b001);
   
     if(input_number==1000)
   {
    
    gpio_output_value(gpio_pin18, 0b000);
    gpio_output_value(gpio_pin13, 0b000);  
    gpio_output_value(gpio_pin12, 0b000);  
    gpio_output_value(gpio_pin16, 0b000);
    gpio_output_value(gpio_pin20, 0b000);
    gpio_output_value(gpio_pin21, 0b000);
    gpio_output_value(gpio_pin19, 0b000);
    
    
   }
    else 
    {   
    gpio_output_value(gpio_pin18, 0b001);
    gpio_output_value(gpio_pin13, 0b001);  
    gpio_output_value(gpio_pin12, 0b001);  
    gpio_output_value(gpio_pin16, 0b001);
    gpio_output_value(gpio_pin20, 0b001);
    gpio_output_value(gpio_pin21, 0b001);
    gpio_output_value(gpio_pin19, 0b001);
}
    
    
  if (alloc_chrdev_region(&first, 0, 1, "my_char_device") < 0) //allocates memory for the character device 
  {
    return -1;
  }
    if ((cl = class_create(THIS_MODULE, "Seven_segment_Display")) == NULL)   // creates a class in /sys directory
  {
    unregister_chrdev_region(first, 1);
    return -1;
  }
 s_pDeviceObject = device_create(cl, NULL, MKDEV(MAJOR(first),0), NULL, "7-segment-display");  //creates device object in /sys/class directory
    if ( s_pDeviceObject == NULL)
  {
    class_destroy(cl);
    unregister_chrdev_region(first, 1);
    return -1;
  }


    cdev_init(&c_dev, &pugs_fops);    //intializes character device in the source tree
    if (cdev_add(&c_dev, MKDEV(MAJOR(first),0), 1) == -1)  //makes the device active in the source tree
  {
    device_destroy(cl, MKDEV(MAJOR(first),0));
    class_destroy(cl);
    unregister_chrdev_region(first, 1);
    return -1;
  
  }


    result = device_create_file(s_pDeviceObject, &dev_attr_period);  //creates a sys file 
    BUG_ON(result < 0);

    return 0;
}

static void __exit my_exit(void)
{   
    cdev_del(&c_dev);
    unregister_chrdev_region(first, 1);
    iounmap(s_gpio_registers);
    device_remove_file(s_pDeviceObject, &dev_attr_period);
    device_destroy(cl, MKDEV(MAJOR(first),0));
    class_destroy(cl);
    gpio_direction(gpio_pin18, 0b000);
    gpio_direction(gpio_pin13, 0b000);  
    gpio_direction(gpio_pin12, 0b000);  
    gpio_direction(gpio_pin16, 0b000);
    gpio_direction(gpio_pin20, 0b000);
    gpio_direction(gpio_pin21, 0b000);
    gpio_direction(gpio_pin19, 0b000);
  
  printk(KERN_INFO "GoodBye: Device unregistered");
    
    
}

module_init(my_init);
module_exit(my_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Shweta_Bulusu <h20180124@goa.bits-pilani.ac.in>& Malaika Afra Taj<h20180118@goa.bits-pilani.ac.in>");
MODULE_DESCRIPTION("Seven segment Display");
