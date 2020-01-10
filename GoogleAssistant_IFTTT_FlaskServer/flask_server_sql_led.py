from flask import Flask, redirect, url_for, request, render_template, jsonify
import json
import urllib2
import datetime
import RPi.GPIO as GPIO

GPIO.setmode(GPIO.BOARD)
GPIO.setup(11, GPIO.OUT)

import mysql.connector
from mysql.connector.constants import ClientFlag

print ("MySQL connector module import succeed")

config = {
	'user':'root',
	'password':'YOUR_PASSWD',
	'host':'YOUR_SQL_IP',
	'client_flags':[ClientFlag.SSL],
	'database':'YOUR_DB_NAME',
	'ssl_ca':'/home/pi/server-ca.pem',
	'ssl_cert':'/home/pi/client-cert.pem',
	'ssl_key':'/home/pi/client-key.pem',
}

app = Flask(__name__)

apiKey = 'YOUR_API_KEY'
baseURL = 'http://api.thingspeak.com/update?api_key=%s' % apiKey

@app.route('/success/<name>')
def success(name):
	return 'state is %s' % name

@app.route('/', methods=['POST', 'GET'])
def main():
    data = request.get_json(silent=True)
    resp = json.dumps(data)
    values = json.loads(resp)

    print(resp)

    name = values['name']
    state = values['state']
    print "Device Name: ", name
    print "Device State: ", state
    
    stateValue = 0
    if state == 'on':
	stateValue = 1
	GPIO.output(11, GPIO.HIGH)
	print "Lamp is turned on."
    elif state == 'off':
    	stateValue = 0
	GPIO.output(11, GPIO.LOW)
	print "Lamp is turned off."

    conn = mysql.connector.connect(**config)

    cursor = conn.cursor(buffered=True)
    sqlQueryStr = """INSERT INTO device (name, state, position, datetime) VALUES (%s, %s, %s, %s)"""
    dt = datetime.datetime.now()
    dtStr = dt.strftime("%Y-%m-%d %H:%M:%S")
    print (dtStr)

    sqlQueryParam = (name, stateValue, "bathroom", dtStr)
    cursor.execute(sqlQueryStr, sqlQueryParam)

    query = urllib2.urlopen(baseURL + '&field1=%s' % (str(stateValue)))
    query.read()
    query.close()

    conn.commit()
    cursor.close()
    conn.close()

    return resp

if __name__ == "__main__":
    app.run(host='0.0.0.0', port=8888, debug=True)
