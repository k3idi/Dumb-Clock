#ifndef I2C_H
#define I2C_H

#include <avr/io.h>
#include <util/delay.h>

#define SCL					PD0
#define SCL_PIN				PIND // PERHAPS UNUSED 
#define SCL_DDR				DDRD
#define SCL_PORT			PORTD
#define SDA					PD1
#define SDA_PIN				PIND
#define SDA_DDR				DDRD
#define SDA_PORT			PORTD

#define SET_SCL_AS_INPUT	(SCL_DDR &= ~(1 << SCL)) // IS THIS EVER NEEDED?
#define SET_SCL_AS_OUTPUT	(SCL_DDR |= (1 << SCL))
#define SET_SCL_LOW			(SCL_PORT &= ~(1 << SCL))
#define SET_SCL_HIGH		(SCL_PORT |= (1 << SCL))
#define SET_SDA_AS_INPUT	(SDA_DDR &= ~(1 << SDA))
#define SET_SDA_AS_OUTPUT	(SDA_DDR |= (1 << SDA))
#define SET_SDA_LOW			(SDA_PORT &= ~(1 << SDA))
#define SET_SDA_HIGH		(SDA_PORT |= (1 << SDA))
#define READ_SDA			((SDA_PIN & (1 << SDA)) >> SDA)

void init_i2c(void);

void start_i2c(void);

void stop_i2c(void);

void send_byte(uint8_t data);

void receive_acknowledge_bit(void);

uint8_t receive_byte(void);

void send_non_acknowledge_bit(void);

#endif