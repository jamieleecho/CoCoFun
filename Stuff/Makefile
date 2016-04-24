CFLAGS=
TARGET=Stuff.dsk
STUFF_BAS=Stuff.bas
STUFF_BIN=Stuff.bin
MESS_DIR=~/Applications/MacSDLMESS/mame0163
MESS=$(MESS_DIR)/mess64

$(TARGET) : $(STUFF_BIN)
	rm -f $@
	decb dskini $@
#	decb copy $(STUFF_BAS) $@,"*.BAS" -t
#	decb attr $@,"*.BAS" -0
#	decb copy $(STUFF_BAS) $@,"STUFF.BAS" -t
#	decb attr $@,"STUFF.BAS" -0
	decb copy $(STUFF_BIN) $@,STUFF.BIN
	decb attr $@,STUFF.BIN -2 -b

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

