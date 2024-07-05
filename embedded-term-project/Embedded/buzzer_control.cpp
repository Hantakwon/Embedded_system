#include "mbed.h"
#include "buzzer_control.h"

#define BUZZER_PIN PC_9

PwmOut buzzer(BUZZER_PIN);
#define NOTE_c_PERIOD 3830 // msec
#define NOTE_d_PERIOD 3400 // msec
#define NOTE_e_PERIOD 3038 // msec
#define NOTE_f_PERIOD 2864 // msec
#define NOTE_g_PERIOD 2550 // msec
#define NOTE_a_PERIOD 2272 // msec
#define NOTE_b_PERIOD 2028 // msec
#define NOTE_C_PERIOD 1912 

int note_periods[] = {
    NOTE_c_PERIOD, NOTE_d_PERIOD, NOTE_e_PERIOD, NOTE_f_PERIOD,
    NOTE_g_PERIOD, NOTE_a_PERIOD ,NOTE_b_PERIOD, NOTE_C_PERIOD
};

typedef enum {
    Note_c=0, Note_d, Note_e, Note_f,
    Note_g, Note_a, Note_b, Note_C
} note_t;

int openMelody[] = {
    Note_e, Note_d, Note_e, Note_f, Note_d, Note_e, Note_f
};

int closeMelody[] = {
    Note_e, Note_f, Note_g, Note_e, Note_f, Note_g, Note_e
};

int warningMelody[] = {
		Note_g, Note_g, Note_e, Note_e,
    Note_c, Note_c, Note_g, Note_g
};

Ticker ticker;
int currentNote = 0;
int* melody;
int melodyLength;

void playNote() {
    if (currentNote < melodyLength) {
        buzzer.period_us(note_periods[melody[currentNote]]);
        buzzer = 0.5;
        currentNote++;
    } else {
        ticker.detach();
        buzzer = 0;
    }
}

void playMelody(int* melodyArray, int length) {
    melody = melodyArray;
    melodyLength = length;
    currentNote = 0;
    ticker.attach(&playNote, 0.5); 
}

void playOpenMelody() {
    playMelody(openMelody, sizeof(openMelody) / sizeof(openMelody[0]));
}

void playLockMelody() {
    playMelody(closeMelody, sizeof(closeMelody) / sizeof(closeMelody[0]));
}

void playWarningMelody() {
		playMelody(warningMelody, sizeof(warningMelody) / sizeof(warningMelody[0]));
}
