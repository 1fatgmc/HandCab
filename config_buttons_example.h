// ********************************************************************************************
// 24-06-17
// false = Counter clockwise  true = clockwise
#define ENCODER_ROTATION_CLOCKWISE_IS_INCREASE_SPEED                     false

// Define what the rotary encoder button does.   (Pushing down on the knob)
// By default it will stop the loco if moving, then a second push will change the direction.
// Uncomment and change the action if you wish it to do something else.
// See static.h or README.md for details on the allowed functions.

// #define ENCODER_BUTTON_ACTION SPEED_STOP_THEN_TOGGLE_DIRECTION 

// true = if the locos(s) are stationary, clicking the encoder button will toggle the direction
// this only takes effect if the ENCODER_BUTTON_ACTION (above) is set to SPEED_STOP_THEN_TOGGLE_DIRECTION
// #define TOGGLE_DIRECTION_ON_ENCODER_BUTTON_PRESSED_WHEN_STATIONAY         true

// rotary encode debounce time
// increase if you find the encoder buttons bounce (activate twice) or you get speed changes when you press the encoder button
// #define ROTARY_ENCODER_DEBOUNCE_TIME 200

// ********************************************************************************************

// define what each button will do as direct press (not in a menu)   * and # cannot be remapped
// see static.h or README.md for details on the allowed functions

 
#define CHOSEN_KEYPAD_0_FUNCTION FUNCTION_0        //lights on/off
#define CHOSEN_KEYPAD_1_FUNCTION FUNCTION_1        //bell
#define CHOSEN_KEYPAD_2_FUNCTION FUNCTION_2        //horn
#define CHOSEN_KEYPAD_3_FUNCTION FUNCTION_3        //changed this to a sound function -- coupler
#define CHOSEN_KEYPAD_4_FUNCTION FUNCTION_9        // changed this to drive hold
#define CHOSEN_KEYPAD_5_FUNCTION FUNCTION_8        // changed this to turn the prime mover (the sound loco) on/off
#define CHOSEN_KEYPAD_6_FUNCTION SPEED_MULTIPLIER  // toggles through the three acceleration delays in stepping up through the speed steps
#define CHOSEN_KEYPAD_7_FUNCTION FUNCTION_0 // Might change this to a sound function since I have it mapped to an external button
#define CHOSEN_KEYPAD_8_FUNCTION FUNCTION_0   // Might change this to a sound function since I have it mapped to an external button
#define CHOSEN_KEYPAD_9_FUNCTION FUNCTION_0 // Might change this to a sound function since I have it mapped to an external button

// text that will appear when you press #
// if you rearrange the items above, modify this text to suit    * and # cannot be remapped here
 
#define CHOSEN_KEYPAD_0_DISPLAY_NAME "Lights"
#define CHOSEN_KEYPAD_1_DISPLAY_NAME "1 Bell"
#define CHOSEN_KEYPAD_2_DISPLAY_NAME "2 Horn"
#define CHOSEN_KEYPAD_3_DISPLAY_NAME "3 Coupler"
#define CHOSEN_KEYPAD_4_DISPLAY_NAME "4 Drive Hold"
#define CHOSEN_KEYPAD_5_DISPLAY_NAME "5 Prime Mover"
#define CHOSEN_KEYPAD_6_DISPLAY_NAME "Acceleration Delays"
#define CHOSEN_KEYPAD_7_DISPLAY_NAME "NA"    // Will relabel this if I change it to a sound function -- See above
#define CHOSEN_KEYPAD_8_DISPLAY_NAME "NA"  // Will relabel this if I change it to a sound function -- See above
#define CHOSEN_KEYPAD_9_DISPLAY_NAME "NA"    // Will relabel this if I change it to a sound function -- See above

// by default, # will show the list above. 
// if you change the following line to true, it will take you to the Loco Function Labels screen directly

#define HASH_SHOWS_FUNCTIONS_INSTEAD_OF_KEY_DEFS         false

// ********************************************************************************************

// speed increase for each click of the encoder 

//#define SPEED_STEP                       4                                           // 5-28-24 commented out
//#define SPEED_STEP_MULTIPLIER            3          // for 'fast' speed steps        // 5-28-24 commented out

// Additional multiplier.  If the multiplier is enabled from the menu, each rotation of the encoder becomes the speedStep * the AdditionalMultiplier

#define SPEED_STEP_ADDITIONAL_MULTIPLIER 2

// by default, the speed will be displayed as the the DCC speed (0-126)
// IMPORTANT: only one should be enabled.  If DISPLAY_SPEED_AS_PERCENT is enabled it will take presidence over DISPLAY_SPEED_AS_0_TO_28
// Note: there will be rounding errors!

