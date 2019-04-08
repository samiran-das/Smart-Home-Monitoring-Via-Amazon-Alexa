/*
 *  This code implementations smart bell and home security
 * Samiran Das - github.com/samiran-das

 */

#include <ESP8266WiFi.h>
#include<String.h>

/
int intruder=0,guest=2;                     // pin 0 as intruder alert and pin2 as guest alert
const char* ssid     = "SSID";            //your SSID of WIFI
const char* password = "password";       // password of Wifi


const char* host = "api.thingspeak.com";

void setup() {
  Serial.begin(115200);
  delay(10);

//connecting to a WiFi network

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
int x,y=0,z,k=0;
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

 x=digitalRead(intruder);
//Serial.println(x);
  if(x==0)
  {
    y=0;
  }
  if(x==1&&y==0)
  {
    Serial.println("Sending Alert Pls Wait.....");
    delay(1000);
  String host1="api.pushingbox.com";
    client.print(String("GET ")  + "/pushingbox?devid=<deviceID_intruderAlert> HTTP/1.1\r\n" +
               "Host: " + host1 + "\r\n" + 
               "Connection: close\r\n\r\n");                       // executing pushing box api
    client.print(String("GET ")  + "/update?api_key=<write_key_intruderAlert>&field1=1 HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");                        //updating intruder notification
               
   
    delay(1000);
    y=1;
  } 


long t1;
 z=digitalRead(guest);
  if(z==0 && (millis()>=t1+300000)) // wait for 5 min to avoid multiple alert sending due to multiple door bell press
  {
    k=0;
  }
  if(x==1&&k==0)
  {
    t1=millis();         // storing curent time stamp in t1
    Serial.println("Sending Alert Pls Wait.....");
    delay(1000);                      //delay for sending alert and switch debouncing
  String host1="api.pushingbox.com";
    client.print(String("GET ")  + "/pushingbox?devid=<deviceID_guestAlert> HTTP/1.1\r\n" +
               "Host: " + host1 + "\r\n" + 
               "Connection: close\r\n\r\n");                       // executing pushing box api
    client.print(String("GET ")  + "/update?api_key=<write_key_guestAlert>&field1=1 HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");                        //updating guest notification
               
    delay(1000);
    k=1;
  } 



  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }
 //Serial.println("closing connection");
}
