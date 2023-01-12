# normal makefile
KDIR ?= /home/repos/linux-stable

default:
	$(MAKE) -C $(KDIR) M=$$PWD
	$(CROSS_COMPILE)strip -g hello.ko
clean:
	$(MAKE) -C $(KDIR) M=$$PWD clean
