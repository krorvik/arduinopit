
#include <ResponsiveAnalogRead.h>

ResponsiveAnalogRead pot1(A10, true);
ResponsiveAnalogRead pot2(A11, true);
ResponsiveAnalogRead pot3(A12, true);
ResponsiveAnalogRead pot4(A13, true);
ResponsiveAnalogRead pot5(A14, true);

// Assume throttle is in idle
int cur_pot_pos = 0;

void setup() {
  
  for (int i = 0; i < 34; i++) {
    pinMode(i, INPUT_PULLUP);
  }

  pot3.setActivityThreshold(10);
  pot4.setActivityThreshold(10);
  pot5.setActivityThreshold(10);
}

// Main loop
void loop() {

  for (int i = 0; i < 34; i++) {
    Joystick.button(i+1, 1 - digitalRead(i));
    Joystick.button(i+1+34, digitalRead(i));
  }

  pot3.update();
  pot4.update();
  pot5.update();

  // Analog/slider inputs. Does not use buttons.
  // Joystick.X(pot1.getValue() * 64);
  // Joystick.Y(pot2.getValue() * 64);
  Joystick.X(pot3.getValue() * 64);
  Joystick.Y(pot4.getValue() * 64);

  // Throttle
  // Raw value
  int t_raw = pot5.getValue();   
  // pot moves from 451 to 645 on a 1024 scale, so 194 is travel - scale up by 1024/194, considering offset
  int translated = (t_raw  - 451) * 5.28;
  // Then we scale actual position from 451 to 1024.
  if (t_raw < 433) {
    // Always set throttle to zero when cutoff    
    if (cur_pot_pos > 433) {
      // Cut off if we are moving down past 30
      Joystick.button(127, 1);
    } else {
      Joystick.button(127, 0);
    }
  } else {
    if (cur_pot_pos < 433) {
      // To idle if we are moving up past 30
      Joystick.button(128, 1);
    } else {
      Joystick.button(128, 0);
    }
    // Only move throttle if not in cutoff   
  }
  // remember where we are - and don't go neg
  Joystick.Yrotate((1024 - min(1024, translated)) * 64);
  cur_pot_pos = t_raw;
}