// uncomment this line to display the speeds a percentage.
// #define DISPLAY_SPEED_AS_PERCENT         true
// uncomment this line to display the speeds as 0-28.
// #define DISPLAY_SPEED_AS_0_TO_28         true

// ********************************************************************************************
// DCC functions in consists
#define CONSIST_FUNCTION_FOLLOW_F0                  CONSIST_ALL_LOCOS            // lights
#define CONSIST_FUNCTION_FOLLOW_F1                  CONSIST_LEAD_LOCO            // bell
#define CONSIST_FUNCTION_FOLLOW_F2                  CONSIST_LEAD_LOCO            // horn
#define CONSIST_FUNCTION_FOLLOW_OTHER_FUNCTIONS     CONSIST_LEAD_LOCO

// ********************************************************************************************

// define what each of the optional additional buttons will do
// see static.h or README.md for details on the allowed functions
// all must be included, just set the ones you don't need to FUNCTION_NULL
// the button numbers relate to the GPIO pins 5,15,25,26,27,32,33
//
// Sumner note:  The following are what I use for the HandCab.  Leave FUNCTION_(0,1,2) as they are.  The othrs for BUTTONS 3-6 can be changed if you want to.

#define CHOSEN_ADDITIONAL_BUTTON_0_FUNCTION FUNCTION_0   // GPIO 5 ---- Turns Lights on/off
#define CHOSEN_ADDITIONAL_BUTTON_1_FUNCTION FUNCTION_1   // GPIO 15 --- Bell
#define CHOSEN_ADDITIONAL_BUTTON_2_FUNCTION FUNCTION_2   // GPIO 25 --- Horn
#define CHOSEN_ADDITIONAL_BUTTON_3_FUNCTION POWER_TOGGLE   // GPIO 26 -- Track Power On/Off
#define CHOSEN_ADDITIONAL_BUTTON_4_FUNCTION E_STOP_CURRENT_LOCO   // GPIO 27 --- Stops Current Loco
#define CHOSEN_ADDITIONAL_BUTTON_5_FUNCTION FUNCTION_8  // GPIO 32 --- changed this to turn the prime mover (the sound loco) on/off
#define CHOSEN_ADDITIONAL_BUTTON_6_FUNCTION SPEED_MULTIPLIER   // GPIO 33 --- 3 Accelleration Delays

// *******************************************************************************************************************
// additional / optional commands
// these can be any legitimate WiThrottle protocol command encosed in quotes or double quotes
// refer to https://www.jmri.org/help/en/package/jmri/jmrit/withrottle/Protocol.shtml

// alert message
// #define CUSTOM_COMMAND_1 "HMHello World"
// set route R500  (DCCEX system route perfix is 'R')
// #define CUSTOM_COMMAND_2 "PRA2R500"
// #define CUSTOM_COMMAND_3 ""
// #define CUSTOM_COMMAND_4 ""
// #define CUSTOM_COMMAND_5 ""
// #define CUSTOM_COMMAND_6 ""
// #define CUSTOM_COMMAND_7 ""

// ********************************************************************************************

// uncomment and/or correct one (only) of the #define lines below if you need to override the default .9 inch oLED display settings 
//
// Please select a constructor line for below depending on your display
// U8g2 Constructor List (Frame Buffer)
// The complete list is available here: https://github.com/olikraus/u8g2/wiki/u8g2setupcpp
// Please update the pin numbers according to your setup. Use U8X8_PIN_NONE if the reset pin is not connected

// this is one of the common .9 inch OLED displays and is included by default
// #define OLED_TYPE U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE, /* clock=*/ 22, /* data=*/ 23);

// Sumner Note: This is one of the common 1.3 inch OLED displays and the one that is used in the HandCab. 
// Don't change the following define if you use the stock case and recommended OLED.
 #define OLED_TYPE U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE, /* clock=*/ 22, /* data=*/ 23);

// *******************************************************************************************************************
// Debugging

// DccExController console debug messages are ENABLED by default
// to disable to console messages, uncomment and set the following to  1    (i.e. 0 = on  1 = off)
// #define DCCEXCONTROLLER_DEBUG    0

// DCCEXProtocol console debug messages are DISABLED by default
// to enable to console messages, uncomment and set the following to  0    (i.e. 0 = on  1 = off)
// #define DCCEXPROTOCOL_DEBUG    1

// console debug messages are enabled with the defines above
// the default level is 1
// 0 = errors only 1 = default level 2 = verbose
// #define DEBUG_LEVEL    1

// *******************************************************************************************************************
// Default latching/momentary functions - Direct mapping only 

// uncomment any of these lines if you wish to change.  latching = true or momentary = false
// F1 & F1 default to momentary
//#define F0_LATCHING false
//#define F1_LATCHING true
//#define F2_LATCHING true

