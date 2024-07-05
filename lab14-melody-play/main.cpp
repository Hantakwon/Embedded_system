#include "mbed.h"

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
	NOTE_g_PERIOD, NOTE_a_PERIOD ,NOTE_a_PERIOD, NOTE_C_PERIOD
};

typedef enum {
	Note_c=0, Note_d, Note_e, Note_f,
	Note_g, Note_a, Note_b, Note_C
} note_t;

int melody[] = {
	Note_c, Note_c, Note_g, Note_g, Note_a, Note_a, Note_g
};

void setup() {
	buzzer.write(0);
}

int main() {
		int cnt = sizeof(melody) / sizeof(melody[0]);
		setup();
		while(1) {
			for (int i=0; i<cnt; i++) {
				buzzer.period_us(note_periods[melody[i]]);
				buzzer = 0.5;
				wait_ms(500);
			}
		buzzer = 0;
		wait(3.0);
		}
}
