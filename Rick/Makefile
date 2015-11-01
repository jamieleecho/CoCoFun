CFLAGS=
TARGET=Rick.dsk
RICK_BAS=Rick.bas
RICK_BIN=Rick.bin
MESS_DIR=~/Applications/MacSDLMESS/mame0163
MESS=$(MESS_DIR)/mess64

$(TARGET) : $(RICK_BAS) $(RICK_BIN)
	rm -f $@
	decb dskini $@
	decb copy $(RICK_BAS) $@,"*.BAS" -t
	decb attr $@,"*.BAS" -0
	decb copy $(RICK_BAS) $@,RICK.BAS -t
	decb attr $@,RICK.BAS -0
	decb copy $(RICK_BIN) $@,RICK.BIN
	decb attr $@,RICK.BIN -2 -b
	decb copy Guy.bin $@,GUY.BIN
	decb attr $@,Guy.BIN -2 -b
	decb copy Pix.bin $@,PIX.BIN
	decb attr $@,PIX.BIN -2 -b

$(RICK_BIN) : Rick.c 
	cmoc $(CFLAGS) $< 

%.bin : %.c
	cmoc $(CFLAGS) $< 

clean :
	rm -rf $(TARGET) $(RICK_BIN) *.i *.lst Rick.asm *.hex

run : $(TARGET)
	$(MESS) coco3 -rompath $(MESS_DIR)/roms -window -flop1 $(TARGET) -cfg_directory ../cfgs/rgb

run_cmp : $(TARGET)
	$(MESS) coco3 -rompath $(MESS_DIR)/roms -window -flop1 $(TARGET) -cfg_directory ../cfgs/cmp

debug : $(TARGET)
	$(MESS) coco3 -rompath $(MESS_DIR)/roms -window -flop1 $(TARGET) -cfg_directory ../cfgs/rgb -debug

debug_cmp : $(TARGET)
	$(MESS) coco3 -rompath $(MESS_DIR)/roms -window -flop1 $(TARGET) -cfg_directory ../cfgs/cmp -debug



