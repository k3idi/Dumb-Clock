#ifndef SENSORS_H
#define SENSORS_H

void init_sensors(void);
void init_ADC(void);
void start_ADC_conversion(void);
void set_ADC_channel(uint8_t channel);
float get_temp_1(void);
float get_temp_2(void);
float get_humidity(void);
float get_light_level(void);
float get_lcd_dimness_factor(uint16_t light_level, uint16_t minimum, uint16_t maximum);
void update_sensor_values(void);

typedef struct {
	float* sensor_values;
	float average_value;
	int array_size;
	int insertion_index;
} Sensor_Value;

extern Sensor_Value temp_1;
extern Sensor_Value temp_2;
extern Sensor_Value humidity;
extern Sensor_Value light_level;

#endif