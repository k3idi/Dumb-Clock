#include "music.h"

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

Notes default_notes[2] = {{E4, 3}, {F3, 3}};
Tune default_tune = {180, &default_notes[0], ARRAY_SIZE(default_notes), 0};
	
Notes game_over[16] = {{D5, 0.5}, {C5, 0.5}, {A4s, 0.5}, {A4, 0.5}, {C5, 0.5}, {A4s, 0.5}, {A4, 0.5}, {G4, 0.5}, {A4s, 0.5}, {A4, 0.5}, {G4, 0.5}, {F4, 0.5}, {A4, 0.5}, {G4, 0.5}, {F4, 0.5}, {E4, 0.5}};
Tune game_over_tune = {300, &game_over[0], ARRAY_SIZE(game_over), 0};
	
Notes among_us[7] = {{C4, 1}, {D4s, 1}, {F4, 1}, {F4s, 1}, {F4, 1}, {D4s, 1}, {C4, 1}};
Tune among_us_tune = {240, &among_us[0], ARRAY_SIZE(among_us), 0};
	
Notes victory[14] = {{B3, 0.5}, {C4s, 0.5}, {D4s, 0.5}, {E4, 1.5}, {G4s, 0.5}, {B4, 0.5}, {G4s, 0.5}, {F4s, 1.5}, {G4s, 0.5}, {A4, 0.5}, {B4, 0.5}, {C5s, 1.5}, {D5s, 1.5}, {E5, 1.5}};
Tune victory_tune = {140, &victory[0], ARRAY_SIZE(victory), 0};
	
Notes running_in_the_90s[25] = {{A4, 0.25}, {E4, 0.25}, {A4, 0.25}, {E5, 0.25}, {D5, 0.25}, {C5, 0.25}, {A4, 0.75}, {E4, 0.25}, {A4, 0.25}, {E5, 0.25}, {D5, 0.25}, {C5, 0.25}, {A4, 0.5}, {C5, 0.25}, {B4, 0.25}, {A4, 0.25}, {C5, 0.5}, {B4, 0.25}, {G4, 0.5}, {C5, 0.25}, {B4, 0.25}, {A4, 0.25}, {D5, 0.5}, {B4, 0.25}, {G4, 0.5}};
Tune running_in_the_90s_tune = {83, &running_in_the_90s[0], ARRAY_SIZE(running_in_the_90s), 0};

Notes sweet_dreams[32] = {{C3, 0.5}, {G3, 0.5}, {C4, 0.5}, {G3, 0.5}, {D4s, 0.5}, {G3, 0.5}, {C4, 0.5}, {G3, 0.5}, {G3s, 0.5}, {D4s, 0.5}, {C5, 0.5}, {D4s, 0.5}, {G3, 0.5}, {D4, 0.5}, {A4s, 0.5}, {D4, 0.5}, {C3, 0.5}, {G3, 0.5}, {C4, 0.5}, {G3, 0.5}, {D4s, 0.5}, {G3, 0.5}, {C4, 0.5}, {G3, 0.5}, {G3s, 0.5}, {D4s, 0.5}, {C5, 0.5}, {D4s, 0.5}, {G3, 0.5}, {D4, 0.5}, {A4s, 0.5}, {D4, 0.5}};
Tune sweet_dreams_tune = {151, &sweet_dreams[0], ARRAY_SIZE(sweet_dreams), 0};

Notes run[20] = {{G1s, 0.75}, {G3s, 0.25}, {B3, 0.25}, {G1s, 0.25}, {F3s, 0.25}, {G3s, 0.25}, {A3s, 0.5}, {G1s, 0.5}, {F3s, 0.5}, {G3s, 0.5}, {G1s, 0.75}, {G3s, 0.25}, {B3, 0.25}, {G1s, 0.25}, {D3s, 0.25}, {F3s, 0.25}, {G3, 0.5}, {D1s, 0.5}, {G3s, 0.5}, {A3s, 0.5}};
Tune run_tune = {75, &run[0], ARRAY_SIZE(run), 0};

