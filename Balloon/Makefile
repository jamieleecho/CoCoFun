CFLAGS=
TARGET=Balloon.dsk
BALLOON_BAS=Balloon.bas
BALLOON_BIN=Balloon.bin
MESS_DIR=~/Applications/MacSDLMESS/mame0163
MESS=$(MESS_DIR)/mess64

$(TARGET) : $(BALLOON_BAS) $(BALLOON_BIN)
	rm -f $@
	decb dskini $@
	decb copy $(BALLOON_BAS) $@,"*.BAS" -t
	decb attr $@,"*.BAS" -0
	decb copy $(BALLOON_BAS) $@,BALLOON.BAS -t
	decb attr $@,BALLOON.BAS -0
	decb copy $(BALLOON_BIN) $@,BALLOON.BIN
	decb attr $@,BALLOON.BIN -2 -b
	decb copy Balloonr.bin $@,BALLOONR.BIN
	decb attr $@,BALLOONR.BIN -2 -b
	decb copy Ballon.bin $@,BALLON.BIN
	decb attr $@,BALLON.BIN -2 -b
	decb copy Binary1.fnt $@,BINARY1.FNT
	decb attr $@,BINARY1.FNT -2 -b
	decb copy Scene1.bin $@,SCENE1.BIN
	decb attr $@,SCENE1.BIN -2 -b

$(BALLOON_BIN) : Balloon.c CoCoMisc.c
	cmoc $(CFLAGS) $<

%.bin : %.c
	cmoc $(CFLAGS) $< 

clean :
	rm -rf $(TARGET) $(BALLOON_BIN) *.i *.lst *.asm *.hex

run : $(TARGET)
	$(MESS) coco3 -rompath $(MESS_DIR)/roms -window -flop1 $(TARGET) -cfg_directory ../cfgs/rgb

run_cmp : $(TARGET)
	$(MESS) coco3 -rompath $(MESS_DIR)/roms -window -flop1 $(TARGET) -cfg_directory ../cfgs/cmp

debug : $(TARGET)
	$(MESS) coco3 -rompath $(MESS_DIR)/roms -window -flop1 $(TARGET) -cfg_directory ../cfgs/rgb -debug

debug_cmp : $(TARGET)
	$(MESS) coco3 -rompath $(MESS_DIR)/roms -window -flop1 $(TARGET) -cfg_directory ../cfgs/cmp -debug


