CFLAGS=-I../Splinter --wfo
TARGET=Scratch.dsk
SCRATCH_BIN=Scratch.bin
MESS_DIR=~/Applications/MacSDLMESS/mame0163
MESS=$(MESS_DIR)/mess64

$(TARGET) : $(SCRATCH_BIN)
	rm -f $@
	decb dskini $@
	decb copy $(SCRATCH_BIN) $@,SCRATCH.BIN
	decb attr $@,SCRATCH.BIN -2 -b

%.bin : %.c
	cmoc $(CFLAGS) $< 

clean :
	rm -rf $(TARGET) *.bin *.i *.lst *.asm *.hex

debug : $(TARGET)
	$(MESS) coco3 -rompath $(MESS_DIR)/roms -window -flop1 $(TARGET) -cfg_directory ../cfgs/rgb

debug_cmp : $(TARGET)
	$(MESS) coco3 -rompath $(MESS_DIR)/roms -window -flop1 $(TARGET) -cfg_directory ../cfgs/cmp


