from flask import Flask, redirect, url_for, request, render_template, jsonify
import json
import urllib2
import datetime

import mysql.connector
from mysql.connector.constants import ClientFlag

print ("MySQL connector module import succeed")

config = {
	'user':'root',
	'password':'',
	'host':'',
	'client_flags':[ClientFlag.SSL],
	'database':'room1',
	'ssl_ca':'/home/pi/server-ca.pem',
	'ssl_cert':'/home/pi/client-cert.pem',
	'ssl_key':'/home/pi/client-key.pem',
}

app = Flask(__name__)

apiKey = ''
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
	elif state == 'off':
		stateValue = 0

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


