#!/usr/bin/python
import sys
import Adafruit_DHT
import time

pin = sys.argv[1]

try:
	while True:
		humidity, temperature = Adafruit_DHT.read_retry(Adafruit_DHT.DHT11, pin)
		if humidity is not None and temperature is not None:
			print('Temp={0:0.1f}*  Humidity={1:0.1f}%'.format(temperature, humidity))
			time.sleep(3)
		else:
			print('Failed to get reading. Try again!')
			sys.exit(1)
except KeyboardInterrupt:
	sys.exit(1)

