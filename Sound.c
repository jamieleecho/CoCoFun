/*
  Sound.c
  
  This software is for demonstration purposes only. Use at your own
  risk.

  Sound functions
*/


#ifndef _Sound_c
#define _Sound_c


asm void SoundFirq() {
  asm { 
    pshs a
    lda $ff93
    lda Sound    
    beq Sound00
    sta $ff20
    clr Sound
    puls a
    rti
Sound00
    sta $ff20
    lda #$fc
    sta Sound
    puls a
    rti
Sound
    fdb 0
  }
}


void SoundInit() {
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
  *CoCoMiscGIMETimer = 3585;
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
