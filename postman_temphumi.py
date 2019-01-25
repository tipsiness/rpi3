import sys
import Adafruit_DHT
import time
import requests

url = "https://api.artik.cloud/v1.1/messages"

headers = {
	'Content-Type': "application/json",
	'Authorization': "Bearer 371cbd04bffb495daa398f8d6940a398",
	'cache-control': "no-cache",
	'Postman-Token': "1502b7e6-12b7-410b-b5b9-0046e0a06972"
}

try:
	while True:
		humi, temp = Adafruit_DHT.read_retry(Adafruit_DHT.DHT11, 4)

		payload = "{\"data\": {\"temperature\": " +str(temp)+  ",\"humidity\": " +str(humi)+ "},\"sdid\": \"4f550ef3656c474ba755c763f91698f5\", \"type\": \"message\"}"

		response = requests.request("POST", url, data=payload, headers=headers)
		print(response.text)
		time.sleep(int(sys.argv[1]))
except KeyboardInterrupt:
	print "Terminated."
	sys.exit(0)


