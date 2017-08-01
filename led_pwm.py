import RPi.GPIO as GPIO
import time

GPIO.setmode(GPIO.BOARD)

RANGE = 60
LED = 11

GPIO.setup(LED, GPIO.OUT)
LED = GPIO.PWM(LED, RANGE)

LED.start(0)
delay = 0.05

try:
    while True:
        for i in range(0, RANGE+1):
            LED.ChangeDutyCycle(i)
            time.sleep(delay)
        for i in range(RANGE, -1, -1):
            LED.ChangeDutyCycle(i)
            time.sleep(delay)

except KeyboardInterrupt:
    LED.stop()
    GPIO.cleanup()
