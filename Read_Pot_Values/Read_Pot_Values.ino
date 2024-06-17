 // Sketch to read the pot values that will go in a config file. 
 // V. 24-06-17 

  // Define Pot connections and buttons:
    const int ThrottlePin = 39;   //ESP32 Pin for the Throttle pot.
    const int BrakePin = 34;      //ESP32 Pin for the brake pot.
    const int ReverserPin =35;    //ESP32 Pin for the reverser pot.        
  
   // variables for storing the potentiometer values
int ThrottleValue = 0;   // Used when reading the throttle pot.
int BrakeValue = 0;      // Used when reading the brake pot.
int ReverserValue = 0;   // Used when reading the reveser pot
int incomingByte = 0; // for incoming serial data


//c
   // Use the following for the value at each notch step on the throttle.
int N0 = 0; // Notch 0 value
int N1 = 0; // Notch 1 value
int N2 = 0; //   "    "
int N3 = 0; //   "    "
int N4 = 0; //   "    " 
int N5 = 0; //   "    " 
int N6 = 0; //   "    "
int N7 = 0; //   "    "
int N8 = 0; // Notch 8 value

// Use the following numbers for the config file that shows the range of each throttle notch.
int N0L = 0;  // The bottom range number for notch 0
int N0H = 0;  // The upper range numbe for notch 0
int N1L = 0;  // The bottom range number for notch 1
int N1H = 0;  // The upper range numbe for notch 1
int N2L = 0;  // The bottom range number for notch 2
int N2H = 0;  // The upper range numbe for notch 2
int N3L = 0;  // The bottom range number for notch 3
int N3H = 0;  // The upper range number for notch 3
int N4L = 0;  //      "    "
int N4H = 0;  //      "    " 
int N5L = 0;  //      "    " 
int N5H = 0;  //      "    " 
int N6L = 0;  //      "    " 
int N6H = 0;  //      "    " 
int N7L = 0;  //      "    " 
int N7H = 0;  //      "    "
int N8L = 0;  // The bottom range number for notch 8
int N8H = 0;  // The upper range number for notch 8

// Use these to read in the upper and lower number for the pot range for the brake and reverser.

int BrakeLow = 0;  // the lowest range value for the brake pot.
int BrakeHigh = 0; // the highest range value for the brake pot.
int BrakeRange = 0;  // The range between BrakeLow and BrakeHigh
int Brake10Percent = 0; // 10 Pecent of Brake Range
int Brake20Percent = 0; // 20 Pecent of Brake Range
int ReverserLow = 0; // the lowest range value for the reverser pot.
int ReverserHigh = 0; // the highest range value for the reverser pot.
int Reverser10Percent = 0;

// Use the following numbers for the config file that shows the 5 divisions the brake range is divide into.

int Brake0L = 0;  // the bottom range number for when brake is off.
int Brake0H = 0;  // the upper range number for when the brake is off.
int Brake1L = 0;  // bottom range number when the brake is barely on (range 1).
int Brake1H = 0;  // upper range number when the brake is barely on (range 1).
int Brake2L = 0;  // bottom range number when the brake is applied more (range 2).
int Brake2H = 0;  // upper range number when the brake is applied more (range 2).
int Brake3L = 0;  // bottom range number when the brake is applied more (range 3).
int Brake3H = 0;  // upper range number when the brake is applied more (range 3).
int Brake4L = 0;  // bottom range number when the brake is applied more (range 4).
int Brake4H = 0;  // upper range number when the brake is applied more (range 4).
int Brake5L = 0;  // bottom range number when the brake is full on (range 5).
int Brake5H = 0;  // upper range number when the brake is full on (range 5)

// Use the following numbers for the config file that shows the 3 divisions the reverser range is divide into.

int reverseLow = 0;
int reverseHigh = 0;
int neutralLow = 0;
int neutralHigh = 0;
int forwardLow = 0;
int forwardHigh = 0;


void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);

 while(!Serial);
  }

