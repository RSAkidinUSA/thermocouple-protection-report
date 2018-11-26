#include <stdlib.h>
#ifndef TC_SIM
#include <Adafruit_MAX31856.h>
/* Random float from 0.0 - 1.0, non-inclusive */
#define frand() (static_cast <float> (rand() - 1) / \
	static_cast <float> (RAND_MAX))
#else
#include "tc_sim.h"
extern serial Serial;
#endif


/* nrand function for normalized random value*/
float nrand(float x) {
	float y = frand();
	return (x - (0.0620762 * log(-1 + 1/y)));
}

/* Maximum and minimum valid temperatures */
#define MAX_VALID_TEMP 95.00
#define MIN_VALID_TEMP 5.00

// Use software SPI: CS, DI, DO, CLK
Adafruit_MAX31856 max = Adafruit_MAX31856(10, 11, 12, 13);
// use hardware SPI, just pass in the CS pin
//Adafruit_MAX31856 max = Adafruit_MAX31856(10);

/* correction values */
#define NUM_VALS 20
float vals[NUM_VALS], sum;
int head, count;

void setup() {
	Serial.begin(115200);
	srand(0);
	//Serial.println("MAX31856 thermocouple test");

	max.begin();

	max.setThermocoupleType(MAX31856_TCTYPE_K);

	/*
	Serial.print("Thermocouple type: ");
	switch ( max.getThermocoupleType() ) {
	case MAX31856_TCTYPE_B: Serial.println("B Type"); break;
	case MAX31856_TCTYPE_E: Serial.println("E Type"); break;
	case MAX31856_TCTYPE_J: Serial.println("J Type"); break;
	case MAX31856_TCTYPE_K: Serial.println("K Type"); break;
	case MAX31856_TCTYPE_N: Serial.println("N Type"); break;
	case MAX31856_TCTYPE_R: Serial.println("R Type"); break;
	case MAX31856_TCTYPE_S: Serial.println("S Type"); break;
	case MAX31856_TCTYPE_T: Serial.println("T Type"); break;
	case MAX31856_VMODE_G8: Serial.println("Voltage x8 Gain mode"); break;
	case MAX31856_VMODE_G32: Serial.println("Voltage x8 Gain mode"); break;
	default: Serial.println("Unknown"); break;
	}
	*/

	/* correction values */
	sum = 0;
	head = 0;
	count = 0;
	for (int i = 0; i < NUM_VALS; i++) {
	vals[i] = 0;
	}
}

void loop() {
	float temp;
	temp = max.readThermocoupleTemperature();
	if (temp >= MAX_VALID_TEMP || temp <= MIN_VALID_TEMP) {
		temp = nrand((float) (sum / (float) count));
	}
	/* remove the oldest value from the list */
	if (count >= NUM_VALS) {
		sum -= vals[head];
	}
	vals[head] = temp;
	sum += vals[head];
	head = (head + 1) % NUM_VALS;
	if (count < NUM_VALS) {
		count++;
	}
	Serial.println((float) (sum / (float) count));

	//Serial.print("Thermocouple Volt: "); 
	//Serial.println(analogRead(A0));
	// Check and print any faults
	uint8_t fault = max.readFault();
	if (fault) {
		if (fault & MAX31856_FAULT_CJRANGE) {
			Serial.println("Cold Junction Range Fault");
		}
		if (fault & MAX31856_FAULT_TCRANGE) {
			Serial.println("Thermocouple Range Fault");
		}
		if (fault & MAX31856_FAULT_CJHIGH) {
			Serial.println("Cold Junction High Fault");
		}
		if (fault & MAX31856_FAULT_CJLOW) {
			Serial.println("Cold Junction Low Fault");
		}
		if (fault & MAX31856_FAULT_TCHIGH) {
			Serial.println("Thermocouple High Fault");
		}
		if (fault & MAX31856_FAULT_TCLOW) {
			Serial.println("Thermocouple Low Fault");
		}
		if (fault & MAX31856_FAULT_OVUV) {
			Serial.println("Over/Under Voltage Fault");
		}
		if (fault & MAX31856_FAULT_OPEN) {
			Serial.println("Thermocouple Open Fault");
		}
	}
	delay(100);
}