// Floating point functions that call Color Basic routines.
// Some functions require Extended Color Basic.
//
// By Pierre Sarrazin <http://sarrazip.com/>
// This file is in the public domain.
//
// Version 0.1.0 - 2016-??-?? - First public release.

#include <coco.h>

enum
{
    // Addresses of the two unpacked floating point accumulators used by
    // the functions in this file.
    //
    FP0ADDR = 0x004F,
    FP1ADDR = 0x005C,

    // Fields of the two FPAs.
    //
    FP0EXP = FP0ADDR,
    FP1EXP = FP1ADDR,
    FP0SGN = 0x0054,
    FP1SGN = 0x0061,
    RESSGN = 0x0062,
    FPSBYT = 0x0063,

    // Sizes.
    //
    FPUNPACKEDSIZE =  6,  // in bytes
    FPSTRBUFSIZ    = 38,  // minimum buffer length to pass to fpstr()
};


typedef struct Float
{
    byte buffer[FPUNPACKEDSIZE];
} Float;


// Save FP0 in buffer[], unpacked.
//
asm void fp0save(Float *buffer)
{
    asm
    {
        ldx     2,s     // buffer argument
        ldd     FP0ADDR
        std     ,x++
        ldd     2+FP0ADDR
        std     ,x++
        ldd     4+FP0ADDR
        std     ,x
    }
}


// Save FP1 in buffer[], unpacked.
//
asm void fp1save(Float *buffer)
{
    asm
    {
        ldx     2,s     // buffer argument
        ldd     FP1ADDR
        std     ,x++
        ldd     2+FP1ADDR
        std     ,x++
        ldd     4+FP1ADDR
        std     ,x
    }
}


// Save FP0 and FP1 in buffer[], unpacked.
//
asm void fp01save(Float buffer[2])
{
    asm
    {
        ldx     2,s
        ldd     FP0ADDR
        std     ,x++
        ldd     2+FP0ADDR
        std     ,x++
        ldd     4+FP0ADDR
        std     ,x++
        ldd     FP1ADDR
        std     ,x++
        ldd     2+FP1ADDR
        std     ,x++
        ldd     4+FP1ADDR
        std     ,x
    }
}


// Load FP0 from buffer[], which must be unpacked.
//
asm void fp0load(Float *buffer)
{
    asm
    {
        ldx     2,s     // buffer argument
        ldd     ,x++
        std     FP0ADDR
        ldd     ,x++
        std     2+FP0ADDR
        ldd     ,x
        std     4+FP0ADDR
    }
}


// Load FP1 from buffer[], which must be unpacked.
//
asm void fp1load(Float *buffer)
{
    asm
    {
        ldx     2,s     // buffer argument
        ldd     ,x++
        std     FP1ADDR
        ldd     ,x++
        std     2+FP1ADDR
        ldd     ,x
        std     4+FP1ADDR
    }
}


// Load FP0 and FP1 from buffer[], which must be a pair of unpacked FPAs.
//
asm void fp01load(Float buffer[2])
{
    asm
    {
        ldx     2,s     // buffer argument
        ldd     ,x++
        std     FP0ADDR
        ldd     ,x++
        std     2+FP0ADDR
        ldd     ,x++
        std     4+FP0ADDR
        ldd     ,x++
        std     FP1ADDR
        ldd     ,x++
        std     2+FP1ADDR
        ldd     ,x
        std     4+FP1ADDR
    }
}


// Copies FP0 to FP1.
//
asm void fp0tofp1()
{
    asm
    {
        jsr     $BC5F
    }
}


// Copies FP1 to FP0.
//
asm void fp1tofp0()
{
    asm
    {
        jsr     $BC4A
    }
}


// Swaps FP1 to FP0.
//
asm void fpswap()
{
    asm
    {
        ldd     FP0ADDR
        ldx     FP1ADDR
        std     FP1ADDR
        stx     FP0ADDR

        ldd     2+FP0ADDR
        ldx     2+FP1ADDR
        std     2+FP1ADDR
        stx     2+FP0ADDR

        ldd     4+FP0ADDR
        ldx     4+FP1ADDR
        std     4+FP1ADDR
        stx     4+FP0ADDR
    }
}


