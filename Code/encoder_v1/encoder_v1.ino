#include <Joystick.h>

const Joystick_ Joystick;

// Indicators
const uint8_t pwr_led = 5;

// Rotary Inputs
const uint8_t sw = 5;
const uint8_t clk = 3;
const uint8_t dt = 2;

// Joystick Outputs
const uint8_t js_sw = 0;
const uint8_t js_ccw = 1;
const uint8_t js_cw = 2;

// Button State
int lastButtonState = 0;

// Rotary State
volatile long count = 0;
long lastCountVal = 0;

void encoder_isr()
{
  // Serial.println("ISR");
  // Table contains 16 positions which we can align with the first 4 bits
  // of our old_value variable.
  //
  // Ex:
  //  old_val = 0010 0010 (Binary 34)
  //
  //  > We get the important bits by adding a 0000 1111 value
  //  important_bits = old_val & 0b1111
  //
  //       0010 0010
  //     & 0000 1111
  //     -----------
  //       0000 0010 (Binary 3)
  //
  //  We can then use this to look up the next value in the lookup_table
  //
  static const int8_t lookup_table[] = {0, -1, 1, 0, 1, 0, 0, -1, -1, 0, 0, 1, 0, 1, -1, 0};
  static const uint8_t rotaryIntervalsPerDetent = 2;
  static int enc_val = 0;
  static int8_t old_val = 3;

  // Shift Previous State
  // Ex: 0000 0010 => 0000 1000
  old_val <<= 2;

  // If CLK is high, then add a 1 to the 2 pos in the old_val binary
  if (digitalRead(clk))
    old_val |= 0b0010;

  // If DT is high, then add a 1 to the 2 pos in the old_val binary
  if (digitalRead(dt))
    old_val |= 0b0001;

  // Use the first 4 bits of the old_val value to get a
  enc_val += lookup_table[(old_val & 0b1111)];
  
  // We want to align our events with the encoder we are using.  Some
  // have 4 state changes per "detent" and some have 2.  To manage this
  // we store the intervals per detent in a variable
  if (enc_val > 1)
  {
    long changeValue = 1;

    count = count + changeValue;
    enc_val = 0;
  }

  // We need to invert the math for counter-clockwise
  if (enc_val < -1)
  {
    long changeValue = -1;

    count = count + changeValue;
    enc_val = 0;
  }
}

void temp() {
  Serial.println("Temp");
}

void setup()
{
  pinMode(sw, INPUT_PULLUP);
  pinMode(clk, INPUT_PULLUP);
  pinMode(dt, INPUT_PULLUP);
  pinMode(pwr_led, OUTPUT);

  digitalWrite(pwr_led, HIGH);

  attachInterrupt(digitalPinToInterrupt(clk), encoder_isr, CHANGE);
  attachInterrupt(digitalPinToInterrupt(dt), encoder_isr, CHANGE);

  lastButtonState = digitalRead(sw);

  Joystick.begin();
  Serial.begin(9600);
  // Need to wait for the Serial module to boot before we can use it in the setup
  while (!Serial) ;

  Serial.println("Script Starting");
}

void loop()
{
  // delay(20);
  // Read Button Value
  lastButtonState = checkPinState(sw, lastButtonState, js_sw);

  // Check if the encoder state has changed
  if (count != lastCountVal)
  { 
    // Trigger the joystick buttons based on the direction of the counter
    if (count > lastCountVal)
    {
      // Clockwise
      Joystick.setButton(js_cw, HIGH);
      delay(40);
      Joystick.setButton(js_cw, LOW);
    }
    else
    {
      Joystick.setButton(js_ccw, HIGH);
      delay(40);
      Joystick.setButton(js_ccw, LOW);
    }

    lastCountVal = count;
    
  }
}

int checkPinState(uint8_t pin, int prev, uint8_t jsOut)
{
  int val = digitalRead(pin);

  if (val != prev)
  {
    Joystick.setButton(jsOut, val);
  }

  return val;
}
