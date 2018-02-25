#include <avr/io.h>
#include <avr/interrupt.h>


//These are for Timer1
#define PRESCALE0_1 _BV(CS10)
#define PRESCALE0_8 _BV(CS11)
#define PRESCALE0_64 (_BV(CS10) | _BV(CS11))
#define PRESCALE0_256 _BV(CS12)
#define PRESCALE0_1024 (_BV(CS12) | _BV(CS10))


// Define data/clock/latch pins
const int pinData  = 6; // PA6
const int pinClock = 7; // PA7
const int pinLatch = 8; // PB2

int counter = 0;
volatile byte data[3];
volatile int row_num = 0;

volatile unsigned long cycle = 0;


// the setup routine runs once when you press reset:
void setup() {
  // Set up timers
  TCCR1A = 0;
  TCCR1B = 0;
  
  TCCR1B |= (1 << WGM12);  // configure timer1 for CTC mode
  TIMSK1 |= (1 << OCIE1A); // enable the CTC interrupt
  
  // sei();                // enable global interrupts
  OCR1A = 833;             // Should return frequency ~1000hz
  TCCR1B |= PRESCALE0_1;

  
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
  int i;
  
  cycle_RGB();
  animate_left_right();
  //all_on();

  /*
  for (i = 0; i < 3; i++) {
    cycle_RGB();
  }
  
  all_on();
  
  for (i = 0; i < 1; i++) {
    animate_left_right();
  }
  */
}

void animate_left_right() {
  for (counter = 0; counter < 24; counter++) {
    if (counter < 8) {
      data[0] =   1 << counter;
      data[1] =   0;
      data[2] =   0;
    } else if (counter < 16) {
      data[0] =   0;
      data[1] =   1 << (counter-8);
      data[2] =   0;
    } else {
      data[0] =   0;
      data[1] =   0;
      data[2] =   1 << (counter-16);
    }
    delayCycle(1000);
  }
}

void cycle_RGB() {
  red();
  green();
  blue();
}

void red() {
  data[0] = 255; data[1] = 0; data[2] = 0;
  delayCycle(1000);
}
void green() {
  data[0] = 0; data[1] = 255; data[2] = 0;
  delayCycle(1000);
}
void blue() {
  data[0] = 0; data[1] = 0; data[2] = 255;
  delayCycle(1000);
}

void all_on() {
  data[0] = 255; data[1] = 255; data[2] = 255;
  delayCycle(1000);
}

void delayCycle(long waitFor) {
  unsigned long last = cycle;
  unsigned long waitUntil = last + waitFor;

  while (cycle < waitUntil) yield();
}


ISR(TIM1_COMPA_vect) { 
  if (row_num >= 4) row_num = 0;
  
  // Enable row
  shiftOut(pinData, pinClock, MSBFIRST, (1 << row_num));
  
  // Shift out 1 row
  shiftOut(pinData, pinClock, MSBFIRST, data[2]);
  shiftOut(pinData, pinClock, MSBFIRST, data[1]);
  shiftOut(pinData, pinClock, MSBFIRST, data[0]);

  // Latch on the outputs and then off
  digitalWrite(pinLatch, HIGH);
  digitalWrite(pinLatch, LOW);

  row_num++;
  cycle++;
}
