// Title: ESP01WiFiSwitch
// Author: Mijaz Mukundan
// License : MIT License
// Description : This code allows ESP01 module to control 4 devices by using fauxmo library to emulate a Philiphs device
// this code is based on FauxmoESP library that can be found at : https://github.com/simap/fauxmoesp


#include <Arduino.h>
#ifdef ESP32
    #include <WiFi.h>
#else
    #include <ESP8266WiFi.h>
#endif

#include "fauxmoESP.h"


// Edit 'credentials.h' according to your router configuration
#include "credentials.h"

fauxmoESP fauxmo;

// -----------------------------------------------------------------------------
// Uncomment the following line if you want to see debug outputs on serial monitor, but this causes only two devices to be detected and controlled
//#define DEBUG 

#define SERIAL_BAUDRATE     115200

// Define device control pins
#define DEVICE_ONE          0 //GPIO0 connected to Relay1/IN1
#define DEVICE_TWO         2 //GPIO2 connected to Relay2/IN2
#define DEVICE_THREE          3 //RX Pin connected to Relay3/IN3
#define DEVICE_FOUR         1 //TX Pin connected to Relay4/IN4

//Define device identification, these are the names you will invoke e.g. "Alexa, turn on 'fan'"
//Change these to suit your own needs
#define ID_DEVICE_ONE    "fan"
#define ID_DEVICE_TWO    "balcony lights"
#define ID_DEVICE_THREE  "left light"
#define ID_DEVICE_FOUR   "right light"


// -----------------------------------------------------------------------------
// Wifi
// -----------------------------------------------------------------------------


void wifiSetup() {

    // Set WIFI module to STA mode
    WiFi.mode(WIFI_STA);

    // Connect
    #ifdef DEBUG
    Serial.printf("[WIFI] Connecting to %s ", WIFI_SSID);
    #endif
    WiFi.begin(WIFI_SSID, WIFI_PASS);

    // Wait
    while (WiFi.status() != WL_CONNECTED) {
      #ifdef DEBUG
        Serial.print(".");
      #endif
        delay(100);
    }
    #ifdef DEBUG
      Serial.println();
      
  
      // Connected!
      Serial.printf("[WIFI] STATION Mode, SSID: %s, IP address: %s\n", WiFi.SSID().c_str(), WiFi.localIP().toString().c_str());
    #endif
}

void setup() {

   
    
    #ifdef DEBUG
    // Init serial port and clean garbage
    Serial.begin(SERIAL_BAUDRATE);
    Serial.println();
    Serial.println();
    #else
    // Use this to turn the TX and RX pins of ESP01 to GPIO
    pinMode(1,FUNCTION_3);
    pinMode(3,FUNCTION_3);

    pinMode(DEVICE_THREE, OUTPUT);
    pinMode(DEVICE_FOUR, OUTPUT);

    delay(100);

    digitalWrite(DEVICE_THREE, HIGH);
    digitalWrite(DEVICE_FOUR, HIGH);
    #endif

    
    pinMode(DEVICE_ONE, OUTPUT);
    pinMode(DEVICE_TWO, OUTPUT);
    
    delay(100);

    digitalWrite(DEVICE_ONE, HIGH);
    digitalWrite(DEVICE_TWO, HIGH);

     // Wifi
    wifiSetup();

    // By default, fauxmoESP creates it's own webserver on the defined port
    // The TCP port must be 80 for gen3 devices (default is 1901)
    // This has to be done before the call to enable()
    fauxmo.createServer(true); // not needed, this is the default value
    fauxmo.setPort(80); // This is required for gen3 devices

    // You have to call enable(true) once you have a WiFi connection
    // You can enable or disable the library at any moment
    // Disabling it will prevent the devices from being discovered and switched
    fauxmo.enable(true);

    // You can use different ways to invoke alexa to modify the devices state:
    // "Alexa, turn fan on"
    // "Alexa, turn on fan"

    // Add virtual devices
    fauxmo.addDevice(ID_DEVICE_ONE);
    fauxmo.addDevice(ID_DEVICE_TWO);
    #ifdef DEBUG

    #else
      fauxmo.addDevice(ID_DEVICE_THREE);
      fauxmo.addDevice(ID_DEVICE_FOUR);
    #endif


    fauxmo.onSetState([](unsigned char device_id, const char * device_name, bool state, unsigned char value) {
        
        // Callback when a command from Alexa is received. 
        // You can use device_id or device_name to choose the element to perform an action onto (relay, LED,...)
        // State is a boolean (ON/OFF) and value a number from 0 to 255 (if you say "set kitchen light to 50%" you will receive a 128 here).
        // Just remember not to delay too much here, this is a callback, exit as soon as possible.
        // If you have to do something more involved here set a flag and process it in your main loop.
        #ifdef DEBUG
        Serial.printf("[MAIN] Device #%d (%s) state: %s value: %d\n", device_id, device_name, state ? "ON" : "OFF", value);
        #endif
        // Checking for device_id is simpler if you are certain about the order they are loaded and it does not change.
        // Otherwise comparing the device_name is safer.
        // Note that the relay we use in this project is ACTIVE LOW so when the state is true or "ON" the pin has to be driven LOW and vice versa

        if (strcmp(device_name, ID_DEVICE_ONE)==0) 
        {
            digitalWrite(DEVICE_ONE, state ? LOW : HIGH);
        } 
        
        else if (strcmp(device_name, ID_DEVICE_TWO)==0) 
        {
            digitalWrite(DEVICE_TWO, state ? LOW : HIGH);
        } 
        
        else if (strcmp(device_name, ID_DEVICE_THREE)==0)
        {
            digitalWrite(DEVICE_THREE, state ? LOW : HIGH);
        } 
//        
        else if (strcmp(device_name, ID_DEVICE_FOUR)==0) 
        {
            digitalWrite(DEVICE_FOUR, state ? LOW : HIGH);
        } 

    });

}

void loop() {

    // fauxmoESP uses an async TCP server but a sync UDP server
    // Therefore, we have to manually poll for UDP packets
    fauxmo.handle();

    // This is a sample code to output free heap every 5 seconds
    // This is a cheap way to detect memory leaks
    #ifdef DEBUG
      static unsigned long last = millis();
      if (millis() - last > 5000) {
          last = millis();
          Serial.printf("[MAIN] Free heap: %d bytes\n", ESP.getFreeHeap());
      }
    #endif

}
