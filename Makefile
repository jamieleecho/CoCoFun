CFLAGS=-a09=/usr/local/bin/lwasm -I/usr/local/share/cmoc
BREAKOUT_BAS=AsmGameFiles/Breakout.bas
BREAKOUT_BIN=AsmGameFiles/Breakout.bin

CoCoFun.dsk : Foo.bin $(BREAKOUT_BAS) $(BREAKOUT_BIN)
	rm -f $@
	decb dskini $@
	decb copy Foo.bin $@,FOO.BIN
	decb attr $@,FOO.BIN -2 -b
	decb copy $(BREAKOUT_BAS) $@,BREAKOUT.BAS -t
	decb attr $@,BREAKOUT.BAS -0
	decb copy $(BREAKOUT_BIN) $@,BREAKOUT.BIN
	decb attr $@,BREAKOUT.BIN -2 -b

$(BREAKOUT_BIN) : $(patsubst %.bin,%.asm,$(BREAKOUT_BIN))
	lwasm -b -o $@ $<

%.bin : %.c
	cmoc $< $(CFLAGS)

clean :
	rm -rf CoCoFun.dsk *.bin *.i *.lst *.asm *.hex cfg $(BREAKOUT_BIN)

debug : CoCoFun.dsk
	~/Applications/MacSDLMESS/mess0155/mess coco3 -rompath ~/Applications/MacSDLMESS/mess0155/roms -window -flop1 CoCoFun.dsk
