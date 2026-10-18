#ifndef ENCODER_H
#define ENCODER_H

#include <avr/io.h>
#include <stdbool.h>

#define ENCODER_1			PA2
#define ENCODER_1_PIN		PINA
#define ENCODER_2			PA3
#define ENCODER_2_PIN		PINA

#define ENCODER_1_HIGH		ENCODER_1_PIN & (1 << ENCODER_1)
#define ENCODER_2_HIGH		ENCODER_2_PIN & (1 << ENCODER_2)
#define ENCODER_1_LOW		!(ENCODER_1_PIN & (1 << ENCODER_1))
#define ENCODER_2_LOW		!(ENCODER_2_PIN & (1 << ENCODER_2))

extern bool encoder_1_currently_high;
extern bool encoder_2_currently_high;
extern int encoder_counter;
extern bool encoder_value_changed;

void encoder_update(void);

#endif