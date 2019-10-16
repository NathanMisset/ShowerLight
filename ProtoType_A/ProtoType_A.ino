#include "ThingSpeak.h"
#include "secrets.h"
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <FastLED.h>
#include "pitches.h"

#define Humid_PIN D1
#define Sound_PIN D6
#define LED_PIN D4
#define LED_COUNT 3

CRGB leds[LED_COUNT];

//WiFi information
char ssid[] = SECRET_SSID;   // your network SSID (name) 
char pass[] = SECRET_PASS;   // your network password
int keyIndex = 0;            // your network key Index number (needed only for WEP)
WiFiClient  client;


// ThingSpeak information
char thingSpeakAddress[] = "api.thingspeak.com";
unsigned long myChannelNumber = 871843;
char* myreadAPIKey = "7112IEISHV53QY3K";
unsigned int dataFieldOne   = 1;  //Humid                     
unsigned int dataFieldTwo   = 2;  //Light                                                    
   

//Sounds
// notes in the melody:
  int melodyMain[] = {
  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
 float noteDurationsMain[] = {
  4, 8, 8, 4, 4, 4, 4, 4
};

//misc Variable
int number = 0;

float checkData1;
float checkData2;

void setup() {
  Serial.begin(115200);  // Initialize serial
  WiFi.mode(WIFI_STA); 
  FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, LED_COUNT);
  leds[0].setRGB(10,0,0);
  FastLED.show();
}

void loop() {
  connectWiFi();
  //checkData2 = readDataTS(dataFieldTwo);
  checkData1 = readDataTS(dataFieldOne);
  
  Serial.println(String(checkData2));
  //High Humidity Detected
  if(checkData1 > 208){ // && checkData2 < 200
    leds[0].setRGB(0,10,0);
    leds[1].setRGB(0,10,0);
    leds[2].setRGB(10,0,0);
    FastLED.show();
    PlayerMelody(melodyMain, noteDurationsMain);
    //BathRoom Light are on
  //}else if(checkData2 > 40){ && checkData1 < 208
  //  leds[0].setRGB(0,10,0);
  //  leds[1].setRGB(10,0,0);
  // leds[2].setRGB(0,10,0);
  // FastLED.show();
  //  PlayerMelody(melodyMain, noteDurationsMain);
    // Low Light and Low Humidity
  } else{ 
    leds[0].setRGB(0,10,0);
    leds[1].setRGB(0,10,0);
    leds[2].setRGB(0,10,0);
    FastLED.show();
  }
  
 

  delay(15000);
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
      delay(500);
      leds[0].setRGB(0,0,0);
      FastLED.show();
      delay(4500);     
      }
    ThingSpeak.begin(client);
    Serial.println( "Connected" );
    leds[0].setRGB(0,10,0);
    FastLED.show();
    PlayerMelody(melodyMain, noteDurationsMain);
    }
  }

float readDataTS( int TSField ){
  leds[0].setRGB(0,10,0);
  FastLED.show();
  delay(500);
  leds[0].setRGB(0,0,0);
  FastLED.show();
      
  float data =  ThingSpeak.readFloatField( myChannelNumber, TSField, myreadAPIKey );
  Serial.println( " Data read from ThingSpeak: " + String( data, 9 ) );
  return data;
  
}

int PlayerMelody( int melody[],  float noteDurations[]) {
  for (int thisNote = 0; thisNote < 8; thisNote++) {

    int noteDuration = 1000 / noteDurations[thisNote];

    tone(Sound_PIN, melody[thisNote], noteDuration);

    int pauseBetweenNotes = noteDuration * 1.30;

    delay(pauseBetweenNotes);

    noTone(8);
    return 0;
  }
}
