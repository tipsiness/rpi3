from flask import Flask, redirect, url_for, request, render_template, jsonify
import json
import urllib2
import RPi.GPIO as GPIO

GPIO.setmode(GPIO.BOARD)
GPIO.setup(11, GPIO.OUT)

apiKey = ''
baseURL = 'http://api.thingspeak.com/update?api_key=%s' % apiKey

app = Flask(__name__)

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
	
  query = urllib2.urlopen(baseURL + '&field1=%s' % (str(stateValue)))
  query.read()
  query.close()
    
  return resp

if __name__ == "__main__":
    app.run(host='0.0.0.0', port=8888, debug=True)
