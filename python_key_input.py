import RPi.GPIO as GPIO
import time

KEY_UP = 11
KEY_DOWN = 13
LED = 12

GPIO.setmode(GPIO.BOARD)
GPIO.setup(LED, GPIO.OUT)
GPIO.setup(KEY_UP, GPIO.IN, pull_up_down=GPIO.PUD_UP)
GPIO.setup(KEY_DOWN, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)

def cbfunction(self):
	print "callback function."
	GPIO.output(LED, GPIO.HIGH)
	time.sleep(0.5)
	GPIO.output(LED, GPIO.LOW)

GPIO.add_event_detect(KEY_UP, GPIO.FALLING, callback=cbfunction, bouncetime=200)
GPIO.add_event_detect(KEY_DOWN, GPIO.RISING, callback=cbfunction, bouncetime=200)

time.sleep(20)

GPIO.remove_event_detect(KEY_UP)
GPIO.remove_event_detect(KEY_DOWN)

GPIO.cleanup()

