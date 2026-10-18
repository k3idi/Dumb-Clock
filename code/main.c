/*
 * Dumb Clock.c
 *
 * Created: 8/25/2026 3:58:11 PM
 * Author : Keidi
 */ 

// next up: sensors and ADC

#ifndef F_CPU
#define F_CPU 2000000UL
#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcd.h"
#include "music.h"
#include "MCP7940N.h"
#include "i2c.h"
#include "sensors.h"
#include "timer.h"
#include "string.h" 
#include "encoder.h"
#include "button.h"


int main(void)
{
	MCUCR |= (1 << JTD); // to use portf as standard i/o port
	MCUCR |= (1 << JTD);
	
	BUZZER_DDR |= (1 << BUZZER);
	
	init_i2c();
	init_lcd();
	init_ADC();
	init_sensors();
	init_timer_0();
	init_timer_1();
	init_timer_2();
	init_timer_3();
	sei();
	
	display_on();
	clear_display();
	write_lcd_both_rows("JOOUUUUU! :]", "Aeg on vale :[");
	
	_delay_ms(1500);
	
	bool time_set = false;
	
	CD_in_jukebox = default_tune;

	Time time;
	
	time.year = get_year();
	time.month = get_month();
	time.day = get_day();
	time.weekday = get_weekday();
	time.hours = get_hours();
	time.minutes = get_minutes();
	time.seconds = get_seconds();
	time.max_days = 31; // max_days is the maximum number of days in the given month on that year

	int previous_seconds = 60;
	int seconds;
	
	char weekdays[7][16] = {"esmasp2ev       ", "teisip2ev       ", "kolmap2ev       ", "neljap2ev       ", "reede           ", "laup2ev         ", "pyhap2ev        "}; 
	char editing_position[3][16] = {"^               ", "   ^            ", "      ^         "}; 
	int editing_position_index = 0; 
		
	typedef enum { 
		DATE, WEEKDAY, CLOCK, TIMESET
		} Setting_Order;
	Setting_Order setting_order = DATE;
	
	typedef enum { 
		NORMAL_TIME, TIME_IN_SECONDS, REMINDER, SCREEN_ROLL, FUN_TIMES, GAME_DIFFICULTY, GAME_LOAD_SCREEN, MEMORY_CHECK, LOSE_SCREEN, WIN_SCREEN
		} Display_States;
	Display_States display_state = TIME_IN_SECONDS;
	Display_States previous_display_state = TIME_IN_SECONDS;
	
	
	uint32_t previous_timestamp_measuring = 0;
	uint32_t measuring_interval = 200000; // microseconds, = 1/measuring frequency
	
	// move:
	uint8_t sequence_game_difficulty = 10;
	uint8_t sequence_game_difficulty_min = 5;
	uint8_t sequence_game_difficulty_max = 100;
	uint8_t normal_time_display_counter = 0; // seconds, should equal sequence_game_difficulty
	uint32_t game_load_screen_duration = 1500000; // microseconds
	uint32_t game_load_screen_start_time = 0;
	uint32_t game_difficulty_show_duration = 1000000; // microseconds
	uint32_t game_difficulty_start_time = 0;
	uint32_t game_single_sequence_duration = 500000; // microseconds
	uint32_t game_single_sequence_start_time = 0;
	uint32_t game_sequence_clear_duration = 70000;
	uint32_t game_end_screen_duration = 1500000;
	uint32_t game_end_screen_start_time = 0;
	uint8_t game_sequence_counter = 0;
	uint8_t game_sequence_next_position = 0;
	int game_positions[3] = {0, 5, 10}; 
	uint8_t game_sequence[sequence_game_difficulty_max];
	uint8_t game_user_input = 0;
	bool user_input_inserted = false;
	uint32_t screen_roll_frame_duration = 150000; // microseconds
	uint32_t screen_roll_frame_start_time = 0;
	uint8_t screen_roll_frame = 0;
	char backscreen_row1[16] = "  o_o      o_o  ";
	char backscreen_row2[16] = "      o__o      ";
	snprintf(&backscreen_row1[0], 16, "  %c%c%c      %c%c%c  ", BIG_EYE, MOUTH_3, SMALL_EYE, SMALL_EYE, MOUTH_2, BIG_EYE);
	snprintf(&backscreen_row2[0], 16, "      %c%c%c%c      ", BIG_EYE, MOUTH_1, MOUTH_1, SMALL_EYE);
	backscreen_row1[15] = ' ';
	backscreen_row2[15] = ' ';
	uint32_t reminder_duration = 2000000;
	uint32_t reminder_start_time = 0;
	
	bool should_randomize_backlight = false;
	
	char random_characters[32];
	for (int i = 0; i < 32; i++) {
		random_characters[i] = (uint8_t)(rand() % 256);
	}
	
	int jolly_tune_index = 0;
	int total_jolly_tunes = 5;
	
	Tune jolly_tunes[5] = {running_in_the_90s_tune, sweet_dreams_tune, run_tune, chopin_waltz_tune, snowy_tune};
	
    while (1) 
    {
		if (time_passed >= 2000000000) { // idk, big number = vibes
			time_passed = 0;
			previous_timestamp_measuring = 0;
			game_difficulty_start_time = 0;
			game_load_screen_start_time = 0;
			game_single_sequence_start_time = 0;
			game_end_screen_start_time = 0;
			screen_roll_frame_start_time = 0;
			reminder_start_time = 0;
		}
		
		if (time_passed > previous_timestamp_measuring + measuring_interval) {
			previous_timestamp_measuring = time_passed;
			update_sensor_values();
			lcd_dimness = get_lcd_dimness_factor((uint16_t)light_level.average_value, 50, 400);
			adjust_lcd_dimness();
		}
		
		if (encoder_value_changed && time_set) {
			encoder_value_changed = false;
			if (display_state == NORMAL_TIME || display_state == TIME_IN_SECONDS || display_state == GAME_DIFFICULTY) {
				if (display_state != GAME_DIFFICULTY) {
					previous_display_state = display_state;
				}
				game_difficulty_start_time = time_passed;
				display_state = GAME_DIFFICULTY;
				sequence_game_difficulty += encoder_counter;
				encoder_counter = 0;
				sequence_game_difficulty = sequence_game_difficulty * (sequence_game_difficulty >= sequence_game_difficulty_min && sequence_game_difficulty <= sequence_game_difficulty_max) +
				sequence_game_difficulty_min * (sequence_game_difficulty < sequence_game_difficulty_min) + sequence_game_difficulty_max * (sequence_game_difficulty > sequence_game_difficulty_max);
			}
			encoder_counter = 0;
		}
		
		encoder_update();
		
		if (!time_set) {
			switch (setting_order) {
				case DATE:
					if (encoder_value_changed) {
						encoder_value_changed = false;
						char row1[17];
						edit_date(&time, editing_position_index, encoder_counter, &row1[0]);
						encoder_counter = 0;
						write_lcd_first_row(&row1[0]);
						write_lcd_second_row(&editing_position[editing_position_index][0]);
					}
					break;
				case WEEKDAY:
					if (encoder_value_changed) {
						encoder_value_changed = false;
						time.weekday += encoder_counter;
						time.weekday = time.weekday * (time.weekday >= 0 && time.weekday < 7) + 0 * (time.weekday < 0) + 6 * (time.weekday > 6);
						encoder_counter = 0;
						write_lcd_first_row(&weekdays[time.weekday][0]);
						write_lcd_second_row("");
					}
					break;
				case CLOCK:
					if (encoder_value_changed) {
						encoder_value_changed = false;
						char row1[17];
						edit_time(&time, editing_position_index, encoder_counter, &row1[0]);
						encoder_counter = 0;
						write_lcd_first_row(&row1[0]);
						write_lcd_second_row(&editing_position[editing_position_index][0]);
					}
					break;			
				case TIMESET:	
					time_set = true;
					encoder_value_changed = false;
					set_year(time.year);
					set_month(time.month);
					set_day(time.day);
					set_weekday(time.weekday);
					set_hours(time.hours);
					set_minutes(time.minutes);
					set_seconds(time.seconds);
					break;
			}
		} else {
			switch (display_state) {
				case NORMAL_TIME:
					seconds = get_seconds();
					if (previous_seconds != seconds) {
						previous_seconds = seconds;
						char row1[17];
						get_time_str(&row1[0]);
						write_lcd_first_row(&row1[0]);
						char row2[17];
						get_date_str(&row2[0]);
						write_lcd_second_row(row2);
						normal_time_display_counter--;
						if (normal_time_display_counter == 0) {
							display_state = TIME_IN_SECONDS;
							should_randomize_backlight = true;
						}
					}
					break;
				case TIME_IN_SECONDS:
					seconds = get_seconds();
					if (previous_seconds != seconds) {
						previous_seconds = seconds;
						uint32_t time_in_seconds = get_time_in_seconds();
						char row1[17];
						float data_1 = temp_1.average_value;
						int data_1_left = (int)data_1;
						int data_1_right = abs(((int)(data_1*10)) % 10); 
						float data_2 = humidity.average_value;
						int data_2_left = (int)data_2;
						int data_2_right = abs(((int)(data_2*10)) % 10);
						char row2[17];
						snprintf(&row1[0], 17, "      %lu           ", time_in_seconds);
						snprintf(&row2[0], 17, "%d.%d%c      %d.%d%c                ", data_1_left, data_1_right, DEGREE_SIGN, data_2_left, data_2_right, '%');
						write_lcd_first_row(&row1[0]);
						write_lcd_second_row(&row2[0]);
						if (should_randomize_backlight) {
							randomize_backlight_colour();
							should_randomize_backlight = false;
						}
					}
					break;
				case GAME_DIFFICULTY:
					if (time_passed < game_difficulty_start_time + game_difficulty_show_duration) {
						char row1[17];
						snprintf(&row1[0], 17, "Raskusaste: %u", sequence_game_difficulty);
						write_lcd_first_row(row1);
						write_lcd_second_row("");
					} else {
						display_state = previous_display_state;
					}
					break;
				case GAME_LOAD_SCREEN:
					if (time_passed > game_load_screen_start_time + game_load_screen_duration) {
						display_state = FUN_TIMES;
						game_single_sequence_start_time = time_passed;
						game_sequence_counter = sequence_game_difficulty;
						game_sequence_next_position = rand() % 3;
						game_sequence[--game_sequence_counter] = game_sequence_next_position;
						char row2[16] = "                ";
						row2[game_positions[game_sequence_next_position]] = 0xff;
						write_lcd_both_rows("", &row2[0]);
						game_sequence_next_position = rand() % 3;
					}
					break;
				case FUN_TIMES:
					if (time_passed > game_single_sequence_start_time + game_single_sequence_duration - game_sequence_clear_duration) {
						write_lcd_both_rows("", "");
					}
					if (time_passed > game_single_sequence_start_time + game_single_sequence_duration) {
						game_single_sequence_start_time = time_passed;
						if (game_sequence_counter > 0) {
							game_sequence[--game_sequence_counter] = game_sequence_next_position;
							char row2[16] = "                ";
							row2[game_positions[game_sequence_next_position]] = 0xff;
							write_lcd_both_rows("", &row2[0]);
							game_sequence_next_position = rand() % 3;
						} else {
							display_state = MEMORY_CHECK;
							game_sequence_counter = sequence_game_difficulty;
						}
					}
					break;
				case MEMORY_CHECK:
					if (user_input_inserted) {
						if (time_passed > game_single_sequence_start_time + game_single_sequence_duration) {
							write_lcd_both_rows("", "");
							user_input_inserted = false;
							game_single_sequence_start_time = time_passed;
							// check if insertion was correct
							if (game_user_input == game_sequence[--game_sequence_counter]) {
								// insertion was correct
								if (game_sequence_counter == 0) {
									display_state = WIN_SCREEN;
									game_end_screen_start_time = time_passed;
									write_lcd_both_rows("  SA            ", "      V6ITSID!  ");
									stop_song();
									start_song(victory_tune);
								}
							} else {
								display_state = LOSE_SCREEN;
								game_end_screen_start_time = time_passed;
								start_song(game_over_tune);
							}
						} 
					}
					break;
				case WIN_SCREEN:
					if (time_passed > game_end_screen_start_time + game_end_screen_duration) {
						// display normal time for game_difficulty seconds
						display_state = NORMAL_TIME;
						normal_time_display_counter = sequence_game_difficulty;
					} else {
						randomize_backlight_colour();
					}
					break;
				case LOSE_SCREEN:
					if (time_passed < game_end_screen_start_time + game_end_screen_duration) {
						// scramble screen
						write_lcd_both_rows(&random_characters[0], &random_characters[16]);
						for (int i = 0; i < 32; i++) {
							random_characters[i]++;
						}
						randomize_backlight_colour();
					} else {
						display_state = TIME_IN_SECONDS;
					}
					break;
				case SCREEN_ROLL:
					if (time_passed > screen_roll_frame_start_time + screen_roll_frame_duration) {
						screen_roll_frame++;
						screen_roll_frame_start_time = time_passed;
						randomize_backlight_colour();
						uint32_t time_in_seconds = get_time_in_seconds();
						char row1[49];
						char row2[49];
						float data_1 = temp_1.average_value;
						int data_1_left = (int)data_1;
						int data_1_right = abs(((int)(data_1*10)) % 10);
						float data_2 = humidity.average_value;
						int data_2_left = (int)data_2;
						int data_2_right = abs(((int)(data_2*10)) % 10);
						snprintf(&row1[0], 17, "      %lu           ", time_in_seconds);
						snprintf(&row2[0], 17, "%d.%d%c      %d.%d%c                ", data_1_left, data_1_right, DEGREE_SIGN, data_2_left, data_2_right, '%');
						snprintf(&row1[16], 17, "%s", backscreen_row1);
						snprintf(&row2[16], 17, "%s", backscreen_row2);
						snprintf(&row1[32], 17, "      %lu           ", time_in_seconds);
						snprintf(&row2[32], 17, "%d.%d%c      %d.%d%c                ", data_1_left, data_1_right, DEGREE_SIGN, data_2_left, data_2_right, '%');
						write_lcd_first_row(&row1[screen_roll_frame]);
						write_lcd_second_row(&row2[screen_roll_frame]);
						if (screen_roll_frame >= 32) {
							display_state = TIME_IN_SECONDS;
						}
					}
					break;
				case REMINDER:
					if (time_passed > reminder_start_time + reminder_duration) {
						display_state = TIME_IN_SECONDS;
					} else {
						seconds = get_seconds();
						if (previous_seconds != seconds) {
							previous_seconds = seconds;
							uint32_t time_in_seconds = get_time_in_seconds();
							char row1[17];
							snprintf(&row1[0], 17, "     %c%lu%c           ", ARROW_POINTING_RIGHT, time_in_seconds, ARROW_POINTING_LEFT);
							write_lcd_both_rows(&row1[0], "");
						}
					}
					break;
			}
		}
		
		if (!kell_button_previously_pressed && kell_button_pressed) {
			kell_button_previously_pressed = true;
			if (time_set) {
				if (display_state == MEMORY_CHECK) {
					if (!user_input_inserted) {
						game_user_input = 0;
						user_input_inserted = true;
						game_single_sequence_start_time = time_passed;
						char row2[16] = "                ";
						row2[game_positions[game_user_input]] = 0xff;
						write_lcd_both_rows("", &row2[0]);
					}
				} else if (display_state == TIME_IN_SECONDS) {
					display_state = REMINDER;
					reminder_start_time = time_passed;
					uint32_t time_in_seconds = get_time_in_seconds();
					char row1[17];
					snprintf(&row1[0], 17, "     %c%lu%c           ", ARROW_POINTING_RIGHT, time_in_seconds, ARROW_POINTING_LEFT);
					write_lcd_both_rows(&row1[0], "");
				} else if (display_state != WIN_SCREEN && display_state != LOSE_SCREEN) {
					display_state = TIME_IN_SECONDS;
				}
			}
		} else if (kell_button_previously_pressed && !kell_button_pressed) {
			kell_button_previously_pressed = false;
		}
		
		if (!vajuta_mind_button_previously_pressed && vajuta_mind_button_pressed) {
			vajuta_mind_button_previously_pressed = true;
			if (!time_set && (setting_order == 0 || setting_order == 2)) {
				editing_position_index += 1;
				editing_position_index *= (editing_position_index < 3);
				write_lcd_second_row(&editing_position[editing_position_index][0]);
			}
			if (time_set) {
				if (display_state == MEMORY_CHECK) {
					if (!user_input_inserted) {
						game_user_input = 1;
						user_input_inserted = true;
						game_single_sequence_start_time = time_passed;
						char row2[16] = "                ";
						row2[game_positions[game_user_input]] = 0xff;
						write_lcd_both_rows("", &row2[0]);
					}
				} else if (display_state != WIN_SCREEN && display_state != LOSE_SCREEN) {
					randomize_backlight_colour();
					display_state = SCREEN_ROLL;
					screen_roll_frame_start_time = time_passed;
					screen_roll_frame = 0;
					stop_song();
					start_song(jolly_tunes[jolly_tune_index++]);
					if (jolly_tune_index >= total_jolly_tunes) {
						jolly_tune_index = 0;
					}
				}
			}
		} else if (vajuta_mind_button_previously_pressed && !vajuta_mind_button_pressed) {
			vajuta_mind_button_previously_pressed = false;
		}
		
		if (!kell_fr_button_previously_pressed && kell_fr_button_pressed) {
			kell_fr_button_previously_pressed = true;
			if (!time_set) {
				randomize_backlight_colour();
				setting_order += 1;
				encoder_value_changed = true;
				editing_position_index = 0;
			} else {
				if (display_state == MEMORY_CHECK) {
					if (!user_input_inserted) {
						game_user_input = 2;
						user_input_inserted = true;
						game_single_sequence_start_time = time_passed;
						char row2[16] = "                ";
						row2[game_positions[game_user_input]] = 0xff;
						write_lcd_both_rows("", &row2[0]);
					}
				} else if (display_state != MEMORY_CHECK && display_state != WIN_SCREEN && display_state != LOSE_SCREEN) {
					stop_song();
					start_song(among_us_tune);
					write_lcd_both_rows("!J2LGI HOOLEGA! ", " ! !! !!! !! !  ");
					display_state = GAME_LOAD_SCREEN;
					game_load_screen_start_time = time_passed;
				}
			}
		} else if (kell_fr_button_previously_pressed && !kell_fr_button_pressed) {
			kell_fr_button_previously_pressed = false;
		}
    }
}




