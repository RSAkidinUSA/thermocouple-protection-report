#include "tc_sim.h"

#include <fstream>
#include <stdlib.h>

serial Serial;
extern Adafruit_MAX31856 max;
std::ifstream inFile;

int main (int argc, char **argv) {
	if (argc >= 2) {
		max.setSrc((char *)argv[1]);
	} else {
		cout << "Usage: " << argv[0] << " datafile" << endl;
		return 1;
	}
	setup();
	/* re-randomize random for simulation */
	srand(time(NULL));
	while (1) {
		loop();
	}
	return 0;
}

/* Thermocouple functions */

Adafruit_MAX31856::Adafruit_MAX31856(int pin_cs, int pin_di, int pin_do, 
		int pin_clk) {

}

void Adafruit_MAX31856::setSrc(char *filename) {
	inFile.close();
	if (filename) {
		strncpy(src, filename, FILE_NAME_MAX - 1);
		inFile.open(src, std::ifstream::in);
	} else {
		cout << "Unable to open file\n";
		exit(1);
	}
}

void Adafruit_MAX31856::begin(void) {
	// cout << "Began" << endl;
}

uint8_t Adafruit_MAX31856::readFault(void) {
	return 0;
}

void Adafruit_MAX31856::setThermocoupleType(int t) {

}

/* how often to start random */
#define RAND_START_FREQ	10
/* How long to stay random */
#define RAND_STOP_FREQ	4
/* How frequently to stay random for a longer interval */
#define RAND_LONG_NUM	4
/* How long to stay random for long interval */
#define RAND_CYCLES_MAX	100

/* Random upper bound generation between UB_MAX and UB_MIN */
#define UB_MAX 100.00
#define UB_MIN 98.00
#define ubrand() (UB_MIN + static_cast <float> (rand()) / \
		(static_cast <float> (RAND_MAX/(UB_MAX - UB_MIN))))

/* Random upper bound generation between UB_MAX and UB_MIN */
#define LB_MAX 2.00
#define LB_MIN 0.00
#define lbrand() (LB_MIN + static_cast <float> (rand()) / \
		(static_cast <float> (RAND_MAX/(LB_MAX - LB_MIN))))

float Adafruit_MAX31856::readThermocoupleTemperature(void) {
	static bool useRandom = false;
	static bool LBnotUB = false;
	static int randCount = 0, randCycles = 0;
	float f;
	char junk;
	/* read in next value from file */
	inFile >> f;
	
	/* if bad value skip it */
	while (inFile.fail()) { 
		inFile.clear();
		inFile >> junk;
		/* If file ends restart */
		if (inFile.eof()) { 
			inFile.clear();
			inFile.close();
			exit(0);
		}
		inFile >> f;
	}

	/* Randomly set it to a random value */
	if (useRandom) {
		/* use upper or lower bound based on selection */
		f = LBnotUB ? lbrand() : ubrand();
		if (randCount == RAND_LONG_NUM - 1) {
			if (++randCycles >= RAND_CYCLES_MAX) {
				useRandom = false;
			}
		} else if (!(rand() % RAND_STOP_FREQ)) {
			useRandom = false;
		}
	} else if (!(rand() % RAND_START_FREQ)) {
		useRandom = true;
		LBnotUB = (bool) (rand() % 2);
		randCount++;
		randCount %= RAND_LONG_NUM;
		randCycles = 0;
	}

	/* output the actual value to stderr */
	cerr << f << endl;

	return f;
}

/* Serial functions */

void serial::begin(int t) {

}

void serial::println(float f) {
	cout << f << endl;
}

void serial::println(const char *c) {
	cout << c << endl;
}