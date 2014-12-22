#include "EEGheadset.h"

const uint8_t PIN_SWITCH = 3;

const uint8_t PIN_LED_RED = 9;
const uint8_t PIN_LED_GREEN = 5;
const uint8_t PIN_LED_BLUE = 6;

const uint8_t DELTA = 0;
const uint8_t THETA = 1;
const uint8_t LOW_ALPHA = 2;
const uint8_t HIGH_ALPHA = 3;
const uint8_t LOW_BETA = 4;
const uint8_t HIGH_BETA = 5;
const uint8_t LOW_GAMMA = 6;
const uint8_t MID_GAMMA = 7;

//value that is used right now
const uint8_t USED_VALUE = LOW_ALPHA;

//less than this value is red light
const uint8_t RED_VALUE = 50;

//between RED_VALUE and this is green light, more - blue light
const uint8_t GREEN_VALUE = 80;

//value on which switch is triggered
const uint8_t SWITCH_TRIGGER = 80;

const uint8_t POOR_QUALITY = 200;

//
//uint8_t meterPqAttMedLastValue[3] = {
//  0};
//uint8_t meterPqAttMedCurrentValue[3] = {
//  0};

uint8_t previousValue = 0;

EEGheadset eeg_headset(Serial1);

void react(uint8_t value) {
//  uint8_t attention = meterPqAttMedLastValue[1];

  Serial.println(String("value: ") + value);

  if(value < RED_VALUE) {
    ledColor(0xFF, 0, 0);
  } 
  else if(value < GREEN_VALUE) {
    ledColor(0, 0xFF, 0);
  } 
  else {
    ledColor(0, 0, 0xFF);
  }

  if(value >= SWITCH_TRIGGER) {
    switchOn();
  } 
  else {
    switchOff();
  }
}

void switchOn() {
  Serial.println("=====SWITCHED ON=====");
  analogWrite(PIN_SWITCH, 0xFF);
}

void switchOff() {
  Serial.println("=====SWITCHED OFF=====");
  analogWrite(PIN_SWITCH, 0);
}

void ledColor(byte R, byte G, byte B) {
  analogWrite(PIN_LED_RED, R);
  analogWrite(PIN_LED_GREEN, G);
  analogWrite(PIN_LED_BLUE, B);
}

void setup() {
  eeg_headset.begin(115200);
  eeg_headset.useAutoConnect = true;

  Serial.begin(115200);
  pinMode(PIN_SWITCH, OUTPUT);
  pinMode(PIN_LED_RED, OUTPUT);
  pinMode(PIN_LED_GREEN, OUTPUT);
  pinMode(PIN_LED_BLUE, OUTPUT);

  ledColor(0, 0, 0);

  switchOff();
}

void loop() {
  eeg_headset.getData();
  uint8_t value = eeg_headset.getEEGPower()[USED_VALUE];
  uint8_t quality = eeg_headset.getPoorQuality();
  //  Serial.println(String("Attention: ") + attention + String(", meditation: ") 
//  + meditation + String(", pq: ") + poorQuality);

//  meterPqAttMedLastValue[0] = poorQuality;
  if(quality < POOR_QUALITY) {
    if(value != previousValue) {
      previousValue = value;
      react(value);
    } 
//    meterPqAttMedCurrentValue[1] = attention;
//    if(meterPqAttMedCurrentValue[1]!=meterPqAttMedLastValue[1]) {
//      meterPqAttMedLastValue[1] = meterPqAttMedCurrentValue[1];
//      traffic();
//    } 
//    else {
//      meterPqAttMedCurrentValue[1] = 0;	
//    }
//
//    meterPqAttMedCurrentValue[2] = meditation;
//    if(meterPqAttMedCurrentValue[2]!=meterPqAttMedLastValue[2]) {
//      meterPqAttMedLastValue[2] = meterPqAttMedCurrentValue[2];
//    } 
//    else {
//      meterPqAttMedCurrentValue[2] = 0;
//    }

    //
  } else {
    //problems with connection
    ledColor(0, 0, 0);
    switchOff();
  }

}



