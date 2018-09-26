ifneq ($(KERNELRELEASE),)
obj-m := led_drv.o
else
PWD := $(shell pwd)
KDIR := /home/daniel/raspberry/linux

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf-

PHONY: clean
clean:
	rm -rf *.cmd *.o *.mod.c *.ko *.symvers *.order *.tem_versions
endif