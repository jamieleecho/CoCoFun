CFLAGS=
TARGET=Breakout.dsk
BREAKOUT_BAS=Breakout.bas
BREAKOUT_BIN=Breakout.bin
MESS_DIR=~/Applications/MacSDLMESS/mame0163
MESS=$(MESS_DIR)/mess64

$(TARGET) : $(BREAKOUT_BAS) $(BREAKOUT_BIN)
	rm -f $@
	decb dskini $@
	decb copy $(BREAKOUT_BAS) $@,BREAKOUT.BAS -t
	decb attr $@,BREAKOUT.BAS -0
	decb copy $(BREAKOUT_BIN) $@,BREAKOUT.BIN
	decb attr $@,BREAKOUT.BIN -2 -b

$(BREAKOUT_BIN) : Breakout.c GrafxData.c BreakoutScore.c BreakoutBall.c Blitter.c Bricks.c FontData.c BreakoutBall.c CoCoMisc.c Sound.c FixedPoint.c
	cmoc $(CFLAGS) $< 

%.bin : %.c
	cmoc $(CFLAGS) $< 

clean :
	rm -rf $(TARGET) *.bin *.i *.lst *.asm *.hex

debug : $(TARGET)
	$(MESS) coco3 -rompath $(MESS_DIR)/roms -window -flop1 $(TARGET) -cfg_directory ../cfgs/rgb

debug_cmp : $(TARGET)
	$(MESS) coco3 -rompath $(MESS_DIR)/roms -window -flop1 $(TARGET) -cfg_directory ../cfgs/cmp


