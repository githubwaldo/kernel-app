ifneq ($(KERNELRELEASE),)
obj-m := ramdisk.o
//ramdisk-objs :=  ramdisk.o
else
KERNELDIR := /usr/src/kernels/3.10.0-693.11.1.el7.x86_64/
all:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
clean:
	rm -rf *.ko* *.o *.mod.* Module.symvers modules.order .*.cmd .tmp_versions
endif

