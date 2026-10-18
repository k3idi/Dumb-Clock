// timer.c

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include "timer.h"
#include "lcd.h"
#include "music.h"


uint32_t timer0_counter_1 = 0; // ticks
uint32_t time_passed = 0; // microseconds

void init_timer_0(void) { // used to see how much time has passed, for lcd led PWM and
	TCCR0A = 0;
	TCCR0B = 0; // clock stopped for now.
	
	TCNT0 = 0;
	TIMSK0 = (1 << TOIE0); // overflow interrupt enabled
	
	TCCR0B |= (1 << CS00); // start clock.
}

void init_timer_1(void) { // used for lcd colour channel PWM duty cycle
	TCCR1A = 0; // 00 00 00 00 COM3A1:COM3A0 COM3B1:COM3B0 COM3C1:COM3C0 WGM31:WGM30
	TCCR1B = 0; // 00 - 00 000 : the clock is stopped right now (000) ICNC3:ICES3 - WGM33:WGM32 CS32:CS31:CS30 (set CS to 001 to start the timer with prescaler 1)
	TCCR1C = 0; // nothing cool, just force output compare for all channels disabled or whatever
	TIMSK1 = (1 << OCIE1C) | (1 << OCIE1B) | (1 << OCIE1A); // -- 0 - 1 1 1 0 : input capture interrupt disabled, output compare A, B and C interrupt enabled, overflow interrupt disabled
	
	TCNT1 = 0; // zero the timer
	
	OCR1A = blue_duty_cycle;
	OCR1B = red_duty_cycle;
	OCR1C = green_duty_cycle;
	
	TCCR1B |= (1 << CS10); // start the timer with clock prescaler 1
}

void init_timer_2(void) {
	// 250 to 2000 Hz for buzzer, means counting to 1000 to 8000 with prescaler 1. the maximum value of an 8-bit counter is 256.
	
	TCCR2A = 0; // COM2A1 COM2A0 COM2B1 COM2B0 - - WGM21 WGM20, COM is compare output mode (00 means normal port operation), WGM is waveform generation mode (000 - normal mode)
	TCCR2B = 0; // FOC2A FOC2B – – WGM22 CS22 CS21 CS20, clock prescaler 1, but right now clock is stopped (CS = 000)
	ASSR = 0; // – EXCLK AS2 TCN2UB OCR2AUB OCR2BUB TCR2AUB TCR2BUB , asynchronous clock source not connected
	TIFR2 = 0; // clear flags
	TCNT2 = 0;
	
	TIMSK2 = (1 << OCIE2A); // – – – – – OCIE2B OCIE2A TOIE2, output compare A interrupt enabled
	OCR2A = 98;
}

void init_timer_3(void) {
	// WGM = 0000 : normal operation
	TCCR3A = 0; // 00 00 00 00 COM3A1:COM3A0 COM3B1:COM3B0 COM3C1:COM3C0 WGM31:WGM30
	TCCR3B = 0; // 00 - 00 000 : the clock is stopped right now (000) ICNC3:ICES3 - WGM33:WGM32 CS32:CS31:CS30 (set CS to 001 to start the timer with prescaler 1)
	TCCR3C = 0; // nothing cool, just force output compare for all channels disabled or whatever
	TIMSK3 = (1 << OCIE3A); // -- 0 - 0 0 1 0 : input capture interrupt disabled, output compare A interrupt enabled, B and C disabled, overflow interrupt disabled

	TCNT3 = 0; // zero the timer

	OCR3A = 4545 / 2;

	TIFR3 = 0; // clear all flags, can check for this thing's necessity and the way i clear falgs, cause in the datasheet it says to write 1s on the register to clear them
}

ISR (TIMER0_OVF_vect) {
	timer0_counter_1 += 256;
	if (timer0_counter_1 >= PWM_PERIOD) { // maybe get this out of the interrupt, but maybe not because otherwise it could take too long to execute cause i have all sorts of crap in main()
		timer0_counter_1 = 0;
		TCCR1B &= ~(1 << CS10); // stop timer
		TCCR0B &= ~(1 << CS00); // stop timer
		BLUE_PORT |= (1 << BLUE_PIN_NR);
		RED_PORT |= (1 << RED_PIN_NR);
		GREEN_PORT |= (1 << GREEN_PIN_NR);
		TIMSK1 |= (1 << OCIE1A) | (1 << OCIE1B) | (1 << OCIE1C); // enable output compare A, B and C interrupts
		TCNT1 = 0; // zero the timer
		TCNT0 = 0; // zero the timer
		OCR1A = blue_duty_cycle;
		OCR1B = red_duty_cycle;
		OCR1C = green_duty_cycle;
		TCCR1B |= (1 << CS10); // start timer
		TCCR0B |= (1 << CS00); // start timer
	}
	time_passed += 128;
}

ISR (TIMER1_COMPA_vect) {
	BLUE_PIN = (1 << BLUE_PIN_NR); // toggle pin
	TIMSK1 &= ~(1 << OCIE1A); // disable output compare A interrupt
}

ISR (TIMER1_COMPB_vect) {
	RED_PIN = (1 << RED_PIN_NR); // toggle pin
	TIMSK1 &= ~(1 << OCIE1B); // disable output compare B interrupt
}

ISR (TIMER1_COMPC_vect) {
	GREEN_PIN = (1 << GREEN_PIN_NR); // toggle pin
	TIMSK1 &= ~(1 << OCIE1C); // disable output compare C interrupt
}

ISR (TIMER3_COMPA_vect) {
	BUZZER_PIN = (1 << BUZZER); // toggle pin
	TCNT3 = 0; // zero the timer
	OCR3A = current_pitch;
}

ISR (TIMER2_COMPA_vect) {
	TCCR2B = 0; // stop the timer
	TCNT2 = 0; // zero the timer
	
	note_duration_counter++;
	bool song_over = false;
	if (note_duration_counter == note_duration) {
		note_duration_counter = 0;
		CD_in_jukebox.note_index++;
		if (CD_in_jukebox.note_index == CD_in_jukebox.length) {
			song_over = true;
			CD_in_jukebox.note_index = 0;
			TCCR3B &= ~((1 << CS32) | (1 << CS31) | (1 << CS30)); // stop buzzer frequency timer
			TCNT3 = 0; // zero buzzer frequency timer
			} else {
			note_duration = (uint16_t)(20.0 / (float)((float)CD_in_jukebox.BPM / 60.0 / CD_in_jukebox.notes[CD_in_jukebox.note_index].duration));
			current_pitch = CD_in_jukebox.notes[CD_in_jukebox.note_index].pitch;
		}
		
	}
	if (!song_over) {
		TCCR2B = (1 << CS22) | (1 << CS21) | (1 << CS20); // start the timer with a prescaler of 1024 (controls note duration)
	}
}





