#include <avr/io.h>
#include <stdio.h>
#include "sensors.h"

#define V_TEMP_1			PF1
#define V_TEMP_1_DDR		DDRF
#define V_TEMP_2			PF5
#define V_TEMP_2_DDR		DDRF
#define V_LIGHT				PF2
#define V_LIGHT_DDR			DDRF
#define V_HUM				PF4
#define V_HUM_DDR			DDRF
#define ADC_CH_TEMP_1		0b00000001
#define ADC_CH_TEMP_2		0b00000101
#define ADC_CH_LIGHT		0b00000010
#define ADC_CH_HUM			0b00000100
#define ADC_VREF_VCC		(1 << REFS0)
#define ADC_VREF_256		((1 << REFS0) | (1 << REFS1))
#define VDD					5.05 // measured
#define V_256				2.56

#define SENSOR_ARRAY_SIZE	50

float temp_1_values[SENSOR_ARRAY_SIZE];
float temp_2_values[SENSOR_ARRAY_SIZE];
float humidity_values[SENSOR_ARRAY_SIZE];
float light_level_values[SENSOR_ARRAY_SIZE];

Sensor_Value temp_1 = {temp_1_values, 0, SENSOR_ARRAY_SIZE, 0};
Sensor_Value temp_2 = {temp_2_values, 0, SENSOR_ARRAY_SIZE, 0};
Sensor_Value humidity = {humidity_values, 0, SENSOR_ARRAY_SIZE, 0};
Sensor_Value light_level = {light_level_values, 0, SENSOR_ARRAY_SIZE, 0};

void init_sensors(void) {
	V_TEMP_1_DDR &= ~(1 << V_TEMP_1);
	V_TEMP_2_DDR &= ~(1 << V_TEMP_2);
	V_LIGHT_DDR &= ~(1 << V_LIGHT);
	V_HUM_DDR &= ~(1 << V_HUM);
	
	/*
	temp_1.array_size = SENSOR_ARRAY_SIZE;
	temp_1.insertion_index = 0;
	temp_1.sensor_values = &temp_1_values[0];
	
	temp_2.array_size = SENSOR_ARRAY_SIZE;
	temp_2.insertion_index = 0;
	temp_2.sensor_values = &temp_2_values[0];

	humidity.array_size = SENSOR_ARRAY_SIZE;
	humidity.insertion_index = 0;
	humidity.sensor_values = &humidity_values[0];
	
	light_level.array_size = SENSOR_ARRAY_SIZE;
	light_level.insertion_index = 0;
	light_level.sensor_values = &light_level_values[0];*/
}

void init_ADC(void) {
	ADMUX = (1 << REFS0) | (ADC_CH_TEMP_1); // ref is VCC
	//ADMUX = (1 << REFS0) | (1 << REFS1) | (ADC_CH_TEMP_1); // ref is 2.56V
	ADCSRA = (1 << ADEN) | (1 << ADPS0);
	ADCSRB = 0;
	DIDR0 = (1 << ADC5D) | (1 << ADC4D) | (1 << ADC2D) | (1 << ADC1D);
}

void start_ADC_conversion(void) {
	ADCSRA |= (1 << ADSC);
}

void set_ADC_channel(uint8_t channel) {
	ADMUX &= (0b11100000);
	ADMUX |= channel;
}

void set_ADC_vref(uint8_t vref) {
	ADMUX &= ~((1 << REFS0) | (1 << REFS1));
	ADMUX |= vref;
}

float get_temp_1(void) { // add averaging filter to each sensor reading
	set_ADC_channel(ADC_CH_TEMP_1);
	float voltage;
	set_ADC_vref(ADC_VREF_256);
	start_ADC_conversion();
	while (ADCSRA & (1 << ADSC)) {}
	uint16_t ADC_reading = ADC;
	if (ADC_reading >= 1023) {
		set_ADC_vref(ADC_VREF_VCC);
		start_ADC_conversion();
		while (ADCSRA & (1 << ADSC)) {}
		ADC_reading = ADC;
		voltage = (float)ADC_reading / 1024 * VDD;
		set_ADC_vref(ADC_VREF_256); // set it back to 2.56, otherwise the capacitor on vref pin won't discharge fast enough when trying to estimate against 2.56V
	} else {
		voltage = (float)ADC_reading / 1024 * V_256;
	}

	float degrees = -10 + (voltage - 0.2) / 2.8 * 135;
	return degrees;
}

