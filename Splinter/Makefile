CFLAGS=--wfo
TARGET=Splinter.dsk
SPLINTER_BAS=Splinter.bas
SPLINTER_BIN=Splinter.bin
MESS_DIR=~/Applications/mame
MESS=$(MESS_DIR)/mess64

$(TARGET) : $(SPLINTER_BAS) $(SPLINTER_BIN)
	rm -f $@
	decb dskini $@
	decb copy $(SPLINTER_BAS) $@,"*.BAS" -t
	decb attr $@,"*.BAS" -0
	decb copy $(SPLINTER_BAS) $@,SPLINTER.BAS -t
	decb attr $@,SPLINTER.BAS -0
	decb copy $(SPLINTER_BIN) $@,SPLINTER.BIN
	decb attr $@,SPLINTER.BIN -2 -b

$(SPLINTER_BIN) : Splinter.c GrafxData.c SplinterScore.c SplinterBall.c Blitter.c Bricks.c FontData.c SplinterBall.c CoCoMisc.c Sound.c FixedPoint.c Vector2d.c
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


