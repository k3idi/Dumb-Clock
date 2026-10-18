#include <avr/io.h>
#include "MCP7940N.h"
#include "i2c.h"
#include <stdio.h>


uint8_t read_register(uint8_t register_address) {
	start_i2c();
	send_byte(WRITE_CONTROL_BYTE);
	receive_acknowledge_bit();
	send_byte(register_address);
	receive_acknowledge_bit();
	start_i2c();
	send_byte(READ_CONTROL_BYTE);
	receive_acknowledge_bit();
	uint8_t data = receive_byte();
	send_non_acknowledge_bit();
	stop_i2c();
	return data;
}

uint8_t read_random_register(void) {
	start_i2c();
	send_byte(READ_CONTROL_BYTE);
	receive_acknowledge_bit();
	uint8_t data = receive_byte();
	//send_acknowledge_bit();
	receive_acknowledge_bit();
	stop_i2c();
	return data;
}

void write_register(uint8_t register_address, uint8_t data) {
	start_i2c();
	send_byte(WRITE_CONTROL_BYTE);
	receive_acknowledge_bit();
	send_byte(register_address);
	receive_acknowledge_bit();
	send_byte(data);
	receive_acknowledge_bit();
	stop_i2c();
}

int get_year(void) {
	uint8_t data = read_register(RTCYEAR);
	int year = 2000 + (int)(data >> 4) * 10 + (int)(data & (0b00001111));
	return year;
}

int get_month(void) {
	uint8_t data = read_register(RTCMTH);
	data &= ~(0b11100000);
	int month = (int)(data >> 4) * 10 + (int)(data & (0b00001111));
	return month;
}

int get_day(void) {
	uint8_t data = read_register(RTCDATE);
	int date = (int)(data >> 4) * 10 + (int)(data & (0b00001111));
	return date;
}

int get_weekday(void) {
	uint8_t data = read_register(RTCWKDAY);
	data &= ~(0b11111000);
	int weekday = (int)data;
	weekday -= 1;
	return weekday;
}

int get_hours(void) {
	uint8_t data = read_register(RTCHOUR);
	data &= ~(0b11000000);
	int hours = ((int)(data >> 4)) * 10 + (int)(data & (0b00001111));
	return hours;
}

int get_minutes(void) {
	uint8_t data = read_register(RTCMIN);
	data &= ~(0b10000000);
	int minutes = ((int)(data >> 4)) * 10 + (int)(data & (0b00001111));
	return minutes;
}

int get_seconds(void) {
	uint8_t data = read_register(RTCSEC);
	data &= ~(0b10000000);
	int seconds = ((int)(data >> 4)) * 10 + (int)(data & (0b00001111));
	return seconds;
}

void set_year(int year) {
	year = year % 1000;
	year = year % 100;
	uint8_t ones = (uint8_t)(year % 10);
	uint8_t tens = (uint8_t)((year - (year % 10)) / 10);
	uint8_t data = (tens << 4) | ones;
	write_register(RTCYEAR, data);
	uint8_t month_register = read_register(RTCMTH);
	if (year % 4 == 0) { // maybe leapyear
		if (year % 100 == 0 && year % 400 != 0) { // not leapyear
			month_register &= ~(1 << 5);
		} else { // leapyear
			month_register |= (1 << 5);
		}
	} else { // not leapyear
		month_register &= ~(1 << 5);
	}
}

void set_month(int month) {
	uint8_t ones = (uint8_t)(month % 10);
	uint8_t tens = (uint8_t)((month - (month % 10)) / 10);
	uint8_t month_register = read_register(RTCMTH);
	month_register &= (0b00100000);
	month_register |= (tens << 4) | ones;
	write_register(RTCMTH, month_register);
}

void set_day(int day) {
	uint8_t ones = (uint8_t)(day % 10);
	uint8_t tens = (uint8_t)((day - (day % 10)) / 10);
	uint8_t data = (tens << 4) | ones;
	write_register(RTCDATE, data);
}

void set_weekday(int weekday) {
	weekday += 1;
	uint8_t ones = (uint8_t)(weekday % 10) & (0b00000111);
	uint8_t weekday_register = read_register(RTCWKDAY);
	weekday_register &= (0b00111000);
	weekday_register |= ones;
	write_register(RTCWKDAY, weekday_register);
}

void set_hours(int hours) {
	uint8_t ones = (uint8_t)(hours % 10);
	uint8_t tens = (uint8_t)((hours - (hours % 10)) / 10) & (0b00000011);
	uint8_t hours_register = read_register(RTCHOUR);
	hours_register &= (0b01000000);
	hours_register |= (tens << 4) | ones;
	write_register(RTCHOUR, hours_register);
}

void set_minutes(int minutes) {
	uint8_t ones = (uint8_t)(minutes % 10);
	uint8_t tens = (uint8_t)((minutes - (minutes % 10)) / 10);
	uint8_t data = (tens << 4) | ones;
	write_register(RTCMIN, data);
}

void set_seconds(int seconds) {
	uint8_t ones = (uint8_t)(seconds % 10);
	uint8_t tens = (uint8_t)((seconds - (seconds % 10)) / 10) & (0b00000111);
	uint8_t seconds_register = read_register(RTCSEC);
	seconds_register &= (0b10000000);
	seconds_register |= (tens << 4) | ones | (0b10000000); // also start the counting
	write_register(RTCSEC, seconds_register);
}

uint32_t get_time_in_seconds(void) {
	int hours = get_hours();
	int minutes = get_minutes();
	int seconds = get_seconds();
	uint32_t time_in_seconds = 60 * 60 * (uint32_t)hours + 60 * (uint32_t)minutes + (uint32_t)seconds;
	return time_in_seconds;
}

