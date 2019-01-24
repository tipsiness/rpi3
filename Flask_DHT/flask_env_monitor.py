from flask import Flask, render_template
import datetime
import sys
import Adafruit_DHT
import time

app = Flask(__name__)

@app.route("/")
def readDHT():
	humidity, temperature = Adafruit_DHT.read_retry(Adafruit_DHT.DHT11, 4)
	localtime = datetime.datetime.now()
	strTime = localtime.strftime("%Y-%m-%d %H:%M")

	templateData = {
		'title': 'DHT11 Temperature & Humidity',
		'time': strTime,
		'humi': humidity,
		'temp': temperature
	}

	return render_template('temphumi.html', **templateData)

if __name__ == "__main__":
	app.run(host='0.0.0.0', port=9999, debug=True)

