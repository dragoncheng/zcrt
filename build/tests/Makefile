##############################################################################
#				copyleft @ 2010 2011 Late Lee
# file name:Makefile
# A simple Makefile for device driver by Late Lee from www.latelee.org
# based on LDD3 and other guys works.

# 
# note:
#      You need to change your module name & obj file(s),and you may
#      also need to change 'KERNELDIR'.
#      I hope you can see your module(sth like xx.ko) if you are lucky enough.
##############################################################################

# debug or not
#DEBUG = y
ifeq ($(DEBUG), y)
		DEBFLAGS = -O -g
else
		DEBFLAGS = -O1
endif

EXTRA_CFLAGS += $(DEBFLAGS) -D__KERNAL__
EXTRA_CFLAGS += -I$(INCDIR)

ifeq ($(ARCH),powerpc)
	KERNELDIR := /opt/ppc_kernal
else
	KERNELDIR := /lib/modules/$(shell uname -r)/build
endif

########## change your module name here
MODULE = testzcrt

# obj-m => module
# obj-y => kernel
# foo.o -> foo.ko
########## change your obj file(s) here
$(MODULE)-objs := test_ptzcrt.o test_api_impl.o test_api_decode.o ../../zcrtlib/zarraylist.o ../../zcrtlib/zbuf.o ../../zcrtlib/zcmm.o ../../zcrtlib/zevent.o ../../zcrtlib/zhash.o ../../zcrtlib/zlinklist.o ../../zcrtlib/zlog.o ../../zcrtlib/zmodule.o ../../zcrtlib/zschedule.o ../../zcrtlib/ztimer.o ../../zcrtlib/zcrt_os_kernal.o ../../zcrtlib/zcrt_mem_kernal.o ../../zcrtlib/zstr.o ../../zipc/zipc_parse.o ../../zipc/zipc_server.o ../../zipc/zipc_server_kernal.o ../../zipc/zipc_tlv.o

ifneq ($(KERNELRELEASE), )
		obj-m := $(MODULE).o
else
		PWD := $(shell pwd)

all:
		$(MAKE_BEGIN)
		@echo 
		@if \
		$(MAKE) -C $(KERNELDIR) M=$(PWD) INCDIR=$(PWD)/../../ modules;\
		 #$(MAKE) -C $(KERNELDIR) M=$(PWD) modules;\
		then $(MAKE_DONE);\
		else \
		$(MAKE_ERR);\
		exit 1; \
		fi
endif

clean:
		$(CLEAN_BEGIN)
		rm -rf *.cmd *.o *.ko *.mod.c *.symvers *.order *.markers \
		.tmp_versions .*.cmd *~ .*.d *.ko.unsigned
		rm -rf ../../zcrtlib/*.o ../../zcrtlib/*.mod.c ../../zcrtlib/*.ko ../../zcrtlib/.*.cmd
		rm -rf ../../zipc/*.o ../../zipc/*.mod.c ../../zipc/*.ko ../../zipc/.*.cmd
		rm -rf ../.tmp_versions
		$(CLEAN_END)

install:
		@echo -e "$(COLOR3) Note:"
		@echo -e "To install or not install,that is a question.$(RESET)"

modules:
		@echo -e "$(COLOR3)Do not need to do this. Just rnu 'make'. $(RESET)"

modules_install:
		@echo -e "$(COLOR3)Do not need to do this.$(RESET)"
love:
		@echo -e "$(COLOR3)To make or not to make, that is a question.$(RESET)"

.PHONY:all clean install love modules modules_install
#xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
### nothing
OFFSET=\x1b[21G    # 21 col
COLOR1=\x1b[0;32m  # all --> green
COLOR2=\x1b[1;35m  # clean --> pink
COLOR3=\x1b[1;31m  # error --> red
RESET=\x1b[0m

CLEAN_BEGIN=@echo -e "$(OFFSET)$(COLOR2)Cleaning up ...$(RESET)"
CLEAN_END=@echo -e "$(OFFSET)$(COLOR2)[Done.]$(RESET)"

MAKE_BEGIN=@echo -ne "$(OFFSET)$(COLOR1)Compiling ...$(RESET)"
### I do not forget "@", but it DOES NOT need "@"
MAKE_DONE=echo -e "$(OFFSET)$(COLOR1)[Job done!]$(RESET)"
MAKE_ERR=echo -e "$(OFFSET)$(COLOR3)[Oops!Error occurred]$(RESET)"
### nothing end here

############# Makefile end here