float get_temp_2(void) {
	set_ADC_channel(ADC_CH_TEMP_2);
	set_ADC_vref(ADC_VREF_256);
	start_ADC_conversion();
	while (ADCSRA & (1 << ADSC)) {}
	uint16_t ADC_reading = ADC;
	float voltage;
	if (ADC_reading >= 1023) {
		set_ADC_vref(ADC_VREF_VCC);
		start_ADC_conversion();
		while (ADCSRA & (1 << ADSC)) {}
		ADC_reading = ADC;
		voltage = (float)ADC_reading / 1024 * VDD;
		set_ADC_vref(ADC_VREF_256); // set it back to 2.56, otherwise the capacitor on vref pin won't discharge fast enough when trying to estimate against 2.56V
	} else {
		voltage = (float)ADC_reading / 1024 * V_256;
	}
	float degrees = -45 - 17.5 / 0.8 + 175 / 0.8 * voltage / VDD;
	return degrees;
}

float get_humidity(void) {
	set_ADC_channel(ADC_CH_HUM);
	set_ADC_vref(ADC_VREF_256);
	start_ADC_conversion();
	while (ADCSRA & (1 << ADSC)) {}
	uint16_t ADC_reading = ADC;
	float voltage;
	if (ADC_reading >= 1023) {
		set_ADC_vref(ADC_VREF_VCC);
		start_ADC_conversion();
		while (ADCSRA & (1 << ADSC)) {}
		ADC_reading = ADC;
		voltage = (float)ADC_reading / 1024 * VDD;
		set_ADC_vref(ADC_VREF_256); // set it back to 2.56, otherwise the capacitor on vref pin won't discharge fast enough when trying to estimate against 2.56V
	} else {
		voltage = (float)ADC_reading / 1024 * V_256;
	}
	
	float humidity = -12.5 + 125 * voltage / VDD;
	return humidity;
}

float get_light_level(void) { // returns a number between 0 and 1000
	set_ADC_channel(ADC_CH_LIGHT);
	set_ADC_vref(ADC_VREF_256);
	start_ADC_conversion();
	while (ADCSRA & (1 << ADSC)) {}
	uint16_t ADC_reading = ADC;
	float voltage;
	if (ADC_reading >= 1023) {
		set_ADC_vref(ADC_VREF_VCC);
		start_ADC_conversion();
		while (ADCSRA & (1 << ADSC)) {}
		ADC_reading = ADC;
		voltage = (float)ADC_reading / 1024 * VDD;
		set_ADC_vref(ADC_VREF_256); // set it back to 2.56, otherwise the capacitor on vref pin won't discharge fast enough when trying to estimate against 2.56V
	} else {
		voltage = (float)ADC_reading / 1024 * V_256;
	}
	
	float light_level = voltage / VDD * 1000;
	return light_level;
}

float get_lcd_dimness_factor(uint16_t light_level, uint16_t minimum, uint16_t maximum) {
	if (maximum < minimum) {
		return 1;
	}
	float dimness_factor = (light_level >= minimum && light_level <= maximum) * ((float)(light_level - minimum) / (float)(maximum - minimum)) + (light_level > maximum) * 1; 
	if (dimness_factor < 0.2) {
		return 0.2;
	} else if (dimness_factor > 1) {
		return 1;
	}
	return dimness_factor;
}

float find_average(float* array, int array_size) {
	float average = 0;
	for (int i = 0; i < array_size; i++) {
		average += array[i];
	}
	return average / array_size;
}

void sensor_update(Sensor_Value* sensor, float new_value) {
	sensor->sensor_values[sensor->insertion_index++] = new_value;
	sensor->average_value = find_average(sensor->sensor_values, sensor->array_size);
	if (sensor->insertion_index >= sensor->array_size) {
		sensor->insertion_index = 0;
	}
}

void update_sensor_values(void) {
	float new_temp_1 = get_temp_1();
	float new_temp_2 = get_temp_2();
	float new_humidity = get_humidity();	
	float new_light_level = get_light_level();
	
	sensor_update(&temp_1, new_temp_1);
	sensor_update(&temp_2, new_temp_2);
	sensor_update(&humidity, new_humidity);
	sensor_update(&light_level, new_light_level);
}


