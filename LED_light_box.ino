/*
  Blink (modified slightly for ATTiny84/44 ICs
  Turns on an LED on for one second, then off for one second, repeatedly.

  This example code is in the public domain.
 */

// Define data/clock/latch pins
const int pinData  = 6;
const int pinClock = 7;
const int pinLatch = 8;

byte data[3];
int counter = 0;

// the setup routine runs once when you press reset:
void setup() {
  // initialize the digital pin as an output.
  pinMode(pinData,  OUTPUT);
  pinMode(pinClock, OUTPUT);
  pinMode(pinLatch, OUTPUT);

  // Hold latch low
  digitalWrite(pinLatch, LOW);

  counter = 0;
}

// the loop routine runs over and over again forever:
void loop() {
  if (counter >= 24) counter = 0;

  if (counter < 8) {
    data[0] = 1 << counter;
    data[1] = 0;
    data[2] = 0;
  } else if (counter < 16) {
    data[0] = 0;
    data[1] = 1 << (counter-8);
    data[2] = 0;
  } else {
    data[0] = 0;
    data[1] = 0;
    data[2] = 1 << (counter-16);
  }

  counter++;

  // For each frame
  for (int i=0; i<25; i++) {
    // For each row
    for (int j=0; j<4; j++) {
      // Shift out 1 row
      shiftOut(pinData, pinClock, LSBFIRST, data[0]);
      shiftOut(pinData, pinClock, LSBFIRST, data[1]);
      shiftOut(pinData, pinClock, LSBFIRST, data[2]);
      
      //shiftOut(pinData, pinClock, LSBFIRST, (byte) (test<<i));
  
      // Enable row
      shiftOut(pinData, pinClock, LSBFIRST, (1<<j));
  
      // Latch on the outputs and then off
      digitalWrite(pinLatch, HIGH);
      delay(1);
      digitalWrite(pinLatch, LOW);
    }
  }
  
}
