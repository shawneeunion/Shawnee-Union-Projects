// Example of debouncing multiple switches
// Author: Nick Gammon
// Date: 2 January 2013
// Released for public use.

#include <math.h>

#include "Wire.h"
#include "LiquidCrystal.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_TSL2561_U.h>

#define    M_LOG2E   1.4426950408889634074


LiquidCrystal lcd(3, 2, 4);

Adafruit_TSL2561_Unified tsl = Adafruit_TSL2561_Unified(TSL2561_ADDR_FLOAT, 12345);


int iso_c=0;
int iso[] = {100, 200, 400, 800, 1600, 3200};
int f_c=0;
float f[] = {1.4, 2, 2.8, 5.6, 8, 11, 16, 22};
float speedx;
float ev = 2; // eventually this will be read in by a third button press, to make it easy I'm directly giving it a number. 
float ev2;
int iso2;
int f2;

const unsigned long debounceTime = 20;   // milliseconds
const int numSwitches = 5;  // how many switches, for arrays below

// where switches are connected
const byte switchPin [numSwitches] = { 8, 9, 10, 11, 12 };
// when last changed state
unsigned long lastPressedTime [numSwitches];
// current state
byte switchState [numSwitches];
// have we changed state in any switch?
bool stateChange = false;

void setupSwitches ()
  {
  for (int i = 0; i < numSwitches; i++)
    {
    pinMode (switchPin [i], INPUT_PULLUP);
    switchState [i] = HIGH;  // not pressed
    lastPressedTime [i] = 0;
    }  // end of for each switch
  }  // end of setupSwitches
  
  void displaySensorDetails(void)
{
  sensor_t sensor;
  tsl.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" lux");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" lux");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" lux");  
  Serial.println("------------------------------------");
  Serial.println("");
  delay(500);
}

  
  void configureSensor(void)
{
  /* You can also manually set the gain or enable auto-gain support */
  // tsl.setGain(TSL2561_GAIN_1X);      /* No gain ... use in bright light to avoid sensor saturation */
  // tsl.setGain(TSL2561_GAIN_16X);     /* 16x gain ... use in low light to boost sensitivity */
  tsl.enableAutoRange(true);            /* Auto-gain ... switches automatically between 1x and 16x */
  
  /* Changing the integration time gives you better sensor resolution (402ms = 16-bit data) */
  tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_13MS);      /* fast but low resolution */
  // tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_101MS);  /* medium resolution and speed   */
  // tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_402MS);  /* 16-bit data but slowest conversions */

  /* Update these values depending on what you've set above! */  
  Serial.println("------------------------------------");
  Serial.print  ("Gain:         "); Serial.println("Auto");
  Serial.print  ("Timing:       "); Serial.println("13 ms");
  Serial.println("------------------------------------");
}

  
void setup ()
  {
  Serial.begin (9600);
  setupSwitches ();

 lcd.begin(16, 2);
   lcd.setBacklight(HIGH);
   
   
  Serial.println("Light Sensor Test"); Serial.println("");
  
  /* Initialise the sensor */
  if(!tsl.begin())
  {
    /* There was a problem detecting the ADXL345 ... check your connections */
    Serial.print("Ooops, no TSL2561 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }
  
  /* Display some basic information on this sensor */
  displaySensorDetails();
  
  /* Setup the sensor gain and integration time */
  configureSensor();
  }  // end of setup

 
// check switches
void checkSwitches ()
  {
  unsigned long now = millis ();
  for (int i = 0; i < numSwitches; i++)
    {
    byte currentState = digitalRead (switchPin [i]);
    if ((currentState != switchState [i]) &&
        (now - lastPressedTime [i] >= debounceTime))
      {
      switchState [i] = currentState;  
      lastPressedTime [i] = now;
      stateChange = true;   // a switch has changed state
      }  // end of changed state
    }  // end of for each switch
  }  // end of checkSwitches
  
void debugSwitchStatus ()
  {
  if (stateChange)
    {
    for (int i = 0; i < numSwitches; i++)
     {
     if (switchState [i] == LOW) 
       {
         Serial.println(i);
         if (i==0) {
           if (f_c == 8) {
             f_c = 0;
           }          
           if (f_c == 7) {
             f_c = -1;
           }   
           f_c++;
           xprintLCD(); 
           if (f_c == 7) {
             f_c = -1;
           }
         }
         
         if (i==4) {
           
           if (f_c == 0) {
             f_c = 8;
           }
           if (f_c == -1) {
             f_c = 7;
           }
           f_c=f_c -1;
           xprintLCD(); 
           if (f_c == 0) {
             f_c = 8;
           }
         }
         if (i==1) {
           
           if (iso_c == 6) {
             iso_c = 0;
           }
           if (iso_c == 5) {
             iso_c = -1;
           }
           iso_c++;
           xprintLCD();
           if (iso_c == 5) {
             iso_c = -1;
           }
         }
         if (i==2) {
           sensors_event_t event;
           tsl.getEvent(&event);

    Serial.print(event.light); Serial.println(" lux");
    float bam=event.light; 
           float ev_ = (log(bam/2.5))/(log(2));
           xprintLCD();
           ev = ev_;
         }
         if (i==3) {   
           
             if (iso_c == -1) {
             iso_c = 5;
             }
            
           if (iso_c == 0) {
           iso_c = 6;
           } 
           iso_c=iso_c -1;
           xprintLCD();
           if (iso_c == 0) {
           iso_c = 6;
           }
         }
     

         delay(100);
       }
     }  // end of for
    stateChange = false;  
    }    
  }  // end of debugSwitchStatus
  
void loop ()
  {
  checkSwitches ();
  debugSwitchStatus ();
  }  // end of loop
  
  void xprintLCD(){
 lcd.setCursor(0,0);
 lcd.print("ISO:");
 lcd.print(iso[iso_c]);
 int iso2 = iso[iso_c];
 lcd. print(" F#:");
 lcd.print(f[f_c]);
 float f2 = f[f_c];
 lcd.setCursor(0,1);
 int ev2 = (ev + (log(iso2/100))/(log(2)));
 speedx =(pow(f2,2) / pow (2,ev2));
 if (speedx < 1){ 
     //I want to turn this into a function
     lcd.print("1/");
     lcd.print(1/ speedx);
 }else{
     lcd.print(speedx);
 }
 lcd.print(" EV:");
lcd.print(ev); 
 Serial.print("ISO");
 Serial.print(iso_c);
 Serial.print("--");
 Serial.print(iso2);
 Serial.print("  F:");
 Serial.print(f_c);
 Serial.print("--");
 Serial.print(f2);
 Serial.print(" EV:");
 Serial.print(ev);
 Serial.print("..");
 Serial.print(ev2);
 Serial.print(" Speed:");
 Serial.println(speedx);
}
