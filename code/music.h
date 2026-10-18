#ifndef music_H
#define music_H

//#include <stdlib.h>
#include <avr/io.h>


#define BUZZER				PE6
#define BUZZER_DDR			DDRE
#define BUZZER_PIN			PINE

// presuming a clock frequency of 2_000_000 and a timer prescaler of 1 and a 16-bit timer

#define	A0	36364
#define	A0s	34317
#define	B0	32394
#define	C1	30581
#define	C1s	28860
#define	D1	27241
#define	D1s	25714
#define	E1	24272
#define	F1	22910
#define	F1s	21622
#define	G1	20408
#define	G1s	19264
#define	A1	18182
#define	A1s	17161
#define	B1	16197
#define	C2	15288
#define	C2s	14430
#define	D2	13620
#define	D2s	12857
#define	E2	12134
#define	F2	11453
#define	F2s	10811
#define	G2	10204
#define	G2s	9631
#define	A2	9091
#define	A2s	8581
#define	B2	8099
#define	C3	7645
#define	C3s	7216
#define	D3	6811
#define	D3s	6428
#define	E3	6068
#define	F3	5727
#define	F3s	5405
#define	G3	5102
#define	G3s	4816
#define	A3	4545
#define	A3s	4290
#define	B3	4050
#define	C4	3822
#define	C4s	3608
#define	D4	3405
#define	D4s	3214
#define	E4	3034
#define	F4	2863
#define	F4s	2703
#define	G4	2551
#define	G4s	2408
#define	A4	2273
#define	A4s	2145
#define	B4	2025
#define	C5	1911
#define	C5s	1804
#define	D5	1703
#define	D5s	1607
#define	E5	1517
#define	F5	1432
#define	F5s	1351
#define	G5	1276
#define	G5s	1204
#define	A5	1136
#define	A5s	1073
#define	B5	1012
#define	C6	956
#define	C6s	902
#define	D6	851
#define	D6s	804
#define	E6	758
#define	F6	716
#define	F6s	676
#define	G6	638
#define	G6s	602
#define	A6	568
#define	A6s	536
#define	B6	506
#define	C7	478
#define	C7s	451
#define	D7	426
#define	D7s	402
#define	E7	379
#define	F7	358
#define	F7s	338
#define	G7	319
#define	G7s	301
#define	A7	284
#define	A7s	268
#define	B7	253
#define	C8	239

typedef struct {
	uint16_t pitch;
	float duration; // number of beats
}Notes;

typedef struct {
	uint16_t BPM;
	Notes *notes;
	int length; // length of notes array
	int note_index;
}Tune;

Notes default_notes[2];
Tune default_tune;

Notes game_over[16];
Tune game_over_tune;

Notes among_us[7];
Tune among_us_tune;

Notes victory[14];
Tune victory_tune;

Notes running_in_the_90s[25];
Tune running_in_the_90s_tune;

Notes sweet_dreams[32];
Tune sweet_dreams_tune;

Notes run[20];
Tune run_tune;

Notes chopin_waltz[19];
Tune chopin_waltz_tune;

//Notes fallen_down[23];
//Tune fallen_down_tune;

Notes snowy[28];
Tune snowy_tune;

extern uint16_t note_duration_counter;
extern uint16_t note_duration;
extern Tune CD_in_jukebox;
extern uint16_t current_pitch;

void start_song(Tune song_name);
void stop_song(void);

#endif