Notes chopin_waltz[19] = {{E3, 2}, {A3, 1}, {B3, 1}, {C4, 2}, {C4, 2}, {D4, 1}, {E4, 1}, {F4, 4}, {B3, 1}, {C4, 1}, {D4, 1}, {A4, 1}, {G4, 1}, {F4, 1}, {E4, 0.25}, {F4, 0.25}, {E4, 0.5}, {D4s, 1}, {E4, 4}};
Tune chopin_waltz_tune = {220, &chopin_waltz[0], ARRAY_SIZE(chopin_waltz), 0};

//Notes fallen_down[23] = {{F5s, 1}, {C5s, 1}, {F5s, 1}, {C5s, 1}, {F5s, 1}, {C5s, 1}, {F5s, 1}, {C5s, 1}, {F5s, 1}, {C5s, 1}, {F5s, 1}, {C5s, 1}, {B4, 1}, {A4, 1}, {C5s, 2}, {A4, 1}, {B4, 1}, {E5, 1}, {D5s, 1}, {E5, 1}, {F5s, 1}, {D5s, 1}, {B4, 1}};
//Tune fallen_down_tune = {300, &fallen_down[0], ARRAY_SIZE(fallen_down), 0};

Notes snowy[28] = {{D4, 3}, {D3, 0.5}, {G3, 0.5}, {A3, 1}, {B3, 1}, {A3, 0.25}, {B3, 0.25}, {A3, 0.5}, {G3, 1}, {D4, 3}, {D3, 0.5}, {G3, 0.5}, {A3, 1}, {B3, 1}, {A3, 0.25}, {B3, 0.25}, {A3, 0.5}, {G3, 1}, {C4s, 3}, {C3s, 0.5}, {E3, 0.5}, {F3s, 1}, {G3s, 1}, {A3, 0.25}, {B3, 0.25}, {A3, 1}, {D4, 0.5}, {B3, 3}};
Tune snowy_tune = {240, &snowy[0], ARRAY_SIZE(snowy), 0};

// I want to have a minimum of 0.25 of a beat and a maximum of 300 BPM. That means that the smallest unit is 20 steps in a second (300/60/0.25). 
// The maximum nr of consecutive beats per note is 4. If more are needed then just put multiple of these consecutively. The minimum BPM is 30. This means 0.125 steps in a second (30/60/4).

// I make interrupt every 10_000 ticks. if prescaler is 128, then count to 78 for frequency 200 (10_000 ticks out of 2_000_000)
// to get the required amount of steps then knowign the frequency of the timer is 200 and the frequency of the duration is x, then gotta count to 200/x 

uint16_t note_duration_counter = 0;
uint16_t note_duration;
Tune CD_in_jukebox;
uint16_t current_pitch;

void start_song(Tune song_name) { // move
	CD_in_jukebox = song_name;
	note_duration = (uint16_t)(20.0 / (float)((float)CD_in_jukebox.BPM / 60.0 / CD_in_jukebox.notes[CD_in_jukebox.note_index].duration)); //x = BPM / 60 / duration, note_duration = 200 / x
	TCCR2B = (1 << CS22) | (1 << CS21) | (1 << CS20); // start the timer with a prescaler of 1024 (controls note duration)
	current_pitch = CD_in_jukebox.notes[CD_in_jukebox.note_index].pitch;
	TCCR3B |= (1 << CS30); // start timer (controls buzzer frequency)
}

void stop_song(void) { // move
	TCCR2B = 0; // stop the timer
	TCNT2 = 0; // zero the timer
	TCCR3B &= ~((1 << CS32) | (1 << CS31) | (1 << CS30)); // stop buzzer frequency timer
	TCNT3 = 0;
	CD_in_jukebox.note_index = 0;
	note_duration_counter = 0;
}



