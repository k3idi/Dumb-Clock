// LCD.c

//#include <avr/io.h>
#include <avr/io.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcd.h"
#include "string.h"


void adjust_lcd_dimness(void) {
	uint16_t longest_duty_cycle = blue_duty_cycle * (blue_duty_cycle > red_duty_cycle && blue_duty_cycle > green_duty_cycle) + red_duty_cycle * (red_duty_cycle > blue_duty_cycle && red_duty_cycle > green_duty_cycle) +
	green_duty_cycle * (green_duty_cycle > blue_duty_cycle && green_duty_cycle > red_duty_cycle);
	blue_duty_cycle = (uint16_t)((float)blue_duty_cycle / (float)longest_duty_cycle * PWM_PERIOD * lcd_dimness);
	red_duty_cycle = (uint16_t)((float)red_duty_cycle / (float)longest_duty_cycle * PWM_PERIOD * lcd_dimness);
	green_duty_cycle = (uint16_t)((float)green_duty_cycle / (float)longest_duty_cycle * PWM_PERIOD * lcd_dimness);
}

void randomize_backlight_colour(void) {
	blue_duty_cycle = rand() % PWM_PERIOD;
	red_duty_cycle = rand() % PWM_PERIOD;
	green_duty_cycle = rand() % PWM_PERIOD;
	adjust_lcd_dimness();
}

void set_lcd_parameters(void) {
	EN_low;
	RS_low;
	RW_low;
	DB7_low;
	DB6_low;
	DB5_high;
	DB4_high; // high = 8 bit interface, set low for 4 bit interface
	DB3_high; // high = 2 line lcd, set low for 1 line lcd
	DB2_low; // low = 5x8 font size, set high for 5x11 font size
	EN_high;
	_delay_us(100);
	EN_low;
	_delay_us(100);
}

void set_cursor_increment(void) {
	EN_low;
	RS_low;
	RW_low;
	DB7_low;
	DB6_low;
	DB5_low;
	DB4_low;
	DB3_low;
	DB2_high;
	DB1_low;
	DB0_low;
	EN_high;
	_delay_us(100);
	EN_low;
	_delay_us(100);
}

void set_cursor_decrement(void) {
	EN_low;
	RS_low;
	RW_low;
	DB7_low;
	DB6_low;
	DB5_low;
	DB4_low;
	DB3_low;
	DB2_high;
	DB1_high;
	DB0_low;
	EN_high;
	_delay_us(100);
	EN_low;
	_delay_us(100);
}

void init_lcd(void) {
	_delay_ms(100);
	RED_DDR |= (1 << RED_PIN_NR);
	RED_PORT &= ~(1 << RED_PIN_NR);
	BLUE_DDR |= (1 << BLUE_PIN_NR);
	BLUE_PORT &= ~(1 << BLUE_PIN_NR);
	GREEN_DDR |= (1 << GREEN_PIN_NR);
	GREEN_PORT &= ~(1 << GREEN_PIN_NR);
	RS_DDR |= (1 << RS);
	RW_DDR |= (1 << RW);
	EN_DDR |= (1 << EN);
	DB7_DDR |= (1 << DB7);
	DB6_DDR |= (1 << DB6);
	DB5_DDR |= (1 << DB5);
	DB4_DDR |= (1 << DB4);
	DB3_DDR |= (1 << DB3);
	DB2_DDR |= (1 << DB2);
	DB1_DDR |= (1 << DB1);
	DB0_DDR |= (1 << DB0);
	set_lcd_parameters();
	_delay_ms(10);
	set_lcd_parameters();
	_delay_ms(10);
	set_lcd_parameters();
	_delay_ms(10);
	set_cursor_increment();
	
	blue_duty_cycle = 8000;
	red_duty_cycle = 1000;
	green_duty_cycle = 1000;
	lcd_dimness = 1;
}

void clear_display(void) {
	EN_low;
	RS_low;
	RW_low;
	DB7_low;
	DB6_low;
	DB5_low;
	DB4_low;
	DB3_low;
	DB2_low;
	DB1_low;
	DB0_high;
	EN_high;
	_delay_us(2000);
	EN_low;
	_delay_us(2000);
}

void return_home(void) {
	EN_low;
	RS_low;
	RW_low;
	DB7_low;
	DB6_low;
	DB5_low;
	DB4_low;
	DB3_low;
	DB2_low;
	DB1_high;
	EN_high;
	_delay_us(2000);
	EN_low;
	_delay_us(2000);
}

void display_on(void) {
	EN_low;
	RS_low;
	RW_low;
	DB7_low;
	DB6_low;
	DB5_low;
	DB4_low;
	DB3_high;
	DB2_high;
	DB1_low;
	DB0_low;
	EN_high;
	_delay_us(100);
	EN_low;
	_delay_us(100);
}

