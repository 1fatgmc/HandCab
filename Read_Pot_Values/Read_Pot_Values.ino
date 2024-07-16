 // Sketch to read the pot values that will go in a config file. 
 // V. 24-06-17 

#include "config.h"       
#include "static.h"       
  
// Define Pot pins:
// These need to be defined in config.h    Copy and alter config_example.h as need to create config.h
//
const int throttlePin = THROTTLE_PIN;   //ESP32 Pin for the Throttle pot.
const int brakePin = BRAKE_PIN;      //ESP32 Pin for the brake pot.
const int reverserPin =REVERSER_PIN;    //ESP32 Pin for the reverser pot. 
const int serialSpeed = SERIAL_SPEED;

// variables for storing the potentiometer values
int throttleValue = 0;   // Used when reading the throttle pot.
int brakeValue = 0;      // Used when reading the brake pot.
int reverserValue = 0;   // Used when reading the reveser pot
int incomingByte = 0; // for incoming serial data

int notchNumber = 0;

//c
// Use the following for the value at each notch step on the throttle.
int notchValue[9] = {0,0,0,0,0,0,0,0,0}; // Notch 0 value

// Use the following numbers for the config file that shows the range of each throttle notch.
int notchRangeLow[9] = {0,0,0,0,0,0,0,0,0};  // The bottom range number for notch X
int notchRangeHigh[9] = {0,0,0,0,0,0,0,0,0};  // The upper range numbe for notch X
int alternateNotchRangeValue[9] = {0,0,0,0,0,0,0,0,0};  // alternate calculation

// Use these to read in the upper and lower number for the pot range for the brake and reverser.

int brakeLow = 0;  // the lowest range value for the brake pot.
int brakeHigh = 0; // the highest range value for the brake pot.
int brakeRange = 0;  // The range between BrakeLow and BrakeHigh
int brake10Percent = 0; // 10 Pecent of Brake Range
int brake20Percent = 0; // 20 Pecent of Brake Range

int reverserLow = 0; // the lowest range value for the reverser pot.
int reverserHigh = 0; // the highest range value for the reverser pot.
int reverser10Percent = 0;

int alternateReverserRangeValue[2] = {0,0};

// Use the following numbers for the config file that shows the 5 divisions the brake range is divide into.

int brakeRangeLow[6] = {0,0,0,0,0,0};  // the bottom range number for when brake is at position x.
int brakeRangeHigh[6] = {0,0,0,0,0,0};  // the upper range number for when the brake is at position x.
int alternateBrakeRangeValue[6] = {0,0,0,0,0,0};  // alternate calculation

// Use the following numbers for the config file that shows the 3 divisions the reverser range is divide into.

int reverseLow = 0;
int reverseHigh = 0;
int neutralLow = 0;
int neutralHigh = 0;
int forwardLow = 0;
int forwardHigh = 0;

void printBreak() {
  Serial.println();
  Serial.println("------------------------------------------");
}

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(serialSpeed);
  while(!Serial);
}

