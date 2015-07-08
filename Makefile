CFLAGS=-a09=/usr/local/bin/lwasm -I/usr/local/share/cmoc

CoCoFun.dsk : Foo.bin
	rm -f $@
	decb dskini $@
	decb copy Foo.bin $@,FOO.BIN
	decb attr $@,FOO.BIN -2 -b

%.bin : %.c
	cmoc $< $(CFLAGS)

clean :
	rm -rf CoCoFun.dsk *.bin *.i *.lst *.asm *.hex cfg

debug : CoCoFun.dsk
	~/Applications/MacSDLMESS/mess0155/mess coco3 -rompath ~/Applications/MacSDLMESS/mess0155/roms -window -flop1 CoCoFun.dsk
