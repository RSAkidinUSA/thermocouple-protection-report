#include <stdint.h>
#include <cstring>
#include <unistd.h>
#include <iostream>
#include <math.h>

using std::cout;
using std::cerr;
using std::endl;

#define MAX31856_FAULT_CJRANGE	0x01
#define MAX31856_FAULT_TCRANGE	0x02
#define MAX31856_FAULT_CJHIGH	0x04
#define MAX31856_FAULT_CJLOW	0x08
#define MAX31856_FAULT_TCHIGH	0x10
#define MAX31856_FAULT_TCLOW	0x20
#define MAX31856_FAULT_OVUV		0x40
#define MAX31856_FAULT_OPEN		0x80

#define MAX31856_TCTYPE_K		0x01

#define FILE_NAME_MAX	64

#ifdef DELAY_FOR_REAL
#define delay(a) usleep(a * 1000)
#else
#define delay(a) asm("nop")
#endif

class Adafruit_MAX31856 {
	char src[FILE_NAME_MAX];
public:
	uint8_t readFault(void);
	void begin(void);
	void setThermocoupleType(int t);
	float readThermocoupleTemperature(void);
	void setSrc(char *filename);
	Adafruit_MAX31856(int pin_cs, int pin_di, int pin_do, int pin_clk);
};

class serial {

public:
	void begin(int t);
	void println(float f);
	void println(const char *c);
};

/* Random float from 0.0 - 1.0, non-inclusive */
#define frand() (static_cast <float> (rand() - 1) / \
		static_cast <float> (RAND_MAX))

/* Functions */
void setup();
void loop();