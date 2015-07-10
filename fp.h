// Floating point functions that call Color Basic routines.
// Some functions require Extended Color Basic.
//
// By Pierre Sarrazin <http://sarrazip.com/>
//

#include <cmoc.h>

// Addresses of the two unpacked floating point accumulators used by
// the functions in this file.
//
#define FP0ADDR 0x004F
#define FP1ADDR 0x005C

// Fields of the two FPAs.
//
#define FP0EXP FP0ADDR
#define FP1EXP FP1ADDR
#define FP0SGN 0x0054
#define FP1SGN 0x0061
#define RESSGN 0x0062
#define FPSBYT 0x0063

// Sizes.
//
#define FPUNPACKEDSIZE  6  /* in bytes */
#define FPSAVEBUFSIZ   12  /* two accumulators */
#define FPSTRBUFSIZ    38  /* minimum buffer length to pass to fpwrite0() */


// Parses the given decimal floating point and stores it in FP0.
// Avoid passing a string that would trigger a Basic error message.
// Use capital E for exponent.
// Preserves FP1.
//
void fpparse0(char *asciiDecimal)
{
    char saveBuffer[FPUNPACKEDSIZE];
    memcpy(saveBuffer, (char *) FP1ADDR, FPUNPACKEDSIZE);  // protect against $B71D
    asm
    {
        ldx     asciiDecimal
        clrb
fpparse_10:             // get string length in B
        incb
        tst     ,x+
        bne     fpparse_10
        decb

        ldx     asciiDecimal
        pshs    u       // preserve frame ptr against Basic
        jsr     $B71D   // jump into Basic's VAL() function
        puls    u
    }
    memcpy((char *) FP1ADDR, saveBuffer, FPUNPACKEDSIZE);
}


// Initializes FP0 from a value in -128..127.
//
void fpfromchar(signed char value)
{
    asm
    {
        ldb     value
        jsr     $BC7C
    }
}


// Initializes FP0 from a value in 0..255.
//
void fpfromuchar(unsigned char value)
{
    asm
    {
        jsr     $B4F3
    }
}


// Initializes FP0 from a value in 0..65535.
//
void fpfromuint(unsigned int value)
{
    asm
    {
        jsr     $B4F4
    }
}


// Save FP0 in buffer[], unpacked.
//
void fpsave0(char buffer[FPUNPACKEDSIZE])
{
    memcpy(buffer, (char *) FP0ADDR, FPUNPACKEDSIZE);
}


// Save FP1 in buffer[], unpacked.
//
void fpsave1(char buffer[FPUNPACKEDSIZE])
{
    memcpy(buffer, (char *) FP1ADDR, FPUNPACKEDSIZE);
}


// Save FP0 and FP1 in buffer[], unpacked.
//
void fpsave01(char buffer[FPSAVEBUFSIZ])
{
    memcpy(buffer,                  (char *) FP0ADDR, FPUNPACKEDSIZE);
    memcpy(buffer + FPUNPACKEDSIZE, (char *) FP1ADDR, FPUNPACKEDSIZE);
}


// Load FP0 from buffer[], which must be unpacked.
//
void fpload0(char buffer[FPUNPACKEDSIZE])
{
    memcpy((char *) FP0ADDR, buffer, FPUNPACKEDSIZE);
}


// Load FP1 from buffer[], which must be unpacked.
//
void fpload1(char buffer[FPUNPACKEDSIZE])
{
    memcpy((char *) FP1ADDR, buffer, FPUNPACKEDSIZE);
}


// Load FP0 and FP1 from buffer[], which must be a pair of unpacked FPAs.
//
void fpload01(char buffer[FPSAVEBUFSIZ])
{
    memcpy((char *) FP0ADDR, buffer,                  FPUNPACKEDSIZE);
    memcpy((char *) FP1ADDR, buffer + FPUNPACKEDSIZE, FPUNPACKEDSIZE);
}


// Writes FP0 in ASCII decimal to a string buffer.
//
// dest: must point to a buffer of at least FPSTRBUFSIZ characters,
// including room for the '\0' terminator.
//
// Returns the address in the buffer of the first non-blank character.
// (A positive number's first character is a space. A negative number's
// first character is a minus sign.)
//
// Examples:
// char a[FPSTRBUFSIZ]; printf("%s\n", fpwrite0(a));
// If one wants to print the leading space of a positive number:
// char a[FPSTRBUFSIZ]; fpwrite0(a); printf("%s\n", a);
//
char *fpwrite0(char *dest)
{
    char saveBuffer[FPSAVEBUFSIZ];  // accumulators saved here, restored after calling Basic, because call trashes them
    fpsave01(saveBuffer);
    char *firstNonSpace;
    asm
    {
        pshs    u       // preserve frame ptr

        ldu     dest    // tell Basic where to write string
        jsr     $BDDC

        puls    u       // restore frame ptr

        ldx     dest
        ldb     ,x      // check 1st char
        cmpb    #32     // is it space?
        bne     fpdecimal_end   // no: number is negative
        leax    1,x     // skip space
fpdecimal_end:
        stx     firstNonSpace   // return addr of 1st non-space char
    }
    fpload01(saveBuffer);
    return firstNonSpace;
}


// Copies FP0 to FP1.
//
void fp0to1()
{
    asm
    {
        jsr     $BC5F
    }
}


