var webSocketUrl = "wss://api.artik.cloud/v1.1/websocket?ack=true";
var device_id = "";
var device_token = "";

var WebSocket = require('ws');
var isWebSocketReady = false;
var ws = null;

var puts = require('sys').puts;

setInterval(function() {
		readAdcCds();
		//sendSensorValueToArtikCloud();
		}, 5000);

function getTimeMillis(){
	return parseInt(Date.now().toString());
}

function start() {
	//Create the WebSocket connection
	isWebSocketReady = false;
	ws = new WebSocket(webSocketUrl);
	ws.on('open', function() {
			console.log("WebSocket connection is open ....");
			register();
			});
	ws.on('message', function(data) {
			console.log("Received message: " + data + '\n');
			//handleRcvMsg(data);
			});
	ws.on('close', function() {
			console.log("WebSocket connection is closed ....");
			exitClosePins();
			});
}

function register(){
	console.log("Registering device on the WebSocket connection");
	try{
		var registerMessage = 
			'{"type":"register", "sdid":"'+device_id+'", "Authorization":"bearer '+device_token+'", "cid":"'+getTimeMillis()+'"}';
		console.log('Sending register message ' + registerMessage + '\n');
		ws.send(registerMessage, {mask: true});
		isWebSocketReady = true;
	}
	catch (e) {
		console.error('Failed to register messages. Error in registering message: ' + e.toString());
	}    
}

function readAdcCds() {
	var spi = require('spi-device'), mcp3204;

	// The MCP3008 is on bus 0 and it's device 0
	mcp3204 = spi.open(0, 0, function (err) {
		var message = [{
			sendBuffer: new Buffer([0x06, 0x00, 0x00]), 
			receiveBuffer: new Buffer(3),               
			byteLength: 3,
			speedHz: 1000000 
		}];

		if (err) throw err;

		mcp3204.transfer(message, function (err, message) {
			var rawValue,
			voltage,
			brightness;

			if (err) throw err;

			rawValue = ((message[0].receiveBuffer[1] & 0x0F) << 8) + message[0].receiveBuffer[2];
			//voltage = rawValue * 3.3 / 1023;
			//celcius = (voltage - 0.5) * 100;
			voltage = rawValue * (5.0 / 4096);

			var resistor = 5000.0/voltage - 1000.0;

			resistor = resistor / 1000;
			if (resistor >= 10) resistor = 10;
			if (resistor < 1) resistor = 1;

			resistor = 11 - resistor;

			brightness = resistor * 19 + 10;

			var lux = brightness;
			//console.log(lux);

			sendSensorValueToArtikCloud(lux);
		});
	});
}

function sendSensorValueToArtikCloud(lux){
	try{
		ts = ', "ts": '+getTimeMillis();
		//var brig = Math.floor((Math.random() * 100) + 1);
		//readAdcCds();
		var data = {
			"brightness": lux 
		};
		var payload = '{"sdid":"'+device_id+'"'+ts+', "data": '+JSON.stringify(data)+', "cid":"'+getTimeMillis()+'"}';        
		console.log('Sending payload ' + payload + '\n');
		ws.send(payload, {mask: true});
	} catch (e) {
		console.error('Error in sending a message: ' + e.toString() +'\n');
	}    
}

function exitClosePins() {
	console.log('Exit and destroy all pins!');
	process.exit();
}

start();

