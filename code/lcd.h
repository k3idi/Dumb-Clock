#ifndef LDC_H
#define LCD_H

/* The following parameters have to be set before using.*/

#ifndef F_CPU
#define F_CPU 2000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>

#define PWM_PERIOD			10000 // pwm period in ticks to be counted by a timer counter, this value depends on the prescaler (in this case it is 1). it corresponds to a frequency of 200 Hz.
#define BLUE_PIN_NR			PA1
#define BLUE_DDR			DDRA
#define BLUE_PORT			PORTA
#define BLUE_PIN			PINA
#define RED_PIN_NR			PF3
#define RED_DDR				DDRF
#define RED_PORT			PORTF
#define RED_PIN				PINF
#define GREEN_PIN_NR		PA0
#define GREEN_DDR			DDRA
#define GREEN_PORT			PORTA
#define GREEN_PIN			PINA

#define DEGREE_SIGN				0b11011111
#define BIG_EYE					0b01001111
#define SMALL_EYE				0b01101111
#define MOUTH_1					0b01011111
#define MOUTH_2					0b10100001
#define MOUTH_3					0b10100011
#define ARROW_POINTING_RIGHT	0b01111110
#define ARROW_POINTING_LEFT		0b01111111

#define RS				PE0
#define RS_DDR			DDRE
#define RS_PORT			PORTE

#define RW				PE1
#define RW_DDR			DDRE
#define RW_PORT			PORTE

#define EN				PA7
#define EN_DDR			DDRA
#define EN_PORT			PORTA

#define DB7				PC7
#define DB7_DDR			DDRC
#define DB7_PORT		PORTC
#define DB7_PIN			PINC

#define DB6				PC6
#define DB6_DDR			DDRC
#define DB6_PORT		PORTC
#define DB6_PIN			PINC

#define DB5				PC5
#define DB5_DDR			DDRC
#define DB5_PORT		PORTC
#define DB5_PIN			PINC

#define DB4				PC4
#define DB4_DDR			DDRC
#define DB4_PORT		PORTC
#define DB4_PIN			PINC

#define DB3				PC3
#define DB3_DDR			DDRC
#define DB3_PORT		PORTC
#define DB3_PIN			PINC

#define DB2				PC2
#define DB2_DDR			DDRC
#define DB2_PORT		PORTC
#define DB2_PIN			PINC

#define DB1				PC1
#define DB1_DDR			DDRC
#define DB1_PORT		PORTC
#define DB1_PIN			PINC

#define DB0				PC0
#define DB0_DDR			DDRC
#define DB0_PORT		PORTC
#define DB0_PIN			PINC

/* Now everything is set and nothing has to be modified heretofore (if you have a 2 line 5x8 font lcd with an 8 bit data interface).*/

#define DB7_high		(DB7_PORT |= (1 << DB7))
#define DB7_low			(DB7_PORT &= ~(1 << DB7))

#define DB6_high		(DB6_PORT |= (1 << DB6))
#define DB6_low			(DB6_PORT &= ~(1 << DB6))

#define DB5_high		(DB5_PORT |= (1 << DB5))
#define DB5_low			(DB5_PORT &= ~(1 << DB5))

#define DB4_high		(DB4_PORT |= (1 << DB4))
#define DB4_low			(DB4_PORT &= ~(1 << DB4))

#define DB3_high		(DB3_PORT |= (1 << DB3))
#define DB3_low			(DB3_PORT &= ~(1 << DB3))

#define DB2_high		(DB2_PORT |= (1 << DB2))
#define DB2_low			(DB2_PORT &= ~(1 << DB2))

#define DB1_high		(DB1_PORT |= (1 << DB1))
#define DB1_low			(DB1_PORT &= ~(1 << DB1))

#define DB0_high		(DB0_PORT |= (1 << DB0))
#define DB0_low			(DB0_PORT &= ~(1 << DB0))

#define RS_high			(RS_PORT |= (1 << RS))
#define RS_low			(RS_PORT &= ~(1 << RS))

#define RW_high			(RW_PORT |= (1 << RW))
#define RW_low			(RW_PORT &= ~(1 << RW))

#define EN_high			(EN_PORT |= (1 << EN))
#define EN_low			(EN_PORT &= ~(1 << EN))

uint16_t blue_duty_cycle; // blue channel pwm high duration in timer counter ticks
uint16_t red_duty_cycle; // red channel pwm high duration in timer counter ticks
uint16_t green_duty_cycle; // green channel pwm high duration in timer counter ticks
float lcd_dimness;


void adjust_lcd_dimness(void);
void randomize_backlight_colour(void);
void init_lcd(void);
void clear_display(void);
void return_home(void);
void set_cursor_increment(void);
void set_cursor_decrement(void);
void display_on(void);
void display_off(void);
void cursor_on(void);
void cursor_off(void);
void blinking_cursor_on(void);
void blinking_cursor_off(void);
void move_cursor_right(void);
void move_cursor_left(void);
void shift_display_right(void);
void shift_display_left(void);
void set_lcd_parameters();
void set_cgram_address(uint8_t address);
void set_ddram_address(uint8_t address);
uint8_t read_busy_flag(void);
uint8_t read_address_counter(void);
void write_data_to_ram(uint8_t data_to_write);
uint8_t read_data_from_cgram(uint8_t cgram_address);
uint8_t read_data_from_ddram(uint8_t ddram_address);
void write_lcd_both_rows(char* first_row_sentence, char* second_row_sentence);
void write_lcd_first_row(char* first_row_sentence);
void write_lcd_second_row(char* second_row_sentence);
void write_lcd_row_with_size(int row, char* sentence, int size);

#endif