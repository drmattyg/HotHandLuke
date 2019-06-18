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

const int RELAYS[]  = {100, 8, 7, 14, 1, 13, 2, 12, 3, 11, 4, 10, 5, 9, 6, 15, 0};

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
  Serial.begin(9600);
}
void testRelays() {
	for(int i = 1; i <= 16; i++) {
		int last_i = i - 1;
		if(i == 1) {
			last_i = 16;
		}
		mcp.digitalWrite(RELAYS[last_i], HIGH);
		mcp.digitalWrite(RELAYS[i], LOW);
		Serial.print("i ");
		Serial.print(RELAYS[i]);
		Serial.print(", last i ");
		Serial.print(RELAYS[last_i]);
		Serial.print("\n");
		delay(1000);
	}
}

void testARelay(int i) {
	while(true) {
		mcp.digitalWrite(i, LOW);
		delay(1000);
		mcp.digitalWrite(i, HIGH);
		delay(1000);
	}
}

void loop() {
	testRelays();
	//testARelay(RELAYS[11]);
}