void loop() {
 
  Serial.println();
  Serial.println("enter c to continue.");  

  while(true) // remain here until told to break
  {
    if(Serial.available() > 0) // did something come in?
      if(Serial.read() == 'c') // is that something the char C?
        break;
  }
  
  printBreak();

  // Reading potentiometer value for the throttle in all 9 notches
  for (notchNumber=0; notchNumber<9; notchNumber++) {
    Serial.println();
    Serial.print("Put Throttle in Notch "); 
    Serial.print(notchNumber); Serial.println(" and then enter 'c' to continue");  // Prepare to read notch 1
    while(true) // remain here until told to break
    {
      if(Serial.available() > 0) // did something come in?
        if(Serial.read() == 'c') // is that something the char C?
          break;
    }

    throttleValue = analogRead(throttlePin);
    notchValue[notchNumber] = throttleValue;
    Serial.print ("Throttle Notch "); Serial.print(notchNumber); 
    Serial.print(" value is "); Serial.print (notchValue[notchNumber]);
    Serial.println();
  }
 
  // ------------------------------------------------------------------
  //  Read the Brake resistance in full off and full on.

  printBreak();

  //Brake Off reading
  Serial.println();
  Serial.print("Put brake in 'full off' and then enter 'c' to continue");  // Prepare to read notch 2
  while(true) // remain here until told to break
  {
    if(Serial.available() > 0) // did something come in?
      if(Serial.read() == 'c') // is that something the char C?
        break;
  }
  
  brakeValue = analogRead(brakePin);
  brakeLow = brakeValue;
  Serial.println();
  Serial.print ("Brake Off is "); Serial.print (brakeLow);
  Serial.println();

  //Brake On reading
  Serial.println();
  Serial.print("Put brake in 'full On' and then enter 'c' to continue");  // Prepare to read notch 2
  while(true) // remain here until told to break
  {
    if(Serial.available() > 0) // did something come in?
      if(Serial.read() == 'c') // is that something the char C?
        break;
  }
  brakeValue = analogRead(brakePin);
  brakeHigh  = brakeValue;
  Serial.println();
  Serial.print ("Brake Full On is "); Serial.println(brakeHigh);


  // ------------------------------------------------------------------
  //  Read the Reverser resistance in full reverse and full forward.

  printBreak();

  //Reverser full reverse reading
  Serial.println();
  Serial.print("Put Reverser in 'full reverse' and then enter 'c' to continue");  // Prepare to read notch 2
  while(true) // remain here until told to break
  {
    if(Serial.available() > 0) // did something come in?
      if(Serial.read() == 'c') // is that something the char C?
        break;
  }
  reverserValue = analogRead(reverserPin);
  reverserLow = reverserValue;
  Serial.println();
  Serial.print ("Reverser in full reverse is "); Serial.println(reverserLow);

  //Reverser full forward reading
  Serial.println();
  Serial.print("Put Reverser in 'full forward' and then enter 'c' to continue");  // Prepare to read notch 2
  while(true) // remain here until told to break
  {
    if(Serial.available() > 0) // did something come in?
      if(Serial.read() == 'c') // is that something the char C?
        break;
  }
  reverserValue = analogRead(reverserPin);
  reverserHigh  = reverserValue;
  Serial.println();
  Serial.print ("Reverser in full reverse is ");
  Serial.println(reverserHigh);


  //                  End of Pot Value Collection
  // ==========================================================

  printBreak();

  // Find the Throttle Values for config file for upper and lower range numbers

  for (int i=0; i<9; i++) {
    if (i==0) notchRangeLow[i] = notchValue[0];
    else      notchRangeLow[i] = notchRangeHigh[i-1] + 1;

    if (i<8) notchRangeHigh[i] = (notchValue[i+1] - notchValue[i]) /2 + notchValue[i];
    else     notchRangeHigh[i] = notchValue[8];
  }

  // alternate calculation
  for (int i=0; i<8; i++) {
    alternateNotchRangeValue[i] = (notchValue[i+1] - notchValue[i]) /2 + notchValue[i];
  }

  Serial.println();
  for (int i=0; i<9; i++) {
    Serial.print("Throttle Range "); Serial.print(i); 
    Serial.print(" Low: "); Serial.print(notchRangeLow[i]);
    Serial.print(" High: "); Serial.println(notchRangeHigh[i]);
  }

  // ------------------------------------------------------------------
  // Find Brake values for Config file

  brakeRange = brakeHigh-brakeLow;
  brake10Percent = brakeRange * .10;
  brake20Percent = brakeRange * .20;

  Serial.println();
  Serial.print("BrakeRange = "); Serial.println(brakeRange);
  Serial.print("Brake10Percent Number = "); Serial.println(brake10Percent);
  Serial.print("Brake20Percent Number = "); Serial.println(brake20Percent);


  // Brake Range 0
  // 1st braking Range -- No Brake (range 0)
  brakeRangeLow[0] = brakeLow;  // the lower range number for when the brake is off.
  brakeRangeHigh[0] = brakeLow + brake10Percent;  // the upper range number for when the brake is off.

  // Brake Range 1-4
  for (int i=1; i<5; i++) {
    brakeRangeLow[i] = brakeRangeHigh[i-1] + 1;  // bottom range number when the brake is barely on (range 1).
    brakeRangeHigh[i] = brakeRangeLow[i] + brake20Percent;  // upper range number when the brake is barely on (range 1).
  }

  // Brake Range 5
  brakeRangeLow[5] = brakeRangeHigh[4] + 1;  // bottom range number when the brake is full on on (range 5).
  brakeRangeHigh[5] = brakeHigh;  // upper range number when the brake is full on (range 5).

  // alternate calculations - should get the same result
  int brakeRange = brakeHigh - brakeLow;
  int previousStep = brakeLow;
  int nextStep = 0;
  for (int i=0;i<5;i++) {
    if (i==0) {      
      nextStep = previousStep + brakeRange*0.1;
    } else {
      nextStep = previousStep + brakeRange*0.2;
    }
    alternateBrakeRangeValue[i] = nextStep;
    previousStep = nextStep;
  }

  Serial.println();
  for (int i=0; i<6; i++) {
    Serial.print("Brake Range "); Serial.print(i); 
    Serial.print(" Low: "); Serial.print(brakeRangeLow[i]);
    Serial.print(" High: "); Serial.println(brakeRangeHigh[i]);
  }
      
  // ------------------------------------------------------------------
  // Find Reverser values for Config file
 
  reverser10Percent = ((reverserHigh - reverserLow) * .1);
 
  reverseLow = reverserLow;  // the lower range number for when in reverse.
  reverseHigh = (reverseLow + reverser10Percent);  // the upper range number for when in reverse.

  neutralLow = reverseHigh +1;  // the lower range number for when in neutral.
  neutralHigh = (neutralLow + (reverser10Percent * 7));  // the upper range number for when in neutral.

  forwardLow = neutralHigh +1;  // the lower range number for when in neutral.
  forwardHigh = (reverserHigh);  // the upper range number for when in neutral.

  // alternate calulations
  int reverserRange = reverserHigh - reverserLow;
  alternateReverserRangeValue[0] = reverserLow + (reverserRange*.30);
  alternateReverserRangeValue[1] = reverserLow + (reverserRange*.70);

  Serial.println();
  Serial.print("Reverser Reverse Low: "); Serial.print(reverseLow); Serial.print(" High: "); Serial.println(reverseHigh);
  Serial.print("Reverser Neutral Low: "); Serial.print(neutralLow); Serial.print(" High: "); Serial.println(neutralHigh);
  Serial.print("Reverser Forward Low: "); Serial.print(forwardLow); Serial.print(" High: "); Serial.println(forwardLow);


  //====================   End of Calculations =========================


  // ================= Start of Print All Numbers Out ======================
  
  //  Print the throttle values for the config .h file to the screen

  printBreak();

  Serial.println();
  Serial.println("Numbers for the config file.  Write/copy these numbers down in order. ");   
  Serial.println();
  Serial.print("#define THROTTLE_POT_NOTCH_VALUES { ");   
  for (int i=0; i<9; i++) { 
    Serial.print(notchRangeLow[i]);
    if (i<8) Serial.print(","); 
  }
  Serial.println(" }");

  //   ============================ Print the brake values for the config .h file to the screen

  Serial.print("#define BRAKE_POT_VALUES  { ");
  for (int i=1; i<6; i++) { 
    Serial.print(brakeRangeLow[i]);
    if (i<5) Serial.print(","); 
  }
  Serial.println(" }");

  //   ============================ Print the Reverser values for the config .h file to the screen
 
  Serial.print("#define REVERSER_POT_VALUES { ");
  Serial.print(neutralLow); Serial.print(","); Serial.print(neutralHigh);
  Serial.println(" }");

  // ------------------------------------------------------------------
  // alternate values

  Serial.println();
  Serial.println("OPTIONAL Alternate values");
  Serial.println();
  Serial.println("These give slightly different feel/response.");
  Serial.println("If you use the recalibrate feature in HandCab then the");
  Serial.println("these are the values that will be calculated.");
  Serial.println();

  Serial.print("#define THROTTLE_POT_NOTCH_VALUES { ");   
  for (int i=0; i<8; i++) { 
    Serial.print(alternateNotchRangeValue[i]);
    if (i<7) Serial.print(","); 
  }
  Serial.println(" }");

  Serial.print("#define BRAKE_POT_VALUES { ");   
  for (int i=0; i<5; i++) { 
    Serial.print(alternateBrakeRangeValue[i]);
    if (i<4) Serial.print(","); 
  }
  Serial.println(" }");

  Serial.print("#define REVERSER_POT_VALUES { ");   
  for (int i=0; i<2; i++) { 
    Serial.print(alternateReverserRangeValue[i]);
    if (i<1) Serial.print(","); 
  }
  Serial.println(" }");

  // ------------------------------------------------------------------

  Serial.println();
  Serial.print("Enter 'c' to continue");

  while(true) // remain here until told to break
  {
    if(Serial.available() > 0) // did something come in?
      if(Serial.read() == 'c') // is that something the char C?
        break;
  }


  // ============================ End of Print All Numbers Out ===========================

  //     =============  Shutdown or Re-run ==================

  while(true) // remain here until told to break
  {
    Serial.println(".");
    Serial.println("Turn off ESP32 or enter 'c' to continue and read pots again."); 
    delay (4000); 
    if(Serial.available() > 0) {       // did something come in?
      if(Serial.read() == 'c')
        break;
    }
  }

} // End of Void Loop
