// encoder.c

#include <avr/io.h>
#include <stdbool.h>
#include "encoder.h"

bool encoder_1_currently_high = true;
bool encoder_2_currently_high = true;
int encoder_counter = 0;
bool encoder_value_changed = true;

void encoder_update(void) {
	if (ENCODER_1_LOW && encoder_1_currently_high) { // falling edge
		encoder_1_currently_high = false;
		if (ENCODER_2_HIGH) {
			encoder_counter++;
			encoder_value_changed = true;
		}
		} else if (ENCODER_1_HIGH && !encoder_1_currently_high) {
		encoder_1_currently_high = true;
	}
			
	if (ENCODER_2_LOW && encoder_2_currently_high) { // falling edge
		encoder_2_currently_high = false;
		if (ENCODER_1_HIGH) {
			encoder_counter--;
			encoder_value_changed = true;
		}
		} else if (ENCODER_2_HIGH && !encoder_2_currently_high) {
		encoder_2_currently_high = true;
	}
}


