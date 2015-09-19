/*
  Sound.c
  
  This software is for demonstration purposes only. Use at your own
  risk.

  Sound functions
*/


#ifndef _Sound_c
#define _Sound_c


#include "Sound.h"


/** Represents a packet of sound */
typedef struct SoundPacket {
  /** Duration in cycles - 0 means done. */
  unsigned duration;

  /** Current freq count */
  byte freqCount;    

  /** 0 means waiting for 0 to finish */
  byte mode;

  /** Time to wait for 0 swing */
  byte freq1;

  /** Time to wait for positive swing */
  byte freq2;

  /** Current volume */
  byte volume;
} SoundPacket;


/** Pointer to the sound packet */
#define SoundPtr ((SoundPacket *)0xe00)


void SoundPlay(unsigned duration, byte freq1, byte freq2, byte volume) {
  asm {
   ldx #$e00
   lda freq1
   sta SoundPacketFreq1,x
   lda freq2
   sta SoundPacketFreq2,x
   lda volume
   sta SoundPacketVolume,x
   ldd duration
   std SoundPacketDuration,x
  }
}


void SoundPlayAndWait(unsigned duration, byte freq1, byte freq2, byte volume) {
  SoundPlay(duration, freq1, freq2, volume);
  SoundWait();
}


void SoundWait() {
  while (SoundPtr->duration);
}


asm void SoundFirq() {
  asm { 
SoundPacketDuration equ $00
SoundPacketFreqCount equ $02
SoundPacketMode equ $03
SoundPacketFreq1 equ $04
SoundPacketFreq2 equ $05
SoundPacketVolume equ $06

    pshs d,x
    lda $ff93
    ldx #$e00

* Load duration
    ldd ,x
    beq SoundDone
    subd #1
    std ,x

* Dec freqCount, do nothing if not at zero
    dec SoundPacketFreqCount,x
    bne SoundDone

* Time to switch, check mode
    lda SoundPacketMode,x
    beq SoundSwingPositive

SoundSwingZero
    clra
    sta SoundPacketMode,x
    sta $ff20
    lda SoundPacketFreq1,x
    sta SoundPacketFreqCount,x
    bra SoundDone
    
SoundSwingPositive
    lda SoundPacketVolume,x
    sta SoundPacketMode,x
    sta $ff20
    lda SoundPacketFreq2,x
    sta SoundPacketFreqCount,x

SoundDone
    puls d,x
    rti
  }
}


void SoundInit() {
  // Clear memory
  memset(SoundPtr, 0, sizeof(SoundPtr));

  // Disable and turn off interrupts
  asm { orcc #$50 }
  byte disableInterrupts = 0x34;
  byte soundEnable = 0x8;
  *CoCoMiscPIA1ControlRegisterA = disableInterrupts;
  *CoCoMiscPIA1ControlRegisterB = disableInterrupts;
  *CoCoMiscPIA2ControlRegisterA = disableInterrupts;
  *CoCoMiscPIA2ControlRegisterB = disableInterrupts | soundEnable;

  // Bleed off existing interrupts
  byte val = *CoCoMiscPIA1DataRegisterA;
  val = *CoCoMiscPIA1DataRegisterB;
  val = *CoCoMiscPIA2DataRegisterA;
  val = *CoCoMiscPIA2DataRegisterB;

  // Setup the FIRQ jump
  CoCoMiscFIRQJumpVector[0] = 0x7e;
  CoCoMiscFIRQJumpVector[1] = (byte)(((unsigned)(SoundFirq)) >> 0x8);
  CoCoMiscFIRQJumpVector[2] = (byte)((unsigned)(SoundFirq)) & 0xff;

  // Enable FIRQ
  *CoCoMiscGIMETimer = 448;
  *CoCoMiscGIMEInit0 = 0x5e;
  *CoCoMiscGIMEInit1 |= 0x20;
  *CoCoMiscGIMEFIRQEnable = 0x20;
  
  // Bleed off GIME interrupts
  val = *CoCoMiscGIMEIRQEnable;
  val = *CoCoMiscGIMEFIRQEnable;

  // Enable interrupts
  asm { andcc #$aF }
  return;
}


#endif
