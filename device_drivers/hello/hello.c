/**
 * @file    hello.c
 * @author  Rajmani Arya
 * Compile:
 * $ make # creates several files
 * $ sudo insmod hello.ko # insert module
 * $ dmesg | tail -n 1  # to see kernel message
 * $ sudo rmmod hello.ko # remove module
 * $ dmesg | tail -n 1 
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Rajmani Arya");
MODULE_DESCRIPTION("A simple Linux driver for welcome to LKM");
MODULE_VERSION("0.1");

static int __init hello_init(void){
   printk(KERN_INFO "Raj Arya, Welcome to Linux Kernel Module Programming.\n");
   return 0;
}

static void __exit hello_exit(void){
   printk(KERN_INFO "Raj Arya, Goodbye !\n");
}

module_init(hello_init);
module_exit(hello_exit);
