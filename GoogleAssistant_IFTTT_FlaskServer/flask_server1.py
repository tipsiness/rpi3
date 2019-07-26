from flask import Flask, redirect, url_for, request, render_template, jsonify
import json

app = Flask(__name__)

@app.route('/success/<name>')
def success(name):
	return 'state is %s' % name

@app.route('/', methods=['POST', 'GET'])
def main():
	data = request.get_json(silent=True)
	resp = json.dumps(data)
	print(resp)
	return resp

if __name__ == "__main__":
    app.run(host='0.0.0.0', port=8888, debug=True)


