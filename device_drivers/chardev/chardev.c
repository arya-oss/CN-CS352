#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

#include <linux/mutex.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <asm/uaccess.h>

#define BUFSIZE 256
#define DEVICE_NAME "arya_char"
#define CLASS_NAME "arya"

static DEFINE_MUTEX(arya_mutex); 
static int major;
static char message[BUFSIZE] = {0};
static int msg_length;
static int numOpens=0;
static struct class* aryaClass = NULL;
static struct device* aryaDevice = NULL;

int device_open(struct inode *pinode, struct file *pfile) {
	if(!mutex_trylock(&arya_mutex)){
		printk(KERN_ALERT "arya_char: Device in use by another process");
		return -EBUSY;
	}
	numOpens++;
	printk(KERN_INFO "arya_char: Device has been opened %d times.\n", numOpens);
	return 0;
}

ssize_t device_read(struct file *pfile, char __user *buffer, size_t length, loff_t *offset) {
	int err_count = 0;
	err_count = copy_to_user(buffer, message, msg_length);

	if (err_count==0){
		printk(KERN_INFO "arya_char: Sent %d characters to the user\n", msg_length);
		return 0;
	}
	else {
		printk(KERN_INFO "arya_char: Failed to send %d characters to the user\n", err_count);
		return -EFAULT;
	}
}

ssize_t device_write(struct file *pfile, char __user *buffer, size_t length, loff_t *offset) {
	sprintf(message, "%s(%d letters)", buffer, (int)length);
	msg_length = strlen(message);
	printk(KERN_INFO "arya_char: Received %d characters from the user\n", length);
	return length;
}

int device_close(struct inode *pinode, struct file *pfile) {
	mutex_unlock(&arya_mutex);
	printk(KERN_INFO "arya_char: Device successfully closed.\n");
	return 0;
}

struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = device_open,
	.read = device_read,
	.write = device_write,
	.release = device_close,
};

static int __init device_init(void) {
	mutex_init(&arya_mutex);
	printk(KERN_INFO "arya_char: Initalizing the Char Device.");
	major = register_chrdev(0, DEVICE_NAME, &fops);
	if(major < 0) {
		printk(KERN_ALERT "arya_char: failed to register major number.\n");
		return major;
	}
	printk(KERN_INFO "arya_char: correctly registered with a major number\n");

	aryaClass = class_create(THIS_MODULE, CLASS_NAME);
	if(IS_ERR(aryaClass)) {
		unregister_chrdev(major, DEVICE_NAME);
		printk(KERN_ALERT "arya_char: Failed to register a device class\n");
		return PTR_ERR(aryaClass);
	}
	printk(KERN_INFO "arya_char: class registered successfully\n");
	aryaDevice = device_create(aryaClass, NULL, MKDEV(major, 0), NULL, DEVICE_NAME);
	if(IS_ERR(aryaDevice)) {
		class_destroy(aryaClass);
		unregister_chrdev(major, DEVICE_NAME);
		printk(KERN_ALERT "arya_char: Failed to create a device\n");
		return PTR_ERR(aryaDevice);
	}
	printk(KERN_INFO "arya_char: device created successfully\n");

}

static void __exit device_exit(void) {
	device_destroy(aryaClass, MKDEV(major, 0));
	class_unregister(aryaClass);
	class_destroy(aryaClass);
	unregister_chrdev(major, DEVICE_NAME);
	mutex_destroy(&arya_mutex);
	printk(KERN_INFO "arya: Bye from Kernel.\n");
}

module_init(device_init);
module_exit(device_exit);

MODULE_AUTHOR("Rajmani Arya");
MODULE_DESCRIPTION("A simple character device");
MODULE_LICENSE("GPL");
MODULE_VERSION("0.1");