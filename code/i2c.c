//#include <avr/io.h>
#include "i2c.h"

void init_i2c(void) {
	SET_SCL_AS_OUTPUT;
	SET_SDA_AS_OUTPUT;
	SET_SCL_HIGH;
}

void start_i2c(void) {
	SET_SDA_AS_OUTPUT;
	SET_SCL_HIGH;
	SET_SDA_HIGH;
	_delay_us(1);
	SET_SDA_LOW;
	_delay_us(1);
	SET_SCL_LOW;
	//SET_SDA_HIGH; // maybe unnecessary
	_delay_us(1);
	//SET_SDA_AS_INPUT;
	//_delay_us(1);
	/*SET_SCL_HIGH;
	_delay_us(5);
	SET_SCL_LOW;
	_delay_us(1);*/
	
}

void stop_i2c(void) {
	SET_SDA_AS_OUTPUT;
	SET_SCL_HIGH;
	SET_SDA_LOW;
	_delay_us(1);
	SET_SDA_HIGH;
	_delay_us(1);
	//SET_SCL_LOW;
}




void send_byte(uint8_t data) {
	SET_SDA_AS_OUTPUT;
	_delay_us(1);
	//SET_SCL_LOW; // maybe
	for (int i = 7; i >= 0; i--) {
		SDA_PORT &= ~(1 << SDA);
		SDA_PORT |= ((data & (1 << i)) >> i) << SDA;
		_delay_us(1);
		SET_SCL_HIGH;
		_delay_us(1); // 1 us might not be enough though it should be by the datasheet
		SET_SCL_LOW;
		_delay_us(1);
	}
}

void receive_acknowledge_bit(void) {
	// acknowledge bit:
	SET_SDA_AS_INPUT;
	_delay_us(1);
	SET_SCL_HIGH;
	_delay_us(1); // 1 us might not be enough though it should be by the datasheet
	SET_SCL_LOW;
	_delay_us(1);
}

uint8_t receive_byte(void) {
	SET_SDA_AS_INPUT;
	_delay_us(1);
	uint8_t data = 0;
	for (int i = 7; i >= 0; i--) {
		SET_SCL_HIGH;
		_delay_us(1);
		data |= READ_SDA << i;
		SET_SCL_LOW;
		_delay_us(1);
	}
	return data;
}

void send_non_acknowledge_bit(void) {
	SET_SDA_AS_OUTPUT;
	_delay_us(1);
	SET_SCL_LOW;
	SET_SDA_HIGH;
	_delay_us(1);
	SET_SCL_HIGH;
	_delay_us(1);
	//SET_SDA_HIGH;
	//_delay_us(1);
	SET_SCL_LOW;
	_delay_us(1);
}
