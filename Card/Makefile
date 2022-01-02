TARGET=Card.dsk
TARGET_BAS=$(TARGET:.dsk=.bas)

MESS_DIR=~/Applications/mame
MESS=$(MESS_DIR)/mame64

$(TARGET) : $(TARGET_BAS) suprplay.bin snow.bin winter.cm3
	rm -f $@
	decb dskini $@
	decb copy $(TARGET_BAS) $@,"*.BAS" -t
	decb attr $@,"*.BAS" -0
	decb copy $(TARGET_BAS) $@,$(shell echo $(TARGET_BAS) | tr a-z A-Z) -t
	decb attr $@,$(shell echo $(TARGET_BAS) | tr a-z A-Z) -0
	decb copy suprplay.bin $@,SUPRPLAY.BIN
	decb attr $@,SUPRPLAY.BIN -2 -b
	decb copy snow.bin $@,SNOW.BIN
	decb attr $@,SNOW.BIN -2 -b
	decb copy winter.cm3 $@,WINTER.CM3
	decb attr $@,WINTER.CM3 -2 -b

$(TARGET_BIN) : $(C_OBJS)
	cmoc $(CFLAGS) $(LDFLAGS) -o $@ $^

clean :
	rm -rf $(TARGET)

run : $(TARGET)
	$(MESS) coco3 -rompath $(MESS_DIR)/roms -window -flop1 $(TARGET) -cfg_directory ../cfgs/rgb

run_cmp : $(TARGET)
	$(MESS) coco3 -rompath $(MESS_DIR)/roms -window -flop1 $(TARGET) -cfg_directory ../cfgs/cmp

debug : $(TARGET)
	$(MESS) coco3 -rompath $(MESS_DIR)/roms -window -flop1 $(TARGET) -cfg_directory ../cfgs/rgb -debug

debug_cmp : $(TARGET)
	$(MESS) coco3 -rompath $(MESS_DIR)/roms -window -flop1 $(TARGET) -cfg_directory ../cfgs/cmp -debug


