CFLAGS=
BREAKOUT_BAS=AsmGameFiles/Breakout.bas
BREAKOUT_BIN=Breakout.bin

CoCoFun.dsk : Foo.bin Breakout.bin $(BREAKOUT_BAS) $(BREAKOUT_BIN) Oops.bin
	rm -f $@
	decb dskini $@
	decb copy Foo.bin $@,FOO.BIN
	decb attr $@,FOO.BIN -2 -b
	decb copy $(BREAKOUT_BAS) $@,BREAKOUT.BAS -t
	decb attr $@,BREAKOUT.BAS -0
	decb copy $(BREAKOUT_BIN) $@,BREAKOUT.BIN
	decb attr $@,BREAKOUT.BIN -2 -b
	decb copy Oops.bin $@,OOPS.BIN
	decb attr $@,OOPS.BIN -2 -b

Breakout.bin : Breakout.c GrafxData.c BreakoutScore.c BreakoutBall.c Blitter.c Bricks.c \
			   FontData.c BreakoutBall.c CoCoMisc.c
	cmoc $(CFLAGS) $< 

%.bin : %.c
	cmoc $(CFLAGS) $< 

clean :
	rm -rf CoCoFun.dsk *.bin *.i *.lst *.asm *.hex cfg $(BREAKOUT_BIN)

debug : CoCoFun.dsk
	~/Applications/MacSDLMESS/mess0155/mess coco3 -rompath ~/Applications/MacSDLMESS/mess0155/roms -window -flop1 CoCoFun.dsk -cfg_directory rgb

debug_cmp : CoCoFun.dsk
	~/Applications/MacSDLMESS/mess0155/mess coco3 -rompath ~/Applications/MacSDLMESS/mess0155/roms -window -flop1 CoCoFun.dsk -cfg_directory cmp