void display_off(void) {
	EN_low;
	RS_low;
	RW_low;
	DB7_low;
	DB6_low;
	DB5_low;
	DB4_low;
	DB3_high;
	DB2_low;
	DB1_low;
	DB0_low;
	EN_high;
	_delay_us(100);
	EN_low;
	_delay_us(100);
}

void cursor_on(void) {
	EN_low;
	RS_low;
	RW_low;
	DB7_low;
	DB6_low;
	DB5_low;
	DB4_low;
	DB3_high;
	DB2_high;
	DB1_high;
	DB0_low;
	EN_high;
	_delay_us(100);
	EN_low;
	_delay_us(100);
}

void cursor_off(void) {
	display_on();
}

void blinking_cursor_on(void) {
	EN_low;
	RS_low;
	RW_low;
	DB7_low;
	DB6_low;
	DB5_low;
	DB4_low;
	DB3_high;
	DB2_high;
	DB1_high;
	DB0_low;
	EN_high;
	_delay_us(100);
	EN_low;
	_delay_us(100);
}

void blinking_cursor_off(void) {
	display_on();
}

void move_cursor_right(void) {
	EN_low;
	RS_low;
	RW_low;
	DB7_low;
	DB6_low;
	DB5_low;
	DB4_high;
	DB3_low;
	DB2_high;
	EN_high;
	_delay_us(100);
	EN_low;
	_delay_us(100);
}

void move_cursor_left(void) {
	EN_low;
	RS_low;
	RW_low;
	DB7_low;
	DB6_low;
	DB5_low;
	DB4_high;
	DB3_low;
	DB2_low;
	EN_high;
	_delay_us(100);
	EN_low;
	_delay_us(100);
}

void shift_display_right(void) {
	EN_low;
	RS_low;
	RW_low;
	DB7_low;
	DB6_low;
	DB5_low;
	DB4_high;
	DB3_high;
	DB2_high;
	EN_high;
	_delay_us(100);
	EN_low;
	_delay_us(100);
}

void shift_display_left(void) {
	EN_low;
	RS_low;
	RW_low;
	DB7_low;
	DB6_low;
	DB5_low;
	DB4_high;
	DB3_high;
	DB2_low;
	EN_high;
	_delay_us(100);
	EN_low;
	_delay_us(100);
}

void set_cgram_address(uint8_t address) {
	EN_low;
	RS_low;
	RW_low;
	DB7_low;
	DB6_high;
	DB5_low;
	DB4_low;
	DB3_low;
	DB2_low;
	DB1_low;
	DB0_low;

	DB0_PORT |= (((address & 0b00000001 ) >> 0) << DB0);
	DB1_PORT |= (((address & 0b00000010 ) >> 1) << DB1);
	DB2_PORT |= (((address & 0b00000100 ) >> 2) << DB2);
	DB3_PORT |= (((address & 0b00001000 ) >> 3) << DB3);
	DB4_PORT |= (((address & 0b00010000 ) >> 4) << DB4);
	DB5_PORT |= (((address & 0b00100000 ) >> 5) << DB5);
	
	EN_high;
	_delay_us(100);
	EN_low;
	_delay_us(100);
}

void set_ddram_address(uint8_t address) {
	EN_low;
	RS_low;
	RW_low;
	DB7_high;
	DB6_low;
	DB5_low;
	DB4_low;
	DB3_low;
	DB2_low;
	DB1_low;
	DB0_low;

	DB0_PORT |= (((address & 0b00000001 ) >> 0) << DB0);
	DB1_PORT |= (((address & 0b00000010 ) >> 1) << DB1);
	DB2_PORT |= (((address & 0b00000100 ) >> 2) << DB2);
	DB3_PORT |= (((address & 0b00001000 ) >> 3) << DB3);
	DB4_PORT |= (((address & 0b00010000 ) >> 4) << DB4);
	DB5_PORT |= (((address & 0b00100000 ) >> 5) << DB5);
	DB6_PORT |= (((address & 0b01000000 ) >> 6) << DB6);
	
	EN_high;
	_delay_us(100);
	EN_low;
	_delay_us(100);
}

