/*
  Blitter.c
  
  This software is for demonstration purposes only. Use at your own
  risk.

  Blitting functions
*/


#ifndef _Blitter_c
#define _Blitter_c


byte mmuBuffer[4];
void BlitterInit() {
  memcpy(mmuBuffer, (byte *)0xffa4, sizeof(mmuBuffer));
}


/** 
 * Maps in the screen
 */
void BlitterMapScreen() {
  *((byte *)0xffa4) = 0x30;
  *((byte *)0xffa5) = 0x31;
  *((byte *)0xffa6) = 0x32;
  *((byte *)0xffa7) = 0x33;
}


/**
 * Unmaps the screen
 */
void BlitterUnmapScreen() {
  memcpy((byte *)0xffa4, mmuBuffer, sizeof(mmuBuffer));
}


void BlitterDrawGraphics(byte *bitmap, byte x, byte y) {
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


/**
 * Optimized routine for blitting numeric glyphs.
 * @param bitmap[input] numeric glyph bitmap to draw
 * @param x[input] x byte location
 * @param y[input] y pixel location
 */
void BlitterDrawNumericGlyph(byte *bitmap, byte x, byte y) {
  // Bounds check
  if ((x > 155) || (y > 184))
    return;
  unsigned xx = x;

  BlitterMapScreen();
  asm {

* Load the destination
      lda  y
      ldb  #160
      mul
      addd xx
      addd #$8000
      tfr d,x
      ldy #8

      pshs      u

* Load the source
      ldu bitmap

* Transfer the rows
BlitterDrawNumericGlyphLoop
      ldd 0,u
      std 0,x
      ldd 2,u
      std 2,x
      lda 4,u
      sta 4,x
      leau 6,u
      leax 160,x
      leay -1,y
      bne BlitterDrawNumericGlyphLoop

      puls      u
  }

  BlitterUnmapScreen();
}


void BlitterDrawNumericText(char *text, byte x, byte y) {
  for(char c = *text++; c != 0; c = *text++) {
    if (c < '0') c = '0';
    if (c > '9') c = '9';
    c = c - '0';
    BlitterDrawNumericGlyph((byte *)GrafxDataNumberData + ((unsigned)49 * (unsigned)c), x, y);
    x += 5;
  }
}


void BlitterDrawText(unsigned *fontIndex, byte *fontData,
		     byte foreground, byte background,
		     unsigned x, unsigned y,
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
    unsigned offset = fontIndex[c - 32];
    if (offset < 0) {
      continue;	  
	}

    byte *fontPtr = fontData + offset;
    byte width = *fontPtr++;
    byte height = *fontPtr++;
    byte numBytes = (width + 7) / 8;
    byte *dst = (byte *)0x8000 + (y * 160) + x/2;
	unsigned currentX = x;
	
    for(unsigned jj=0; jj<height; jj++) {
	  byte widthBits = width;
	  currentX = x;
	  unsigned forwardBytes = 0;
	  for(unsigned ii=0; ii<numBytes; ii++) {
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
		    for(unsigned kk=0; kk<8; kk++) {
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


void BlitterDrawText2(unsigned *fontIndex, byte *fontData,
		      byte foreground,
		      unsigned x, unsigned y,
		      byte glyphSpacing,
		      char *text) {
  // Bounds check
  if ((x > 319) || (y > 191))
	return;
  BlitterMapScreen();
  byte fcolor4 = foreground << 4;

  for(char c = *text++; c != 0; c = *text++) {
    // Ignore characters that are out of range
    if ((c < 32) || (c > 127)) {
      continue;
	}

    // Ignore unknown glyphs
    unsigned offset = fontIndex[c - 32];
    if (offset < 0) {
      continue;	  
	}

    byte *fontPtr = fontData + offset;
    byte width = *fontPtr++;
    byte height = *fontPtr++;
    byte numBytes = (width + 7) / 8;
    byte *dst = (byte *)0x8000 + (y * 160) + x/2;
	unsigned currentX = x;
	
    for(unsigned jj=0; jj<height; jj++) {
	  byte widthBits = width;
	  currentX = x;
	  unsigned forwardBytes = 0;
	  for(unsigned ii=0; ii<numBytes; ii++) {
		byte fontByte = *fontPtr++;

		// No more bits???
		for(unsigned kk=0; kk<8; kk++) {
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
			if (fontByte & 0x80)
			  *dst = (*dst & 0xf0) | foreground;
			dst++;
			forwardBytes++;
		  } else {
			if (fontByte & 0x80)
			  *dst = (*dst & 0x0f) | fcolor4;
		  }
		  
		  // Iterate
		  widthBits--;
		  fontByte = fontByte << 1;
		  currentX++;
		}
	  }

	  dst = dst + 160 - forwardBytes;	  
	}

	// Increment to the next character
	x = currentX + glyphSpacing;

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


void BlitterFillRectangle(unsigned x, unsigned y, unsigned width, unsigned height,
			  byte color) {
  BlitterMapScreen();

  // Bounds check
  color = color & 0xf;  
  if ((x > 319) || (y > 191)) return;
  if ((width == 0) || (height == 0)) return;
  unsigned endX = x + width;
  unsigned endY = y + height;
  if (endX > 320) endX = 320;
  if (endY > 192) endY = 192;

  // Figure out what we can quickly blast out in 16-bit chunks
  width = endY - y;
  byte startPixels = (byte)(x & 1); // num pixels before 16-bit blasting
  unsigned blastStartPixel = (x + 1) & 0xfffe; // pixel where 16-bit blasting starts
  byte wordStartX = (byte)(blastStartPixel >> 1); // byte where 16-bit blasting starts
  byte numPixelsAfterBlastStartPixel = (byte)(endX - blastStartPixel);
  byte endPixels = (numPixelsAfterBlastStartPixel & 0x3); // num pixels after blast
  byte blastPixels = numPixelsAfterBlastStartPixel & 0xfc; // num pixels to blast
  byte blastBytes = blastPixels >> 1; // num pixels to blast
  byte wordEndX = (byte)(wordStartX + blastBytes); // byte to stop blasting
  unsigned numLines = endY - y; // total number of lines
  unsigned wordColor = color | (color << 4);
  wordColor = (wordColor << 8) | wordColor;
  unsigned ptr = (0x8000 + (160 * y)) + wordStartX;
  unsigned bytesPerLine = (wordEndX - wordStartX);
  byte skip = 160 - (byte)bytesPerLine;
  unsigned endLine;
  byte temp;
  asm {
    ldd ptr
    ldy numLines
    tfr d,x

Blast16Rec
    addd bytesPerLine
    std endLine

*** Check for the case where we start on an off pixel
    lda startPixels
    cmpa #1
    bne Blast16RecStart
    ldb -1,x
    andb #$f0
    orb color
    stb -1,x

Blast16RecStart
*** Blast out 16-bits at a time
    ldd wordColor
    cmpx endLine
    bhs Blast16EndOfLine

Blast16RecLine
    std ,x++
    cmpx endLine
    blo Blast16RecLine

*** Check for pixels not on word boundary
Blast16EndOfLine
    lda endPixels
    beq Blast16NextLine
    cmpa #2
    bhs BlastLastByte

*** One remaining pixel
    andb #$f0
    stb temp
    ldb ,x
    andb #$0f
    orb temp
    stb ,x
    bra Blast16NextLine

*** At least one full byte remaining
BlastLastByte
    stb ,x
    cmpa #3
    bne Blast16NextLine

*** Final remaining pixel
    andb #$f0
    stb temp
    ldb 1,x
    andb #$0f
    orb temp
    stb 1,x

Blast16NextLine
*** Prepare for the next line
    leay -1,y
    beq Blast16RecDone
    ldb skip
    abx
    tfr x,d
    bra Blast16Rec
Blast16RecDone
 }

  BlitterUnmapScreen();
}


/**
 * Gets rectanguler bitmap data on screen from (x, y)->(x+width-1, y+height-1)
 * in a format suitable BlitterDrawGraphics.
 * NO BOUNDARY CHECKING IS PERFORMED.
 *
 * @param buffer[out] buffer that has at least 1 + ((x + 1) * y) bytes.
 * @param x[in] x coordinate in bytes
 * @param y[in] y coordinate in pixels
 * @param width[in] width in bytes
 * @param height[in] height in pixels
 */
void BlitterGet(byte *buffer, byte x, byte y, byte width, byte height) {
  BlitterMapScreen();
  
  // Copy the rectangle to buffer
  for(byte jj=0; jj<height; jj++) {
    byte *offset = (byte *)(0x8000 + (((unsigned)(jj + y)) * 160) + x);
    for(byte ii=0; ii<width; ii++)
      *buffer++ = *offset++;
    *buffer++ = 0xff;
  }
  *buffer = 0xff;

  BlitterUnmapScreen();  
}


void BlitterInitGrafxDataNumberData(byte *buffer) {
  BlitterMapScreen();
  
  char text[2];
  text[1] = 0;
  for(byte ii='0'; ii<='9'; ii++) {
    text[0] = ii;
    BlitterFillRectangle(0, 0, 10, 8, 0);
    byte offset = (ii == '1') ? 2 : 0;
    BlitterDrawText(FontDataFontIndex, FontDataFontData, 3, 0, offset, 0, 0, text);
    BlitterGet(buffer, 0, 0, 5, 8);
    buffer += 49;
  }

  BlitterUnmapScreen();
}


#endif
