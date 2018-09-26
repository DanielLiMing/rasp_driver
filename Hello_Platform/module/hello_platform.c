/*
 * @Author: Daniel
 * @Date: 2018-09-25 22:20:48
 * @LastEditors: Daniel
 * @LastEditTime: 2018-09-25 23:03:34
 * @Description: 
 * @Email: DanielLiMing@163.com
 * @Company: None
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>

#define DEVICE_NAME     "hello"
#define DRIVER_NAME     "hello"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Daniel");

static int hello_open(struct inode *inode,struct file *file)
{
    printk("Hello world open\r\n");
    return 0;
}

static long hello_ioctl(struct file *file, unsigned int cmd,unsigned long arg)
{
    printk("hello ioctl,cmd=%d, arg = %ld\r\n",cmd,arg);
    return 0;
}
static int hello_release(struct inode *inode, struct file *file)
{
    printk("hello released\r\n");
    return 0;
}

static struct file_operations hello_fops = {
    .owner = THIS_MODULE,
    .open = hello_open,
    .unlocked_ioctl = hello_ioctl,
    .release = hello_release,
};

static struct miscdevice misc_dev = {
    .minor = MISC_DYNAMIC_MINOR,
    .name  = DEVICE_NAME,
    .fops  = &hello_fops,
};

static int hello_probe(struct platform_device *pdev)
{
    printk("hello probe\r\n");
    misc_register(&misc_dev);
    return 0;
}

static int hello_remove(struct platform_device *pdev)
{
    printk("hello removed\r\n");
    return 0;
}
static void hello_shutdown(struct platform_device *pdev)
{
    printk("hello shutdown\r\n");
}

static int hello_suspend(struct platform_device *pdev,pm_message_t pmt)
{
    printk("hello suspend\r\n");
    return 0;
}
static int hello_resume(struct platform_device *pdev)
{
    printk("hello resume\r\n");
    return 0;
}

static struct platform_driver hello_driver = {
    .probe = hello_probe,
    .remove = hello_remove,
    .shutdown = hello_shutdown,
    .suspend = hello_suspend,
    .resume = hello_resume,
    .driver = {
        .name = DRIVER_NAME,
        .owner = THIS_MODULE,
    }
};

static int hello_init(void)
{
    int drv_stat;
    printk("hello init\r\n");
    drv_stat = platform_driver_register(&hello_driver);
    printk("platform driver states:%d\r\n",drv_stat);
    platform_device_register_simple(DRIVER_NAME,-1,NULL,0);
    printk("platform device register simple end\r\n");
    return 0;
}

static void hello_exit(void)
{
    platform_driver_unregister(&hello_driver);
    printk("hello module removed\r\n");
}

module_init(hello_init);
module_exit(hello_exit);