// *******************************************************************************************************************
// Default function labels

// uncomment any of these lines if you wish to change the labels
#define F0_LABEL "Light"
#define F1_LABEL "Bell"
#define F2_LABEL "Horn"

// *******************************************************************************************************************
// encoder hardware
// 
// uncomment and change as needed
// defaults are for the Small ESP32

// large EPS32
// #define ROTARY_ENCODER_A_PIN 33
// #define ROTARY_ENCODER_B_PIN 32
// #define ROTARY_ENCODER_BUTTON_PIN 27

// Small ESP32
// #define ROTARY_ENCODER_A_PIN 12
// #define ROTARY_ENCODER_B_PIN 14
// #define ROTARY_ENCODER_BUTTON_PIN 13

// #define ROTARY_ENCODER_VCC_PIN -1 /* 27 put -1 of Rotary encoder Vcc is connected directly to 3,3V; else you can use declared output pin for powering rotary encoder */
// #define ROTARY_ENCODER_STEPS 2 //depending on your encoder - try 1,2 or 4 to get expected behaviour

// *******************************************************************************************************************
// keypad hardware
//
// uncomment and change as needed
// defaults are for the Small 4x3 keypad

// 4x3 keypad - default
// #define ROW_NUM     4
// #define COLUMN_NUM  3
// #define KEYPAD_KEYS  {'1', '2', '3'}, {'4', '5', '6'}, {'7', '8', '9'}, {'*', '0', '#'}
// #define KEYPAD_ROW_PINS    {19, 18, 17, 16}
// #define KEYPAD_COLUMN_PINS { 4, 0, 2}

// 4x4 keypad
// #define ROW_NUM     4
// #define COLUMN_NUM  4
// #define KEYPAD_KEYS {'1', '2', '3', 'A'},  {'4', '5', '6', 'B'},  {'7', '8', '9', 'C'},  {'*', '0', '#', 'D'}
// #define KEYPAD_ROW_PINS    {19, 18, 17, 16}
// #define KEYPAD_COLUMN_PINS {4, 0, 2, 33}

// in miliseconds

// #define KEYPAD_DEBOUNCE_TIME 10
// #define KEYPAD_HOLD_TIME 200

// *******************************************************************************************************************
// Additional / optional buttons
// For the 4x3 Keypad 7 buttons can be used
// For the 4x4 keypad, only 6 buttons can be used with the 'normal' pins and the last pin MUST be set to -1, 
//                     or one of pins 34,35,36,39 can be used with additional hardware (see below)

// for the 4x3 keypad - default
// To use the additional buttons, adjust the functions assigned to them in config_buttons.h
// #define ADDITIONAL_BUTTONS_PINS      {5,15,25,26,27,32,33}
// #define ADDITIONAL_BUTTONS_TYPE      {INPUT_PULLUP,INPUT_PULLUP,INPUT_PULLUP,INPUT_PULLUP,INPUT_PULLUP,INPUT_PULLUP,INPUT_PULLUP}

// for the 4x4 keypad
// To use the additional buttons, adjust the functions assigned to them in config_buttons.h
// #define ADDITIONAL_BUTTONS_PINS      {5,15,25,26,27,32,-1}  // last pin must be set to -1
// #define ADDITIONAL_BUTTONS_TYPE      {INPUT_PULLUP,INPUT_PULLUP,INPUT_PULLUP,INPUT_PULLUP,INPUT_PULLUP,INPUT_PULLUP,INPUT_PULLUP}

// Note: pins 34,35,36,39 can be used but don't have an internal pullup, 
//       so need additional hardware (resister) if you wish to use one of them

// default = 50
// increase if you find the buttons bounce. i.e. activate twice on a single press

// #define ADDITIONAL_BUTTON_DEBOUNCE_DELAY        50
 #define ADDITIONAL_BUTTON_DEBOUNCE_DELAY 50   // increase if you find the buttons bounce. i.e. activate twice on a single press    

// *******************************************************************************************************************
// Throttle Pot definitions:
// 
 #define THROTTLE_POT_PIN 39
//
// Throttle Pot numbers are generated by the Read_Pot-Values program:

// EXAMPLE: #define THROTTLE_POT_NOTCH_VALUES {1111,1347,1591,1833,2105,2379,2622,2837} //Numbers to left are Sumner's.  Run the Read_Pot-Values program to generate yours.

 #define THROTTLE_POT_NOTCH_VALUES {0000,0000,0000,0000,0000,0000,0000,0000} // Enter these numbers from the numbers generated by Read_Pot_Values program you run first.