void loop() {
 
       //  Serial.println(".");
       Serial.println(".");
    Serial.println("enter c to continue.");  

     while(true) // remain here until told to break
{
  if(Serial.available() > 0) // did something come in?
    if(Serial.read() == 'c') // is that something the char C?
      break;
}
                        // Reading potentiometer value for the throttle in all 9 notches
     Serial.println(".");
     Serial.println(".");
    
   //Notch 0 reading     
   Serial.print("Put Throttle in off position - Notch 0 and then enter 'c' to continue");  // Prepare to read notch 0
  
    while(true) // remain here until told to break
{
  if(Serial.available() > 0) // did something come in?
    if(Serial.read() == 'c')
          break;
}
  ThrottleValue = analogRead(ThrottlePin);
  int N0 = ThrottleValue;
  Serial.println(".");
  Serial.print ("Throttle Notch N0 is ");
  Serial.print (N0);
  Serial.println(".");
  Serial.println(".");
 
 //Notch 1 reading
 Serial.print("Put Throttle in Notch 1 and then enter 'c' to continue");  // Prepare to read notch 1
   while(true) // remain here until told to break
{
  if(Serial.available() > 0) // did something come in?
    if(Serial.read() == 'c') // is that something the char C?
      break;
}
ThrottleValue = analogRead(ThrottlePin);
  int N1 = ThrottleValue;
  Serial.println(".");
  Serial.print ("Throttle Notch N1 is ");
  Serial.print (N1);
  Serial.println(".");
  Serial.println(".");

 //Notch 2 reading
 Serial.print("Put Throttle in Notch 2 and then enter 'c' to continue");  // Prepare to read notch 2
   while(true) // remain here until told to break
{
  if(Serial.available() > 0) // did something come in?
    if(Serial.read() == 'c') // is that something the char C?
      break;
}
ThrottleValue = analogRead(ThrottlePin);
   int N2 = ThrottleValue;
  Serial.println(".");
  Serial.print ("Throttle Notch N2 is ");
  Serial.print (N2);
  Serial.println(".");
  Serial.println(".");

 //Notch 3 reading
 Serial.print("Put Throttle in Notch 3 and then enter 'c' to continue");  // Prepare to read notch 2
   while(true) // remain here until told to break
{
  if(Serial.available() > 0) // did something come in?
    if(Serial.read() == 'c') // is that something the char C?
      break;
}
ThrottleValue = analogRead(ThrottlePin);
   int N3 = ThrottleValue;
  Serial.println(".");
  Serial.print ("Throttle Notch N3 is ");
  Serial.print (N3);
  Serial.println(".");
  Serial.println(".");

//Notch 4 reading
 Serial.print("Put Throttle in Notch 4 and then enter 'c' to continue");  // Prepare to read notch 2
   while(true) // remain here until told to break
{
  if(Serial.available() > 0) // did something come in?
    if(Serial.read() == 'c') // is that something the char C?
      break;
}
ThrottleValue = analogRead(ThrottlePin);
   int N4 = ThrottleValue;
  Serial.println(".");
  Serial.print ("Throttle Notch N4 is ");
  Serial.print (N4);
  Serial.println(".");
  Serial.println(".");

//Notch 5 reading
 Serial.print("Put Throttle in Notch 5 and then enter 'c' to continue");  // Prepare to read notch 2
   while(true) // remain here until told to break
{
  if(Serial.available() > 0) // did something come in?
    if(Serial.read() == 'c') // is that something the char C?
      break;
}
ThrottleValue = analogRead(ThrottlePin);
   int N5 = ThrottleValue;
  Serial.println(".");
  Serial.print ("Throttle Notch N5 is ");
  Serial.print (N5);
  Serial.println(".");
  Serial.println(".");

//Notch 6 reading
 Serial.print("Put Throttle in Notch 6 and then enter 'c' to continue");  // Prepare to read notch 2
   while(true) // remain here until told to break
{
  if(Serial.available() > 0) // did something come in?
    if(Serial.read() == 'c') // is that something the char C?
      break;
}
ThrottleValue = analogRead(ThrottlePin);
   int N6 = ThrottleValue;
  Serial.println(".");
  Serial.print ("Throttle Notch N6 is ");
  Serial.print (N6);
  Serial.println(".");
  Serial.println(".");

//Notch 7 reading
 Serial.print("Put Throttle in Notch 7 and then enter 'c' to continue");  // Prepare to read notch 2
   while(true) // remain here until told to break
{
  if(Serial.available() > 0) // did something come in?
    if(Serial.read() == 'c') // is that something the char C?
      break;
}
ThrottleValue = analogRead(ThrottlePin);
   int N7 = ThrottleValue;
  Serial.println(".");
  Serial.print ("Throttle Notch N7 is ");
  Serial.print (N7);
  Serial.println(".");
  Serial.println(".");

  //Notch 8 reading
 Serial.print("Put Throttle in Notch 8 and then enter 'c' to continue");  // Prepare to read notch 2
   while(true) // remain here until told to break
{
  if(Serial.available() > 0) // did something come in?
    if(Serial.read() == 'c') // is that something the char C?
      break;
}
ThrottleValue = analogRead(ThrottlePin);
   int N8 = ThrottleValue;
  Serial.println(".");
  Serial.print ("Throttle Notch N8 is ");
  Serial.print (N8);
  Serial.println(".");
  Serial.println(".");

                //  Read the Brake resistance in full off and full on.

 //Brake Off reading
 Serial.print("Put brake in 'full off' and then enter 'c' to continue");  // Prepare to read notch 2
   while(true) // remain here until told to break
{
  if(Serial.available() > 0) // did something come in?
    if(Serial.read() == 'c') // is that something the char C?
      break;
}
BrakeValue = analogRead(BrakePin);
   int BrakeLow = BrakeValue;
  Serial.println(".");
  Serial.print ("Brake Off is ");
  Serial.print (BrakeLow);
  Serial.println(".");
  Serial.println(".");

//Brake On reading
 Serial.print("Put brake in 'full On' and then enter 'c' to continue");  // Prepare to read notch 2
   while(true) // remain here until told to break
{
  if(Serial.available() > 0) // did something come in?
    if(Serial.read() == 'c') // is that something the char C?
      break;
}
BrakeValue = analogRead(BrakePin);
  int BrakeHigh  = BrakeValue;
  Serial.println(".");
  Serial.print ("Brake Full On is ");
  Serial.print (BrakeHigh);
  Serial.println(".");
  Serial.println(".");

                //  Read the Reverser resistance in full reverse and full forward.

 //Reverser full reverse reading
 Serial.print("Put Reverser in 'full reverse' and then enter 'c' to continue");  // Prepare to read notch 2
   while(true) // remain here until told to break
{
  if(Serial.available() > 0) // did something come in?
    if(Serial.read() == 'c') // is that something the char C?
      break;
}
ReverserValue = analogRead(ReverserPin);
   int ReverserLow = ReverserValue;
  Serial.println(".");
  Serial.print ("Reverser in full reverse is ");
  Serial.print (ReverserLow);
  Serial.println(".");
  Serial.println(".");

//Reverser full forward reading
 Serial.print("Put Reverser in 'full forward' and then enter 'c' to continue");  // Prepare to read notch 2
   while(true) // remain here until told to break
{
  if(Serial.available() > 0) // did something come in?
    if(Serial.read() == 'c') // is that something the char C?
      break;
}
ReverserValue = analogRead(ReverserPin);
  int ReverserHigh  = ReverserValue;
  Serial.println(".");
  Serial.print ("Reverser in full reverse is ");
  Serial.print (ReverserHigh);
  Serial.println(".");
  Serial.println(".");

                   // Throttle Values for config file for upper and lower range numbers

N0L= N0;                      //Notch 0
N0H = ((N1-N0)/2) + N0;

N1L = N0H + 1;              //Notch 1
N1H = (N2-N1)/2 + N1;

N2L = N1H + 1;              //Notch 2
N2H = (N3-N2)/2 + N2;

N3L = N2H + 1;              //Notch 3
N3H = (N4-N3)/2 + N3;

N4L = N3H + 1;              //Notch 4
N4H = (N5-N4)/2 + N4;

N5L = N4H + 1;              //Notch 5
N5H = (N6-N5)/2 + N5;

N6L = N5H + 1;              //Notch 6
N6H = (N7-N6)/2 + N6;

N7L = N6H + 1;              //Notch 7
N7H = (N8-N7)/2 + N7;

N8L = N7H + 1;              //Notch 8
N8H = N8;

   //                  End of Throttle Routines
   // ==========================================================

   //           Find Brake values for Config file

   BrakeRange = BrakeHigh-BrakeLow;
   Serial.println(".");
   Serial.println("BrakeRange = ");
   Serial.print (BrakeRange);
   Serial.println(".");

   Brake10Percent = BrakeRange * .10;
   Serial.println(".");
   Serial.println("Brake10Percent Number = ");
   Serial.print (Brake10Percent);
   Serial.println(".");

   Brake20Percent = BrakeRange * .20;
   Serial.println(".");
   Serial.println("Brake20Percent Number = ");
   Serial.print (Brake20Percent);
   Serial.println(".");

   // Brake Range 0
   Serial.println(".");  // 1st braking Range -- No Brake (range 0)
   Brake0L = BrakeLow;  // the lower range number for when the brake is off.
   Serial.print (Brake0L);
   Serial.println(".");
   Brake0H = BrakeLow + Brake10Percent;  // the upper range number for when the brake is off.
   Serial.print (Brake0H);
   Serial.println(".");
   Serial.println(".");
   
   // Brake Range 1
    Brake1L = Brake0H + 1;  // bottom range number when the brake is barely on (range 1).
    Serial.print (Brake1L);
    Serial.println(".");
    Brake1H = Brake1L + Brake20Percent;  // upper range number when the brake is barely on (range 1).
    Serial.print (Brake1H);
    Serial.println(".");
    Serial.println(".");

  // Brake Range 2
    Brake2L = Brake1H + 1;  // bottom range number when the brake is on (range 2).
    Serial.print (Brake2L);
    Serial.println(".");
    Brake2H = Brake2L + Brake20Percent;  // upper range number when the brake is on (range 2).
    Serial.print (Brake2H);
    Serial.println(".");
    Serial.println(".");

  // Brake Range 3
    Brake3L = Brake2H + 1;  // bottom range number when the brake is on (range 3).
    Serial.print (Brake3L);
    Serial.println(".");
    Brake3H = Brake3L + Brake20Percent;  // upper range number when the brake is on (range 3).
    Serial.print (Brake3H);
    Serial.println(".");
    Serial.println(".");

 // Brake Range 4
    Brake4L = Brake3H + 1;  // bottom range number when the brake is on (range 4).
    Serial.print (Brake4L);
    Serial.println(".");
    Brake4H = Brake4L + Brake20Percent;  // upper range number when the brake is on (range 4).
    Serial.print (Brake4H);
    Serial.println(".");
    Serial.println(".");
    
  // Brake Range 5
    Brake5L = Brake4H + 1;  // bottom range number when the brake is full on on (range 5).
    Serial.print (Brake5L);
    Serial.println(".");
    Brake5H = BrakeHigh;  // upper range number when the brake is full on (range 5).
    Serial.print (Brake5H);
    Serial.println(".");
    Serial.println(".");
   
  //====================   End of Brake Routines =========================

 // ==================================================================================================================== 

   // ==================== Start Reverser Routines ======================= 
 
  Reverser10Percent = ((ReverserHigh - ReverserLow) * .1);
 
   Serial.println(".");  
   reverseLow = ReverserLow;  // the lower range number for when in reverse.
   Serial.print (reverseLow);
   Serial.println(".");
   reverseHigh = (reverseLow + Reverser10Percent);  // the upper range number for when in reverse.
   Serial.print (reverseHigh);
   Serial.println(".");
   Serial.println(".");   

 Serial.println(".");  
   neutralLow = reverseHigh +1;  // the lower range number for when in neutral.
   Serial.print (neutralLow);
   Serial.println(".");
   neutralHigh = (neutralLow + (Reverser10Percent * 7));  // the upper range number for when in neutral.
   Serial.print (neutralHigh);
   Serial.println(".");
   Serial.println(".");


 Serial.println(".");  
   forwardLow = neutralHigh +1;  // the lower range number for when in neutral.
   Serial.print (forwardLow);
   Serial.println(".");
   forwardHigh = (ReverserHigh);  // the upper range number for when in neutral.
   Serial.print (forwardHigh);
   Serial.println(".");
   Serial.println(".");

   //====================   End of Reverser Routines =========================


   // ============================ Start of Print All Numbers Out  Routine ===========================

                                 // Serial.println = (" Numbers for the throttle ranges to config file ")  
  
  Serial.println ("Numbers for the Throttle Ranges in the config file.  Write these numbers down in order. ");   

   //   ============================ Print the throttle values for the config .h file to the screen
 
 Serial.println(".");
    Serial.println((String)N1L+","+N2L+","+N3L+","+N4L+","+N5L+","+N6L+","+N7L+","+N7H);
         Serial.println(".");
         Serial.println(".");
         Serial.print("Enter 'c' to continue");

     while(true) // remain here until told to break
{
  if(Serial.available() > 0) // did something come in?
    if(Serial.read() == 'c') // is that something the char C?
      break;
}
  Serial.println(".");
         Serial.println(".");

                          // Serial.println = (" Numbers for the Brake Ranges to config file ")
 
   //   ============================ Print the brake values for the config .h file to the screen

  Serial.println ("Numbers for the Brake Ranges in the config file.  Write thes numbers down in order. Enter c to continue.");
  
     while(true) // remain here until told to break
{
  if(Serial.available() > 0) // did something come in?
    if(Serial.read() == 'c') // is that something the char C?
      break;
}

   Serial.println(".");
    Serial.println((String)+Brake1L+","+Brake2L+","+Brake3L+","+Brake4L+","+Brake5L);
    
            Serial.println(".");
               Serial.println(".");
                  Serial.print("Enter 'c' to continue");

     while(true) // remain here until told to break
{
  if(Serial.available() > 0) // did something come in?
    if(Serial.read() == 'c') // is that something the char C?
      break;
}
  Serial.println(".");
         Serial.println(".");

   //   ============================ Print the Reverser values for the config .h file to the screen
 
  Serial.println ("Numbers for the Reverser Ranges in the config file.  Write thes numbers down in order. Enter c to continue.");
  
     while(true) // remain here until told to break
{
  if(Serial.available() > 0) // did something come in?
    if(Serial.read() == 'c') // is that something the char C?
      break;
}

     Serial.println(".");
    Serial.println((String)neutralLow+","+neutralHigh);
    
            Serial.println(".");
               Serial.println(".");
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
