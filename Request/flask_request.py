from time import sleep
from flask import Flask, render_template, request

app = Flask(__name__)
@app.route("/")
def main():
	param = request.args.get('msg', None)

	templateData = {
		'text' : param
	}

	return render_template(‘index.html’, **templateData)

if __name__ == "__main__":
	app.run(host='0.0.0.0', port=9999, debug=True)
	