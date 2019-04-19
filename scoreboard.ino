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

// relays are numbered starting from 1, so RELAYS[0] is nothing
// RELAYS[1] is the mcp pin for relay 1

const int RELAYS[] PROGMEM = [100, 8, 7, 14, 1, 13, 2, 12, 3, 11, 4, 10, 5, 9, 6, 15, 0]

const char DIGIT0[] PROGMEM = {0, 1, 2, 3, 4, 6};
const char DIGIT1[] PROGMEM = {5, 2};
const char DIGIT2[] PROGMEM = {6, 5, 3, 1, 0};
const char DIGIT3[] PROGMEM = {6, 5, 3, 2, 0};
const char DIGIT4[] PROGMEM = {5, 4, 3, 2};
const char DIGIT5[] PROGMEM = {6, 4, 3, 2, 0};
const char DIGIT6[] PROGMEM = {6, 4, 3, 2, 1, 0};
const char DIGIT7[] PROGMEM = {6, 5, 2};
const char DIGIT8[] PROGMEM = {6, 5, 4, 3, 2, 1, 0};
const char DIGIT9[] PROGMEM = {6, 5, 4, 3, 2, 0};
const char* DIGITS[] PROGMEM = {DIGIT0, DIGIT1, DIGIT2, DIGIT3, DIGIT4, DIGIT5, DIGIT6, DIGIT7, DIGIT8, DIGIT9};

int currentDigitVal0[] = {-1, -1};


void sevenSeg(int digitNum, int digitVal, int state) {
  for(int i = 0; i < sizeof(DIGITS[digitVal])) {
    int relayNum = DIGITS[digitVal] + 1 + (digitNum * 10);
    mcp.digitalWrite(RELAYS[relayNum], state);
  }
}

void setDigit(int digitNum, int digitVal) {
  // turn off previous digit
  if(currentDigitVal[digitNum] != -1) {
    sevenSeg(currentDigitVal[digitNum], HIGH);
  }
  sevenSeg(digitNum, digitVal, LOW);
  currentDigitVal[digitNum] = digitVal;
}

void incrementSevenSeg() {
  // low order digit is 1, on the right
  if(currentDigitVal[1] == 9) {
    currentDigitVal[1] = 0;
    currentDigitVal[0]++;
    setDigit(1, 0);
    setDigit(0, currentDigitVal[0]);
  } else {
    currentDigitVal[1]++;
    setDigit(1, currentDigitVal[1]);
  }
}


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
