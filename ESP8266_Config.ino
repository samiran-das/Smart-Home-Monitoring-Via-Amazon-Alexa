/*
 * This code updates wheather information in thingspeak channels and also retrieve control information from thingspeak channel
 * After getting the information it sends the control information to arduino serially
 * Samiran Das - github.com/samiran-das
 */

#include <ESP8266WiFi.h>
#include<String.h>
#include "DHT.h"        // including the library of DHT11 temperature and humidity sensor
#define DHTTYPE DHT11   // DHT 11

#define dht_dpin 2      // define GPIO2 as data input 
DHT dht(dht_dpin, DHTTYPE); 
//------------------------------------------
//       variables
//------------------------------------------
//dht DHT;
char c,e;                       // stores the bytes coming from the server

float temp,h;                   //stores float value of temp and humididty
String humidity,temperature,line;    //stores string value of temp and humididty
int co=0;                       //control the sending of Alert

const char* ssid     = "SSID";            //your SSID of WIFI
const char* password = "password";       // password of Wifi
//--------------------------------------------

const char* host = "api.thingspeak.com";

void setup() {
  dht.begin();
  Serial.begin(115200);
  delay(10);

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
     would try to act as both a client and an access-point and could cause
     network-issues with your other WiFi-devices on your WiFi-network. */
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

int value = 0;

void loop() {
  delay(1000);
  ++value;

  Serial.print("connecting to ");
  Serial.println(host);
  
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }

  Serial.print("Requesting URL: ");
 
 // Mini Weather Station

  h = dht.readHumidity();                    //reads humidity and temperature
  temp = dht.readTemperature();
  if(temp<50.0)
  {
    co=0;
  }
  if(temp>=50.0&&co==0)      // send alert if temperature is above 50 degrees and co is used to avoid multiple alert sending
  {
    Serial.println("Sending Alert please wait.......");
    delay(1000);
    String host1="api.pushingbox.com";
    client.print(String("GET ")  + "/pushingbox?devid=<deviceID_fireAlert> HTTP/1.1\r\n" +
               "Host: " + host1 + "\r\n" + 
               "Connection: close\r\n\r\n");           //pushingbox api to send alert 
    delay(1000);
     client.print(String("GET ")  + "/update?key=<write_key_fireAlert>&field1=1 HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");           //updating thingspeak fire alert channel
    co=1;
  }
  humidity=String(h);
  temperature=String(temp); 
  Serial.println(h);
  Serial.println(temp);   
  client.print(String("GET ")  + "/update?key=<write_key_humidity>&field1="+humidity+" HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");                                                         //update humidity in thingspeak channel
  client.print(String("GET ")  + "/update?key=<write_key_temperature>&field1="+temperature+" HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");                                                       //update temperature in thingspeak channel      



// Sending Control info to arduino Serially
// This will send the request to the server
  client.print(String("GET ")  + "/channels/<channelID_control>/field/field1/last.html HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");         // get information about device control
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }
  
  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    String line = client.readStringUntil('\r');
  }
  Serial.print(line);                         // received by arduino serially
 //Serial.println("closing connection");
}
