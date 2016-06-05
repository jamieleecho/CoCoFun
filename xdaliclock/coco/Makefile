CFLAGS=-I../font --verbose
TARGET=dali.dsk
XDALI_BAS=xdali.bas
XDALI_BIN=xdali.bin
MESS_DIR=~/Applications/MacSDLMESS/mame0163
MESS=$(MESS_DIR)/mess64

$(TARGET) : $(XDALI_BAS) $(XDALI_BIN) testuint.bin testfp.bin testmem.bin \
            testtstr.bin
	rm -f $@
	decb dskini $@
	decb copy $(XDALI_BAS) $@,"*.BAS" -t
	decb attr $@,"*.BAS" -0
	decb copy $(XDALI_BAS) $@,XDALI.BAS -t
	decb attr $@,XDALI.BAS -0
	decb copy $(XDALI_BIN) $@,XDALI.BIN
	decb attr $@,XDALI.BIN -2 -b
	decb copy testuint.bin $@,TESTUINT.BIN
	decb attr $@,TESTUINT.BIN -2 -b
	decb copy testfp.bin $@,TESTFP.BIN
	decb attr $@,TESTFP.BIN -2 -b
	decb copy testmem.bin $@,TESTMEM.BIN
	decb attr $@,TESTMEM.BIN -2 -b
	decb copy testtstr.bin $@,TESTTSTR.BIN
	decb attr $@,TESTTSTR.BIN -2 -b

%.bin : %.c
	cmoc $(CFLAGS) $< 


$(XDALI_BIN) : xdali.c digital.c c_stuff.c time.c
	cmoc $(CFLAGS) $< 

testuint.bin: testuint.c c_stuff.c tester.c setjmp.c UInt32.c
	cmoc $(CFLAGS) $<

testfp.bin: testfp.c c_stuff.c tester.c setjmp.c FixedPoint.c
	cmoc $(CFLAGS) $<


testtstr.bin: testtstr.c c_stuff.c tester.c setjmp.c
	cmoc $(CFLAGS) $<

testmem.bin: testmem.c c_stuff.c tester.c setjmp.c
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


