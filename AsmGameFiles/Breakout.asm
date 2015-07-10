00100 	ORG	$500000110 XAXIS	FCB	1000120 YAXIS	FCB	1000130 MEMAR	FDB	$E0000140 TEMPA	FDB	000150 XCOUNT	FDB	000160 BNK	FCB	000170 START	LDA	XAXIS00180 	CMPA	#15900190 	LBHI	END00200 	LDB	YAXIS00210 	LDX	MEMAR00220 	STA	TEMPA+100230 	CLR	XCOUNT+100240 FIND	PSHS	A00250 	CMPB	#5100260 	BLO	SEL100270 	CMPB	#5100280 	LBEQ	LOONY00290 CONT1	SUBB	#5100300 	CMPB	#5100310 	BLO	SEL200320 	CMPB	#5100330 	LBEQ	LOON00340 CONT2	SUBB	#5100350 	CMPB	#5100360 	BLO	SEL300370 	CMPB	#5100380 	LBEQ	LOON200390 CONT3	SUBB	#5100400 	LDA	#$3300410 	BRA	STBNK00420 SEL1	LDA	#$3000430 	BRA	STBNK00440 SEL2	LDA	#$3100450 	BRA	STBNK00460 SEL3	LDA	#$3200470 STBNK	STA	$FFA300480 	STA	BNK00490 	PULS	A00500 CALC	PSHS	D00510 	LDA	#16000520 	MUL00530 	ADDD	TEMPA00540 	ADDD	#$600000550 TRANS	TFR	D,Y00560 	LDA	BNK00570 	CMPA	#$3100580 	BEQ	AD19200590 	CMPA	#$3200600 	BEQ	AD38400610 	CMPA	#$3300620 	BEQ	AD57600630 	BRA	PULL00640 AD192	LEAY	-32,Y00650 	BRA	PULL00660 AD384	LEAY	-64,Y00670 	BRA	PULL00680 AD576	LEAY	-96,Y00690 PULL	PULS	D00700 CHEK1	ADDA	XCOUNT+100710 	CMPA	#15900720 	BHI	CUTOFF00730 CHEK2	LDA	,X+00740 	CMPA	#$FF00750 	BEQ	ADDY00760 STA	STA	,Y+00770 	INC	XCOUNT+100780 	CMPY	#$800000790 	BNE	GOON00800 	INC	$FFA300810 	LEAY	-$2000,Y00820 GOON	LDA	TEMPA+100830 	BRA	CHEK100840 CUTOFF	LDA	,X+00850 	CMPA	#$FF00860 	BNE	CUTOFF00870 ADDY	LDA	,X+00880 	CMPA	#$FF00890 	BEQ	END00900 	LEAY	160,Y00910 	EXG	Y,D00920 	SUBD	XCOUNT00930 	CLR	XCOUNT+100940 	EXG	Y,D00950 	CMPY	#$800000960 	BLO	STA00970 	LEAY	-$2000,Y00980 	INC	$FFA300990 	BRA	STA01000 END	RTS01010 LOONY	LDA	#3201020 	CMPA	TEMPA+101030 	LBLO	CONT101040 	LBRA	SEL101050 LOON	LDA	#6401060 	CMPA	TEMPA+101070 	LBLO	CONT201080 	LBRA	SEL201090 LOON2	LDA	#9601100 	CMPA	TEMPA+101110 	LBLO	CONT301120 	LBRA	SEL301130 *PADDLE MOVE - 9/23/8901140 ATART	LDA	#401150 	STA	$500001160 	LDD	#$E0001170 	STD	$500201180 	JSR	[$A00A]01190 	LDB	YPAD01200 	LDA	$15B01210 	CMPA	#2001220 	BLO	PADUP01230 	CMPA	#4001240 	BHI	PADDN01250 PTPAD	STB	$500101260 	STB	YPAD01270 	JSR	START01280 	RTS01290 PADUP	CMPB	#201300 	BEQ	PTPAD01310 	DECB01320 	DECB01330 	BRA	PTPAD01340 PADDN	CMPB	#14601350 	BEQ	PTPAD01360 	INCB01370 	INCB01380 	BRA	PTPAD01390 YPAD	FCB	7201400 BTART	LDX	#102401410 	LDD	#$785001420 	STD	$500001430 BEGIN	LDA	,X+01440 	LDY	#$100001450 	CMPA	#25501460 	BEQ	CEND01470 	TSTA01480 	BEQ	BGO01490 	LEAY	49,Y01500 	CMPA	#101510 	BEQ	BGO01520 	LEAY	49,Y01530 	CMPA	#201540 	BEQ	BGO01550 	LEAY	49,Y01560 	CMPA	#301570 	BEQ	BGO01580 	LEAY	49,Y01590 	CMPA	#401600 	BEQ	BGO01610 	LEAY	49,Y01620 	CMPA	#501630 	BEQ	BGO01640 	LEAY	49,Y01650 	CMPA	#601660 	BEQ	BGO01670 	LEAY	49,Y01680 	CMPA	#701690 	BEQ	BGO01700 	LEAY	49,Y01710 	CMPA	#801720 	BEQ	BGO01730 	LEAY	49,Y01740 BGO	STY	$500201750 	PSHS	X01760 	JSR	START01770 	PULS	X01780 	INC	$500001790 	INC	$500001800 	INC	$500001810 	JSR	BEGIN01820 CEND	RTS01830 SCORE	FDB	001840 	FDB	001850 	FDB	001860 RESULT	FDB	001870 	FDB	001880 CTART	LDX	#SCORE+301890 	LDY	#RESULT+301900 	ANDCC	#$FE01910 SUN	LDA	,-X01920 	ADCA	#$1001930 	DAA01940 	STA	,-Y01950 	LDA	,-X01960 	ADCA	#001970 	DAA01980 	STA	,-Y01990 	LDA	,-X02000 	ADCA	#002010 	DAA02020 	STA	,-Y02030 CEGINR	LDA	RESULT02040 	STA	SCORE02050 	PSHS	A02060 	ANDA	#24002070 	RORA02080 	RORA02090 	RORA02100 	RORA02110 	CMPA	#1002120 	BLO	JNXT02130 	CLRA02140 JNXT	TFR	A,B02150 	PULS	A02160 	ANDA	#1502170 	EXG	A,B02180 	STD	102402190 CEGNA	LDA	RESULT+102200 	STA	SCORE+102210 	PSHS	A02220 	ANDA	#24002230 	RORA02240 	RORA02250 	RORA02260 	RORA02270 	ANDA	#1502280 	TFR	A,B02290 	PULS	A02300 	ANDA	#1502310 	EXG	A,B02320 	STD	102602330 CEGNB	LDA	RESULT+202340 	STA	SCORE+202350 	PSHS	A02360 	ANDA	#24002370 	RORA02380 	RORA02390 	RORA02400 	RORA02410 	TFR	A,B02420 	PULS	A02430 	ANDA	#1502440 	EXG	A,B02450 	STD	102802460 	LDA	#25502470 	STA	103002480 	JSR	BTART02490 	RTS02500 ADX	FCB	102510 ADY	FCB	102520 XX	FCB	2002530 YY	FCB	1602540 DEGIN	LDD	#002550 	CLR	6549702560 	ORCC	#8002570 	STD	SCORE02580 	STD	SCORE+202590 	STD	102402600 	STD	102602610 	STD	102802620 	LDA	#402630 	STA	NMEN02640 HERE	LDA	#2002650 	LDB	#202660 	STD	XX02670 	LDB	#202680 	CLR	COBOL02690 	JSR	$E69E02700 	LDA	#$FF02710 	STA	$FF9A02720 	LDA	#5002730 	LDB	#202740 	STD	$500002750 	LDD	#$130002760 	STD	$500202770 	JSR	START02780 	LDA	#6502790 	LDB	#202800 	STD	$500002810 	LDD	#$130002820 	STD	$500202830 	JSR	START02840 	LDB	#202850 	LDA	#8002860 	STD	$500002870 	LDD	#$130002880 	STD	$500202890 	JSR	START02900 	LDB	#202910 	LDA	#9502920 	STD	$500002930 	LDD	#$130002940 	STD	$500202950 	JSR	START02960 	LDA	#25502970 	STA	103002980 	JSR	BTART02990 	LDX	#PAL03000 	LDY	#$FFB003010 BRT	LDA	,X+03020 	STA	,Y+03030 	CMPA	#6403040 	BNE	BRT03050 	PSHS	U03060 	LDY	#LINE103070 	LDX	#BPOS03080 	LDU	#LINE203090 ROT1	LDA	,X+03100 	STA	,Y+03110 	STA	,U+03120 	CMPX	#BPD+103130 	BNE	ROT103140 	LDY	#LINE303150 	LDU	#LINE403160 	LDX	#BPOS03170 ROL	LDA	,X+03180 	STA	,Y+03190 	STA	,U+03200 	CMPX	#BPD+103210 	BNE	ROL03220 	PULS	U03230 	LDA	NMEN03240 	LDB	#25503250 	STD	102403260 	LDD	#$786F03270 	STD	$500003280 	LDX	#102403290 	JSR	BEGIN03300 REJIN	LDA	#3003310 	LDB	#303320 	STD	XX03330 	LDD	#$010503340 	STD	SLPX03350 	STD	FLGX03360 REGIN	JSR	ATART03370 	LDX	#$E8203380 	STX	$500203390 GLGL	LDA	FLGX03400 	TSTA03410 	BEQ	TDK03420 	DECA03430 	STA	FLGX03440 	LDB	XX03450 	ADDB	ADX03460 	CMPB	#11003470 	BHI	NXT03480 	CMPB	#503490 	LBLO	ZZTP03500 	STB	$500003510 	STB	XX03520 TDK	LDA	FLGY03530 	TSTA03540 	LBEQ	RCHK03550 	DECA03560 	STA	FLGY03570 	LDB	YY03580 	ADDB	ADY03590 	CMPB	#17903600 	BHI	NXT103610 	CMPB	#203620 	BLO	NTX103630 	STB	$500103640 	STB	YY03650 	LDB	XX03660 	STB	$500003670 MCDLT	JSR	START03680 	LDA	XX03690 	CMPA	#4303700 	LBLO	GLOR03710 	CMPA	#5003720 	LBLO	FNCK03730 GLOR	CMPA	#5803740 	LBLO	GLR03750 	CMPA	#6503760 	LBLO	FNCK103770 GLR	CMPA	#7303780 	LBLO	GLOR203790 	CMPA	#8003800 	LBLO	FNCK203810 GLOR2	CMPA	#8803820 	LBLO	GLOR103830 	CMPA	#9503840 	LBLO	FNCK303850 GLOR1	JSR	[$A000]03860 	CMPA	#303870 	LBEQ	PAUSE03880 TAG	LDA	#6803890 	CMPA	COBOL03900 	LBNE	REGIN03910 	LBRA	HERE03920 NXT	LDA	#-103930 	STA	ADX03940 	LBRA	REGIN03950 NTX	LDA	#103960 	STA	ADX03970 	LBRA	REGIN03980 NXT1	LDA	#-103990 	STA	ADY04000 	LBRA	DUKE04010 NTX1	LDA	#104020 	STA	ADY04030 	LBRA	DUKE04040 ZZTP	LDA	YPAD04050 	CMPA	#704060 	BHI	BLOB04070 	ADDA	#3204080 	BRA	BLAR04090 BLOB	CMPA	YY04100 	BNE	ZZZZ04110 	LDD	#304120 	JSR	$B4F404130 	JSR	$BF1F04140 	JSR	$B3ED04150 	STB	SLPX04160 	BRA	NTX04170 ZZZZ	SUBA	#704180 	CMPA	YY04190 	BHI	MISS04200 	ADDA	#4404210 BLAR	CMPA	YY04220 	BLO	MISS04230 	LDD	#304240 	JSR	$B4F404250 	JSR	$BF1F04260 	JSR	$B3ED04270 	STB	SLPX04280 	BRA	NTX04290 MISS	DEC	NMEN04300 	LDA	NMEN04310 	LDB	#$FF04320 	STD	102404330 	LDX	#102404340 	LDD	#$786F04350 	STD	$500004360 	JSR	BEGIN04370 	LDD	#$200004380 	STD	$500204390 	LDD	XX04400 	STD	$500004410 	JSR	START04420 	LDA	NMEN04430 	TSTA04440 	BEQ	POLIN04450 	LBRA	REJIN04460 NMEN	FCB	404470 PAL	FCB	12704480 	FCB	11804490 	FCB	11004500 	FCB	10204510 	FCB	8904520 	FCB	8004530 	FCB	7904540 	FCB	12004550 	FCB	7104560 	FCB	9804570 	FCB	10404580 	FCB	9604590 	FCB	7404600 	FCB	6604610 	FCB	7304620 	FCB	6404630 BPOS	FCB	204640 	FCB	1304650 	FCB	2404660 	FCB	3504670 	FCB	4604680 	FCB	5704690 	FCB	6804700 	FCB	7904710 	FCB	9004720 	FCB	10104730 	FCB	11204740 	FCB	12304750 	FCB	13404760 	FCB	14504770 	FCB	15604780 	FCB	16704790 BPD	FCB	17804800 POLIN	JSR	[$A000]04810 	CMPA	#004820 	BEQ	POLIN04830 	LBRA	DEGIN04840 PAUSE	JSR	[$A000]04850 	CMPA	#304860 	BNE	PAUSE04870 	LBRA	TAG04880 RCHK	LDA	FLGX04890 	TSTA04900 	BEQ	RESET04910 DUKE	LDD	XX04920 	STD	$500004930 	LBRA	MCDLT04940 RESET	LDD	SLPX04950 	STD	FLGX04960 	LBRA	GLGL04970 SLPX	FCB	504980 SLPY	FCB	204990 FLGX	FCB	505000 FLGY	FCB	205010 *	END	DEGIN05020 FNCK	LDX	#LINE105030 LDCK	LDA	,X05040 	CMPA	YY05050 	BEQ	CLRBLK05060 	LBLO	FRCK05070 GOOP	BHI	RCK05080 OVFLW	LEAX	1,X05090 	CMPX	#LINA+105100 	BNE	LDCK05110 	LBRA	GLOR05120 RCK	LDA	YY05130 	ADDA	#1105140 	STA	YY05150 	LDA	,X05160 	CMPA	YY05170 	BLO	FIXY05180 	LDB	YY05190 	SUBB	#1105200 	STB	YY05210 	BRA	OVFLW05220 FIXY	LDB	YY05230 	SUBB	#1105240 	STB	YY05250 CLRBLK	LDB	#25505260 	PSHS	X05270 	STB	,X05280 	LDY	#$200005290 	STY	$500205300 	TFR	A,B05310 	LDA	#5005320 	STD	$500005330 	JSR	START05340 	JSR	CTART05350 	INC	COBOL05360 	LDD	#305370 	JSR	$B4F405380 	JSR	$BF1F05390 	JSR	$B3ED05400 	PSHS	D05410 	ANDB	#105420 	TSTB05430 	BEQ	MRM05440 	LDA	ADX05450 	CMPA	#105460 	BNE	MRM05470 	LDA	#-105480 	STA	ADX05490 MRM	PULS	D05500 	STB	SLPX	05510 	LDD	#305520 	JSR	$B4F405530 	JSR	$BF1F05540 	JSR	$B3ED05550 	STB	SLPY05560 	LDD	#$FF0105570 	STA	$8C05580 	JSR	$A95105590 	PULS	X05600 	LBRA	OVFLW05610 FRCK	ADDA	#1405620 	CMPA	YY05630 	BHI	FREEZE05640 	LBRA	GOOP05650 FREEZE	SUBA	#1405660 	BRA	CLRBLK05670 LINE1	FCB	205680 	FCB	1305690 	FCB	2405700 	FCB	3505710 	FCB	4605720 	FCB	5705730 	FCB	6805740 	FCB	7905750 	FCB	9005760 	FCB	10105770 	FCB	11205780 	FCB	12305790 	FCB	13405800 	FCB	14505810 	FCB	15605820 	FCB	16705830 LINA	FCB	17805840 FNCK1	LDX	#LINE205850 LDCK1	LDA	,X05860 	CMPA	YY05870 	BEQ	CLRBL105880 	LBLO	FRCK105890 GOOP1	BHI	RCK105900 OVFLW1	LEAX	1,X05910 	CMPX	#LINA1+105920 	BNE	LDCK105930 	LBRA	GLOR105940 RCK1	LDA	YY05950 	ADDA	#1105960 	STA	YY05970 	LDA	,X05980 	CMPA	YY05990 	BLO	FIXY106000 	LDB	YY06010 	SUBB	#1106020 	STB	YY06030 	BRA	OVFLW106040 FIXY1	LDB	YY06050 	SUBB	#1106060 	STB	YY06070 CLRBL1	LDB	#25506080 	PSHS	X06090 	STB	,X06100 	LDY	#$200006110 	STY	$500206120 	TFR	A,B06130 	LDA	#6506140 	STD	$500006150 	JSR	START06160 	JSR	CTART06170 	INC	COBOL06180 	LDD	#306190 	JSR	$B4F406200 	JSR	$BF1F06210 	JSR	$B3ED06220 	PSHS	D06230 	ANDB	#106240 	TSTB06250 	BEQ	MRMR06260 	LDA	ADX06270 	CMPA	#106280 	BNE	MRMR06290 	LDA	#-106300 	STA	ADX06310 MRMR	PULS	D06320 	STB	SLPX	06330 	LDD	#306340 	JSR	$B4F406350 	JSR	$BF1F06360 	JSR	$B3ED06370 	PSHS	B06380 	ANDB	#106390 	TSTB06400 	BEQ	MEME06410 MEME	PULS	B06420 	STB	SLPY06430 	LDD	#$FF0106440 	STA	$8C06450 	JSR	$A95106460 	PULS	X06470 	LBRA	OVFLW106480 FRCK1	ADDA	#1406490 	CMPA	YY06500 	BHI	FREEZ106510 	LBRA	GOOP106520 FREEZ1	SUBA	#1406530 	BRA	CLRBL106540 LINE2	FCB	206550 	FCB	1306560 	FCB	2406570 	FCB	3506580 	FCB	4606590 	FCB	5706600 	FCB	6806610 	FCB	7906620 	FCB	9006630 	FCB	10106640 	FCB	11206650 	FCB	12306660 	FCB	13406670 	FCB	14506680 	FCB	15606690 	FCB	16706700 LINA1	FCB	17806710 FNCK2	LDX	#LINE306720 LDCK2	LDA	,X06730 	CMPA	YY06740 	BEQ	CLRBL206750 	LBLO	FRCK206760 GOOP2	BHI	RCK206770 OVFLW2	LEAX	1,X06780 	CMPX	#LINA2+106790 	BNE	LDCK206800 	LBRA	GLOR106810 RCK2	LDA	YY06820 	ADDA	#1106830 	STA	YY06840 	LDA	,X06850 	CMPA	YY06860 	BLO	FIXY206870 	LDB	YY06880 	SUBB	#1106890 	STB	YY06900 	BRA	OVFLW206910 FIXY2	LDB	YY06920 	SUBB	#1106930 	STB	YY06940 CLRBL2	LDB	#25506950 	PSHS	X06960 	STB	,X06970 	LDY	#$200006980 	STY	$500206990 	TFR	A,B07000 	LDA	#8007010 	STD	$500007020 	JSR	START07030 	JSR	CTART07040 	INC	COBOL07050 	LDD	#307060 	JSR	$B4F407070 	JSR	$BF1F07080 	JSR	$B3ED07090 	PSHS	D07100 	ANDB	#107110 	TSTB07120 	BEQ	MRMR107130 	LDA	ADX07140 	CMPA	#107150 	BNE	MRMR107160 	LDA	#-107170 	STA	ADX07180 MRMR1	PULS	D07190 	STB	SLPX	07200 	LDD	#307210 	JSR	$B4F407220 	JSR	$BF1F07230 	JSR	$B3ED07240 	PSHS	B07250 	ANDB	#107260 	TSTB07270 	BEQ	MEME107280 MEME1	PULS	B07290 	STB	SLPY07300 	LDD	#$FF0107310 	STA	$8C07320 	JSR	$A95107330 	PULS	X07340 	LBRA	OVFLW207350 FRCK2	ADDA	#1407360 	CMPA	YY07370 	BHI	FREEZ207380 	LBRA	GOOP207390 FREEZ2	SUBA	#1407400 	BRA	CLRBL207410 LINE3	FCB	207420 	FCB	1307430 	FCB	2407440 	FCB	3507450 	FCB	4607460 	FCB	5707470 	FCB	6807480 	FCB	7907490 	FCB	9007500 	FCB	10107510 	FCB	11207520 	FCB	12307530 	FCB	13407540 	FCB	14507550 	FCB	15607560 	FCB	16707570 LINA2	FCB	17807580 FNCK3	LDX	#LINE407590 LDCK3	LDA	,X07600 	CMPA	YY07610 	BEQ	CLRBL307620 	LBLO	FRCK307630 GOOP3	BHI	RCK307640 OVFLW3	LEAX	1,X07650 	CMPX	#LINA3+107660 	BNE	LDCK307670 	LBRA	GLOR107680 RCK3	LDA	YY07690 	ADDA	#1107700 	STA	YY07710 	LDA	,X07720 	CMPA	YY07730 	BLO	FIXY307740 	LDB	YY07750 	SUBB	#1107760 	STB	YY07770 	BRA	OVFLW307780 FIXY3	LDB	YY07790 	SUBB	#1107800 	STB	YY07810 CLRBL3	LDB	#25507820 	PSHS	X07830 	STB	,X07840 	LDY	#$200007850 	STY	$500207860 	TFR	A,B07870 	LDA	#9507880 	STD	$500007890 	JSR	START07900 	JSR	CTART07910 	INC	COBOL07920 	LDD	#307930 	JSR	$B4F407940 	JSR	$BF1F07950 	JSR	$B3ED07960 	PSHS	D07970 	ANDB	#107980 	TSTB07990 	BEQ	MRMR208000 	LDA	ADX08010 	CMPA	#108020 	BNE	MRMR208030 	LDA	#-108040 	STA	ADX08050 MRMR2	PULS	D08060 	STB	SLPX	08070 	LDD	#308080 	JSR	$B4F408090 	JSR	$BF1F08100 	JSR	$B3ED08110 	PSHS	B08120 	ANDB	#108130 	TSTB08140 	BEQ	MEME208150 MEME2	PULS	B08160 	STB	SLPY08170 	LDD	#$FF0108180 	STA	$8C08190 	JSR	$A95108200 	PULS	X08210 	LBRA	OVFLW308220 FRCK3	ADDA	#1408230 	CMPA	YY08240 	BHI	FREEZ308250 	LBRA	GOOP308260 FREEZ3	SUBA	#1408270 	BRA	CLRBL308280 LINE4	FCB	208290 	FCB	1308300 	FCB	2408310 	FCB	3508320 	FCB	4608330 	FCB	5708340 	FCB	6808350 	FCB	7908360 	FCB	9008370 	FCB	10108380 	FCB	11208390 	FCB	12308400 	FCB	13408410 	FCB	14508420 	FCB	15608430 	FCB	16708440 LINA3	FCB	17808450 COBOL	FCB	008460 BRKOUT	LDX	#$E0008470 	LDY	#DATA08480 LDDATA	LDD	,Y++08490 	STD	,X++08500 	CMPX	#$E00+614408510 	BNE	LDDATA08520 	CLR	$7108530 	LBRA	DEGIN08540 DATA	FCB	008550 	END	BRKOUT