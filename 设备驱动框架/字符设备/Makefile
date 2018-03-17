ifneq ($(KERNELRELEASE),)
obj-m := memdev.o
//cdev-objs :=  memdev.o
else
KERNELDIR := /usr/src/kernels/3.10.0-693.11.1.el7.x86_64/
all:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
	gcc app.c -oapp
clean:
	rm -rf *.ko* *.o *.mod.* Module.symvers modules.order .*.cmd .tmp_versions
	rm -rf app
endif

