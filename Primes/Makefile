CFLAGS=-O2
LDFLAGS=--org=0x3000 --limit=0x7fff
TARGET=Primes.dsk
TARGET_BIN=$(TARGET:.dsk=.bin)
TARGET_C=$(TARGET:.dsk=.c)

MESS_DIR=~/Applications/mame
MESS=$(MESS_DIR)/mame64

C_SOURCES=$(wildcard *.c)
C_OBJS=$(patsubst %.c,%.o,$(C_SOURCES))

$(TARGET) : $(TARGET_BIN)
	rm -f $@
	decb dskini $@
	decb copy $(TARGET_BIN) $@,$(shell echo $(TARGET_BIN) | tr a-z A-Z)
	decb attr $@,$(shell echo $(TARGET_BIN) | tr a-z A-Z) -2 -b

$(TARGET_BIN) : $(C_OBJS)
	cmoc $(CFLAGS) $(LDFLAGS) -o $@ $^

%.o : %.c
	cmoc $(CFLAGS) -c $<

clean :
	rm -rf $(TARGET) $(C_OBJS) $(TARGET_BIN) *.i *.lst *.asm *.hex *.s *.map *.link

run : $(TARGET)
	$(MESS) coco3 -rompath $(MESS_DIR)/roms -window -flop1 $(TARGET) -cfg_directory ../cfgs/rgb

run_cmp : $(TARGET)
	$(MESS) coco3 -rompath $(MESS_DIR)/roms -window -flop1 $(TARGET) -cfg_directory ../cfgs/cmp

debug : $(TARGET)
	$(MESS) coco3 -rompath $(MESS_DIR)/roms -window -flop1 $(TARGET) -cfg_directory ../cfgs/rgb -debug

debug_cmp : $(TARGET)
	$(MESS) coco3 -rompath $(MESS_DIR)/roms -window -flop1 $(TARGET) -cfg_directory ../cfgs/cmp -debug


