CFLAGS=
TARGET=Demo.dsk
DEMO_BAS=Demo.bas
DEMO_BIN=Demo.bin
MESS_DIR=~/Applications/MacSDLMESS/mame0163
MESS=$(MESS_DIR)/mess64

$(TARGET) : $(DEMO_BIN)
	rm -f $@
	decb dskini $@
#	decb copy $(DEMO_BAS) $@,"*.BAS" -t
#	decb attr $@,"*.BAS" -0
#	decb copy $(DEMO_BAS) $@,"DEMO.BAS" -t
#	decb attr $@,"DEMO.BAS" -0
	decb copy $(DEMO_BIN) $@,DEMO.BIN
	decb attr $@,DEMO.BIN -2 -b

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

