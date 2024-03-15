#include <linux/init.h>
#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <linux/slab.h>

#define MAX_SIZE 1024
#define DEVICE_NAME "Willdev"

static ssize_t mychardev_read(struct file *file, char __user *buf, size_t count, loff_t *offset);
static ssize_t mychardev_write(struct file *file, const char __user *buf, size_t count, loff_t *offset);
static loff_t mychardev_llseek(struct file *file, loff_t offset, int whence); 

//struct containing the listing of file operations of the driver module.
static const struct file_operations mychardev_fops = {
	.owner      = THIS_MODULE,
	.read       = mychardev_read,
	.write       = mychardev_write,
	.llseek		=mychardev_llseek,
};

struct mychar_device_data {
	struct cdev cdev;
};

static int dev_major = 0;
static char *local_buff;
static int data_size = MAX_SIZE;
/*
static int mychardev_uevent(struct device *dev, struct kobj_uevent_env *env)
{
	add_uevent_var(env, "DEVMODE=%#o", 0666);
	return 0;
}
*/
//use insmod "device name" to call this one.
static int __init mychardev_init(void)
{
	dev_major = register_chrdev(0 , DEVICE_NAME, &mychardev_fops);

	if (dev_major < 0) {
		printk(KERN_ALERT "Registering char device failed with %d\n", dev_major);
		return dev_major;
	}
	printk(KERN_INFO"device major #: %d\n",dev_major);
	local_buff = kzalloc(MAX_SIZE, GFP_KERNEL);
	return 0;
}

//run rmmod "driver name" to execute this command.
static void __exit mychardev_exit(void)
{
	unregister_chrdev(dev_major, DEVICE_NAME);
	kfree(local_buff);//release kernel's "heap" memory.
	printk(KERN_INFO "kernel module has been released");

}

static ssize_t mychardev_read(struct file *file, char __user *buf, size_t count, loff_t *offset)
{
	size_t datalen = strlen(local_buff);

	if (count > datalen) {
		count = datalen;
	}

	if (copy_to_user(buf, local_buff, count)) {
		return -EFAULT;
	}

	return count;
}

static loff_t mychardev_llseek(struct file *file, loff_t offset, int whence){

	loff_t position;

	switch(whence){
		case 0:
			position = offset;
			break;
		case 1:
			position = file->f_pos +offset;
			break;
		case 2:
			position = data_size+offset;
			break;
		default:
			return -EINVAL;
	}

	if(position <0 || position > data_size){
		return -EINVAL;
	}
	file->f_pos = position;
	return position;
}

static ssize_t mychardev_write(struct file *file, const char __user *buf, size_t count, loff_t *offset)
{
	size_t ncopied;

	if (count > MAX_SIZE) {
		count = MAX_SIZE;
	}

	ncopied = copy_from_user(local_buff, buf, count);

	if (ncopied == 0) {
		printk("Copied %zd bytes from the user\n", count);
	} else {
		printk("Could't copy %zd bytes from the user\n", ncopied);
	}
	//forces a terminating null as the first character after char string.
	local_buff[count] = 0;

	printk("Data from the user: %s\n", local_buff);

	return count;
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("William Newby");

module_init(mychardev_init);
module_exit(mychardev_exit);
