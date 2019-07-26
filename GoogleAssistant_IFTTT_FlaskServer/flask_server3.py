from flask import Flask, redirect, url_for, request, render_template, jsonify
import json
import urllib2

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

        query = urllib2.urlopen(baseURL + '&field1=%s' % (str(stateValue)))
        query.read()
        query.close()

	return resp

if __name__ == "__main__":
    app.run(host='0.0.0.0', port=8888, debug=True)


