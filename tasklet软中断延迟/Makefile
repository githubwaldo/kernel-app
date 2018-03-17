ifneq ($(KERNELRELEASE),)
obj-m := tasklet.o
tasklet-objs :=  main.o
else
KERNELDIR := /usr/src/kernels/2.6.32-642.15.1.el6.x86_64
all:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
clean:
	rm -rf *.ko *.o *.mod.* *.symvers *.order *.ko.* .tmp_versions .*.cmd
endif
