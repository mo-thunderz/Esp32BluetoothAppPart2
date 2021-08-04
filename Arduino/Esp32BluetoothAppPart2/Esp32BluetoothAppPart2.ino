// --------------------------------------------------
//
// ESP32 Bluetooth App part 2 -> bi-directional cummunication
//
// Code for bi-directional Bluetooth communication demonstration between the ESP32 and mobile phone (with MIT inventor app). 
// device used for tests: ESP32-WROOM-32D
// 
// App on phone has three buttons. Each button switches on a port on the ESP32. 
// After 1 second the ESP32 switches off the buttons on the phone. 
// Communication between phone and ESP32 is as follows:
// Button 1: 11 for ON and 10 for OFF
// Button 2: 21 for ON and 20 for OFF
// Button 3: 31 for ON and 30 for OFF
//
// Written by mo thunderz (last update: 4.8.2021)
//
// --------------------------------------------------

// this header is needed for Bluetooth Serial -> works ONLY on ESP32
#include "BluetoothSerial.h" 

// definition of the delaytime between button on and button off
#define DELAY 1000                      // delay is in milliseconds, so 1000ms corresponds to 1s

// init Class:
BluetoothSerial ESP_BT; 

// init PINs: assign any pin on ESP32
int led_pin_1 = 4;
int led_pin_2 = 0;
int led_pin_3 = 2;                      // On some ESP32 pin 2 is an internal LED, mine did not have one

// Parameters for Bluetooth interface and timing
int incoming;                           // variable to store byte received from phone 
unsigned long now;                      // variable to store current "time" using millis() function
unsigned long time_button1;             // timestamp for button1 to 3 -> stores time when button is enabled
unsigned long time_button2;
unsigned long time_button3;

void setup() {
  Serial.begin(19200);
  ESP_BT.begin("ESP32_Control");        // Name of your Bluetooth interface -> will show up on your phone

  pinMode (led_pin_1, OUTPUT);          // Define output ports to connect LEDs
  pinMode (led_pin_2, OUTPUT);
  pinMode (led_pin_3, OUTPUT);
}

void loop() {
  now = millis();                       // Store current time

  // time-out check -> check if a button is on and whether it needs to be switched off
  if(digitalRead(led_pin_1) and now > time_button1 + DELAY) {     // if output port (led_pin_1) is active and if the delay time has passed, the button is to be switched off
    digitalWrite(led_pin_1, 0);                                   // set output port to 0
    ESP_BT.write(10);                                             // send byte to phone indicateding that Button 1 is to be set to 0 -> 10
    Serial.print("Button 1 timeout - value: 0");                  // write to serial port for easy debugging
  }
  if(digitalRead(led_pin_2) and now > time_button2 + DELAY) {
    digitalWrite(led_pin_2, 0);
    ESP_BT.write(20);                                             // send byte to phone indicateding that Button 2 is to be set to 0 -> 20
    Serial.print("Button 2 timeout - value: 0");
  }
  if(digitalRead(led_pin_3) and now > time_button3 + DELAY) {
    digitalWrite(led_pin_3, 0);
    ESP_BT.write(30);                                             // send byte to phone indicateding that Button 3 is to be set to 0 -> 30
    Serial.print("Button 3 timeout - value: 0");
  }
  
  // -------------------- Receive Bluetooth signal ----------------------
  if (ESP_BT.available()) 
  {
    incoming = ESP_BT.read(); //Read what we receive and store in "incoming"

    // separate button ID from button value -> button ID is 10, 20, 30, etc, value is 1 or 0
    int button = floor(incoming / 10);
    int value = incoming % 10;
    
    switch (button) {
      case 1:  
        Serial.print("Button 1:"); Serial.println(value);
        digitalWrite(led_pin_1, value);
        if(value == 1)                                            // check if the button is switched on
          time_button1 = now;                                     // if button is switched on, write the current time to the timestamp
        break;
      case 2:  
        Serial.print("Button 2:"); Serial.println(value);
        digitalWrite(led_pin_2, value);
        if(value == 1)
          time_button2 = now;
        break;
        case 3:  
        Serial.print("Button 3:"); Serial.println(value);
        digitalWrite(led_pin_3, value);
        if(value == 1)
          time_button3 = now;
        break;
        break;
    }
  }
}
