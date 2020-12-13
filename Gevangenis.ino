#include <LedControl.h>
LedControl lc = LedControl(12,11,10,2); 

int cel;
int etage;
int onoff;
long randNumber;

const int ledTower1Pin =  3;
const int ledTower2Pin =  5;
const int ledGatePin =    6;
const int switchGatePin = 7;
const int touch1Pin = 8;

// Variables will change:
int ledState = LOW;             // ledState used to set the LED
int alarmState = LOW; 

int readingInput;

// Generally, you should use "unsigned long" for variables that hold time
// The value will quickly become too large for an int to store
unsigned long previousMillisBlinkTower = 0;        // will store last time LED was updated
unsigned long previousMillisBlinkGate = 0;        // will store last time LED was updated
unsigned long previousMillisCelLight = 0;        // will store last time LED was updated

bool fadeCountUp = false;
bool fadeCountUpGate = false;

unsigned char fadeLedValue = 0;
unsigned char fadeLedValueGate = 0;
unsigned char fadeStep = 8;


// constants won't change:
const long intervalBlinkTower = 8;           // interval at which to blink (milliseconds)
const long intervalBlinkGate = 6;           // interval at which to blink (milliseconds)
const long intervalCelLight = 10000;           // interval at which to blink (milliseconds)

unsigned long alarmCounter = 0;

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(ledTower1Pin, OUTPUT);
  pinMode(ledTower2Pin, OUTPUT);
  pinMode(ledGatePin, OUTPUT);
  pinMode(switchGatePin, INPUT);
  pinMode(touch1Pin, INPUT);

  lc.shutdown(0,false);
  lc.shutdown(1,false);

  // set the brightness to a medium values
  lc.setIntensity(0,8);                       // 0 = low; 15 = high
  lc.setIntensity(1,8);                       // 0 = low; 15 = high

  // check whether the matrix works 
  testloop();

  // clear the display
  lc.clearDisplay(0);    
  lc.clearDisplay(1);   

  // initial position
  randomSeed(analogRead(0)); 
}

// the loop function runs over and over again forever
void loop() {
  unsigned long currentMillis = millis();

  readingInput = digitalRead(touch1Pin);
  if (readingInput == HIGH) {
      alarmState = HIGH;
      alarmCounter = 10000; // reset when touched
  }

  if (alarmCounter > 0 && alarmState == HIGH) {
    if (currentMillis - previousMillisBlinkTower >= intervalBlinkTower) {
      // save the last time you blinked the LED
      previousMillisBlinkTower = currentMillis;
  
      if (fadeCountUp == false) {
        if ((fadeLedValue - fadeStep) < 0) {
          fadeCountUp = true;
          fadeLedValue = 0;
  
          if (ledState == LOW) {
            ledState = HIGH;
          } else {
            ledState = LOW;
          }
          
        } else {
          fadeLedValue -= fadeStep;
        }
      } else if (fadeCountUp == true) {
        if ((fadeLedValue + fadeStep) > 255) {
          fadeCountUp = false;
          fadeLedValue = 255;              
        } else {
          fadeLedValue += fadeStep;
        }
      }
  
      // if the LED is off turn it on and vice-versa:
      if (ledState == LOW) {
        analogWrite(ledTower2Pin, 0);
        analogWrite(ledTower1Pin, fadeLedValue);
      } else {     
        analogWrite(ledTower2Pin, fadeLedValue);
        analogWrite(ledTower1Pin, 0);
      }

    
      alarmCounter--;
  
      if (alarmCounter == 0) {
        alarmState = LOW;
        analogWrite(ledTower1Pin, 0);
        analogWrite(ledTower2Pin, 0);
      }
    }
  }

  readingInput = digitalRead(switchGatePin);
  if ((fadeLedValueGate > 0 || readingInput == HIGH ) && currentMillis - previousMillisBlinkGate >= intervalBlinkGate) {
    // save the last time you blinked the LED
    previousMillisBlinkGate = currentMillis;

    if (fadeCountUpGate == false) {
      if ((fadeLedValueGate - fadeStep) < 0) {
        fadeCountUpGate = true;
        fadeLedValueGate = 0;        
      } else {
        fadeLedValueGate -= fadeStep;
      }
    } else if (fadeCountUpGate == true) {
      if ((fadeLedValueGate + fadeStep) > 255) {
        fadeCountUpGate = false;
        fadeLedValueGate = 255;              
      } else {
        fadeLedValueGate += fadeStep;
      }
    }

    analogWrite(ledGatePin, fadeLedValueGate);
  }

  if (currentMillis - previousMillisCelLight >= intervalCelLight) {  
   previousMillisCelLight = currentMillis;
   etage = random(1, 5);                       // picks house between 0 and 6 
   cel = random(1, 23);                       // picks row (= light) between 0 and 7 
   int onoff_rnd = random(0, 2);                       // picks on/off between 0 and 2    
   int cnt = random(0, 10);                       // picks random count of lights between 0 and 10 

    for (int i=0;i<cnt;i++) {      
      onoff = (onoff_rnd > 0 ? true : false);
      setlight(etage, cel, onoff);
    } 
  }
}

// ============ SUBROUTINES ==============================================

void testloop() {
  // clear the display
  lc.clearDisplay(0);    
  lc.clearDisplay(1);    
  
  for (int etage = 1; etage <= 4;etage++) { 
    for (int cel = 1; cel <= 22;cel++) { 
          // Turn on and off every single light for testing
          setlight(etage, cel, true);            // light on
          delay(100);
          setlight(etage, cel, false);            // light off
    }
  }
}

void setlight(int etage, int cel, bool onoff) {
  int disp = 0;
  int row = etage;
  int col = cel;

  if (cel>16) {
    disp = 1;
    col = cel-16;
  } else if (cel > 8) {
    col = cel-8;
    row = row+4;
  }
  // else etage blijft etage, cel blijft cel
    
  lc.setLed(disp, (col-1), (row-1), onoff);            // device,  row, col, light on
}
