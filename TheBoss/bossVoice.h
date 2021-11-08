#include "commonHeads.h"

#ifndef BOSSVOICE_H
#define BOSSVOICE_H

// PWM Pins
#define SPEAKER 3					//PTB3

#define NOTE_CD notes[0]
#define NOTE_D notes[1]
#define NOTE_E notes[2]
#define NOTE_F notes[3]
#define NOTE_G notes[4]
#define NOTE_A notes[5]
#define NOTE_B notes[6]
#define NOTE_CU notes[7]
#define NOTE_DU notes[8]
#define NOTE_EU notes[9]
#define NOTE_FU notes[10]
#define NOTE_GU notes[11]
#define NOTE_AU notes[12]

//Audio Choice
#define SILENCE								0x00
#define CHALLENGE_TIME				0x01
#define ENDING_TIME						0x02
#define WIFI_CONNECT					0x03

//Voice Functions
void InitAudio(void);
void play_note(uint32_t freq);
void stop_music(void);
void play_end_song(void);
void play_moving_song(void);
void play_wifi_song(void);

#endif // !BOSSVOICE_H