// Copies FP1 to FP0.
//
void fp1to0()
{
    asm
    {
        jsr     $BC4A
    }
}


// Adds FP1 to FP0. Result in FP0.
// Trashes FP1.
//
void fpadd1to0()
{
    asm
    {
        // Compute sign of result.
        ldb     FP0SGN
        eorb    FP1SGN
        stb     RESSGN

        pshs    u      //TODO: needed?
        lda     $5C    // load exponent of FPA1
        ldb     $4F    // load exponent of FPA0
        jsr     $B9C5
        puls    u
    }
}


// Substracts FP1 from FP0. Result in FP0.
// Trashes FP1.
//
void fpsub1from0()
{
    asm
    {
        com     FP1SGN  // reduce FP0 - FP1 to FP0 + (-FP1)
    }
    fpadd1to0();
}


// Multiplies FP0 by FP1. Result in FP0.
// Trashes FP1.
//
void fpmul0by1()
{
    asm
    {
        // Compute sign of result.
        ldb     FP0SGN
        eorb    FP1SGN
        stb     RESSGN

        pshs    u
        lda     FP1EXP
        jsr     $BACC
        puls    u
    }
}


// Divides FP1 by FP0. Result in FP0.
//
void fpdiv1by0()
{
    asm
    {
        // Compute sign of result.
        ldb     FP0SGN
        eorb    FP1SGN
        stb     RESSGN

        pshs    u
        lda     FP1EXP
        jsr     $BB91
        puls    u
    }
}


// Negates FP0.
//
void fpneg0()
{
    asm
    {
        com     FP0SGN
    }
}


// Returns -1, 0 or +1 depending on the sign of FP0.
//
char fpsgn0()
{
    char sign;
    asm
    {
        jsr     $BC6D       // result in B (-1, 0, +1)
        stb     sign
    }
    return sign;
}


// Changes FP0 to its absolute value.
//
void fpabs0()
{
    asm
    {
        clr     FP0SGN
    }
}


// Computes FP1^FP0. Result in FP0.
// Trashes FP1.
// Requires Extended Color Basic.
//
void fppow()
{
    asm
    {
        clr     FPSBYT
        lda     FP1EXP      // for $8489
        ldb     FP0EXP      // set CC on FP0EXP for $8489
        pshs    u
        jsr     $8489
        puls    u
    }
}


// Computes the square root of FP0 and leaves the result in FP0.
// Trashes FP1.
// Requires Extended Color Basic.
//
void fpsqrt()
{
    asm
    {
        pshs    u
        jsr     $8480
        puls    u
    }
}


// Stores the integer part of FP0 in *result, if possible.
//
// Returns false if FP0 is out of the -32768..32767 range;
// *result is the undefined.
// Returns true upon success, with the integer value in *result.
//
// result: This pointer must not be null.
//
char fptoint(int *result)
{
    char success;
    asm
    {
        lda     FP0EXP
        cmpa    #128+16     // is absolute value > 2**15?
        bhi     fptoint_outofrange
        blo     fptoint_ok
        // Exponent is 16, so only allowed value is -32768.
        ldd     FP0EXP+1
        cmpd    #32768
        bne     fptoint_outofrange
fptoint_ok:
        jsr     $B3F0
        std     [result]
        ldb     #1          // success
        bra     fptoint_end
fptoint_outofrange:
        clrb
fptoint_end:
        stb     success
    }
    return success;
}


// Returns -1, 0 or +1 depending on whether FP0 is lower than, equal to
// or greater than FP1.
//
char fpcmp0(char rightSide[FPUNPACKEDSIZE])
{
    char result;
    asm
    {
        ldx     rightSide   // rightSide is a pointer, not an array, despite the []
        lda     5,x         // "pack" sign of rightSide into MS byte of mantissa
        ora     #$7F        // $00 or $FF becomes $7F or $FF
        anda    1,x
        sta     1,x
        jsr     $BC96       // compare FP0 to rightSide via X
        stb     result      // 0 if equal, 1 if FP0 > FP1, -1 otherwise
        lda     1,x         // restore bit 7 of MS byte of mantissa
        ora     #$80
        sta     1,x
    }
    return result;
}


// Computes (a - b) ** 2. Result in FP0.
// Trashes FP1.
//
void fpsqdiff(char a[FPUNPACKEDSIZE], char b[FPUNPACKEDSIZE])
{
    fpload0(b);
    fpload1(a);
    fpsub1from0();  // FP0 = b - a
    fp0to1();
    fpmul0by1();
}


// Computes the square of the distance of the 2D segment going from
// (a[0], a[1]) to (b[0], b[1]). Result in FP0.
// Trashes FP1.
//
void fp2dsqdist(char a[2][FPUNPACKEDSIZE], char b[2][FPUNPACKEDSIZE])
{
    char dx2[FPUNPACKEDSIZE];
    fpsqdiff(a[0], b[0]);  // square of x distance
    fpsave0(dx2);
    fpsqdiff(a[1], b[1]);  // square of y distance
    fpload1(dx2);
    fpadd1to0();
}


// Computes the distance of the 2D segment going from
// (a[0], a[1]) to (b[0], b[1]). Result in FP0.
// Trashes FP1.
//
void fp2ddist(char a[2][FPUNPACKEDSIZE], char b[2][FPUNPACKEDSIZE])
{
    fp2dsqdist(a, b);
    fpsqrt();
}
