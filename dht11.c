#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <unistd.h>

#define DHTPIN 7

unsigned char get_value(void);

int data[] = {0, 0, 0, 0, 0};

int main(int argc, char *argv[]) {
	int temp, humi, i, j, tmp;

	while (1) {
		pinMode(DHTPIN, OUTPUT);

		digitalWrite(DHTPIN, HIGH);
		delay(1);

		digitalWrite(DHTPIN, LOW);
		delay(18);

		digitalWrite(DHTPIN, HIGH);
		delayMicroseconds(40);

		pinMode(DHTPIN, INPUT);

		delayMicroseconds(160);

		for (i = 0; i < 5; i++) {
			for (j = 0; j < 8; j++) {
				if (j < 7) {
					data[i] |= get_value();
					data[i] = data[i] << 1;
				} else {
					data[i] |= get_value();
				}
			}
		}

		tmp = 0;

		for (i = 0; i < 4; i++) {
			tmp += data[i];
		}
		if (data[4] == (tmp & 0xFF)) {
			humi = data[0];
			temp = data[2];

			printf("Temperature: %d, Humidity: %d\n", temp, humi);
		} else {
			printf("Parity Error\n");
			printf("data[0]: %d, data[1]: %d, data[2]: %d, data[3]: %d, \
					data[4]: %d\n", data[0], data[1], data[2], data[3], data[4]);
		}
		delay(4000);
	}


	return 0;
}

unsigned char get_value(void) {
	unsigned char va = 0;
	int state;

	while (digitalRead(DHTPIN) == 0);

	delayMicroseconds(28);

	if (digitalRead(DHTPIN) == 1) {
		va |= 1;

		while (digitalRead(DHTPIN) == 1);
	}

	return va;
}

