/* each direction of the joystick will have a
column in the matrix associated*/
const byte joystick_up = 0;
const byte joystick_down = 1;
const byte joystick_left = 2;
const byte joystick_right = 3;

/* joystick bounds that will identify in which
direction is the user pointing at: up, down, left, right */
const int joystick_up_bound = 768;                    // 512 + 256
const int joystick_down_bound = 256;                  // 512 - 256
const int joystick_right_bound = 768;                 // 512 + 256
const int joystick_left_bound = 256;                  // 512 - 256
const int joystick_middle_interval[2] = {492, 532};   // 512 - 20, 512 + 20

const byte joystick_last_direction_interval = 500;
const byte joystick_switch_debounce_interval = 100;

struct Joystick {
  byte pin_SW;
  byte pin_X;
  byte pin_Y;

  byte direction;
  int X_axis_value, Y_axis_value;
  
  bool current_switch_state;
  bool last_switch_state; 

  unsigned long int switch_debounce_time;
  unsigned long int last_direction_change;

  Joystick(byte pin_SW, byte pin_X, byte pin_Y): pin_SW(pin_SW), pin_X(pin_X), pin_Y(pin_Y){
    current_switch_state = LOW;
    last_switch_state = LOW; 
     
    switch_debounce_time = 0;
    last_direction_change = 0;
  } 

  void switchHandler();
  void movementHandler();
};

void Joystick::switchHandler(){
  bool switch_state = !digitalRead(pin_SW);

  if (switch_state != last_switch_state) {
    // if the state has changed, update the last debounce time of the button
    switch_debounce_time = millis();
  } 

  if ((millis() - switch_debounce_time) > joystick_switch_debounce_interval) {
    /* because it passed a certain debounce time, it means that the reading state
    is not a noise, so the current state of the button must be changed*/
    if (current_switch_state != switch_state) {
      current_switch_state = switch_state;    
    }
  } 

  last_switch_state = switch_state;
}

void Joystick::movementHandler(){
  if ((millis() - last_direction_change) <= joystick_last_direction_interval) {
    /*if the minimum interval of time betweens joystick direction changes
    has not passed, do not evaluate the joystick's state*/
    direction = -1;
    return ;
  }

  last_direction_change = millis();

  X_axis_value = analogRead(pin_X);
  Y_axis_value = analogRead(pin_Y);

  // check in which direction is the joystick pointing at
  if (X_axis_value > joystick_up_bound && joystick_middle_interval[0] < Y_axis_value < joystick_middle_interval[1]) {
    direction = joystick_up;        // joystick is pointing up
  } else if (Y_axis_value > joystick_right_bound && joystick_middle_interval[0] < X_axis_value < joystick_middle_interval[1]) {
    direction = joystick_right;     // joystick is pointing right
  } else if (X_axis_value < joystick_down_bound && joystick_middle_interval[0] < Y_axis_value < joystick_middle_interval[1]) {
    direction = joystick_down;      // joystick is pointing down
  } else if (Y_axis_value < joystick_left_bound && joystick_middle_interval[0] < X_axis_value < joystick_middle_interval[1]) {
    direction = joystick_left;      // joystick is pointing left 
  }
}