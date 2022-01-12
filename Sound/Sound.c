#include <coco.h>



typedef struct SoundPacket {
    byte period;
    byte cycles;
    byte volume;
} SoundPacket;



SoundPacket song[] = {
    { 36, 109, 0x40 },
    { 32, 123, 0x40 },
    { 30, 131, 0x40 },
    { 36, 109, 0x40 },
    { 32, 123, 0x40 },
    { 30, 131, 0x40 },
    { 36, 109, 0x40 },
    { 32, 123, 0x40 },
    { 30, 131, 0x40 },
    { 32, 123, 0x40 },
    { 36, 109, 0x40 },
    { 32, 123, 0x40 },
    { 36, 109, 0x40 },
    {  0,   0, 0x00 }
};


static byte songIndex = 0xff;
static byte period = 0;
static byte cycles = 0;
static byte volume = 0;



/**
 * Function used for processing sound samples.
 */
interrupt void SoundInterruptHandler(void) {
    asm {
        lda     $ff92                           release interrupt
        inc     $ffbd                           cycle color palette

SoundInterruptHandlerLoadCycle
        lda     cycles                          cycle finished?
        bne     SoundInterruptHandlerPlaySound  no, keep playing it

* Goto the next note and put packet in temp space
        inc     songIndex                       move onto the next note
        ldb     songIndex                       load the index
        addb    songIndex                         convert to byte offest
        addb    songIndex                         (3 byte structure)
        leax    song                            load song pointer
        leax    b,x                               add offset
        ldd     ,x++                            xfer song packet to scratch
        sta     period
        stb     cycles
        ldb     ,x
        stb     volume

* Are we done playing the song? if so, lets start over
        bne     SoundInterruptHandlerPlaySound  if volume is zero, we are done
        clr     cycles                          clear cycles to begin new note
        lda     #$ff                            point soundIndex BEFORE first
        sta     songIndex                         packet
        bra     SoundInterruptHandlerLoadCycle  try again

SoundInterruptHandlerPlaySound
        ldb     volume
        bita    #1                              odd number of cycles?
        beq     SoundInterruptHandlerFinish     no, play rising edge
        clrb                                    play falling edge

SoundInterruptHandlerFinish
        stb     $ff20                           play the sound
        dec     cycles                          decrease remaining cycles
        clra                                    restart 12-bit timer
        ldb     period
        std     $ff94
    }
}


/**
 * Must be invoked before running other Sound* functions.
 */
void SoundInit(void) {
    // From Assembly Language Programming For The COCO 3, Laurence A. Tepolt
    // pp. 51
    asm {
        orcc    #$50                       lockout ints
        lda     #$34                        turn off
        ldx     #$ff00                         interrupts
        sta     1,x                              from
        sta     3,x                                all
        sta     $21,x                               PIA S
        ora     #8                          enable sound
        sta     $23,x
        lda     ,x                          bleed off
        lda     2,x                           interrupts
        lda     $20,x                           from 
        lda     $22,x                             PIA S
        lda     #$ee                        enable ACVC      
        sta     $90,x                         IRQ intrpts
        lda     #$20                            from
        sta     $92,x                             TMR 
        lda     $92,x                       bleed off IRQ
        lda     $93,x                       and FIRQ intrpts
        lda     $91,x
        anda    #$e0
        sta     $91,x
        lda     #$7e                        set up secondary
        leax    SoundInterruptHandler        jump
        sta     $10c                            table
        stx     $10d

        ldd     #0
        std     $ff94

        andcc   #$ef                      enable IRQ intrpts
    }
}




int main(void) {
    SoundInit();
    while(TRUE);
    return 0;
}
