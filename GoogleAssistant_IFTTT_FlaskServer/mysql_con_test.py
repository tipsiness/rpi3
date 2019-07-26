import mysql.connector
from mysql.connector.constants import ClientFlag

print ("MySQL connector module import succeed")

config = {
	'user':'root',
	'password':'root',
	'host':'35.229.99.14',
	'client_flags':[ClientFlag.SSL],
	'database':'room1',
	'ssl_ca':'/home/pi/server-ca.pem',
	'ssl_cert':'/home/pi/client-cert.pem',
	'ssl_key':'/home/pi/client-key.pem',
}

conn = mysql.connector.connect(**config)

conn.close()

