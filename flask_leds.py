from flask import Flask, render_template, request
import RPi.GPIO as GPIO

app = Flask(__name__)
GPIO.setmode(GPIO.BCM)

leds = {
	23: {'name' : 'Red LED', 'state' : GPIO.LOW},
	24: {'name' : 'Green LED', 'state' : GPIO.LOW},
	25: {'name' : 'Blue LED', 'state' : GPIO.LOW}
}

for led in leds:
	GPIO.setup(led, GPIO.OUT)
	GPIO.output(led, GPIO.LOW)

def getGpioState():
	for led in leds:
		leds[led]['state'] = GPIO.input(led)
	return leds

@app.route("/")
def main():
	gpioState = {
		'leds' : getGpioState()
	}

	return render_template('leds.html', **gpioState)

def action(led, act):
	led = int(led)
	leds = getGpioState()
	dev = leds[led]['name']

	if act == "on":
		GPIO.output(led, GPIO.HIGH)
		msg = "Truned " + dev + " on."
	elif act == "off":
		GPIO.output(led, GPIO.LOW)
		msg = "Truned " + dev + " off."
	elif act == "toggle":
		GPIO.output(led, not GPIO.input(led))
		msg = "Toggled " + dev + "."
	else:
		msg = "Undefined action!"

	gpioState = {
		'msg' : msg,
		'leds' : getGpioState()
	}

	return render_template('main.html', **gpioState)

if __name__ == "__main__":
	app.run(host='0.0.0.0', port=9999, debug=True)

