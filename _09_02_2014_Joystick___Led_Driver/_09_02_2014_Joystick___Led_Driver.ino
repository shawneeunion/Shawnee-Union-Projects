// 09.02 finally got it to work. keep wires closer to adjust //
int UD = 0;
int LR = 0; //Setting up controller//

#include "LedControl.h" //  need the library
LedControl lc=LedControl(8,10,9,1); //10 is to CLOCK, 9 = CS, 8=DIN//


void setup() {
  Serial.begin(9600);

  lc.shutdown(0,false);// turn off power saving, enables display
  lc.setIntensity(0,8);// sets brightness (0~15 possible values)
  lc.clearDisplay(0);// clear screen

}

void loop() {
  UD = analogRead(A0);
  LR = analogRead(A1);
  char x_translate = map(LR, 1021, 0, 7, 0); //This maps the values//
  char y_translate = map(UD, 1021, 0, 0, 7);
  
  Serial.print("UD = ");
  Serial.print(UD, DEC);
  Serial.print(", LR = ");
  Serial.print(LR, DEC);
  Serial.print(", x = ");
  Serial.print(x_translate, DEC); 
  Serial.print(", y = ");
  Serial.println(y_translate, DEC); 
    // not in shutdown mode
    lc.clearDisplay(0);
    lc.setLed(0,x_translate,y_translate,true);  
  delay(150); //Mess with this delay to get your joystick correct//
}
