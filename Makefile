
TARGET = iopcvm.elf

CFLAGS += -I$(SDKSTAGE)/usr/include/
CFLAGS += -I$(SDKSTAGE)/usr/include/iopccommon
CFLAGS += -I$(SDKSTAGE)/usr/include/lxc

LDFLAGS += -L$(SDKSTAGE)/usr/lib/ 
LDFLAGS += -lc -lpthread 
LDFLAGS += -llxc

SRC = 
#SRC += main.c
SRC += lxc_start.c

include Makefile.include.app
