CFLAGS=
TARGET=Bouncy.dsk
RENDER_BIN=render1.bin
MESS_DIR=~/Applications/MacSDLMESS/mame0163
MESS=$(MESS_DIR)/mess64

$(TARGET) : $(RENDER_BIN)
	rm -f $@
	decb dskini $@
	decb copy $(RENDER_BIN) $@,RENDER1.BIN
	decb attr $@,RENDER1.BIN -2 -b
	decb copy RENDER1.BAS $@,"*.BAS" -t
	decb attr $@,"*.BAS" -0

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