// *******************************************************************************************************************
// Speed Step definitions:
//
// These numbers are tied to the throttle notches and are defined by you.  They are the speed steps you want for eash of the 8 throttle notches.
// You can use any speed step (0-126) for each of the 8 notches but each one should be a higher speed step than the one before it.
// I speed speed match my speed steps to my throttle and scale speed.  Speed step 4 = 4 scale mph and so on up to speed step 65 is 65 mph.  You can use what works for you.

// EXAMPLE: #define THROTTLE_POT_NOTCH_SPEEDS {4,10,15,25,35,45,55,65}  // Numbers to left are Sumner's & after speed matching the loco to the throttle are 4 mph to 65 mph. 
// EXAMPLE: #define THROTTLE_POT_NOTCH_SPEEDS {9,18,36,54,72,90,108,126}  // Numbers to left are Peter Akers' to give the locos their full speed range

 #define THROTTLE_POT_NOTCH_SPEEDS {000,000,000,000,000,000,000,000}  // Use 0-126. These numbers will be the speed step for each of the 8 throttle notches.  Change them to what you want.

// note: The example values above for the THROTTLE_POT_NOTCH_VALUES pot and the other pots are using a 50k ohm pot.  Other value pots can be used,
//       but a lower value will deplete the battery quicker and higher might be inconsistent. Numbers are generated by Read_Pot_Values program by Sumner Patterson

// *******************************************************************************************************************
// Brake Pot definitions:
//
 #define BRAKE_POT_PIN 34
//
// Brake Pot numbers are generated by the Read_Pot-Values program:

// EXAMPLE: #define BRAKE_POT_VALUES  {841,1224,1602,1980,2365}  //Numbers to left are Sumner's.  Run the Read_Pot-Values program to generate yours.

 #define BRAKE_POT_VALUES  {00000,0000,0000,0000,0000}  //Run the Read_Pot-Values program and add your BRAKE_POT_VALUES to the left.
 
// *******************************************************************************************************************
// Reverser Pot definitions:
//
 #define REVERSER_POT_PIN 35
//
// Reverser Pot numbers are generated by the Read_Pot-Values program:

// EXAMPLE: #define REVERSER_POT_VALUES {981,2141}     //Numbers to left are Sumner's.  Run the Read_Pot-Values program to generate yours.

#define REVERSER_POT_VALUES {0000,0000}  //Run the Read_Pot-Values program and add your REVERSER_POT_VALUES to the left.

// *******************************************************************************************************************
// Acceleration definitions:
//
// Acceleration delays (momentum) are defined by you 'not' the Read_Pot-Values program.) 
//
// The numbers you input here will set up the acceleration delays (momentum). They are defined by you and your needs.
// On a smaller layout you might want the acceleration and delays (momentum) shorter and on a larger layout longer.  
//  
// EXAMPLE: #define ACCELLERATION_DELAY_TIMES {182,415,600} // Numbers to the left are Sumner's. Start with these then experiment some and use numbers that work for you.

 #define ACCELLERATION_DELAY_TIMES {182,415,600} // I'd start with my numbers and then experiment and change them to what you like.

// With the above numbers & my top speed step set at 65 the '182' results in 15 sec. for 0 to 65, '415' = 30 sec. 0 to 65 and '600' = 40 sec. 0 to 65.

// *******************************************************************************************************************
// Braking definitions:
//
// Brake delay (rates) are defined by you 'not' the Read_Pot-Values program.
//
// The numbers you input here will set up the braking delays (rates). They are defined by you and your needs.
// The first number below, '500', is the delay between speed steps when coasting down with the brake off.
// The next 5 numbers are the delay time (rate) between speed steps in the 5 braking notches when stepping down the speed steps. 
//
// EXAMPLE: #define BRAKE_DELAY_TIMES {500,200,125,50,25,0} // Numbers to the left are Sumner's.

 #define BRAKE_DELAY_TIMES {0000,0000,0000,0000,0000,0000}  //I'd start with my numbers and then experiment and change them to what you like.

// *******************************************************************************************************************
// Speed Step Setting  definition:
//
// The amount the DCC speed (number of speed steps) is changed by on each accelleration or brake delay period - default is 1

 #define DCC_SPEED_CHANGE_AMOUNT 1

// *******************************************************************************************************************
// Battery Test
// To use a battery test
// set USE_BATTERY_SLEEP_AT_PERCENT to a value less than zero e.g. -1) to disable the automatic sleep
// #define USE_BATTERY_TEST true
// #define BATTERY_TEST_PIN 36
// #define BATTERY_CONVERSION_FACTOR 1.7
// #define USE_BATTERY_PERCENT_AS_WELL_AS_ICON true
// #define USE_BATTERY_SLEEP_AT_PERCENT -1

// ********************************** End of config_buttons **********************************************************

