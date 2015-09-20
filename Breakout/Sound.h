/*
  Sound.h
  
  This software is for demonstration purposes only. Use at your own
  risk.

  Sound functions
*/


#ifndef _Sound_h
#define _Sound_h


#include <coco.h>


/** Initialize the sound functions */
void SoundInit();

/**
 * Play a sound.
 * @param duration[in] duration in 500 micros
 * @param freq1[in] zero swing of sound in 250 micros
 * @param freq2[in] positive swing of sound in 250 micros
 * @param volume[in] Volume - must be a multiple of 4
 */
void SoundPlay(unsigned duration, byte freq1, byte freq2, byte volume);

/**
 * Play a sound and wait for it to finish.
 * @param duration[in] duration in 500 micros
 * @param freq1[in] zero swing of sound in 250 micros
 * @param freq2[in] positive swing of sound in 250 micros
 * @param volume[in] Volume - must be a multiple of 4
 */
void SoundPlayAndWait(unsigned duration, byte freq1, byte freq2, byte volume);

/** Wait for a sound to finish */
void SoundWait();

#endif
