ifneq ($(KERNELRELEASE),)
obj-m := sys.o
sys-objs :=  main.o
else
KERNELDIR := /usr/src/kernels/3.10.0-693.11.1.el7.x86_64/
all:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
clean:
	rm -rf *.ko* *.o *.mod.* Module.symvers modules.order .*.cmd .tmp_versions
endif

