#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

int relay_port = 27;

int main() {
	int ret;
	char buf[2];
	wiringPiSetup();

	pinMode(relay_port, OUTPUT);

	while (1) {
		ret = read(STDIN_FILENO, buf, 2);

		if (buf[0] == '1') {
			digitalWrite(relay_port, HIGH);
		} else if (buf[0] == '0') {
			digitalWrite(relay_port, LOW);
		} else {
			printf("wrong command.\n");
		}

		usleep(100000);
	}
	return 0;
}