// Copies an accumulator to another.
//
asm void fpcpy(Float *dest, Float *src)
{
    asm
    {
        pshs    u
        ldx     6,s             // src
        ldu     4,s             // dest
        ldd     ,x++
        std     ,u++
        ldd     ,x++
        std     ,u++
        ldd     ,x
        std     ,u
        puls    u,pc
    }
}


// Parses the given decimal floating point and stores it in FP0.
// Avoid passing a string that would trigger a Basic error message.
// Use capital E for exponent.
// Preserves FP1.
//
void fpparse(char *asciiDecimal)
{
    Float saveBuffer;
    fp1save(&saveBuffer);  // protect against $B71D
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
    fp1load(&saveBuffer);
}


// Like fpparse, but returns the result in FP1. Preserves FP0.
//
void fp1parse(char *asciiDecimal)
{
    Float fp0backup;
    fp0save(&fp0backup);  // preserve FP0
    fpparse(asciiDecimal);  // parse input and store result in FP0
    fp0tofp1();  // return result in FP1
    fp0load(&fp0backup);  // restore FP0
}


// Negates FP0.
//
asm void fpneg()
{
    asm
    {
        com     FP0SGN
    }
}


// Initializes FP0 from a value in -128..127.
//
asm void fpfromchar(signed char value)
{
    asm
    {
        ldb     3,s
        jmp     $BC7C
    }
}


// Initializes FP0 from a value in 0..255.
//
asm void fpfromuchar(unsigned char value)
{
    asm
    {
        ldb     3,s
        jmp     $B4F3
    }
}


// Initializes FP0 from a signed value in -32768..32767.
//
asm void fpfromint(int value)
{
    asm
    {
        ldd     2,s
        jmp     $B4F4
    }
}


// Initializes FP0 from a signed value in -32768..32767.
//
asm void fpfromuint(unsigned value)
{
    // Based on routine at $B4F4, but without assuming D is signed.
    asm
    {
        clr     <$06    // set variable type (VALTYP) to numeric
        ldd     2,s
        std     <1+FP0ADDR      // +1 at end would let cpp introduce space
        ldb     #128+16 // 16 = exponent required
        orcc    #1      // set carry so value seen as non-negative (see $BA18)
        jmp     $BC86   // continue with rest of routine started at $B4F4
    }
}


// Color Basic packed float constants:
#define FP_PACKED_MINUS_32768           0xB3DF
#define FP_PACKED_1                     0xBAC5
#define FP_PACKED_10                    0xBB7D
#define FP_PACKED_99999999_POINT_9      0xBDB6
#define FP_PACKED_999999999             0xBDBB
#define FP_PACKED_1E9                   0xBDC0
#define FP_PACKED_ONE_HALF              0xBEC0
#define FP_PACKED_TWO_PI                0xBFBD
#define FP_PACKED_ONE_QUARTER           0xBFC2

// Extended Color Basic packed float constants:
#define FP_PACKED_PI_OVER_2             0x83AB
#define FP_PACKED_HALF_SQRT_2           0x8432
#define FP_PACKED_SQRT_2                0x8437
#define FP_PACKED_MINUS_ONE_HALF        0x843C
#define FP_PACKED_LOG_2                 0x8441

// Loads FP0 from a packed (5-byte) float at the given address.
// The above #defines give the addresses of some notable numbers
// defined by Color Basic.
//
asm void fpfrompacked(byte *packedFloat)
{
    asm
    {
        ldx     2,s
        jsr     $BC14   // copy packed float at X into FP0
    }
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
// char a[FPSTRBUFSIZ]; printf("%s\n", fpstr(a));
// If one wants to print the leading space of a positive number:
// char a[FPSTRBUFSIZ]; fpstr(a); printf("%s\n", a);
//
char *fpstr(char *dest)
{
    Float saveBuffer[2];  // accumulators saved here, restored after calling Basic, because call trashes them
    fp01save(saveBuffer);
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
        bne     fpstr_end   // no: number is negative
        leax    1,x     // skip space
fpstr_end:
        stx     firstNonSpace   // return addr of 1st non-space char
    }
    fp01load(saveBuffer);
    return firstNonSpace;
}


