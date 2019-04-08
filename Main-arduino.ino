//This code is the main program for Arduino and acts as a home automation
// Samiran Das - github.com/samiran-das

#include <SoftwareSerial.h>// import the serial library
#include<String.h>
//-----------------------------------------
//    pin definations
//-----------------------------------------
#define Light 2              // pin 2 to control light
#define fan 3                 // pin 3 to control fan
//------------------------------------------
//       variables
//------------------------------------------
char e;
int p=0,q=0;        //these variables prevent switching ON/OFF of already turned ON/OFF devices

SoftwareSerial home_monitor(10, 11); // RX, TX

char control_data; // the data transmitted over Bluetooth

void setup() {
  // put your setup code here, to run once:
  home_monitor.begin(115200);
  Serial.begin(9600);
  pinMode(13,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (home_monitor.available()){
     control_data=home_monitor.read();   // getting control information from ESP8266 serially
     e=control_data;
     Serial.print(control_data);

 // Performing required operations based on the value of channel

    if(e=='0'&&p==0)         //Light ON            
    {
      digitalWrite(Light,LOW);
      p=1;
    }
     if(e=='1'&&p==1)        //Light off
    {
      digitalWrite(Light,HIGH);
      p=0;
    }
 
    if(e=='2'&&q==0)
    {
      digitalWrite(fan,HIGH);
      q=1;
    }
    if(e=='3'&&q==1)
    {
      digitalWrite(fan,LOW);
      q=0;
    }
  }
}
