#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/random.h>
#include <linux/uaccess.h>
#include "chardev.h"

#define SUCCESS 0
 
static dev_t first; // variable for device number
static struct cdev c_dev; // variable for the character device structure
static struct class *cls;
static int allgn='r';
static uint16_t ch = 0;

/*code for device open, read and close */

static int device_open(struct inode *i, struct file *f)
{
	printk(KERN_INFO "Myadc : open()\n");
	return 0;
}

static int device_close(struct inode *i, struct file *f)
{
	printk(KERN_INFO "Myadc : close()\n");
	return 0;
}

static ssize_t device_read(struct file *f, char __user *buf, size_t len, loff_t *off)
{         
	uint16_t random;
	uint16_t generatedno;
	get_random_bytes(&random, sizeof(random));
        generatedno= random%1023;
	
	printk(KERN_INFO "The random no generated is :%d\n",generatedno);
	if(allgn=='l')
	{
		generatedno= generatedno*64;
	}
	else generatedno=generatedno;


	copy_to_user(buf,&generatedno,sizeof(generatedno));
	return (sizeof(generatedno));
}

long adc8_ioctl(struct file *file,             
                  unsigned int ioctl_num,        /* number and param for ioctl */
                  unsigned long ioctl_param)
		{

                switch(ioctl_num){
                  case ioctl_selectchannel:
			ch = ioctl_param;
                    	printk(KERN_INFO "The selected channel is: %d",ch);
			break;

                  case ioctl_selectallignment:
                    	allgn = (char)ioctl_param;
                    	printk(KERN_INFO "The allignment set is: %c",allgn);
			break;
                }
              return SUCCESS;
  }



static struct file_operations fops =
{
  .owner 	= THIS_MODULE,
  .open 	= device_open,
  .release 	= device_close,
  .read 	= device_read,
  .unlocked_ioctl = adc8_ioctl
};
 
//      INITIALIZATION FUNCTIION
// STEP 1,2 & 3 are to be executed in this function ### 
static int __init mychar_init(void) 
{
	printk(KERN_INFO "Namaste: myadc driver registered");
	
	// STEP 1 : reserve <major, minor>
	if (alloc_chrdev_region(&first, 0, 1, "Nilankan") < 0)
	{
		return -1;
	}
	
	// STEP 2 : dynamically create device node in /dev directory
    if ((cls = class_create(THIS_MODULE, "chardrv")) == NULL)
	{
		unregister_chrdev_region(first, 1);
		return -1;
	}
    if (device_create(cls, NULL, first, NULL, "adc8") == NULL)
	{
		class_destroy(cls);
		unregister_chrdev_region(first, 1);
		return -1;
	}
	
	// STEP 3 : Link fops and cdev to device node
    cdev_init(&c_dev, &fops);
    if (cdev_add(&c_dev, first, 1) == -1)
	{
		device_destroy(cls, first);
		class_destroy(cls);
		unregister_chrdev_region(first, 1);
		return -1;
	}
	return 0;
}
 
static void __exit mychar_exit(void) 
{
	cdev_del(&c_dev);
	device_destroy(cls, first);
	class_destroy(cls);
	unregister_chrdev_region(first, 1);
	printk(KERN_INFO "Bye: myadc driver unregistered\n\n");
}
 
module_init(mychar_init);
module_exit(mychar_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("");
MODULE_DESCRIPTION("Our First Character Driver");