// Prints the given number to the current output device.
// Uses FP0.
//
void fpprint(Float *fp)
{
    fp0load(fp);
    char buf[FPSTRBUFSIZ];
    char *p = fpstr(buf);
    size_t len = strlen(p);
    putstr(p, len);
}


// Adds FP1 to FP0. Result in FP0.
// Trashes FP1.
//
asm void fpadd()
{
    asm
    {
        // Compute sign of result.
        ldb     FP0SGN
        eorb    FP1SGN
        stb     RESSGN

        pshs    u
        lda     $5C    // load exponent of FPA1
        ldb     $4F    // load exponent of FPA0
        jsr     $B9C5
        puls    u
    }
}


// Substracts FP1 from FP0. Result in FP0, i.e., FP0 -= FP1.
// Trashes FP1.
//
asm void fpsub()
{
    asm
    {
        com     FP1SGN  // reduce FP0 - FP1 to FP0 + (-FP1)
        lbra    fpadd
    }
}


// Multiplies FP0 by FP1. Result in FP0.
// Trashes FP1.
//
asm void fpmul()
{
    asm
    {
        // Compute sign of result, as in $BB2F.
        ldb     FP0SGN
        eorb    FP1SGN
        stb     RESSGN

        pshs    u
        lda     FP1EXP
        ldb     FP0EXP          // as in $BB2F; sets N and Z
        jsr     $BACC           // starts with BEQ instruction
        puls    u
    }
}


// Divides FP1 by FP0. Result in FP0, i.e., FP0 = FP1 / FP0.
// FP0 must not be zero initially.
//
asm void fpdiv()
{
    asm
    {
        // Compute sign of result.
        ldb     FP0SGN
        eorb    FP1SGN
        stb     RESSGN

        pshs    u
        lda     FP1EXP
        ldb     FP0EXP          // as in $BB2F; sets N and Z
        jsr     $BB91           // starts with BEQ instruction
        puls    u
    }
}


// Returns -1, 0 or +1 depending on the sign of FP0.
//
char asm fpsgn()
{
    asm
    {
        jmp     $BC6D       // result in B (-1, 0, +1)
    }
}


#define fp0sgn() (fpsgn())


// Returns -1, 0 or +1 depending on the sign of FP1.
//
char asm fp1sgn()
{
    asm
    {
        ldb     FP1EXP
        beq     fp1sgn_end
        ldb     FP1SGN
        rolb
        ldb     #$ff
        bcs     fp1sgn_end
        negb
fp1sgn_end:
    }
}


// Changes FP0 to its absolute value.
//
asm void fpabs()
{
    asm
    {
        clr     FP0SGN
    }
}


#define fp0abs() (fpabs())


// Changes FP0 to its absolute value.
//
asm void fp1abs()
{
    asm
    {
        clr     FP1SGN
    }
}


// Computes FP1^FP0. Result in FP0.
// Trashes FP1.
// Requires Extended Color Basic.
//
asm void fppow()
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
// FP0 must not be negative (or Color Basic will abort with ?FC ERROR).
// Trashes FP1.
// Requires Extended Color Basic.
//
asm void fpsqrt()
{
    asm
    {
        pshs    u
        jsr     $8480
        puls    u
    }
}


// Stores the signed integer part of FP0 in *result, if possible.
//
// Returns false if FP0 is out of the -32767..32767 range;
// *result is then undefined.
// Returns true upon success, with the integer value in *result.
//
// result: This pointer must not be null.
//
// Trashes FP0.
//
// CAUTION: Rounds negative fractional numbers DOWN.
//          This means -3.1 will yield -4.
//
byte asm fptoint(int *result)
{
    asm
    {
        lda     FP0EXP
        cmpa    #128+16     // is absolute value > 2**16?
        bhi     fptoint_outofrange
        blo     fptoint_ok
        // Exponent is 16, so only allowed value is -32768.
        ldd     1+FP0EXP    // +1 at end would let gpp introduce space
        cmpd    #32768
        bhi     fptoint_outofrange
        blo     fptoint_ok
        tst     FP0SGN
        bpl     fptoint_outofrange      // got +32768: won't fit int
fptoint_ok:
        jsr     $B3F0
        std     [2,s]
        ldb     #1          // success
        bra     fptoint_end
fptoint_outofrange:
        clrb
fptoint_end:
    }
}