uint8_t read_busy_flag(void) {
	DB7_DDR &= ~(1 << DB7);
	DB6_DDR &= ~(1 << DB6);
	DB5_DDR &= ~(1 << DB5);
	DB4_DDR &= ~(1 << DB4);
	DB3_DDR &= ~(1 << DB3);
	DB2_DDR &= ~(1 << DB2);
	DB1_DDR &= ~(1 << DB1);
	DB0_DDR &= ~(1 << DB0);
	EN_low;
	RS_low;
	RW_high;

	EN_high;
	_delay_us(100);
	EN_low;
	_delay_us(100);
	
	uint8_t busy_flag = (DB7_PIN & (1 << DB7)) >> DB7;
	
	DB7_DDR |= (1 << DB7);
	DB6_DDR |= (1 << DB6);
	DB5_DDR |= (1 << DB5);
	DB4_DDR |= (1 << DB4);
	DB3_DDR |= (1 << DB3);
	DB2_DDR |= (1 << DB2);
	DB1_DDR |= (1 << DB1);
	DB0_DDR |= (1 << DB0);
	
	return busy_flag;
}

uint8_t read_address_counter(void) {
	DB7_DDR &= ~(1 << DB7);
	DB6_DDR &= ~(1 << DB6);
	DB5_DDR &= ~(1 << DB5);
	DB4_DDR &= ~(1 << DB4);
	DB3_DDR &= ~(1 << DB3);
	DB2_DDR &= ~(1 << DB2);
	DB1_DDR &= ~(1 << DB1);
	DB0_DDR &= ~(1 << DB0);
	EN_low;
	RS_low;
	RW_high;

	EN_high;
	_delay_us(100);
	EN_low;
	_delay_us(100);
	
	uint8_t address_counter = 0;
	address_counter += ((DB6_PIN & (1 << DB6)) >> DB6) << 6;
	address_counter += ((DB5_PIN & (1 << DB5)) >> DB5) << 5;
	address_counter += ((DB4_PIN & (1 << DB4)) >> DB4) << 4;
	address_counter += ((DB3_PIN & (1 << DB3)) >> DB3) << 3;
	address_counter += ((DB2_PIN & (1 << DB2)) >> DB2) << 2;
	address_counter += ((DB1_PIN & (1 << DB1)) >> DB1) << 1;
	address_counter += ((DB0_PIN & (1 << DB0)) >> DB0);
	
	DB7_DDR |= (1 << DB7);
	DB6_DDR |= (1 << DB6);
	DB5_DDR |= (1 << DB5);
	DB4_DDR |= (1 << DB4);
	DB3_DDR |= (1 << DB3);
	DB2_DDR |= (1 << DB2);
	DB1_DDR |= (1 << DB1);
	DB0_DDR |= (1 << DB0);
	
	return address_counter;
}

void write_data_to_ram(uint8_t data_to_write) {
	EN_low;
	RS_high;
	RW_low;
	DB7_low;
	DB6_low;
	DB5_low;
	DB4_low;
	DB3_low;
	DB2_low;
	DB1_low;
	DB0_low;
	
	DB0_PORT |= (((data_to_write & 0b00000001 ) >> 0) << DB0);
	DB1_PORT |= (((data_to_write & 0b00000010 ) >> 1) << DB1);
	DB2_PORT |= (((data_to_write & 0b00000100 ) >> 2) << DB2);
	DB3_PORT |= (((data_to_write & 0b00001000 ) >> 3) << DB3);
	DB4_PORT |= (((data_to_write & 0b00010000 ) >> 4) << DB4);
	DB5_PORT |= (((data_to_write & 0b00100000 ) >> 5) << DB5);
	DB6_PORT |= (((data_to_write & 0b01000000 ) >> 6) << DB6);
	DB7_PORT |= (((data_to_write & 0b10000000 ) >> 7) << DB7);

	EN_high;
	_delay_us(100);
	EN_low;
	_delay_us(100);
}

void write_lcd_both_rows(char* first_row_sentence, char* second_row_sentence) {
	char row1[17];
	snprintf(&row1[0], 17, "%s                ", first_row_sentence);
	
	char row2[17];
	snprintf(&row2[0], 17, "%s                ", second_row_sentence);
	
	set_ddram_address(0x00);
	for (int i = 0; i < 16; i++) {
		write_data_to_ram(row1[i]);
	}
	
	set_ddram_address(0x40);
	for (int i = 0; i < 16; i++) {
		write_data_to_ram(row2[i]);
	}
}

void write_lcd_first_row(char* first_row_sentence) {
	char row1[17];
	snprintf(&row1[0], 17, "%s                ", first_row_sentence);
	
	set_ddram_address(0x00);
	for (int i = 0; i < 16; i++) {
		write_data_to_ram(row1[i]);
	}
}

void write_lcd_second_row(char* second_row_sentence) {
	char row2[17];
	snprintf(&row2[0], 17, "%s                ", second_row_sentence);
	
	set_ddram_address(0x40);
	for (int i = 0; i < 16; i++) {
		write_data_to_ram(row2[i]);
	}
}

