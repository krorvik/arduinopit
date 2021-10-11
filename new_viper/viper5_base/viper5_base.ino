#include <ResponsiveAnalogRead.h>
#include <Encoder.h>

// USER CONFIG

//Adjustable compiletime
const int encoder_count = 0;  // Number of encoders. Will take two digital inputpins per encoder, 0+1, 2+3 and so on. (As well as digital ground of course). 

// L_Aft
/*
const int multipins[] = {0,1,2,3,15,16,17,18,19,20,34,35,36,37,38,47,48,49,50,51,52,53};   // Rotary switches with no unconnected positions
const int upper_spdt[] = {25,46,44,42,13,21};
const int lower_spdt[] = {24,39,41,43,14,22};
*/

// L_Fwd
/*
const int multipins[] = {0,1,2,3};   // Rotary switches with no unconnected positions
const int upper_spdt[] = {30,38,36,34,11,26,24,9};
const int lower_spdt[] = {29,37,35,33,12,27,25,10};
*/

// L_Aux
/*
const int multipins[] = {17, 18, 19, 20, 24, 25, 26, 27, 29, 30, 31};   // Rotary switches with no unconnected positions
const int upper_spdt[] = {4, 10};
const int lower_spdt[] = {3, 9};
*/

// R_Aft
/*
const int multipins[] = {0,1,2,3,4,13,14};   // Rotary switches with no unconnected positions
const int upper_spdt[] = {9, 10, 27, 28, 29,12};
const int lower_spdt[] = {8,11,26,25,24,7};
*/

// R_Fwd
/*
const int multipins[] = {8,9,10,11};   // Rotary switches with no unconnected positions
const int upper_spdt[] = {28,2,7,17,14,0,5,16,12};
const int lower_spdt[] = {27,3,18,19,15,1,6,4,13};
*/
// Centr

const int multipins[] = {};   // Rotary switches with no unconnected positions
const int upper_spdt[] = {};
const int lower_spdt[] = {};


const int digital_input_count = 58;  // Total amount of digital pins available for board


// Analog pots. Remove any unconnected, they generate noise. Observe pins for your PCB layout or chosen hookup.
//const int pot_pins[] = {A10, A11, A21, A22, A25, A26};    // Teensy 3.5
const int pot_pins[] = {A10, A11, A12, A13, A14};         // Teensy 3.2

// END USER CONFIG

// Calculated, derived or not expected to change
Encoder* encoders[encoder_count];
long encoderpositions[encoder_count];
const int pot_count = sizeof(pot_pins) / sizeof(pot_pins[0]);
const int multi_count = sizeof(multipins) / sizeof(multipins[0]);
const int spdt_count = min(sizeof(upper_spdt) / sizeof(upper_spdt[0]), sizeof(lower_spdt) / sizeof(lower_spdt[0]));
ResponsiveAnalogRead* pots[pot_count];

//Wrapper pot functions so we don't try to make pointers to class functions...
void pot1 (const int value) {Joystick.X(value * 64);}
void pot2 (const int value) {Joystick.Y(value * 64);}
void pot3 (const int value) {Joystick.Z(value * 64);}
void pot4 (const int value) {Joystick.Xrotate(value * 64);}
void pot5 (const int value) {Joystick.Yrotate(value * 64);}
void pot6 (const int value) {Joystick.Zrotate(value * 64);}

// Function prototype tells the compiler what to place in array below
typedef void (PotFunc) (int);
PotFunc* potfunctions[] = {pot1, pot2, pot3, pot4, pot5, pot6};

void setup() {
  // Initialize encoders
  for (int i = 0; i < encoder_count; i++) {
    int pin1 = 2*i;
    int pin2 = 2*i +1;
    encoders[i] = new Encoder(pin1, pin2);
    encoderpositions[i] = 0;
  }
  // Set regular digital inputs
  for (int i = 2 * encoder_count; i < digital_input_count; i++) {
    pinMode(i, INPUT_PULLUP);    
  }
  // Set up pots
  for (int i = 0; i < pot_count; i++) {
    pots[i] = new ResponsiveAnalogRead(pot_pins[i], true);
  }
}

void doEncoders() {
  // Just loop
  for (int i = 0; i < encoder_count; i++) {
    long newposition = encoders[i]->read();
    int diff = newposition - encoderpositions[i];        
    // Buttons == pin ids
    int pin1 = 2*i;
    int pin2 = 2*i +1;   
    Joystick.button(pin1 +1, 0);    
    Joystick.button(pin2 +1, 0);
    
    // If neg, raise buttton = pin1
    if (diff < 0) {
      Joystick.button(pin1 +1, 1);
    // If positive, the other one      
    } else if (diff > 0) {
      Joystick.button(pin2 +1, 1);
    }

    // Then remember position so we're ready for next loop
    encoderpositions[i] = newposition;
  }
};

bool multi(int pin) {  
  for (int i = 0; i < multi_count; i++) {
    if (multipins[i] == pin) {
      return true;
    }
  }
  return false;
}

bool SPDT(int pin) {
  boolean found_spdt = false;
  for (int i = 0; i < spdt_count; i++) {
    if (pin == upper_spdt[i] or pin == lower_spdt[i]) {
      found_spdt = true;
      if (digitalRead(upper_spdt[i]) == 1 && digitalRead(lower_spdt[i]) == 1) {
        Joystick.button(upper_spdt[i]+ 1 + digital_input_count, 1);
      } else {
        Joystick.button(upper_spdt[i]+ 1 + digital_input_count, 0);
      }
    }
  }
  return found_spdt;
}

bool regular(int pin) {
  Joystick.button(pin+1+digital_input_count, digitalRead(pin));
  return true;
}

void loop() {
  
  doEncoders();
  
  // Digital inputs
  for (int i = 2* encoder_count; i < digital_input_count; i++) {
    Joystick.button(i+1, 1 - digitalRead(i));
    multi(i) || SPDT(i) || regular(i);
  }

  // Analog inputs
  for (int i = 0; i < pot_count; i++) {
    pots[i]->update();
    float value = pots[i]->getValue();
    potfunctions[i](value);
  }
}
