/*
 * @Author: Daniel
 * @Date: 2018-09-23 18:21:51
 * @LastEditors: Daniel
 * @LastEditTime: 2018-09-23 18:52:49
 * @Description: 
 * @Email: 111111111111
 */



#include <linux/miscdevice.h>
#include <linux/delay.h>
#include <asm/irq.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/mm.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <linux/delay.h>
#include <linux/moduleparam.h>
#include <linux/slab.h>
#include <linux/errno.h>
#include <linux/ioctl.h>
#include <linux/cdev.h>
#include <linux/string.h>
#include <linux/list.h>
#include <linux/pci.h>
#include <asm/uaccess.h>
#include <asm/atomic.h>
#include <asm/unistd.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include <linux/ioport.h>

#define PIN 26

#define BCM2835_GPIO_BASE 0x3f200000
#define BCM2835_GPSET0 0x001c
#define BCM2835_GPFSEL0 0x0000
#define BCM2835_GPCLR0 0x0028

#define BCM2835_GPIO_FSEL_OUT 1

int open_state = 0;

static int bcm2835_gpio_fsel(uint8_t pin, uint8_t mode)
{
    volatile uint32_t *bcm2835_gpio = (volatile uint32_t *)ioremap(BCM2835_GPIO_BASE, 16);
    volatile uint32_t *bcm2835_gpio_fsel = bcm2835_gpio + BCM2835_GPFSEL0/4 + pin/10;
    uint8_t shift = (pin % 10) * 3;
    uint8_t value = mode << shift;
    *bcm2835_gpio_fsel = *bcm2835_gpio_fsel | value;
    printk("fsel address: 0x%lx : %x\n", (long unsigned int)bcm2835_gpio_fsel,*bcm2835_gpio_fsel);
    return 0;
}

static int bcm2835_gpio_set(uint8_t pin)
{
    volatile uint32_t *bcm2835_gpio = (volatile uint32_t *)ioremap(BCM2835_GPIO_BASE, 16);
    volatile uint32_t *bcm2835_gpio_set = bcm2835_gpio + BCM2835_GPSET0/4 + pin/32;
    uint8_t shift = (pin % 32);
    uint8_t value = 1 << shift;
    *bcm2835_gpio_set = *bcm2835_gpio_set | value;
    printk("set address: 0x%lx : %x\n", (long unsigned int)bcm2835_gpio_set, *bcm2835_gpio_set);
    return 0;
}

static int bcm2835_gpio_clr(uint8_t pin)
{
    volatile uint32_t *bcm2835_gpio = (volatile uint32_t *)ioremap(BCM2835_GPIO_BASE, 16);
    volatile uint32_t *bcm2835_gpio_clr = bcm2835_gpio + BCM2835_GPCLR0/4 + pin/32;
    uint8_t shift = pin % 32;
    uint8_t value = 1 << shift;
    *bcm2835_gpio_clr = *bcm2835_gpio_clr | value;
    printk("clr address: 0x%lx : %x\r\n", (long unsigned int)bcm2835_gpio_clr, *bcm2835_gpio_clr);
    return 0;
}

static int leds_open(struct inode *inode,struct file *file)
{
    if(open_state == 0)
    {
        open_state = 1;
        printk("open file success\r\n");
        return 0;
    }
    else
    {
        printk("file has opened\r\n");
        return -1;
    }
}

static long leds_ioctl(struct file *file,unsigned int cmd,unsigned long arg)
{
    switch(cmd)
    {
        case 0:
            bcm2835_gpio_clr(PIN);
            printk("LED OFF\r\n");
            break;
        case 1:
            bcm2835_gpio_set(PIN);
            printk("LED ON!\r\n");
            break;
        default:
            return -EINVAL;
    }
    return 0;
}

static int leds_release(struct inode *inode,struct file *file)
{
    if(open_state == 1)
    {
        open_state = 0;
        printk("Close File Success\r\n");
        return 0;
    }
    else
    {
        printk("The File Has Been Closed\r\n");
        return -1;
    }
}

static const struct file_operations leds_fops = {
    .owner = THIS_MODULE,
    .open = leds_open,
    .unlocked_ioctl = leds_ioctl,
    .release = leds_release,
};

static struct miscdevice misc = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = "my leds",
    .fops = &leds_fops,
};
static int __init leds_init(void)
{
    int ret;
    ret = misc_register(&misc);
    bcm2835_gpio_fsel(PIN,BCM2835_GPIO_FSEL_OUT);
    bcm2835_gpio_set(PIN);
    printk("my leds init\r\n");
    return 0;
}

static void __exit leds_exit(void)
{
    bcm2835_gpio_clr(PIN);
    misc_deregister(&misc);
    printk("my leds exit\r\n");

}
module_init(leds_init);
module_exit(leds_exit);

MODULE_AUTHOR("Daniel");
MODULE_LICENSE("GPL");
