/*
  Blitter.c
  
  This software is for demonstration purposes only. Use at your own
  risk.

  Blitting functions
*/


#ifndef _Blitter_c
#define _Blitter_c


#define BREAKOUT_SCORE_NUM_BYTES 4


byte mmuBuffer[4];
void BlitterInit() {
  memcpy(mmuBuffer, (byte *)0xffa4, sizeof(mmuBuffer));
}


/** 
 * Disables interrupts and maps in the screen
 */
void BlitterMapScreen() {
  asm { orcc      #$50 }
  *((byte *)0xffa4) = 0x30;
  *((byte *)0xffa5) = 0x31;
  *((byte *)0xffa6) = 0x32;
  *((byte *)0xffa7) = 0x33;
}


/**
 * Unmaps the screen and enabled interrupts
 */
void BlitterUnmapScreen() {
  memcpy((byte *)0xffa4, mmuBuffer, sizeof(mmuBuffer));
  asm { andcc     #$af }
}


void blitGraphics2(byte *bitmap, byte x, byte y) {
  // Bounds check
  if ((x > 159) || (y > 191))
    return;

  BlitterMapScreen();
  asm {
      ldx bitmap
      stx  MEMAR
      lda  x
      ldb  y
      std  XAXIS

      orcc      #$50
      pshs      u

* Setup registers to point to memory
* U - src
* X - dst
* Y - current row
* b - running counter of number of bytes
      ldu       MEMAR
      clra
      ldb       YAXIS
      tfr       d,y
      lda       YAXIS
      ldb       #160
yloop mul
      addd      #$8000      
      tfr       d,X
      ldb       XAXIS
      abx

* Transfer one line of pixels
xloop lda       ,u+
      cmpa      #$ff
      beq       yend
      sta       ,x+
      incb
      cmpb      #160
      bcs       xloop

* Skip source bytes due to edge clipping
xskip lda       ,u+
      cmpa      #$ff
      bne       xskip

* Go to the next line
yend  lda       ,u
      cmpa      #$ff
      beq       end_blit
      leay      1,y
      cmpy      #192
      bge       end_blit
      tfr       y,d
      lda       #160
      bra       yloop

* Storage for random variables
XAXIS	FCB	10
YAXIS	FCB	10
MEMAR	FDB	$E00

end_blit
      puls      u
  }

  BlitterUnmapScreen();
}


void blitNumericText(char *text, byte x, byte y) {
  for(char c = *text++; c != 0; c = *text++) {
    if (c < '0') c = '0';
    if (c > '9') c = '9';
    c = c - '0';
    blitGraphics2((byte *)GrafxDataNumberData + ((int)49 * (int)c), x, y);
    x += 3;
  }
}


void BlitterDrawText(int *fontIndex, byte *fontData,
					 byte foreground, byte background,
					 int x, int y,
					 byte glyphSpacing,
					 char *text) {
  // Bounds check
  if ((x > 319) || (y > 191))
	return;

  BlitterMapScreen();
  byte fcolor4 = foreground << 4;
  byte bcolor4 = background << 4;
  byte lookup[] = { bcolor4 | background, bcolor4 | foreground,
                    fcolor4 | background, fcolor4 | foreground };

  for(char c = *text++; c != 0; c = *text++) {
    // Ignore characters that are out of range
    if ((c < 32) || (c > 127)) {
      continue;
	}

    // Ignore unknown glyphs
    int offset = fontIndex[c - 32];
    if (offset < 0) {
      continue;	  
	}

    byte *fontPtr = fontData + offset;
    byte width = *fontPtr++;
    byte height = *fontPtr++;
    byte numBytes = (width + 7) / 8;
    byte *dst = (byte *)0x8000 + (y * 160) + x/2;
	int currentX = x;
	
    for(int jj=0; jj<height; jj++) {
	  byte widthBits = width;
	  currentX = x;
	  int forwardBytes = 0;
	  for(int ii=0; ii<numBytes; ii++) {
		byte fontByte = *fontPtr++;

		if ((widthBits >= 8) && (x < 311)) {
		  if ((currentX & 1) == 0) {
			asm {
              lda  fontByte
              leax  lookup
              ldy  dst

           * most significant bits
              tfr   a,b
              andb  #0x3
              ldb   b,x
              stb   3,y
              rora
              rora

           * next bits
              tfr   a,b
              andb  #0x3
              ldb   b,x
              stb   2,y
              rora
              rora

           * next bits
              tfr   a,b
              andb  #0x3
              ldb   b,x
              stb   1,y
              rora
              rora

           * next bits
              anda  #0x3
              ldb   a,x
              stb   ,y
			}
		  } else {
			byte temp;
			asm {
              leax  lookup
              ldy  dst
              lda  fontByte

           * most significant bit
              tfr   a,b
              andb  #0x1
              rolb
              ldb   b,x
              andb  #0xf0
              stb   temp
              ldb   4,y
              andb  #0xf
              orb   temp
              stb   4,y
              rora

           * next bits
              tfr   a,b
              andb  #0x3
              ldb   b,x
              stb   3,y
              rora
              rora

           * next bits
              tfr   a,b
              andb  #0x3
              ldb   b,x
              stb   2,y
              rora
              rora

           * next bits
              tfr   a,b
              andb  #0x3
              ldb   b,x
              stb   1,y
              rora
              rora

           * least significant bit
              anda  #0x1
              ldb   a,x
              andb  #0x0f
              stb   temp
              ldb   ,y
              andb  #0xf0
              orb   temp
              stb   ,y
			}
		  }
		  currentX += 8;
		  dst += 4;
		  forwardBytes += 4;
		  widthBits -= 8;
		} else if ((widthBits == 4) && (x < 315)) {
		  if ((currentX & 1) == 0) {
			asm {
              lda  fontByte
              leax  lookup
              ldy  dst
              rora
              rora
              rora
              rora

           * most significant bits
              tfr   a,b
              andb  #0x3
              ldb   b,x
              stb   1,y
              rora
              rora

           * next bits
              anda  #0x3
              ldb   a,x
              stb   ,y
			}
		  } else {
			byte temp;
			asm {
              leax  lookup
              ldy  dst
              lda  fontByte
              rora
              rora
              rora
              rora

           * most significant bit
              tfr   a,b
              andb  #0x1
              rolb
              ldb   b,x
              andb  #0xf0
              stb   temp
              ldb   2,y
              andb  #0xf
              orb   temp
              stb   2,y
              rora

           * next bits
              tfr   a,b
              andb  #0x3
              ldb   b,x
              stb   1,y
              rora
              rora

           * least significant bit
              anda  #0x1
              ldb   a,x
              stb   temp
              ldb   ,y
              andb  #0xf0
              orb   temp
              stb   ,y
				}
		  }
		  currentX += 4;
		  dst += 2;
		  forwardBytes += 2;
		  widthBits -= 4;
		} else {
		  // No more bits???
		    for(int kk=0; kk<8; kk++) {
		      if (widthBits == 0)
			break;

		      if (currentX > 319) {
			while(widthBits != 0xff) {
			  if (currentX & 1)
			    dst++;
			  
			  // Iterate
			  widthBits--;
			}
			break;
		      }
		      
		      // Draw the bit
		      if (currentX & 1) {
			byte color = (fontByte & 0x80) ? foreground : background;
			*dst = (*dst & 0xf0) | color;
			dst++;
			forwardBytes++;
		      } else {
			byte color = (fontByte & 0x80) ? fcolor4 : bcolor4;
			*dst = (*dst & 0x0f) | color;
		      }
		      
		      // Iterate
		      widthBits--;
		      fontByte = fontByte << 1;
		      currentX++;
		    }
		}
	  }

	  // Put in whitespace
      byte temp;
	  asm {
         clr temp
         ldx currentX
         ldy dst

WHITELOOP
         lda temp
         cmpa glyphSpacing
         bge CLIPWHITE
         cmpx #319
         bgt CLIPWHITE

         tfr x,d
         andb #1
         beq  WHITELEFTNIBBLE

WHITERIGHTNIBBLE
         lda ,y
         anda #$f0
         ora  background
         sta ,y+
         ldd  forwardBytes
         addd #1
         std  forwardBytes         
         bra  WHITEITER

WHITELEFTNIBBLE         
         lda ,y
         anda #$0f
         ora  bcolor4
         sta ,y

WHITEITER
		 leax 1,x
         inc  temp
         bra  WHITELOOP

CLIPWHITE
         sty  dst
         stx  currentX
		   }

	  dst = dst + 160 - forwardBytes;
	}

	// Increment to the next character
	x = currentX;

	// Bounds check
	if (x > 319)
	  break;
  }

  // Restore MMU and Enable interrupts
  BlitterUnmapScreen();
}


void BlitterClearScreen(byte color) {
  BlitterMapScreen();
  color = color & 0xf;
  memset((byte *)0x8000, (color << 4) | color, 160 * 192);
  BlitterUnmapScreen();
}


#endif
