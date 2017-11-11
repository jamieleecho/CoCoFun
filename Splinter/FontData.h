/*
  FontData.h
  
  This software is for demonstration purposes only. Use at your own
  risk.

  Splinter font data.
*/


#ifndef _FontData_h
#define _FontData_h

#include <coco.h>


// Index into fontData. Maps chars 32->127 to offsets into fontData.
// -1 imples a glyph is not defined for that character.
extern unsigned FontDataFontIndex[];

// Each glyph is preceded by 2 bytes that define the rows and columns per font
// The remaining bytes define 1-bit data organized as bytes for each glyph
extern byte FontDataFontData[];


#endif
