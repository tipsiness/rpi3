from flask import Flask, redirect, url_for, request, render_template, jsonify
import json
import RPi.GPIO as GPIO

GPIO.setmode(GPIO.BOARD)
GPIO.setup(11, GPIO.OUT)

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
    
  return resp

if __name__ == "__main__":
    app.run(host='0.0.0.0', port=8888, debug=True)
