#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <unistd.h>

#define DHTPIN			7

int dht_data[] = {0, 0, 0, 0, 0};

static int read_dht_data() {
	uint8_t laststate = HIGH;
	uint8_t counter = 0;
	uint8_t i, j = 0;

	dht_data[0] = dht_data[1] = dht_data[2] = dht_data[3] = dht_data[4] = 0;
	/* Single-Wire Two-Way Communication */
	pinMode(DHTPIN, OUTPUT);

	/* MCU->DHT, start signal */
	digitalWrite(DHTPIN, HIGH);
	delay(1);

	/* MCU pull down, DHT detect a start signal from MCU */
	digitalWrite(DHTPIN, LOW);
	delay(18);

	/* MCU pull up to receive response signal from DHT */
	digitalWrite(DHTPIN, HIGH);
	delayMicroseconds(40);

	/* MCU read response data consist of temperature and humidity */
	pinMode(DHTPIN, INPUT);
	delayMicroseconds(1);

	laststate = digitalRead(DHTPIN);

	for (i = 1; i <= 82; i++) {
		counter = 0;
		while (digitalRead(DHTPIN) == laststate) {
			counter++;
			delayMicroseconds(1);

			if (counter >= 75)
				break;
		}
		laststate = digitalRead(DHTPIN);

		if ((i > 3) && (i % 2 == 0)) {
			dht_data[j/8] <<= 1;		// read bit '0'
			if (counter > 28)
				dht_data[j/8] |= 1;	// read bit '1'
			j++;
		} 
	}
	if ((j >= 40) && (dht_data[4] == \
				((dht_data[0] + dht_data[1] + dht_data[2] +  \
				  dht_data[3]) & 0xFF))) {

		printf("Humidity = %d %% Temperature = %d *C\n", dht_data[0], dht_data[2]);

		return 1;
	} else {
		printf("Data not good, skip (read bytes: %d).\n", j);
		//printf("0x%04x 0x%04x 0x%04x 0x%04x 0x%04x\n", dht_data[0], dht_data[1], dht_data[2], dht_data[3], dht_data[4]);
		return 0;
	}
}
int main() {
	int ret;

	if (wiringPiSetup() == -1)
		exit(EXIT_FAILURE);

	while (1) {
		read_dht_data();

		delay(2000);
	}

	return 0;
}
