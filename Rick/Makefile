CFLAGS=-O2
LDFLAGS=--org=0x5000 --limit=0x7fff
TARGET=Rick.dsk
TARGET_BAS=$(TARGET:.dsk=.bas)
TARGET_BIN=$(TARGET:.dsk=.bin)
TARGET_C=$(TARGET:.dsk=.c)

MESS_DIR=~/Applications/mame
MESS=$(MESS_DIR)/mame64

C_SOURCES=$(wildcard *.c)
C_OBJS=$(patsubst %.c,%.o,$(C_SOURCES))

$(TARGET) : $(TARGET_BAS) $(TARGET_BIN)
	rm -f $@
	decb dskini $@
	decb copy $(TARGET_BAS) $@,"*.BAS" -t
	decb attr $@,"*.BAS" -0
	decb copy $(TARGET_BAS) $@,$(shell echo $(TARGET_BAS) | tr a-z A-Z) -t
	decb attr $@,$(shell echo $(TARGET_BAS) | tr a-z A-Z) -0
	decb copy $(TARGET_BIN) $@,$(shell echo $(TARGET_BIN) | tr a-z A-Z)
	decb attr $@,$(shell echo $(TARGET_BIN) | tr a-z A-Z) -2 -b
	decb copy $(RICK_BAS) $@,RICK.BAS -t
	decb attr $@,RICK.BAS -0
	decb copy $(RICK_BIN) $@,RICK.BIN
	decb attr $@,RICK.BIN -2 -b
	decb copy Guy.bin $@,GUY.BIN
	decb attr $@,Guy.BIN -2 -b
	decb copy Pix.bin $@,PIX.BIN
	decb attr $@,PIX.BIN -2 -b


$(TARGET_BIN) : $(C_OBJS)
	cmoc $(CFLAGS) $(LDFLAGS) -o $@ $^

%.o : %.c
	cmoc $(CFLAGS) -c $<

clean :
	rm -rf $(TARGET) $(C_OBJS) $(TARGET_BIN) *.i *.lst *.asm *.hex

run : $(TARGET)
	$(MESS) coco3 -rompath $(MESS_DIR)/roms -window -flop1 $(TARGET) -cfg_directory ../cfgs/rgb

run_cmp : $(TARGET)
	$(MESS) coco3 -rompath $(MESS_DIR)/roms -window -flop1 $(TARGET) -cfg_directory ../cfgs/cmp

debug : $(TARGET)
	$(MESS) coco3 -rompath $(MESS_DIR)/roms -window -flop1 $(TARGET) -cfg_directory ../cfgs/rgb -debug

debug_cmp : $(TARGET)
	$(MESS) coco3 -rompath $(MESS_DIR)/roms -window -flop1 $(TARGET) -cfg_directory ../cfgs/cmp -debug


