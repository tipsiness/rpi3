import spidev
import time
import sys, os

spi_dev = spidev.SpiDev()

def read_adc(adc_channel):
	spi_dev.open(0, adc_channel)
	rx_data = spi_dev.xfer2([6, 0, 0])
	adc_value = (rx_data[1] << 8) + rx_data[2]
	return adc_value

try:
	while True:
		print "adc value: " + str(read_adc(0)).zfill(4)

		time.sleep(1)
except KeyboardInterrupt:
	print "Terminating SPI Read"
	spi_dev.close()
	sys.exit(0)



