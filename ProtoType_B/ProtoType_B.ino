#include <ThingSpeak.h>
#include "secret.h"
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <FastLED.h>

#define LED_PIN D2
#define LED_COUNT 3
#define BUTTON_PIN D1

CRGB leds[LED_COUNT];

//WiFi information
char ssid[] = SECRET_SSID;   // your network SSID (name) 
char pass[] = SECRET_PASS;   // your network password
int keyIndex = 0;            // your network key Index number (needed only for WEP)
WiFiClient  client;

int data[15];
int dataCounter = 0;
int completeData = 0;
float timerNow = 0;

bool WPSconnection = false;

// ThingSpeak information
char thingSpeakAddress[] = "api.thingspeak.com";
unsigned long myChannelNumber = 871843;
const char * myWriteAPIKey = "MF5PRQJQPY97OK47";
unsigned int dataFieldOne   = 1;  //Light                     
unsigned int dataFieldTwo   = 2;  //Humid                                                    
unsigned int dataFieldThree = 3;  //Check                   

void setup() {
  Serial.begin(115200);  // Initialize serial
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  WiFi.mode(WIFI_STA); 
  FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, LED_COUNT);
  leds[0].setRGB(10,0,0);
  FastLED.show();
}

void loop() {
   //Serial.println(dataCounter);
   connectWiFi();
   data[dataCounter] = analogRead(A0);
   if(millis() - timerNow > 1000  ){
    timerNow = millis();
    dataCounter++;
   }
   //Serial.println(analogRead(A0));
   if (dataCounter == 14){
    dataCounter = 0;
    completeData = 0;
    for(int i = 0; i < 15; i++){
      completeData += data[i];
    }
    writeDataTS(completeData);
   }
  }

  
// Try to connect to wifi until succesfull
int connectWiFi(){   
  while (WiFi.status() != WL_CONNECTED) {
    // One led light burn blue when connecting/Reconnecting to WiFi
    delay(1000);
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid,pass);
      Serial.print(".");
      leds[0].setRGB(0,0,10);
      FastLED.show();
      leds[0].setRGB(0,0,0);
      FastLED.show();
      delay(5000);     
      }
    ThingSpeak.begin(client);
    Serial.println( "Connected" );
    leds[0].setRGB(0,10,0);
      FastLED.show();
    }
  }

// Writes data to Thingspeak
int writeDataTS(int data ){
  ThingSpeak.setField(1, data / 15);
  
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  
  if(x == 200){
    leds[0].setRGB(0,0,0);
      FastLED.show();
      leds[0].setRGB(0,10,0);
      FastLED.show();
    Serial.println("Channel update successful.");
    } else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
    }
  return 0;
  }




//The following methode is used to connect to WIFI Via WPS 
//But i couldn't send any data via this methode eventhough i was able to connect to wifi 
// This is also the place where i implemented the button
//bool connectWPS() {
//  while (WiFi.status() != WL_CONNECTED) {
//    Serial.println("Waiting for Button press");
//     leds[0].setRGB(255,145,0);
//     FastLED.show()
//  while(digitalRead(BUTTON_PIN) == LOW) yield();
//      Serial.println("WPS button pressed"); 
//      Serial.println("WPS config start");
//      leds[0].setRGB(255,255,0);
//      FastLED.show()
//      bool wpsSuccess = WiFi.beginWPSConfig();
//      if(wpsSuccess) {
//        String newSSID = WiFi.SSID();
//        if(newSSID.length() > 0) {
//          Serial.printf("WPS finished. Connected successfull to SSID '%s'\n", newSSID.c_str());
//          Serial.println(WiFi.status());
//        } else {
//          Serial.println("not connected");
//          return false;
//        }
//      }
//    leds[0].setRGB(0,10,0);
//    FastLED.show();
//    ThingSpeak.begin(client);
//    return true;
//    }
//}
