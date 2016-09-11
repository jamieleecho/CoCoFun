CFLAGS=-O2
TARGET=problems.dsk
PROBLEMS_BIN=problems.bin
MESS_DIR=~/Applications/mame
MESS=$(MESS_DIR)/mess64

$(TARGET) : $(PROBLEMS_BIN)
	rm -f $@
	decb dskini $@
	decb copy $(PROBLEMS_BIN) $@,PROBLEMS.BIN
	decb attr $@,PROBLEMS.BIN -2 -b

$(PROBLEMS_BIN) : problems.c
	cmoc $(CFLAGS) $< 

%.bin : %.c
	cmoc $(CFLAGS) $< 

clean :
	rm -rf $(TARGET) *.bin *.i *.lst *.asm *.hex

run : $(TARGET)
	$(MESS) coco3 -rompath $(MESS_DIR)/roms -window -flop1 $(TARGET) -cfg_directory ../cfgs/rgb

run_cmp : $(TARGET)
	$(MESS) coco3 -rompath $(MESS_DIR)/roms -window -flop1 $(TARGET) -cfg_directory ../cfgs/cmp

debug : $(TARGET)
	$(MESS) coco3 -rompath $(MESS_DIR)/roms -window -flop1 $(TARGET) -cfg_directory ../cfgs/rgb -debug

debug_cmp : $(TARGET)
	$(MESS) coco3 -rompath $(MESS_DIR)/roms -window -flop1 $(TARGET) -cfg_directory ../cfgs/cmp -debug


