ifneq ($(KERNELRELEASE),)
obj-m := hello_platform.o
else
PWD := $(shell pwd)
KDIR := /home/daniel/raspberry/linux

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules  ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf-

PHONY: clean
clean:
	rm -rf *.cmd *.ko *.o *.order *.symvers *.mod.c
endif