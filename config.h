#include "actions.h"

const char* deviceName = "WiT Controller";

const String turnoutPrefixes[maxSsids] = {"NT", "NT", "NT"};  // required if you wish to use turnouts  
// this the prefix of all turnout system names for YOUR, for three wiThrottle servers on the three networks above

const boolean encoderRotationClockwiseIsIncreaseSpeed = false;
// false = Counterclockwise  true = clockwise

const boolean toggleDirectionOnEncoderButtonPressWhenStationary = true;
// true = if the locos(s) are stationary, clicking the encoder button will toggle the direction

const String routePrefixes[maxSsids] = {"IO:AUTO:", "IO:AUTO:", "IO:AUTO:"};  // required if you wish to use routes  
// this is the prefix of all route system names for YOUR system, for three wiThrottle servers on the three networks above

// configure the keypad buttons to perform the actions you wish
// 4x4 keypad
// int buttonActions[14] = { SPEED_STOP,   // 0
//                          FUNCTION_0,    // 1 - lights
//                          FUNCTION_1,    // 2 - bell
//                          FUNCTION_2,    // 3 - horn
//                          FUNCTION_NULL, // 4
//                          SPEED_UP,      // 5
//                          FUNCTION_NULL, // 6
//                          DIRECTION_REVERSE, // 7
//                          SPEED_DOWN,    // 8
//                          DIRECTION_FORWARD  // 9
//                          FUNCTION_NULL, // A
//                          FUNCTION_NULL, // B
//                          FUNCTION_NULL, // C
//                          FUNCTION_NULL // D
// };

//4x3 keypad
int buttonActions[10] = { E_STOP,   // 0
                         FUNCTION_0,    // 1 - lights
                         FUNCTION_1,    // 2 - bell
                         FUNCTION_2,    // 3 - horn
                         FUNCTION_NULL, // 4
                         SPEED_UP,      // 5
                         FUNCTION_NULL, // 6
                         DIRECTION_REVERSE, // 7
                         SPEED_DOWN,    // 8
                         DIRECTION_FORWARD  // 9
};

const String directCommandText[4][3] = {
    {"1.F0", "2.F1", "3.F2"},
    {"4.", "5.V Up", "6."}, 
    {"7.Rev", "8.V Down", "9.Fwd"},
    {"*.Menu", "E Stop", "#.End Menu"}
};