// Stores the unsigned integer part of FP0 in *result, if possible.
//
// Returns false if FP0 is out of the 0..65535 range;
// *result is then undefined.
// Returns true upon success, with the integer value in *result.
//
// result: This pointer must not be null.
//
// Trashes FP0.
//
byte asm fptouint(unsigned int *result)
{
    asm
    {
        ldb     FP0EXP
        beq     fptouint_ok             // FP0 is zero
        tst     FP0SGN
        bmi     fptouint_outofrange     // fail if FP0 < 0
        cmpb    #128+16
        bhi     fptouint_outofrange     // fail if more than 16 bits required
fptouint_ok:
        jsr     $BCC8           // denormalize FP0
        ldd     3+FP0EXP
        std     [2,s]
        ldb     #1              // success
        bra     fptouint_end
fptouint_outofrange:
        clrb
fptouint_end:
    }
}


// Returns -1, 0 or +1 depending on whether FP0 is lower than, equal to
// or greater than 'rightSide'.
//
char fpcmp(Float *rightSide)
{
    char result;
    asm
    {
        ldx     rightSide
        lda     5,x         // "pack" sign of rightSide into MS byte of mantissa
        ora     #$7F        // $00 or $FF becomes $7F or $FF
        anda    1,x
        sta     1,x
        jsr     $BC96       // compare FP0 to rightSide via X
        stb     result      // 0 if equal, 1 if FP0 > rightSide, -1 otherwise
        lda     1,x         // restore bit 7 of MS byte of mantissa of rightSide
        ora     #$80
        sta     1,x
    }
    return result;
}


// Compute sin(FP0), where FP0 is an angle in radians.
// Result in FP0. Trashes FP1.
//
asm void fpsin()
{
    asm
    {
        pshs    u       // preserve frame ptr
        jsr     $BF78
        puls    u
    }
}


// Compute cos(FP0), where FP0 is an angle in radians.
// Result in FP0. Trashes FP1.
// Requires Extended Color Basic.
//
asm void fpcos()
{
    asm
    {
        pshs    u       // preserve frame ptr
        jsr     $8378
        puls    u
    }
}


// Compute tan(FP0), where FP0 is an angle in radians.
// Result in FP0. Trashes FP1.
// Requires Extended Color Basic.
//
asm void fptan()
{
    asm
    {
        pshs    u       // preserve frame ptr
        jsr     $8381
        puls    u
    }
}


// Compute atn(FP0) (the arc in radians whose tangent is FP0).
// Result (between -pi/2 and +pi/2) in FP0. Trashes FP1.
// Requires Extended Color Basic.
//
asm void fpatn()
{
    asm
    {
        pshs    u       // preserve frame ptr
        jsr     $83B0
        puls    u
    }
}


// Compute log(FP0) (natural logarithm), where FP0 is positive.
// Result in FP0. Trashes FP1.
// Requires Extended Color Basic.
//
asm void fplog()
{
    asm
    {
        pshs    u       // preserve frame ptr
        jsr     $8446
        puls    u
    }
}


// Compute exp(FP0) (exponential).
// Result in FP0. Trashes FP1.
// Requires Extended Color Basic.
//
asm void fpexp()
{
    asm
    {
        pshs    u       // preserve frame ptr
        jsr     $84F2
        puls    u
    }
}


// Rounds FP0 down.
//
asm void fpint()
{
    asm
    {
        pshs    u       // preserve frame ptr
        jsr     $BCEE
        puls    u
    }
}


// Determines if FP0 is an integer or not.
//
char fpisint()
{
    Float orig;
    fp0save(&orig);
    fpint();
    return fpcmp(&orig) == 0;
}


// Rounds FP0 towards zero, i.e., positive numers are rounded down
// and negative numbers are rounded up.
//
asm void fpfix()
{
    asm
    {
        pshs    u       // preserve frame ptr
        jsr     $8524
        puls    u
    }
}
