----------------------------------------------------------------------------------
	A simple character device for Linux Kernel
for detail explanation
@see http://derekmolloy.ie/writing-a-linux-kernel-module-part-2-a-character-device/

Author: Rajmani Arya
==================================================================================

compile:
$ make

it will create several files but we need chardev.ko only
now insert module to kernel

$ sudo insmod chardev.ko
to check chardevice inserted
$ ls -l /dev/chardev
crw------- 1 root root 244, 0 Mar 24 12:22 /dev/arya_char

$ echo "hello linux kernel" >> /dev/arya_char
To See Kernel log

$ dmesg | tail