void get_date_str(char* buffer) {
	int year = get_year();
	int month = get_month();
	int date = get_day();
	if (date < 10) {
		if (month < 10) { // how to make this if statement part better?
			snprintf(buffer, 17, " %d/0%d/%d                 ", date, month, year);
		} else {
			snprintf(buffer, 17, " %d/%d/%d                 ", date, month, year);
		}
		} else {
		if (month < 10) {
			snprintf(buffer, 17, "%d/0%d/%d                 ", date, month, year);
		} else {
			snprintf(buffer, 17, "%d/%d/%d                 ", date, month, year);
		}
	}	
}

void get_time_str(char* buffer) {
	int seconds = get_seconds();
	int minutes = get_minutes();
	int hours = get_hours();
	if (hours < 10) {
		if (minutes < 10) {
			if (seconds < 10) {
				snprintf(&buffer[0], 17, " %d:0%d:0%d                 ", hours, minutes, seconds);
				} else {
				snprintf(&buffer[0], 17, " %d:0%d:%d                 ", hours, minutes, seconds);
			}
			} else {
			if (seconds < 10) {
				snprintf(&buffer[0], 17, " %d:%d:0%d                 ", hours, minutes, seconds);
				} else {
				snprintf(&buffer[0], 17, " %d:%d:%d                 ", hours, minutes, seconds);
			}
		}
		} else {
		if (minutes < 10) {
			if (seconds < 10) {
				snprintf(&buffer[0], 17, "%d:0%d:0%d                 ", hours, minutes, seconds);
				} else {
				snprintf(&buffer[0], 17, "%d:0%d:%d                 ", hours, minutes, seconds);
			}
			} else {
			if (seconds < 10) {
				snprintf(&buffer[0], 17, "%d:%d:0%d                 ", hours, minutes, seconds);
				} else {
				snprintf(&buffer[0], 17, "%d:%d:%d                 ", hours, minutes, seconds);
			}
		}
	}
}


void edit_date(Time* time, int editing_position_index, int encoder_counter, char* buffer) {
	int year = time->year;
	int month = time->month;
	int day = time->day;
	int max_days = time->max_days;
	year += encoder_counter * (editing_position_index == 2);
	year = year * (year >= 2000 && year < 2100) + 2000 * (year < 2000) + 2099 * (year > 2099);
	month += encoder_counter * (editing_position_index == 1);
	month = month * (month > 0 && month < 13) + 1 * (month < 1) + 12 * (month > 12);
	if (month == 2) {
		max_days = 28;
		if (month == 2 && year % 4 == 0) {
			max_days = 29;
			if (year % 100 == 0 && year % 400 != 0) {
				max_days = 28;
			}
		}
		} else {
		max_days = 31;
		if (month == 4 || month == 6 || month == 9 || month == 11) {
			max_days = 30;
		}
	}
	
	if (editing_position_index == 0) {
		day += encoder_counter;
		day = day * (day > 0 && day <= max_days) + 1 * (day < 1) + max_days * (day > max_days);
	}
							
	if (day < 10) {
		if (month < 10) { // how to make this if statement part better?
			snprintf(buffer, 17, " %d/0%d/%d                 ", day, month, year);
			} else {
			snprintf(buffer, 17, " %d/%d/%d                 ", day, month, year);
		}
		} else {
		if (month < 10) {
			snprintf(buffer, 17, "%d/0%d/%d                 ", day, month, year);
			} else {
			snprintf(buffer, 17, "%d/%d/%d                 ", day, month, year);
		}
	}
	
	time->year = year;
	time->month = month;
	time->day = day;
	time->max_days = max_days;
}

void edit_time(Time* time, int editing_position_index, int encoder_counter, char* buffer) {		
	int hours = time->hours;
	int minutes = time->minutes;
	int seconds = time->seconds;		
	hours += encoder_counter * (editing_position_index == 0);
	hours = hours * (hours >= 0 && hours < 24) + 0 * (hours < 0) + 23 * (hours > 23); // macro function for this thing?
	minutes += encoder_counter * (editing_position_index == 1);
	minutes = minutes * (minutes > 0 && minutes < 60) + 0 * (minutes < 0) + 59 * (minutes > 59);
	seconds += encoder_counter * (editing_position_index == 2);
	seconds = seconds * (seconds > 0 && seconds < 60) + 0 * (seconds < 0) + 59 * (seconds > 59);
												
	if (hours < 10) {
		if (minutes < 10) {
			if (seconds < 10) {
				snprintf(buffer, 17, " %d:0%d:0%d                 ", hours, minutes, seconds);
				} else {
				snprintf(buffer, 17, " %d:0%d:%d                 ", hours, minutes, seconds);
			}
			} else {
			if (seconds < 10) {
				snprintf(buffer, 17, " %d:%d:0%d                 ", hours, minutes, seconds);
				} else {
				snprintf(buffer, 17, " %d:%d:%d                 ", hours, minutes, seconds);
			}
		}
		} else {
		if (minutes < 10) {
			if (seconds < 10) {
				snprintf(buffer, 17, "%d:0%d:0%d                 ", hours, minutes, seconds);
				} else {
				snprintf(buffer, 17, "%d:0%d:%d                 ", hours, minutes, seconds);
			}
			} else {
			if (seconds < 10) {
				snprintf(buffer, 17, "%d:%d:0%d                 ", hours, minutes, seconds);
				} else {
				snprintf(buffer, 17, "%d:%d:%d                 ", hours, minutes, seconds);
			}
		}
	}
	time->hours = hours;
	time->minutes = minutes;
	time->seconds = seconds;
}
