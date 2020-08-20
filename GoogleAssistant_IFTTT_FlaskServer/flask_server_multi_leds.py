from flask import Flask, redirect, url_for, request, render_template, jsonify
import json
import RPi.GPIO as GPIO
import urllib2

GPIO.setmode(GPIO.BOARD)
GPIO.setup(11, GPIO.OUT)
GPIO.setup(12, GPIO.OUT)

app = Flask(__name__)

apiKey = ''
baseURL = 'http://api.thingspeak.com/update?api_key=%s' % apiKey

@app.route('/', methods=['POST', 'GET'])
def main():
  data = request.get_json(silent=True)
  resp = json.dumps(data)
  values = json.loads(resp)

  print(resp)

  name = values['name']
  state = values['state']
  print "Device name: ", name
  print "Device State: ", state
    
  greenStateValue = 0
  blueStateValue = 0

  if name == 'green':
    if state == 'on':
      greenStateValue = 1
      GPIO.output(11, GPIO.HIGH)
      print "Green Lamp is turned on."
    elif state == 'off':
      greenStateValue = 0
      GPIO.output(11, GPIO.LOW)
      print "Green Lamp is turned off."
  elif name == 'blue':
    if state == 'on':
      blueStateValue = 1
      GPIO.output(12, GPIO.HIGH)
      print "Blue Lamp is turned on."
    elif state == 'off':
      blueStateValue = 0
      GPIO.output(12, GPIO.LOW)
      print "Blue Lamp is turned off."


  query = urllib2.urlopen(baseURL + '&field1=%s&field2=%s' % (str(greenStateValue), str(blueStateValue)))
  query.read()
  query.close()
    
  return resp

if __name__ == "__main__":
    app.run(host='0.0.0.0', port=8888, debug=True)
