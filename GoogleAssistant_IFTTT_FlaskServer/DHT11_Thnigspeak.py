import sys, time
import Adafruit_DHT
import urllib2

apikey = '855JY2QP79R3WGQJ'
baseURL = 'https://api.thingspeak.com/update?api_key=%s' % apikey 

sensor = Adafruit_DHT.DHT22
pin = 4

try:
    while True:
        humidity, temperature = Adafruit_DHT.read_retry(sensor, pin)
        if humidity is not None and temperature is not None:
            print('Temp={0:0.1f}*  Humidity={1:0.1f}%'.format(temperature, humidity))
			
			query = urllib2.urlopen(baseURL + '&field1=%s&field2=%s' % (str(humidity), str(temperature)))
			query.read()
			query.close()
			
        else:
            print('Failed to get reading. Try again!')
            sys.exit(1)

        time.sleep(3)
except KeyboardInterrupt:
    print "terminated"
    sys.exit(0)
