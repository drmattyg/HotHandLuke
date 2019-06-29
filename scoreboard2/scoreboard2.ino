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

const int RELAYS[] = {100, 8, 7, 14, 1, 13, 2, 12, 3, 11, 4, 10, 5, 9, 6, 15, 0};

const char DIGIT0[] = {0, 1, 2, 3, 4, 6};
const char DIGIT1[] = {5, 2};
const char DIGIT2[] = {6, 5, 3, 1, 0};
const char DIGIT3[] = {6, 5, 3, 2, 0};
const char DIGIT4[] = {5, 4, 3, 2};
const char DIGIT5[] = {6, 4, 3, 2, 0};
const char DIGIT6[] = {6, 4, 3, 2, 1, 0};
const char DIGIT7[] = {6, 5, 2};
const char DIGIT8[] = {6, 5, 4, 3, 2, 1, 0};
const char DIGIT9[] = {6, 5, 4, 3, 2, 0};
const char* DIGITS[] = {DIGIT0, DIGIT1, DIGIT2, DIGIT3, DIGIT4, DIGIT5, DIGIT6, DIGIT7, DIGIT8, DIGIT9};
const char* DIGIT_LEN[] = {6, 2, 5, 5, 4, 5, 6, 3, 7, 6};
int currentDigitVal[] = {-1, -1};


void sevenSeg(int digitNum, int digitVal, int state) {
  char buff[50];
  sprintf(buff, "digitNum %d, digitVal %d, state %d", digitNum, digitVal, state);
  Serial.println(buff);
  Serial.println(sizeof(DIGITS[digitVal]));
  char* digit_to_write = DIGITS[digitVal];
  for(int i = 0; i < DIGIT_LEN[digitVal]; i++) {
    int relayNum = digit_to_write[i] + 1 + (digitNum * 7);
    sprintf(buff, "digitVal %d, relayNum %d", digitVal, relayNum);
    Serial.println(buff);
    mcp.digitalWrite(RELAYS[relayNum], state);
  }
}

void clearAll() {
  for(int i = 1; i < sizeof(RELAYS); i++) {
    mcp.digitalWrite(RELAYS[i], i);
  }
}

void setDigit(int digitNum, int digitVal) {

  // turn off previous digit
  clearAll();
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
  for(int i = 8; i <= 12; i++) {
    pinMode(i, INPUT); // RC RX pins
  }
  Serial.begin(9600);
  clearAll();
  setDigit(1, 0);
  setDigit(0, 0);
}

void flashLED(int n) {
  for(int i = 0; i < n; i++) {
    digitalWrite(LED, HIGH);
    delay(FLASH_TIME);
    digitalWrite(LED, LOW);
    delay(FLASH_TIME);
  }
}
int i = 1;
void loop() {
  // read input as int to debounce
  int buttA = digitalRead(RX0);
  int buttB = digitalRead(RX1);
  int buttC = digitalRead(RX2);
  int buttD = digitalRead(RX3);

  //increment by 1
  if(buttD == 1){
    flashLED(1);
    incrementSevenSeg();
    delay(500); //for debounce
    //Serial.println(currentDigitVal[0]);
    //Serial.println(currentDigitVal[1]);
  }
  //increment by 3
  if(buttC == 1){
    flashLED(1);
    incrementSevenSeg();
    incrementSevenSeg();
    incrementSevenSeg();
    delay(500); //for debounce
    //Serial.println(currentDigitVal[0]);
    //Serial.println(currentDigitVal[1]);
  }
  //increment by 5
  if(buttB == 1){
    flashLED(1);
    incrementSevenSeg();
    incrementSevenSeg();
    incrementSevenSeg();
    incrementSevenSeg();
    incrementSevenSeg();
    delay(500); //for debounce
    //Serial.println(currentDigitVal[0]);
    //Serial.println(currentDigitVal[1]);
      
  }
  //reset
  if(buttA == 1){
    flashLED(2);
    setDigit(0,0);
    setDigit(1,0);
    delay(500); //for debounce
    //Serial.println(currentDigitVal[0]);
    //Serial.println(currentDigitVal[1]);
  }
 
} //end loop
