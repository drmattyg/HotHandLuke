#include <Wire.h>
#include "Adafruit_MCP23017.h"
#define LED 13
#define FLASH_TIME 50 // ms
#define RX_ACTIVE 12
#define RX0 8
#define RX1 9
#define RX2 10
#define RX3 11




Adafruit_MCP23017 mcp;
int rx_state = LOW;
int rx_state_new;
int rx_pin_prev[] = {LOW, LOW, LOW, LOW};
int rx_pin_current[] = {LOW, LOW, LOW, LOW};
const int RX_PINS[] = {RX0, RX1, RX2, RX3};
int currentMcpOutput = 0;

// relays are numbered starting from 1, so RELAYS[0] is nothing
// RELAYS[1] is the mcp pin for relay 1

const int RELAYS[] PROGMEM = {100, 8, 7, 14, 1, 13, 2, 12, 3, 11, 4, 10, 5, 9, 6, 15, 0};

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
const char* DIGITS[] = {DIGIT0, DIGIT1, DIGIT2, DIGIT3, DIGIT4, DIGIT5, DIGIT6, DIGIT7, DIGIT8, DIGIT9};

int currentDigitVal[] = {-1, -1};


void sevenSeg(int digitNum, int digitVal, int state) {
  for(int i = 0; i < sizeof(DIGITS[digitVal]); i++) {
    int relayNum = DIGITS[digitVal] + 1 + (digitNum * 10);
    mcp.digitalWrite(RELAYS[relayNum], state);
  }
}

void setDigit(int digitNum, int digitVal) {
  sevenSeg(digitNum, digitVal, LOW);

  // turn off previous digit
  if(currentDigitVal[digitNum] != -1) {
    sevenSeg(digitNum, currentDigitVal[digitNum], HIGH);
  }
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
  for(int i = 8; i <= 12; i++) {
    pinMode(i, INPUT); // RC RX pins
  }
  Serial.begin(115200);
}

void flashLED(int n) {
  for(int i = 0; i < n; i++) {
    digitalWrite(LED, HIGH);
    delay(FLASH_TIME);
    digitalWrite(LED, LOW);
    delay(FLASH_TIME);
  }
}

void loop() {

  int buttA = digitalRead(RX0);
  int buttB = digitalRead(RX1);
  int buttC = digitalRead(RX2);
  int buttD = digitalRead(RX3);

  //increment by 1
  if(buttA == 1){
    flashLED(1);
    incrementSevenSeg();
    delay(500);
    Serial.println(currentDigitVal[0]);
    Serial.println(currentDigitVal[1]);
  }
  //increment by 3
  if(buttB == 1){
    flashLED(1);
    incrementSevenSeg();
    incrementSevenSeg();
    incrementSevenSeg();
    delay(500);
    Serial.println(currentDigitVal[0]);
    Serial.println(currentDigitVal[1]);
  }
  //increment by 5
  if(buttC == 1){
    flashLED(1);
    incrementSevenSeg();
    incrementSevenSeg();
    incrementSevenSeg();
    incrementSevenSeg();
    incrementSevenSeg();
    delay(500);
    Serial.println(currentDigitVal[0]);
    Serial.println(currentDigitVal[1]);
      
  }
  //reset
  if(buttD == 1){
    flashLED(2);
    setDigit(0,0);
    setDigit(1,0);
    delay(500);
    Serial.println(currentDigitVal[0]);
    Serial.println(currentDigitVal[1]);
  }
 /* 
  bool rx_changed[] = {false, false, false, false};
  rx_state = digitalRead(RX_ACTIVE);
  for(int i = 0; i < 4; i++) {
      rx_pin_current[i] = digitalRead(RX_PINS[i]);
      if(rx_pin_current[i] != rx_pin_prev[i]) {
        rx_changed[i] = true;
        rx_pin_prev[i] = rx_pin_current[i];
        flashLED(i);
      }
  }
  if(rx_changed[RX0] && rx_pin_current[RX0]) {
    // RX0 = increment
    incrementSevenSeg();
    Serial.println(currentDigitVal[0]);
    Serial.println(currentDigitVal[1]);
  } else if(rx_changed[RX1] && rx_pin_current[RX1]) {
    // RX1 = set to zero
    setDigit(0, 0);
    setDigit(1, 0);
  }

  if(rx_state != rx_state_new && rx_state) {
    rx_state = rx_state_new;
    //flashLED();
  }
*/ 

} //end loop
