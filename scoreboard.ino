#include <Wire.h>
#include "Adafruit_MCP23017.h"
#define LED 13
#define FLASH_TIME 250 // ms
#define RX_ACTIVE 4
#define RX0 0
#define RX1 1
#define RX2 2
#define RX3 3

Adafruit_MCP23017 mcp;
int rx_state = LOW;
int rx_state_new;
int currentMcpOutput = 0;

const int DIGIT0[] PROGMEM = {0, 1, 2, 3, 4, 6};
const int DIGIT1[] PROGMEM = {5, 2};
const int DIGIT2[] PROGMEM = {6, 5, 3, 1, 0};
const int DIGIT3[] PROGMEM = {6, 5, 3, 2, 0};
const int DIGIT4[] PROGMEM = {5, 4, 3, 2};
const int DIGIT5[] PROGMEM = {6, 4, 3, 2, 0};
const int DIGIT6[] PROGMEM = {6, 4, 3, 2, 1, 0};
const int DIGIT7[] PROGMEM = {6, 5, 2};
const int DIGIT8[] PROGMEM = {6, 5, 4, 3, 2, 1, 0};
const int DIGIT9[] PROGMEM = {6, 5, 4, 3, 2, 0};





void setup() {  
  mcp.begin();      // use default address 0
  for(int i = 0; i < 16; i++) {
    mcp.pinMode(i, OUTPUT);
    mcp.digitalWrite(i, HIGH); // set to open by default
  }
  pinMode(LED, OUTPUT); // on board LED monitors the remote RX
  digitalWrite(LED, LOW);
  for(int i = 0; i < 5; i++) {
    pinMode(i, INPUT); // RC RX pins
  }
}

void flashLED() {
  digitalWrite(LED, HIGH);
  delay(FLASH_TIME);
  digitalWrite(LED, LOW);
}

void loop() {
    rx_state_new = digitalRead(RX_ACTIVE);
    if(rx_state != rx_state_new) {
      rx_state = rx_state_new;
      flashLED();
      if(rx_state) {
        mcp.digitalWrite(currentMcpOutput, LOW);
        mcp.digitalWrite((currentMcpOutput - 1) % 16, HIGH);
      } else {
        mcp.digitalWrite(currentMcpOutput, HIGH);
        mcp.digitalWrite((currentMcpOutput + 1) % 16, LOW);
        currentMcpOutput = (currentMcpOutput+ 1) % 16;
      }
    }
}
