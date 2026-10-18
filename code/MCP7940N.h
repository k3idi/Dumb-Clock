#ifndef MCP7940N_H
#define MCP7940N_H

// device address + read/write control bit
#define READ_CONTROL_BYTE	 0b11011111
#define WRITE_CONTROL_BYTE	 0b11011110

// register addresses
#define RTCSEC			0x00
#define RTCMIN			0x01
#define RTCHOUR			0x02
#define RTCWKDAY		0x03
#define RTCDATE			0x04
#define RTCMTH			0x05
#define RTCYEAR			0x06
#define CONTROL			0x07
#define OSCTRIM			0x08

#define ALM0SEC			0x0A
#define ALM0MIN			0x0B
#define ALM0HOUR		0x0C
#define ALM0WKDAY		0x0D
#define ALM0DATE		0x0E
#define ALM0MTH			0x0F

#define ALM1SEC			0x0A
#define ALM1MIN			0x0B
#define ALM1HOUR		0x0C
#define ALM1WKDAY		0x0D
#define ALM1DATE		0x0E
#define ALM1MTH			0x0F

#define PWRDNMIN		0x18
#define PWRDNHOUR		0x19
#define PWRDNDATE		0x1A
#define PWRDNMTH		0x1B

#define PWRUPMIN		0x1C
#define PWRUPHOUR		0x1D
#define PWRUPDATE		0x1E
#define PWRUPMTH		0x1F

uint8_t read_register(uint8_t register_address);
uint8_t read_random_register(void);
void write_register(uint8_t register_address, uint8_t data);
int get_year(void);
int get_month(void);
int get_day(void);
int get_weekday(void);
int get_hours(void);
int get_minutes(void);
int get_seconds(void);
void set_year(int year);
void set_month(int month);
void set_day(int day);
void set_weekday(int weekday);
void set_hours(int hours);
void set_minutes(int minutes);
void set_seconds(int seconds);
uint32_t get_time_in_seconds(void);
void get_date_str(char* buffer);
void get_time_str(char* buffer);
	
typedef struct {
		int year;
		int month;
		int day;
		int weekday;
		int hours;
		int minutes;
		int seconds;
		int max_days;
	} Time;
	
void edit_date(Time* time, int editing_position_index, int encoder_counter, char* buffer);
void edit_time(Time* time, int editing_position_index, int encoder_counter, char* buffer);

#endif