void write_lcd_row_with_size(int row, char* sentence, int size) { // maybe not used
	if (row == 1) {
		set_ddram_address(0x00);
	} else {
		set_ddram_address(0x40);
	}
	
	for (int i = 0; i < 16; i++) {
		if (i < size) {
			write_data_to_ram(sentence[i]);
		} else {
			write_data_to_ram(' ');
		}
	}
}

/*
// !! maybe unused:
void write_buffer_to_lcd(char* first_row_sentence, int first_row_size, char* second_row_sentence, int second_row_size) {
	clear_display();
	for (int i = 0; i < 16; i++) {
		if (i < first_row_size) {
			write_data_to_ram(first_row_sentence[i]);
		} else {
			write_data_to_ram(" ");
		}
	}
	
	set_ddram_address(0x40);
	for (int i = 0; i < 16; i++) {
		if (i < second_row_size) {
			write_data_to_ram(second_row_sentence[i]);
			} else {
			write_data_to_ram(" ");
		}
	}
}*/

uint8_t read_data_from_cgram(uint8_t cgram_address) {
	set_cgram_address(cgram_address);
	
	DB7_DDR &= ~(1 << DB7);
	DB6_DDR &= ~(1 << DB6);
	DB5_DDR &= ~(1 << DB5);
	DB4_DDR &= ~(1 << DB4);
	DB3_DDR &= ~(1 << DB3);
	DB2_DDR &= ~(1 << DB2);
	DB1_DDR &= ~(1 << DB1);
	DB0_DDR &= ~(1 << DB0);
	EN_low;
	RS_high;
	RW_high;

	EN_high;
	_delay_us(100);
	EN_low;
	_delay_us(100);
	
	uint8_t data = 0;
	data += ((DB7_PIN & (1 << DB7)) >> DB7) << 7;
	data += ((DB6_PIN & (1 << DB6)) >> DB6) << 6;
	data += ((DB5_PIN & (1 << DB5)) >> DB5) << 5;
	data += ((DB4_PIN & (1 << DB4)) >> DB4) << 4;
	data += ((DB3_PIN & (1 << DB3)) >> DB3) << 3;
	data += ((DB2_PIN & (1 << DB2)) >> DB2) << 2;
	data += ((DB1_PIN & (1 << DB1)) >> DB1) << 1;
	data += ((DB0_PIN & (1 << DB0)) >> DB0);
	
	DB7_DDR |= (1 << DB7);
	DB6_DDR |= (1 << DB6);
	DB5_DDR |= (1 << DB5);
	DB4_DDR |= (1 << DB4);
	DB3_DDR |= (1 << DB3);
	DB2_DDR |= (1 << DB2);
	DB1_DDR |= (1 << DB1);
	DB0_DDR |= (1 << DB0);
	
	return data;
}

uint8_t read_data_from_ddram(uint8_t ddram_address) {
	set_ddram_address(ddram_address);
	
	DB7_DDR &= ~(1 << DB7);
	DB6_DDR &= ~(1 << DB6);
	DB5_DDR &= ~(1 << DB5);
	DB4_DDR &= ~(1 << DB4);
	DB3_DDR &= ~(1 << DB3);
	DB2_DDR &= ~(1 << DB2);
	DB1_DDR &= ~(1 << DB1);
	DB0_DDR &= ~(1 << DB0);
	EN_low;
	RS_high;
	RW_high;

	EN_high;
	_delay_us(100);
	EN_low;
	_delay_us(100);
	
	uint8_t data = 0;
	data += ((DB7_PIN & (1 << DB7)) >> DB7) << 7;
	data += ((DB6_PIN & (1 << DB6)) >> DB6) << 6;
	data += ((DB5_PIN & (1 << DB5)) >> DB5) << 5;
	data += ((DB4_PIN & (1 << DB4)) >> DB4) << 4;
	data += ((DB3_PIN & (1 << DB3)) >> DB3) << 3;
	data += ((DB2_PIN & (1 << DB2)) >> DB2) << 2;
	data += ((DB1_PIN & (1 << DB1)) >> DB1) << 1;
	data += ((DB0_PIN & (1 << DB0)) >> DB0);
	
	DB7_DDR |= (1 << DB7);
	DB6_DDR |= (1 << DB6);
	DB5_DDR |= (1 << DB5);
	DB4_DDR |= (1 << DB4);
	DB3_DDR |= (1 << DB3);
	DB2_DDR |= (1 << DB2);
	DB1_DDR |= (1 << DB1);
	DB0_DDR |= (1 << DB0);
	
	return data;
}
