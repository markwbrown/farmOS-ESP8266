/*
farmOS Wi-Fi Sensor on ESP8266
Below is some code I put together to post Sensor Data to farmOS. 
You will need a wi-fi access point, the ssid & credentials to that access point and 
farmOS running on a https endpoint. 
Within farmOS, create your sensor, taking note of the public/private keys and the name of your sensor. You will also need the Fingerprint of your site's ssl cert. See link below on how to obtain that.

The libraries are pretty standard and can be found/managed from Sketch>Include Library>Manage Libraries

Simply plugin the relevant information in the variables below, flash your board and every 30 seconds you'll have a post. Obviously, there are more minute details you'll want to incorporate, and libraries you will no doubt need for various sensors (oneWire, DHT, thermocouple, hall effect sensor, etc. -- this is just a springboard for you getting sensor data into farmOS)

In this example, I'm posting a value of 1200 to the sensor "Wind_001"

If you have questions, feel free to drop me a line at mark@markbrown.space

*/


#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
char ssid[] = "yourSSID";
char pass[] = "yourWiFiPass";
String url = "https://domain.youare.at";
String listener = "/farm/sensor/listener/";
String publicKey = "sensorPublicKey";
String split = "?private_key=";
String privateKey = "sensorPrivateKey";
//to get ssl's certificate fingerprint, go to https://www.grc.com/fingerprints.htm and enter your site's address. https should be used everywhere, and you should never let facebook log you into things. 
String fingerprint = "xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx";

void setup() {

Serial.begin(115200);
WiFi.begin(ssid, pass);

while (WiFi.status() != WL_CONNECTED)

delay(500);
Serial.println("Waiting for connection");

}

void loop() {

if(WiFi.status()== WL_CONNECTED){ //Check WiFi connection status

HTTPClient http;
http.begin(url+listener+publicKey+split+privateKey , fingerprint);

http.addHeader("Content-Type", "application/json");
http.addHeader("Accept", "application/json");

int httpCode = http.sendRequest("POST", "{\"Wind_001\": 1200}");
Serial.print("http result:");
Serial.println(httpCode);
http.writeToStream(&Serial);
http.end();

}else{
Serial.print("Error in Wifi connection");
}

delay(30000); //Send a request every 30 seconds

}