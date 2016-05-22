CFLAGS=-O2
TARGET=Primes.dsk
MESS_DIR=~/Applications/MacSDLMESS/mame0163
MESS=$(MESS_DIR)/mess64

$(TARGET) : primes.bin
	rm -f $@
	decb dskini $@
	decb copy primes.bin $@,PRIMES.BIN
	decb attr $@,PRIMES.BIN -2 -b

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


