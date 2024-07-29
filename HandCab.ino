/**
 * This app turns the ESP32 into a Bluetooth LE keyboard that is intended to act as a dedicated
 * gamepad for the JMRI or any wiThrottle server.

  Instructions:
  - Update WiFi SSIDs and passwords as necessary in config_network.h.
  - Flash the sketch 
 */

#include <WiFi.h>                 // https://github.com/espressif/arduino-esp32/tree/master/libraries/WiFi     GPL 2.1
#include <ESPmDNS.h>              // https://github.com/espressif/arduino-esp32/blob/master/libraries/ESPmDNS (You should be able to download it from here https://github.com/espressif/arduino-esp32 Then unzip it and copy 'just' the ESPmDNS folder to your own libraries folder )
#include <Preferences.h>
#include <WiThrottleProtocol.h>   // https://github.com/flash62au/WiThrottleProtocol                           Creative Commons 4.0  Attribution-ShareAlike
#include <AiEsp32RotaryEncoder.h> // https://github.com/igorantolic/ai-esp32-rotary-encoder                    GPL 2.0
#include <Keypad.h>               // https://www.arduinolibraries.info/libraries/keypad                        GPL 3.0
#include <U8g2lib.h>              // https://github.com/olikraus/u8g2  (Just get "U8g2" via the Arduino IDE Library Manager)   new-bsd
#include <string>

#include "Pangodream_18650_CL.h"  // https://github.com/pangodream/18650CL  

#include "config_network.h"       // LAN networks (SSIDs and passwords)
#include "config_buttons.h"       // keypad buttons assignments
#include "config_keypad_etc.h"    // hardware config - GPIOs - keypad, encoder; oled display type

#include "static.h"
#include "actions.h"

#include "HandCab.h"

#if WITCONTROLLER_DEBUG == 0
 #define debug_print(params...) Serial.print(params)
 #define debug_println(params...) Serial.print(params); Serial.print(" ("); Serial.print(millis()); Serial.println(")")
 #define debug_printf(params...) Serial.printf(params)
#else
 #define debug_print(...)
 #define debug_println(...)
 #define debug_printf(...)
#endif
int debugLevel = DEBUG_LEVEL;

// *********************************************************************************

Preferences nvsPrefs;
bool nvsInit = false;
bool nvsPrefsSaved = false;

// *********************************************************************************

int keypadUseType = KEYPAD_USE_OPERATION;
int encoderUseType = ENCODER_USE_OPERATION;
int encoderButtonAction = ENCODER_BUTTON_ACTION;

boolean menuCommandStarted = false;
String menuCommand = "";
boolean menuIsShowing = false;

String oledText[18] = {"", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""};
bool oledTextInvert[18] = {false, false, false, false, false, false, false, false, false, 
                           false, false, false, false, false, false, false, false, false};

int currentSpeed;
Direction currentDirection;
// int speedStepCurrentMultiplier = 1;

TrackPower trackPower = PowerUnknown;
String turnoutPrefix = "";
String routePrefix = "";

// encoder variables
bool circleValues = true;
int encoderValue = 0;
int lastEncoderValue = 0;

// throttle pot values
bool useRotaryEncoderForThrottle = false; // TODO: no longer needed - need to remove references to this
int throttlePotPin = THROTTLE_POT_PIN;
bool throttlePotUseNotches = THROTTLE_POT_USE_NOTCHES;
int throttlePotNotchValues[] = THROTTLE_POT_NOTCH_VALUES; 
int throttlePotNotchSpeeds[] = THROTTLE_POT_NOTCH_SPEEDS;
int throttlePotNotch = 0;
int throttlePotTargetSpeed = 0;
int lastThrottlePotValue = 0;
int lastThrottlePotHighValue = 0;  // highest of the most recent
int lastThrottlePotValues[] = {0, 0, 0, 0, 0};
int lastThrottlePotReadTime = -1;

// throttle recalibration values
int throttlePotTempValues[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
int throttlePotRecalibratedValues[] = THROTTLE_POT_NOTCH_VALUES; 
int lowestThrottlePotValue = 32768;
int highestThrottlePotValue = -1;

// reverser pot values
int reverserPotPin = REVERSER_POT_PIN;
int reverserPotValues[] = REVERSER_POT_VALUES; 
int lastReverserPotValue = 0;
int lastReverserPotHighValue = 0;  // highest of the most recent
int lastReverserPotValues[] = {0, 0, 0, 0, 0};
int reverserCurrentPosition = REVERSER_POSITION_NEUTRAL;
int lastReverserPotReadTime = -1;

// reverser recalibration values
int reverserPotRecalibratedValues[] = REVERSER_POT_VALUES; 
int lowestReverserPotValue = 32768;
int highestReverserPotValue = -1;

// brake pot values
int brakePotPin = BRAKE_POT_PIN;
int brakePotValues[] = BRAKE_POT_VALUES;
int lastBrakePotValue = 0;
int lastBrakePotHighValue = 0;  // highest of the most recent
int lastBrakePotValues[] = {0, 0, 0, 0, 0};
int brakeCurrentPosition = 0;
int lastBrakePotReadTime = -1;

// brake recalibration values
int brakePotRecalibratedValues[] = BRAKE_POT_VALUES;
int lowestBrakePotValue = 32768;
int highestBrakePotValue = -1;

//general pot values
bool potValuesInitiallyCleared = false;

// Momentum - acceleration and brake
int brakeDelayTimes[] = BRAKE_DELAY_TIMES;
int currentBrakeDelayTime = 0;
int accellerationDelayTimes[] = ACCELLERATION_DELAY_TIMES;
int currentAccellerationDelayTime = accellerationDelayTimes[0];
int currentAccellerationDelayTimeIndex = 0;
int targetSpeed = 0;
PotDirection targetDirection = FORWARD;
double startMomentumTimerMillis = -1;

// E Stop
bool eStopEngaged = false;

// battery test values
bool useBatteryTest = USE_BATTERY_TEST;
#if USE_BATTERY_TEST
  #if USE_BATTERY_PERCENT_AS_WELL_AS_ICON
    ShowBattery showBatteryTest = ICON_AND_PERCENT;
  #else 
    ShowBattery showBatteryTest = ICON_ONLY;
  #endif
#else
  ShowBattery showBatteryTest = NONE;
#endif
bool useBatteryPercentAsWellAsIcon = USE_BATTERY_PERCENT_AS_WELL_AS_ICON;
int lastBatteryTestValue = 0; 
double lastBatteryCheckTime = 0;
#if USE_BATTERY_TEST
 Pangodream_18650_CL BL(BATTERY_TEST_PIN,BATTERY_CONVERSION_FACTOR);
#endif   

// server variables
// boolean ssidConnected = false;
String selectedSsid = "";
String selectedSsidPassword = "";
int ssidConnectionState = CONNECTION_STATE_DISCONNECTED;

// ssid password entry
String ssidPasswordEntered = "";
boolean ssidPasswordChanged = true;
char ssidPasswordCurrentChar = ssidPasswordBlankChar; 

IPAddress selectedWitServerIP;
int selectedWitServerPort = 0;
String selectedWitServerName ="";
int noOfWitServices = 0;
int witConnectionState = CONNECTION_STATE_DISCONNECTED;
String serverType = "";

//found wiThrottle servers
IPAddress foundWitServersIPs[maxFoundWitServers];
int foundWitServersPorts[maxFoundWitServers];
String foundWitServersNames[maxFoundWitServers];
int foundWitServersCount = 0;
bool autoConnectToFirstDefinedServer = AUTO_CONNECT_TO_FIRST_DEFINED_SERVER;
bool autoConnectToFirstWiThrottleServer = AUTO_CONNECT_TO_FIRST_WITHROTTLE_SERVER;
int outboundCmdsMininumDelay = OUTBOUND_COMMANDS_MINIMUM_DELAY;
bool commandsNeedLeadingCrLf = false;

//found ssids
String foundSsids[maxFoundSsids];
long foundSsidRssis[maxFoundSsids];
boolean foundSsidsOpen[maxFoundSsids];
int foundSsidsCount = 0;
int ssidSelectionSource;
double startWaitForSelection;

// wit Server ip entry
String witServerIpAndPortConstructed = "###.###.###.###:#####";
String witServerIpAndPortEntered = DEFAULT_IP_AND_PORT;
boolean witServerIpAndPortChanged = true;

// roster variables
int rosterSize = 0;
int rosterIndex[maxRoster]; 
String rosterName[maxRoster]; 
int rosterAddress[maxRoster];
char rosterLength[maxRoster];

int page = 0;
int functionPage = 0;

// Broadcast msessage
String broadcastMessageText = "";
long broadcastMessageTime = 0;

// remember oLED state
int lastOledScreen = 0;
String lastOledStringParameter = "";
int lastOledIntParameter = 0;
boolean lastOledBooleanParameter = false;
TurnoutAction lastOledTurnoutParameter = TurnoutToggle;
int lastOledPotValuesState = 1;  // 1=Brake, 2=Reverser

// turnout variables
int turnoutListSize = 0;
int turnoutListIndex[maxTurnoutList]; 
String turnoutListSysName[maxTurnoutList]; 
String turnoutListUserName[maxTurnoutList];
int turnoutListState[maxTurnoutList];

// route variables
int routeListSize = 0;
int routeListIndex[maxRouteList]; 
String routeListSysName[maxRouteList]; 
String routeListUserName[maxRouteList];
int routeListState[maxRouteList];

// function states
boolean functionStates[MAX_FUNCTIONS];   // set to maximum possible

// function labels
String functionLabels[MAX_FUNCTIONS];   // set to maximum possible

// consist function follow
int functionFollow[MAX_FUNCTIONS];   // set to maximum possible

// speedstep
int currentSpeedStep;   // set to maximum possible 

// throttle
int currentThrottleIndex = 0;
char currentThrottleIndexChar = '0';

int heartBeatPeriod = 10; // default to 10 seconds
long lastServerResponseTime;  // seconds since start of Arduino
boolean heartbeatCheckEnabled = true;

// used to stop speed bounces
long lastSpeedSentTime = 0;
int lastSpeedSent = 0;
// int lastDirectionSent = -1;
int lastSpeedThrottleIndex = 0;

// don't alter the assignments here
// alter them in config_buttons.h

const char* deviceName = DEVICE_NAME;

static unsigned long rotaryEncoderButtonLastTimePressed = 0;
const int rotaryEncoderButtonEncoderDebounceTime = ROTARY_ENCODER_DEBOUNCE_TIME;   // in miliseconds

const boolean encoderRotationClockwiseIsIncreaseSpeed = ENCODER_ROTATION_CLOCKWISE_IS_INCREASE_SPEED;
// false = Counterclockwise  true = clockwise

//const boolean toggleDirectionOnEncoderButtonPressWhenStationary = TOGGLE_DIRECTION_ON_ENCODER_BUTTON_PRESSED_WHEN_STATIONAY;
// true = if the locos(s) are stationary, clicking the encoder button will toggle the direction

//4x3 keypad only uses 0-9
//4x4 uses all 14 
int buttonActions[14] = { CHOSEN_KEYPAD_0_FUNCTION,
                          CHOSEN_KEYPAD_1_FUNCTION,
                          CHOSEN_KEYPAD_2_FUNCTION,
                          CHOSEN_KEYPAD_3_FUNCTION,
                          CHOSEN_KEYPAD_4_FUNCTION,
                          CHOSEN_KEYPAD_5_FUNCTION,
                          CHOSEN_KEYPAD_6_FUNCTION,
                          CHOSEN_KEYPAD_7_FUNCTION,
                          CHOSEN_KEYPAD_8_FUNCTION,
                          CHOSEN_KEYPAD_9_FUNCTION,
                          CHOSEN_KEYPAD_A_FUNCTION,
                          CHOSEN_KEYPAD_B_FUNCTION,
                          CHOSEN_KEYPAD_C_FUNCTION,
                          CHOSEN_KEYPAD_D_FUNCTION
};

// text that will appear when you press #
const String directCommandText[4][3] = {
    {CHOSEN_KEYPAD_1_DISPLAY_NAME, CHOSEN_KEYPAD_2_DISPLAY_NAME, CHOSEN_KEYPAD_3_DISPLAY_NAME},
    {CHOSEN_KEYPAD_4_DISPLAY_NAME, CHOSEN_KEYPAD_5_DISPLAY_NAME, CHOSEN_KEYPAD_6_DISPLAY_NAME},
    {CHOSEN_KEYPAD_7_DISPLAY_NAME, CHOSEN_KEYPAD_8_DISPLAY_NAME, CHOSEN_KEYPAD_9_DISPLAY_NAME},
    {"* Menu", CHOSEN_KEYPAD_0_DISPLAY_NAME, "# This"}
};

bool oledDirectCommandsAreBeingDisplayed = false;
#ifdef HASH_SHOWS_FUNCTIONS_INSTEAD_OF_KEY_DEFS
  boolean hashShowsFunctionsInsteadOfKeyDefs = HASH_SHOWS_FUNCTIONS_INSTEAD_OF_KEY_DEFS;
#else
  boolean hashShowsFunctionsInsteadOfKeyDefs = false;
#endif

// in case the values are not defined in config_buttons.h
// DO NOT alter the values here 
#ifndef CHOSEN_ADDITIONAL_BUTTON_0_FUNCTION
  #define CHOSEN_ADDITIONAL_BUTTON_0_FUNCTION FUNCTION_NULL
#endif
#ifndef CHOSEN_ADDITIONAL_BUTTON_1_FUNCTION
  #define CHOSEN_ADDITIONAL_BUTTON_1_FUNCTION FUNCTION_NULL
#endif
#ifndef CHOSEN_ADDITIONAL_BUTTON_2_FUNCTION
  #define CHOSEN_ADDITIONAL_BUTTON_2_FUNCTION FUNCTION_NULL
#endif
#ifndef CHOSEN_ADDITIONAL_BUTTON_3_FUNCTION
  #define CHOSEN_ADDITIONAL_BUTTON_3_FUNCTION FUNCTION_NULL
#endif
#ifndef CHOSEN_ADDITIONAL_BUTTON_4_FUNCTION
  #define CHOSEN_ADDITIONAL_BUTTON_4_FUNCTION FUNCTION_NULL
#endif
#ifndef CHOSEN_ADDITIONAL_BUTTON_5_FUNCTION
  #define CHOSEN_ADDITIONAL_BUTTON_5_FUNCTION FUNCTION_NULL
#endif
#ifndef CHOSEN_ADDITIONAL_BUTTON_6_FUNCTION
  #define CHOSEN_ADDITIONAL_BUTTON_6_FUNCTION FUNCTION_NULL
#endif

//optional additional buttons
int additionalButtonActions[MAX_ADDITIONAL_BUTTONS] = {
                          CHOSEN_ADDITIONAL_BUTTON_0_FUNCTION,
                          CHOSEN_ADDITIONAL_BUTTON_1_FUNCTION,
                          CHOSEN_ADDITIONAL_BUTTON_2_FUNCTION,
                          CHOSEN_ADDITIONAL_BUTTON_3_FUNCTION,
                          CHOSEN_ADDITIONAL_BUTTON_4_FUNCTION,
                          CHOSEN_ADDITIONAL_BUTTON_5_FUNCTION,
                          CHOSEN_ADDITIONAL_BUTTON_6_FUNCTION
};
unsigned long lastAdditionalButtonDebounceTime[MAX_ADDITIONAL_BUTTONS] = {0,0,0,0,0,0,0};  // the last time the output pin was toggled
unsigned long additionalButtonDebounceDelay = ADDITIONAL_BUTTON_DEBOUNCE_DELAY;    // the debounce time
boolean additionalButtonRead[MAX_ADDITIONAL_BUTTONS];
boolean additionalButtonLastRead[MAX_ADDITIONAL_BUTTONS];

// *********************************************************************************

void displayUpdateFromWit() {
  debug_print("displayUpdateFromWit(): keyapdeUseType "); debug_print(keypadUseType); 
  debug_print(" menuIsShowing "); debug_print(menuIsShowing);
  debug_println("");
  if ( (keypadUseType==KEYPAD_USE_OPERATION) && (!menuIsShowing) ) {
    writeOledSpeed();
  }
}

// WiThrottleProtocol Delegate class
class MyDelegate : public WiThrottleProtocolDelegate {
  
  public:
    void heartbeatConfig(int seconds) { 
      debug_print("Received heartbeat. From: "); debug_print(heartBeatPeriod); 
      debug_print(" To: "); debug_println(seconds); 
      heartBeatPeriod = seconds;
    }
    void receivedVersion(String version) {    
      debug_printf("Received Version: %s\n",version.c_str()); 
    }
    void receivedServerDescription(String description) {
      debug_print("Received Description: "); debug_println(description);
      serverType = description.substring(0,description.indexOf(" "));
      debug_print("ServerType: "); debug_println(serverType);
      if (serverType.equals("DCC-EX")) {
      // if (description.substring(0,6).equals("DCC-EX")) {
        debug_println("resetting prefixes");
        turnoutPrefix = DCC_EX_TURNOUT_PREFIX;
        routePrefix = DCC_EX_ROUTE_PREFIX;
      }
    }
    void receivedMessage(String message) {
      debug_print("Broadcast Message: ");
      debug_println(message);
      if ( (!message.equals("Connected")) && (!message.equals("Connecting..")) ) {
        broadcastMessageText = String(message);
        broadcastMessageTime = millis();
        refreshOled();
      }
    }
    void receivedAlert(String message) {
      debug_print("Broadcast Alert: ");
      debug_println(message);
      if ( (!message.equals("Connected")) && (!message.equals("Connecting..")) ) {
        broadcastMessageText = String(message);
        broadcastMessageTime = millis();
        refreshOled();
      }
    }
    void receivedSpeedMultiThrottle(char multiThrottle, int speed) {             // Vnnn
      debug_print("Received Speed: ("); debug_print(millis()); debug_print(") throttle: "); debug_print(multiThrottle);  debug_print(" speed: "); debug_println(speed); 
      int multiThrottleIndex = getMultiThrottleIndex(multiThrottle);

      if (currentSpeed != speed) {
        
        // check for bounce. (intermediate speed sent back from the server, but is not up to date with the throttle)
        if ( (lastSpeedThrottleIndex!=multiThrottleIndex)
             || ((millis()-lastSpeedSentTime)>500)
        ) {
          currentSpeed = speed;
          displayUpdateFromWit();
        } else {
          debug_print("Received Speed: skipping response: ("); debug_print(millis()); debug_print(") speed: "); debug_println(speed);
        }
      }
    }
    void receivedDirectionMultiThrottle(char multiThrottle, Direction dir) {     // R{0,1}
      debug_print("Received Direction: "); debug_println(dir); 
      // int multiThrottleIndex = getMultiThrottleIndex(multiThrottle);

      if (currentDirection != dir) {
        currentDirection = dir;
        displayUpdateFromWit();
      }
    }
    void receivedFunctionStateMultiThrottle(char multiThrottle, uint8_t func, bool state) { 
      debug_print("Received Fn: "); debug_print(func); debug_print(" State: "); debug_println( (state) ? "True" : "False" );
      // int multiThrottleIndex = getMultiThrottleIndex(multiThrottle);

      if (functionStates[func] != state) {
        functionStates[func] = state;
        displayUpdateFromWit();
      }
    }
    void receivedRosterFunctionListMultiThrottle(char multiThrottle, String functions[MAX_FUNCTIONS]) { 
      debug_println("Received Fn List: "); 
      // int multiThrottleIndex = getMultiThrottleIndex(multiThrottle);

      for(int i = 0; i < MAX_FUNCTIONS; i++) {
        functionLabels[i] = functions[i];
        debug_print(" Function: "); debug_print(i); debug_print(" - "); debug_println( functions[i] );
      }
    }
    void receivedTrackPower(TrackPower state) { 
      debug_print("Received TrackPower: "); debug_println(state);
      if (trackPower != state) {
        trackPower = state;
        displayUpdateFromWit(); // dummy multithrottle
        refreshOled();
      }
    }
    void receivedRosterEntries(int size) {
      debug_print("Received Roster Entries. Size: "); debug_println(size);
      rosterSize = size;
    }
    void receivedRosterEntry(int index, String name, int address, char length) {
      debug_print("Received Roster Entry, index: "); debug_print(index); debug_println(" - " + name);
      if (index < maxRoster) {
        rosterIndex[index] = index; 
        rosterName[index] = name; 
        rosterAddress[index] = address;
        rosterLength[index] = length;
      }
    }
    void receivedTurnoutEntries(int size) {
      debug_print("Received Turnout Entries. Size: "); debug_println(size);
      turnoutListSize = size;
    }
    void receivedTurnoutEntry(int index, String sysName, String userName, int state) {
      if (index < maxTurnoutList) {
        turnoutListIndex[index] = index; 
        turnoutListSysName[index] = sysName; 
        turnoutListUserName[index] = userName;
        turnoutListState[index] = state;
      }
    }
    void receivedRouteEntries(int size) {
      debug_print("Received Route Entries. Size: "); debug_println(size);
      routeListSize = size;
    }
    void receivedRouteEntry(int index, String sysName, String userName, int state) {
      if (index < maxRouteList) {
        routeListIndex[index] = index; 
        routeListSysName[index] = sysName; 
        routeListUserName[index] = userName;
        routeListState[index] = state;
      }
    }
};

int getMultiThrottleIndex(char multiThrottle) {
    int mThrottle = multiThrottle - '0';
    if ((mThrottle >= 0) && (mThrottle<=5)) {
        return mThrottle;
    } else {
        return 0;
    }
}

char getMultiThrottleChar(int multiThrottleIndex) {
  return '0' + multiThrottleIndex;
}

WiFiClient client;
WiThrottleProtocol wiThrottleProtocol;
MyDelegate myDelegate;
int deviceId = random(1000,9999);

// *********************************************************************************
// wifi / SSID 
// *********************************************************************************

void ssidsLoop() {
  if (ssidConnectionState == CONNECTION_STATE_DISCONNECTED) {
    if (ssidSelectionSource == SSID_CONNECTION_SOURCE_LIST) {
      showListOfSsids(); 
    } else {
      browseSsids();
    }
  }
  
  if (ssidConnectionState == CONNECTION_STATE_PASSWORD_ENTRY) {
    enterSsidPassword();
  }

  if (ssidConnectionState == CONNECTION_STATE_SELECTED) {
    connectSsid();
  }
}

void browseSsids() { // show the found SSIDs
  debug_println("browseSsids()");

  double startTime = millis();
  double nowTime = startTime;

  debug_println("Browsing for ssids");
  clearOledArray(); 
  setAppnameForOled();
  oledText[2] = MSG_BROWSING_FOR_SSIDS;
  writeOledArray(false, false, true, true);

  WiFi.setScanMethod(WIFI_ALL_CHANNEL_SCAN);
  WiFi.setSortMethod(WIFI_CONNECT_AP_BY_SIGNAL);
  
  int tempTimer = millis();
  int numSsids = WiFi.scanNetworks();
  while ( (numSsids == -1)
    && ((nowTime-startTime) <= 10000) ) { // try for 10 seconds
    // delay(250);
    if (millis() > tempTimer + 250) {
      debug_print(".");
      tempTimer = millis();
    }
    nowTime = millis();
  }

  startWaitForSelection = millis();

  foundSsidsCount = 0;
  if (numSsids == -1) {
    debug_println("Couldn't get a wifi connection");

  } else {
    for (int thisSsid = 0; thisSsid < numSsids; thisSsid++) {
      /// remove duplicates (repeaters and mesh networks)
      boolean found = false;
      for (int i=0; i<foundSsidsCount && i<maxFoundSsids; i++) {
        if (WiFi.SSID(thisSsid) == foundSsids[i]) {
          found = true;
          break;
        }
      }
      if (!found) {
        foundSsids[foundSsidsCount] = WiFi.SSID(thisSsid);
        foundSsidRssis[foundSsidsCount] = WiFi.RSSI(thisSsid);
        foundSsidsOpen[foundSsidsCount] = (WiFi.encryptionType(thisSsid) == 7) ? true : false;
        foundSsidsCount++;
      }
    }
    for (int i=0; i<foundSsidsCount; i++) {
      debug_println(foundSsids[i]);      
    }

    clearOledArray(); oledText[10] = MSG_SSIDS_FOUND;

    writeOledFoundSSids("");

    // oledText[5] = menu_select_ssids_from_found;
    setMenuTextForOled(menu_select_ssids_from_found);
    writeOledArray(false, false);

    keypadUseType = KEYPAD_USE_SELECT_SSID_FROM_FOUND;
    ssidConnectionState = CONNECTION_STATE_SELECTION_REQUIRED;

    if ((foundSsidsCount>0) && (autoConnectToFirstDefinedServer)) {
      for (int i=0; i<foundSsidsCount; i++) { 
        if (foundSsids[i] == ssids[0]) {
          ssidConnectionState = CONNECTION_STATE_SELECTED;
          selectedSsid = foundSsids[i];
          getSsidPasswordAndWitIpForFound();
        }
      }
    }
  }
}

void selectSsidFromFound(int selection) {
  debug_print("selectSsid() "); debug_println(selection);

  if ((selection>=0) && (selection < maxFoundSsids)) {
    ssidConnectionState = CONNECTION_STATE_SELECTED;
    selectedSsid = foundSsids[selection];
    getSsidPasswordAndWitIpForFound();
  }
  if (selectedSsidPassword=="") {
    ssidConnectionState = CONNECTION_STATE_PASSWORD_ENTRY;
  }
}

void getSsidPasswordAndWitIpForFound() {
    selectedSsidPassword = "";
    turnoutPrefix = "";
    routePrefix = "";
    if ( (selectedSsid.substring(0,6) == "DCCEX_") && (selectedSsid.length()==12) ) {
      selectedSsidPassword = "PASS_" + selectedSsid.substring(6);
      witServerIpAndPortEntered = "19216800400102560";
      turnoutPrefix = DCC_EX_TURNOUT_PREFIX;
      routePrefix = DCC_EX_ROUTE_PREFIX;
    } else {
      for (int i = 0; i < maxSsids; ++i) {
        if (selectedSsid == ssids[i]) {
          selectedSsidPassword = passwords[i];
          turnoutPrefix = turnoutPrefixes[i];
          routePrefix = routePrefixes[i];
          break;
        }
      }
    }
}

void enterSsidPassword() {
  keypadUseType = KEYPAD_USE_ENTER_SSID_PASSWORD;
  encoderUseType = ENCODER_USE_SSID_PASSWORD;
  if (ssidPasswordChanged) { // don't refresh the screen if nothing nothing has changed
    debug_println("enterSsidPassword()");
    writeOledEnterPassword();
    ssidPasswordChanged = false;
  }
}
void showListOfSsids() {  // show the list from the specified values in config_network.h
  debug_println("showListOfSsids()");
  startWaitForSelection = millis();

  clearOledArray(); 
  setAppnameForOled(); 
  writeOledArray(false, false);

  if (maxSsids == 0) {
    oledText[1] = MSG_NO_SSIDS_FOUND;
    writeOledArray(false, false, true, true);
    debug_println(oledText[1]);
  
  } else {
    debug_print(maxSsids);  debug_println(MSG_SSIDS_LISTED);
    clearOledArray(); oledText[10] = MSG_SSIDS_LISTED;
    
    for (int i = 0; i < maxSsids; ++i) {
      debug_print(i+1); debug_print(": "); debug_println(ssids[i]);
      int j = i;
      if (i>=5) { 
        j=i+1;
      } 
      if (i<=10) {  // only have room for 10
        oledText[j] = String(i) + ": ";
        if (ssids[i].length()<9) {
          oledText[j] = oledText[j] + ssids[i];
        } else {
          oledText[j] = oledText[j] + ssids[i].substring(0,9) + "..";
        }
      }
    }

    if (maxSsids > 0) {
      // oledText[5] = menu_select_ssids;
      setMenuTextForOled(menu_select_ssids);
    }
    writeOledArray(false, false);

    if (maxSsids == 1) {
      selectedSsid = ssids[0];
      selectedSsidPassword = passwords[0];
      ssidConnectionState = CONNECTION_STATE_SELECTED;

      turnoutPrefix = turnoutPrefixes[0];
      routePrefix = routePrefixes[0];
      
    } else {
      ssidConnectionState = CONNECTION_STATE_SELECTION_REQUIRED;
      keypadUseType = KEYPAD_USE_SELECT_SSID;
    }
  }
}

void selectSsid(int selection) {
  debug_print("selectSsid() "); debug_println(selection);

  if ((selection>=0) && (selection < maxSsids)) {
    ssidConnectionState = CONNECTION_STATE_SELECTED;
    selectedSsid = ssids[selection];
    selectedSsidPassword = passwords[selection];
    
    turnoutPrefix = turnoutPrefixes[selection];
    routePrefix = routePrefixes[selection];
  }
}

void connectSsid() {
  debug_println("Connecting to ssid...");
  clearOledArray(); 
  setAppnameForOled();
  oledText[1] = selectedSsid; oledText[2] + "connecting...";
  writeOledArray(false, false, true, true);

  double startTime = millis();
  double nowTime = startTime;

  const char *cSsid = selectedSsid.c_str();
  const char *cPassword = selectedSsidPassword.c_str();

  if (selectedSsid.length()>0) {
    debug_print("Trying Network "); debug_println(cSsid);
    clearOledArray(); 
    setAppnameForOled(); 
    for (int i = 0; i < 3; ++i) {  // Try three times
      oledText[1] = selectedSsid; oledText[2] =  String(MSG_TRYING_TO_CONNECT) + " (" + String(i) + ")";
      writeOledArray(false, false, true, true);

      nowTime = startTime;      
      WiFi.begin(cSsid, cPassword); 

      int j = 0;
      int tempTimer = millis();
      debug_print("Trying Network ... Checking status "); debug_print(cSsid); debug_print(" :"); debug_print(cPassword); debug_println(":");
      while ( (WiFi.status() != WL_CONNECTED) 
            && ((nowTime-startTime) <= SSID_CONNECTION_TIMEOUT) ) { // wait for X seconds to see if the connection worked
        if (millis() > tempTimer + 250) {
          oledText[3] = getDots(j);
          writeOledArray(false, false, true, true);
          j++;
          debug_print(".");
          tempTimer = millis();
        }
        nowTime = millis();
      }

      if (WiFi.status() == WL_CONNECTED) {
        if (selectedSsid.indexOf(SSID_NAME_FOR_COMMANDS_NEED_LEADING_CR_LF)>=0) {  // default is "wftrx_"
          commandsNeedLeadingCrLf = true;
          debug_print(SSID_NAME_FOR_COMMANDS_NEED_LEADING_CR_LF); debug_println(" - Commands need to be sent twice");
        }

        break; 
      } else { // if not loop back and try again
        debug_println("");
      }
    }

    debug_println("");
    if (WiFi.status() == WL_CONNECTED) {
      debug_print("Connected. IP address: "); debug_println(WiFi.localIP());
      oledText[2] = MSG_CONNECTED; 
      oledText[3] = MSG_ADDRESS_LABEL + String(WiFi.localIP());
      writeOledArray(false, false, true, true);
      // ssidConnected = true;
      ssidConnectionState = CONNECTION_STATE_CONNECTED;
      keypadUseType = KEYPAD_USE_SELECT_WITHROTTLE_SERVER;

      // setup the bonjour listener
      if (!MDNS.begin("WiTcontroller")) {
        debug_println("Error setting up MDNS responder!");
        oledText[2] = MSG_BOUNJOUR_SETUP_FAILED;
        writeOledArray(false, false, true, true);
        delay(2000);
        ssidConnectionState = CONNECTION_STATE_DISCONNECTED;
      } else {
        debug_println("MDNS responder started");
      }

    } else {
      debug_println(MSG_CONNECTION_FAILED);
      oledText[2] = MSG_CONNECTION_FAILED;
      writeOledArray(false, false, true, true);
      delay(2000);
      
      WiFi.disconnect();      
      ssidConnectionState = CONNECTION_STATE_DISCONNECTED;
      ssidSelectionSource = SSID_CONNECTION_SOURCE_LIST;
    }
  }
}

// *********************************************************************************
// WiThrottle 
// *********************************************************************************

void witServiceLoop() {
  if (witConnectionState == CONNECTION_STATE_DISCONNECTED) {
    browseWitService(); 
  }

  if (witConnectionState == CONNECTION_STATE_ENTRY_REQUIRED) {
    enterWitServer();
  }

  if ( (witConnectionState == CONNECTION_STATE_SELECTED) 
  || (witConnectionState == CONNECTION_STATE_ENTERED) ) {
    connectWitServer();
  }
}

void browseWitService() {
  debug_println("browseWitService()");

  keypadUseType = KEYPAD_USE_SELECT_WITHROTTLE_SERVER;

  double startTime = millis();
  double nowTime = startTime;

  const char * service = "withrottle";
  const char * proto= "tcp";

  debug_printf("Browsing for service _%s._%s.local. on %s ... ", service, proto, selectedSsid.c_str());
  clearOledArray(); 
  oledText[0] = appName; oledText[6] = appVersion; 
  oledText[1] = selectedSsid;   oledText[2] = MSG_BROWSING_FOR_SERVICE;
  writeOledArray(false, false, true, true);

  noOfWitServices = 0;
  if ( (selectedSsid.substring(0,6) == "DCCEX_") && (selectedSsid.length()==12) ) {
    debug_println(MSG_BYPASS_WIT_SERVER_SEARCH);
    oledText[1] = MSG_BYPASS_WIT_SERVER_SEARCH;
    writeOledArray(false, false, true, true);
    delay(500);
  } else {
    int j = 0;
    while ( (noOfWitServices == 0) 
    && ((nowTime-startTime) <= 10000)) { // try for 10 seconds 
      noOfWitServices = MDNS.queryService(service, proto);
      oledText[3] = getDots(j);
      writeOledArray(false, false, true, true);
      j++;
      debug_print(".");
      nowTime = millis();
    }
    debug_println("");
  }

  foundWitServersCount = noOfWitServices;
  if (noOfWitServices > 0) {
    for (int i = 0; ((i < noOfWitServices) && (i<maxFoundWitServers)); ++i) {
      foundWitServersNames[i] = MDNS.hostname(i);
      // foundWitServersIPs[i] = MDNS.IP(i);
      foundWitServersIPs[i] = ESPMDNS_IP_ATTRIBUTE_NAME;
      foundWitServersPorts[i] = MDNS.port(i);
      if (MDNS.hasTxt(i,"jmri")) {
        String node = MDNS.txt(i,"node");
        node.toLowerCase();
        if (foundWitServersNames[i].equals(node)) {
          foundWitServersNames[i] = "JMRI  (v" + MDNS.txt(i,"jmri") + ")";
        }
      }
    }
  }
  if ( (selectedSsid.substring(0,6) == "DCCEX_") && (selectedSsid.length()==12) ) {
    foundWitServersIPs[foundWitServersCount].fromString("192.168.4.1");
    foundWitServersPorts[foundWitServersCount] = 2560;
    foundWitServersNames[foundWitServersCount] = MSG_GUESSED_EX_CS_WIT_SERVER;
    foundWitServersCount++;
  }

  if (foundWitServersCount == 0) {
    oledText[1] = MSG_NO_SERVICES_FOUND;
    writeOledArray(false, false, true, true);
    debug_println(oledText[1]);
    delay(1000);
    buildWitEntry();
    witConnectionState = CONNECTION_STATE_ENTRY_REQUIRED;
  
  } else {
    debug_print(noOfWitServices);  debug_println(MSG_SERVICES_FOUND);
    clearOledArray(); oledText[4] = MSG_SERVICES_FOUND;

    for (int i = 0; i < foundWitServersCount; ++i) {
      // Print details for each service found
      debug_print("  "); debug_print(i); debug_print(": '"); debug_print(foundWitServersNames[i]);
      debug_print("' ("); debug_print(foundWitServersIPs[i]); debug_print(":"); debug_print(foundWitServersPorts[i]); debug_println(")");
      if (i<5) {  // only have room for 5
        String truncatedIp = ".." + foundWitServersIPs[i].toString().substring(foundWitServersIPs[i].toString().lastIndexOf("."));
        oledText[i] = String(i) + ": " + truncatedIp + ":" + String(foundWitServersPorts[i]) + " " + foundWitServersNames[i];
      }
    }

    if (foundWitServersCount > 0) {
      // oledText[5] = menu_select_wit_service;
      setMenuTextForOled(menu_select_wit_service);
    }
    writeOledArray(false, false);

    if ( (foundWitServersCount == 1) && (autoConnectToFirstWiThrottleServer) ) {
      debug_println("WiT Selection - only 1");
      selectedWitServerIP = foundWitServersIPs[0];
      selectedWitServerPort = foundWitServersPorts[0];
      selectedWitServerName = foundWitServersNames[0];
      witConnectionState = CONNECTION_STATE_SELECTED;
    } else {
      debug_println("WiT Selection required");
      witConnectionState = CONNECTION_STATE_SELECTION_REQUIRED;
    }
  }
}

void selectWitServer(int selection) {
  debug_print("selectWitServer() "); debug_println(selection);

  if ((selection>=0) && (selection < foundWitServersCount)) {
    witConnectionState = CONNECTION_STATE_SELECTED;
    selectedWitServerIP = foundWitServersIPs[selection];
    selectedWitServerPort = foundWitServersPorts[selection];
    selectedWitServerName = foundWitServersNames[selection];
    keypadUseType = KEYPAD_USE_OPERATION;
  }
}

void connectWitServer() {
  // Pass the delegate instance to wiThrottleProtocol
  wiThrottleProtocol.setDelegate(&myDelegate);
#if WITHROTTLE_PROTOCOL_DEBUG == 0
  wiThrottleProtocol.setLogStream(&Serial);
  wiThrottleProtocol.setLogLevel(DEBUG_LEVEL);
#endif

  debug_println("Connecting to the server...");
  clearOledArray(); 
  setAppnameForOled(); 
  oledText[1] = selectedWitServerIP.toString() + " : " + String(selectedWitServerPort); 
  oledText[2] = selectedWitServerName; oledText[3] + MSG_CONNECTING;
  writeOledArray(false, false, true, true);

  if (!client.connect(selectedWitServerIP, selectedWitServerPort)) {
    debug_println(MSG_CONNECTION_FAILED);
    oledText[3] = MSG_CONNECTION_FAILED;
    writeOledArray(false, false, true, true);
    delay(5000);
    
    witConnectionState = CONNECTION_STATE_DISCONNECTED;
    ssidConnectionState = CONNECTION_STATE_DISCONNECTED;
    ssidSelectionSource = SSID_CONNECTION_SOURCE_LIST;
    witServerIpAndPortChanged = true;

  } else {
    debug_print("Connected to server: ");   debug_println(selectedWitServerIP); debug_println(selectedWitServerPort);

    // Pass the communication to WiThrottle. + Set the mimimum period between sent commands
    wiThrottleProtocol.connect(&client, outboundCmdsMininumDelay);
    debug_println("WiThrottle connected");

    wiThrottleProtocol.setDeviceName(deviceName);  
    wiThrottleProtocol.setDeviceID(String(deviceId));  
    wiThrottleProtocol.setCommandsNeedLeadingCrLf(commandsNeedLeadingCrLf);

    witConnectionState = CONNECTION_STATE_CONNECTED;
    setLastServerResponseTime(true);

    oledText[3] = MSG_CONNECTED;
    if (!hashShowsFunctionsInsteadOfKeyDefs) {
      // oledText[5] = menu_menu;
      setMenuTextForOled(menu_menu);
    } else {
      // oledText[5] = menu_menu_hash_is_functions;
      setMenuTextForOled(menu_menu_hash_is_functions);
    }
    writeOledArray(false, false, true, true);

    keypadUseType = KEYPAD_USE_OPERATION;
  }
}

void enterWitServer() {
  keypadUseType = KEYPAD_USE_ENTER_WITHROTTLE_SERVER;
  if (witServerIpAndPortChanged) { // don't refresh the screen if nothing nothing has changed
    debug_println("enterWitServer()");
    clearOledArray(); 
    setAppnameForOled(); 
    oledText[1] = MSG_NO_SERVICES_FOUND_ENTRY_REQUIRED;
    oledText[3] = witServerIpAndPortConstructed;
    // oledText[5] = menu_select_wit_entry;
    setMenuTextForOled(menu_select_wit_entry);
    writeOledArray(false, false, true, true);
    witServerIpAndPortChanged = false;
  }
}

void disconnectWitServer() {
  debug_println("disconnectWitServer()");
  releaseAllLocos();
  wiThrottleProtocol.disconnect();
  debug_println("Disconnected from wiThrottle server\n");
  clearOledArray(); oledText[0] = MSG_DISCONNECTED;
  writeOledArray(false, false, true, true);
  witConnectionState = CONNECTION_STATE_DISCONNECTED;
  witServerIpAndPortChanged = true;
}

void witEntryAddChar(char key) {
  if (witServerIpAndPortEntered.length() < 17) {
    witServerIpAndPortEntered = witServerIpAndPortEntered + key;
    debug_print("wit entered: ");
    debug_println(witServerIpAndPortEntered);
    buildWitEntry();
    witServerIpAndPortChanged = true;
  }
}

void witEntryDeleteChar(char key) {
  if (witServerIpAndPortEntered.length() > 0) {
    witServerIpAndPortEntered = witServerIpAndPortEntered.substring(0, witServerIpAndPortEntered.length()-1);
    debug_print("wit deleted: ");
    debug_println(witServerIpAndPortEntered);
    buildWitEntry();
    witServerIpAndPortChanged = true;
  }
}

void ssidPasswordAddChar(char key) {
  ssidPasswordEntered = ssidPasswordEntered + key;
  debug_print("password entered: ");
  debug_println(ssidPasswordEntered);
  ssidPasswordChanged = true;
  ssidPasswordCurrentChar = ssidPasswordBlankChar;
}

void ssidPasswordDeleteChar(char key) {
  if (ssidPasswordEntered.length() > 0) {
    ssidPasswordEntered = ssidPasswordEntered.substring(0, ssidPasswordEntered.length()-1);
    debug_print("password char deleted: ");
    debug_println(ssidPasswordEntered);
    ssidPasswordChanged = true;
    ssidPasswordCurrentChar = ssidPasswordBlankChar;
  }
}

void buildWitEntry() {
  debug_println("buildWitEntry()");
  witServerIpAndPortConstructed = "";
  for (int i=0; i < witServerIpAndPortEntered.length(); i++) {
    if ( (i==3) || (i==6) || (i==9) ) {
      witServerIpAndPortConstructed = witServerIpAndPortConstructed + ".";
    } else if (i==12) {
      witServerIpAndPortConstructed = witServerIpAndPortConstructed + ":";
    }
    witServerIpAndPortConstructed = witServerIpAndPortConstructed + witServerIpAndPortEntered.substring(i,i+1);
  }
  debug_print("wit Constructed: ");
  debug_println(witServerIpAndPortConstructed);
  if (witServerIpAndPortEntered.length() < 17) {
    witServerIpAndPortConstructed = witServerIpAndPortConstructed + witServerIpAndPortEntryMask.substring(witServerIpAndPortConstructed.length());
  }
  debug_print("wit Constructed: ");
  debug_println(witServerIpAndPortConstructed);

  if (witServerIpAndPortEntered.length() == 17) {
     selectedWitServerIP.fromString( witServerIpAndPortConstructed.substring(0,15));
     selectedWitServerPort = witServerIpAndPortConstructed.substring(16).toInt();
  }
}

// *********************************************************************************
//   Non Volitile Storage (flash memory)
// *********************************************************************************

void setupPreferences(bool forceClear) {
  
  nvsPrefs.begin("HandCabPrefs", true);        //  open it in readOnly mode.
  nvsInit = nvsPrefs.isKey("prefsSaved");    // Test for the existence of the "already initialized" key.
  if ( (nvsInit == false) || (forceClear) ) {
    debug_println("Initialising non-volitile storage ");
    // If nsvInit is 'false', the key "nvsInit" does not yet exist therefore this
    //  must be our first-time run.
    nvsPrefs.end();                             // close the namespace in RO mode and...

    nvsPrefs.begin("HandCabPrefs", false);       // reopen it in readWrite mode.
    nvsPrefs.putBool("nvsInit", true);          // remeber that we have initilaised it
    nvsPrefs.putBool("prefsSaved", false);      // but we have not store any values yet

    nvsInit = nvsPrefs.isKey("prefsSaved");
  } else { 
     if (nvsPrefs.getBool("prefsSaved")) {
        readPreferences();
     }
  }
  nvsPrefs.end();                             // Close the namespace
}

void readPreferences() {
  debug_println("Reading preferences from non-volitile storage ");
  nvsPrefs.begin("HandCabPrefs", true);        //  open it in readonly mode.
  if (nvsInit) {
    char key[3];
    key[2] = 0;

    key[0] = 'T';
    for (int i=0; i<8; i++) {
      key[1] = '0'+i;
      throttlePotNotchValues[i] = nvsPrefs.getInt(key);
      debug_print(key); debug_print(" - "); debug_println(throttlePotNotchValues[i]);
    }
    key[0] = 'R';
    for (int i=0; i<2; i++) {
      key[1] = '0'+i;
      reverserPotValues[i] = nvsPrefs.getInt(key);
      debug_print(key); debug_print(" - "); debug_println(reverserPotValues[i]);
    }
    key[0] = 'B';
    for (int i=0; i<5; i++) {
      key[1] = '0'+i;
      brakePotValues[i] = nvsPrefs.getInt(key);
      debug_print(key); debug_print(" - "); debug_println(brakePotValues[i]);
    }
  } else {
    debug_println("Non-volitile storage not initialised");
  }
   nvsPrefs.end();                             // Close the namespace
}

void writePreferences() {
  debug_println("Writing preferences to non-volitile storage ");
  nvsPrefs.begin("HandCabPrefs", false);        //  open it in readWrite mode.
  if (nvsInit) {
    char key[3];
    key[2] = 0;

    key[0] = 'T';
    for (int i=0; i<8; i++) {
      key[1] = '0'+i;
      nvsPrefs.putInt(key,throttlePotNotchValues[i]);
      debug_print(key); debug_print(" - "); debug_println(throttlePotNotchValues[i]);
    }
    key[0] = 'R';
    for (int i=0; i<2; i++) {
      key[1] = '0'+i;
      nvsPrefs.putInt(key,reverserPotValues[i]);
      debug_print(key); debug_print(" - "); debug_println(reverserPotValues[i]);
    }
    key[0] = 'B';
    for (int i=0; i<5; i++) {
      key[1] = '0'+i;
      nvsPrefs.putInt(key,brakePotValues[i]);
      debug_print(key); debug_print(" - "); debug_println(brakePotValues[i]);
    }
    nvsPrefs.putBool("prefsSaved", true); 
  } else {
    debug_println("Non-volitile storage not initialised");
  }
   nvsPrefs.end();                             // Close the namespace
}

void clearPreferences() {
  setupPreferences(true); // force a reset 
}

// *********************************************************************************
//   Rotary Encoder
// *********************************************************************************

AiEsp32RotaryEncoder rotaryEncoder = AiEsp32RotaryEncoder(ROTARY_ENCODER_A_PIN, ROTARY_ENCODER_B_PIN, ROTARY_ENCODER_BUTTON_PIN, ROTARY_ENCODER_VCC_PIN, ROTARY_ENCODER_STEPS);
void IRAM_ATTR readEncoderISR() {
  rotaryEncoder.readEncoder_ISR();
}

void rotary_onButtonClick() {
   if (encoderUseType == ENCODER_USE_OPERATION) {
    if ( (keypadUseType!=KEYPAD_USE_SELECT_WITHROTTLE_SERVER)
        && (keypadUseType!=KEYPAD_USE_ENTER_WITHROTTLE_SERVER)
        && (keypadUseType!=KEYPAD_USE_SELECT_SSID) 
        && (keypadUseType!=KEYPAD_USE_SELECT_SSID_FROM_FOUND) ) {

      if ( (millis() - rotaryEncoderButtonLastTimePressed) < rotaryEncoderButtonEncoderDebounceTime) {   //ignore multiple press in that specified time
        debug_println("encoder button debounce");
        return;
      }
      rotaryEncoderButtonLastTimePressed = millis();

      doDirectAction(encoderButtonAction);

      debug_println("encoder button pressed");
      writeOledSpeed();
    }  else {
      deepSleepStart();
    }
   } else {
    if (ssidPasswordCurrentChar!=ssidPasswordBlankChar) {
      ssidPasswordEntered = ssidPasswordEntered + ssidPasswordCurrentChar;
      ssidPasswordCurrentChar = ssidPasswordBlankChar;
      writeOledEnterPassword();
    }
   }
}

void rotary_loop() {
  if (rotaryEncoder.encoderChanged()) {   //don't print anything unless value changed
    
    encoderValue = rotaryEncoder.readEncoder();
    debug_print("Encoder From: "); debug_print(lastEncoderValue);  debug_print(" to: "); debug_println(encoderValue);

    if ( (millis() - rotaryEncoderButtonLastTimePressed) < rotaryEncoderButtonEncoderDebounceTime) {   //ignore the encoder change if the button was pressed recently
      debug_println("encoder button debounce - in Rotary_loop()");
      return;
    }

    if (abs(encoderValue-lastEncoderValue) > 800) { // must have passed through zero
      if (encoderValue > 800) {
        lastEncoderValue = 1001; 
      } else {
        lastEncoderValue = 0; 
      }
      debug_print("Corrected Encoder From: "); debug_print(lastEncoderValue); debug_print(" to: "); debug_println(encoderValue);
    }
 
    if (encoderUseType == ENCODER_USE_OPERATION) {
      if (wiThrottleProtocol.getNumberOfLocomotives(currentThrottleIndexChar)>0) {
        if (encoderValue > lastEncoderValue) {
          if (abs(encoderValue-lastEncoderValue)<50) {
            encoderSpeedChange(true, currentSpeedStep);
          } else {
            encoderSpeedChange(true, currentSpeedStep*speedStepMultiplier);
          }
        } else {
          if (abs(encoderValue-lastEncoderValue)<50) {
            encoderSpeedChange(false, currentSpeedStep);
          } else {
            encoderSpeedChange(false, currentSpeedStep*speedStepMultiplier);
          }
        } 
      }
    } else { // (encoderUseType == ENCODER_USE_SSID_PASSWORD) 
        if (encoderValue > lastEncoderValue) {
          if (ssidPasswordCurrentChar==ssidPasswordBlankChar) {
            ssidPasswordCurrentChar = 66; // 'B'
          } else {
            ssidPasswordCurrentChar = ssidPasswordCurrentChar - 1;
            if ((ssidPasswordCurrentChar < 32) ||(ssidPasswordCurrentChar > 126) ) {
              ssidPasswordCurrentChar = 126;  // '~'
            }
          }
        } else {
          if (ssidPasswordCurrentChar==ssidPasswordBlankChar) {
            ssidPasswordCurrentChar = 64; // '@'
          } else {
            ssidPasswordCurrentChar = ssidPasswordCurrentChar + 1;
            if (ssidPasswordCurrentChar > 126) {
              ssidPasswordCurrentChar = 32; // ' ' space
            }
          }
        }
        ssidPasswordChanged = true;
        writeOledEnterPassword();
    }
    lastEncoderValue = encoderValue;
  }
  
  if (rotaryEncoder.isEncoderButtonClicked()) {
    rotary_onButtonClick();
  }
}

void encoderSpeedChange(bool rotationIsClockwise, int speedChange) {
  if (encoderRotationClockwiseIsIncreaseSpeed) {
    if (rotationIsClockwise) {
      speedUp(speedChange);
    } else {
      speedDown(speedChange);
    }
  } else {
    if (rotationIsClockwise) {
      speedDown(speedChange);
    } else {
      speedUp(speedChange);
    }
  }
}

// *********************************************************************************
//   Throttle Pot
// *********************************************************************************

void throttlePot_loop(bool forceRead) {
  if ( (millis() < lastThrottlePotReadTime + 100) 
    && (!forceRead) ) { // only ready it one every x seconds
    return;
  }
  lastThrottlePotReadTime = millis();

  // Read the throttle pot to see what notch it is on.
  int currentThrottlePotNotch = throttlePotNotch;
  int potValue = ( analogRead(throttlePotPin) );  //Reads the analog value on the throttle pin.
  potValue = ( analogRead(throttlePotPin) );  //Reads the analog value on the throttle pin.

  // average out the last x values from the pot
  int noElements = sizeof(lastThrottlePotValues) / sizeof(lastThrottlePotValues[0]);
  int avgPotValue = 0;
  for (int i=1; i<noElements; i++) {
    lastThrottlePotValues[i-1] = lastThrottlePotValues[i];
    avgPotValue = avgPotValue + lastThrottlePotValues[i-1];
  }
  lastThrottlePotValues[noElements-1] = potValue;
  avgPotValue = (avgPotValue + potValue) / noElements;

  // get the highest recent value
  lastThrottlePotHighValue = -1;
  for (int i=0; i<noElements; i++) {
    if (lastThrottlePotValues[i] > lastThrottlePotHighValue) 
    lastThrottlePotHighValue = lastThrottlePotValues[i];
  }

  // save the lowest and higest pot values seen
  if (avgPotValue<lowestThrottlePotValue) lowestThrottlePotValue = avgPotValue;
  if (avgPotValue>highestThrottlePotValue) highestThrottlePotValue = avgPotValue;

  // only do something if the pot value is sufficiently different
  // or deliberate read 
  if ( (avgPotValue<lastThrottlePotValue-5) || (avgPotValue>lastThrottlePotValue+5)
  || (forceRead) )  { 
    lastThrottlePotValue = avgPotValue;
    noElements = sizeof(throttlePotNotchValues) / sizeof(throttlePotNotchValues[0]);

    throttlePotNotch = -99;
    for (int i=0; i<noElements; i++) {
      if (avgPotValue < throttlePotNotchValues[i]) {    /// Check to see if it is in notch i
        if (i==0) { //notch 1 is always speed zero
          throttlePotTargetSpeed = 0;
        } else {// use the speed values element 1 less than the notch number
          throttlePotTargetSpeed = throttlePotNotchSpeeds[i-1];
          if (debugLevel > 1) { debug_print("throttlePot_loop() notch: "); debug_print(i); debug_print(" - "); debug_println(avgPotValue); }
        } 
        throttlePotNotch = i;
        break;
      }                
    } 
    if (throttlePotNotch == -99) { //didn't find it, so it must be above the last element in the pot values
      throttlePotNotch = noElements;
      throttlePotTargetSpeed = throttlePotNotchSpeeds[noElements-1];
      if (debugLevel > 1) { debug_print("throttlePot_loop() above max: setting notch: "); debug_print(throttlePotNotch); debug_print(" - ");  debug_println(avgPotValue); }
    }
    if(debugLevel > 1) { debug_print("throttlePot_loop() current notch: "); debug_print(currentThrottlePotNotch); debug_print(" new: "); debug_println(throttlePotNotch); }
    if ( (throttlePotNotch!=currentThrottlePotNotch) 
    || (forceRead) ) {
         if(DEBUG_LEVEL>0) { debug_print("throttlePot_loop() request changing speed to: ");   debug_println(throttlePotTargetSpeed); }
          targetSpeed = throttlePotTargetSpeed;
          targetSpeedAndDirectionOverride();
          if (!forceRead) startMomentumTimerMillis = millis(); // don't reset the timer on a forced read
    }
    refreshOled();
  }
}

// *********************************************************************************
//   Reverser Pot
// *********************************************************************************

void reverserPot_loop() {
  if (millis() < lastReverserPotReadTime + 100) {
    return;
  }
  lastReverserPotReadTime = millis();

  int lastReverserPosition = reverserCurrentPosition;
  int potValue = ( analogRead(reverserPotPin) );  //Reads the analog value on the reverser pin.
  potValue = ( analogRead(reverserPotPin) );  //Reads the analog value on the reverser pin.

  // average out the last x values from the pot
  int noElements = sizeof(lastReverserPotValues) / sizeof(lastReverserPotValues[0]);
  int avgPotValue = 0;
  for (int i=1; i<noElements; i++) {
    lastReverserPotValues[i-1] = lastReverserPotValues[i];
    avgPotValue = avgPotValue + lastReverserPotValues[i-1];
  }
  lastReverserPotValues[noElements-1] = potValue;
  avgPotValue = (avgPotValue + potValue) / noElements;

  // get the highest recent value
  lastReverserPotHighValue = -1;
  for (int i=0; i<noElements; i++) {
    if (lastReverserPotValues[i] > lastReverserPotHighValue) 
    lastReverserPotHighValue = lastReverserPotValues[i];
  }

  // save the lowest and higest pot values seen
  if (avgPotValue<lowestReverserPotValue) lowestReverserPotValue = avgPotValue;
  if (avgPotValue>highestReverserPotValue) highestReverserPotValue = avgPotValue;

  // only do something if the pot value is different
  if ( (avgPotValue<lastReverserPotValue-5) || (avgPotValue>lastReverserPotValue+5) ) { 
    // debug_print("Reverser Pot Value: "); debug_println(potValue);
    lastReverserPotValue = avgPotValue;

    if (lastReverserPotValue < reverserPotValues[0]) {
      reverserCurrentPosition = REVERSER_POSITION_REVERSE;
    } else if (lastReverserPotValue < reverserPotValues[1]) {
      reverserCurrentPosition = REVERSER_POSITION_NEUTRAL;
    } else {
      reverserCurrentPosition = REVERSER_POSITION_FORWARD;
    }

    if (lastReverserPosition != reverserCurrentPosition) {
      if (reverserCurrentPosition==REVERSER_POSITION_FORWARD) { 
        debug_println("Reverser - Forward");
        // changeDirection(Forward);
        targetDirection = FORWARD;
      } else if (reverserCurrentPosition==REVERSER_POSITION_REVERSE) {
        debug_println("Reverser - Reverse");
        // changeDirection(Reverse);
        targetDirection = REVERSE;
      } else {
        debug_println("Reverser - Neutral");
        targetDirection = NEUTRAL;
      }
      throttlePot_loop(true);  // recheck the throttle position
      targetSpeedAndDirectionOverride();
    }
  }
  refreshOled();
}

// *********************************************************************************
//   Brake Pot
// *********************************************************************************

void brakePot_loop() {
  if (millis() < lastBrakePotReadTime + 100) {
    return;
  }
  lastBrakePotReadTime = millis();
  int potValue = ( analogRead(brakePotPin) );  //Reads the analog value on the brake pin.
  potValue = ( analogRead(brakePotPin) );  //Reads the analog value on the brake pin.

  // average out the last x values from the pot
  int noElements = sizeof(lastBrakePotValues) / sizeof(lastBrakePotValues[0]);
  int avgPotValue = 0;
  for (int i=1; i<noElements; i++) {
    lastBrakePotValues[i-1] = lastBrakePotValues[i];
    avgPotValue = avgPotValue + lastBrakePotValues[i-1];
  }
  lastBrakePotValues[noElements-1] = potValue;
  avgPotValue = (avgPotValue + potValue) / noElements;

  // get the highest recent value
  lastBrakePotHighValue = -1;
  for (int i=0; i<noElements; i++) {
    if (lastBrakePotValues[i] > lastBrakePotHighValue) 
    lastBrakePotHighValue = lastBrakePotValues[i];
  }

  // save the lowest and highest pot values seen
  if (avgPotValue<lowestBrakePotValue) lowestBrakePotValue = avgPotValue;
  if (avgPotValue>highestBrakePotValue) highestBrakePotValue = avgPotValue;

  // only do something if the pot value is different
  if ( (avgPotValue<lastBrakePotValue-5) || (avgPotValue>lastBrakePotValue+5) ) { 
    // debug_print("Brake Pot Value: "); debug_println(potValue);
    lastBrakePotValue = avgPotValue;

    noElements = sizeof(brakePotValues) / sizeof(brakePotValues[0]);
    currentBrakeDelayTime = -1;
    brakeCurrentPosition = -1;
    for (int i=0; i<noElements; i++) {
      if (avgPotValue < brakePotValues[i]) {    /// Check to see if it is < value in i
        brakeCurrentPosition = i;
        break;
      }
    }
    if (brakeCurrentPosition == -1)  { // didn't find it the list 
      brakeCurrentPosition = noElements;  // use the last value (brakeDelayTimes has n-one more element than brakePotValues)
    }

    currentBrakeDelayTime = brakeDelayTimes[brakeCurrentPosition];
    throttlePot_loop(true);  // recheck the throttle position
    targetSpeedAndDirectionOverride();

    refreshOled();
  }
}


// *********************************************************************************
//  Speed loop - adjust speeds (momentum)
// *********************************************************************************

void speedAdjust_loop() {
  if (wiThrottleProtocol.getNumberOfLocomotives(getMultiThrottleChar(0)) > 0) {
    int changeAmount = 0;
    if (currentSpeed!=targetSpeed) {
      if (debugLevel>1) {
        debug_print("speedAdjust_loop() requested change: target: "); debug_print(targetSpeed);
        debug_print(" current: "); debug_println(currentSpeed);
      }
      if (currentSpeed>targetSpeed) {  // need to brake
        if (millis() - startMomentumTimerMillis >= currentBrakeDelayTime) {   // Check to see if the delay period has elasped.
          changeAmount = -1 * DCC_SPEED_CHANGE_AMOUNT;
          if (currentSpeed-changeAmount < targetSpeed) changeAmount = targetSpeed-currentSpeed;  // only relevant if the spped change is greater that 1
        }
      }
      else { // need to accelerate
        if (millis() - startMomentumTimerMillis >= currentAccellerationDelayTime) {
          startMomentumTimerMillis = millis();
          changeAmount = 1 * DCC_SPEED_CHANGE_AMOUNT;
          if (currentSpeed+changeAmount > targetSpeed) changeAmount = currentSpeed-targetSpeed;  // only relevant if the spped change is greater that 1
        }
      }
      if (changeAmount!=0) {  
        if (debugLevel>0) {
          debug_print("speedAdjust_loop() actually changing speed: target: "); debug_print(targetSpeed);
          debug_print(" current: "); debug_println(currentSpeed);
        }
        startMomentumTimerMillis = millis(); //restart timer
        speedSet(currentSpeed + changeAmount);
      }
    }
  } else { // at target speed
    startMomentumTimerMillis = -1; // use -1 to indicate were are not currently using it
  }
}

// * * * * * * * * * * * * * * * * * * * * * * * *

//Take the pot values and adjust/override the target speed.
void targetSpeedAndDirectionOverride() {

  // check the brake
  if (brakeCurrentPosition>0) { // ignore throttle and the reverser
    targetSpeed = 0;
  }

  if (eStopEngaged) {
    targetSpeed = 0;
  }

  // check the reverser
  if (reverserCurrentPosition==REVERSER_POSITION_FORWARD) {
    if (currentSpeed==0) {
      changeDirection(Forward);
      return;
    }

  } else if (reverserCurrentPosition==REVERSER_POSITION_NEUTRAL) {
    targetSpeed = 0;
    return;
    
  } else { // REVERSER_POSITION_REVERSE
    if (currentSpeed==0) {
      changeDirection(Reverse);
      return;
    }

  }
}

// *********************************************************************************
//   keypad
// *********************************************************************************

void keypadEvent(KeypadEvent key) {
  switch (keypad.getState()){
  case PRESSED:
    debug_print("Button "); debug_print(String(key - '0')); debug_println(" pushed.");
    doKeyPress(key, true);
    break;
  case RELEASED:
    doKeyPress(key, false);
    debug_print("Button "); debug_print(String(key - '0')); debug_println(" released.");
    break;
  case HOLD:
    debug_print("Button "); debug_print(String(key - '0')); debug_println(" hold.");
    break;
  case IDLE:
    debug_print("Button "); debug_print(String(key - '0')); debug_println(" idle.");
    break;
  default:
    debug_print("Button "); debug_print(String(key - '0')); debug_println(" unknown.");
  }
}


// *********************************************************************************
//   Optional Additional Buttons
// *********************************************************************************

void initialiseAdditionalButtons() {
  for (int i = 0; i < MAX_ADDITIONAL_BUTTONS; i++) { 
    if (additionalButtonActions[i] != FUNCTION_NULL) { 
      debug_print("Additional Button: "); debug_print(i); debug_print(" pin:"); debug_println(additionalButtonPin[i]);
      if (additionalButtonPin[i]>=0) {
        pinMode(additionalButtonPin[i], additionalButtonType[i]);
        if (additionalButtonType[i] == INPUT_PULLUP) {
          additionalButtonLastRead[i] = HIGH;
        } else {
          additionalButtonLastRead[i] = LOW;
        }
      }
      lastAdditionalButtonDebounceTime[i] = 0;
    }
  }
}

void additionalButtonLoop() {
  int buttonRead;
  for (int i = 0; i < MAX_ADDITIONAL_BUTTONS; i++) {   
    if ( (additionalButtonActions[i] != FUNCTION_NULL) && (additionalButtonPin[i]>=0) ) {
        buttonRead = digitalRead(additionalButtonPin[i]);

      if (additionalButtonLastRead[i] != buttonRead) { // on procces on a change
        if ((millis() - lastAdditionalButtonDebounceTime[i]) > additionalButtonDebounceDelay) {   // only process if there is sufficent delay since the last read
          lastAdditionalButtonDebounceTime[i] = millis();
          additionalButtonRead[i] = buttonRead;

          if ( ((additionalButtonType[i] == INPUT_PULLUP) && (additionalButtonRead[i] == LOW)) 
              || ((additionalButtonType[i] == INPUT) && (additionalButtonRead[i] == HIGH)) ) {
            debug_print("Additional Button Pressed: "); debug_print(i); debug_print(" pin:"); debug_print(additionalButtonPin[i]); debug_print(" action:"); debug_println(additionalButtonActions[i]); 
            if (wiThrottleProtocol.getNumberOfLocomotives(currentThrottleIndexChar) > 0) { // only process if there are locos aquired
              doDirectAdditionalButtonCommand(i,true);
            } else { // check for actions not releted to a loco
              int buttonAction = additionalButtonActions[i];
              if (buttonAction >= 500) {
                  doDirectAdditionalButtonCommand(i,true);
              }
            }
          } else {
            if (debugLevel > 1) { debug_print("Additional Button Released: "); debug_print(i); debug_print(" pin:"); debug_print(additionalButtonPin[i]); debug_print(" action:"); debug_println(additionalButtonActions[i]); }
            if (wiThrottleProtocol.getNumberOfLocomotives(currentThrottleIndexChar) > 0) { // only process if there are locos aquired
              doDirectAdditionalButtonCommand(i,false);
            } else { // check for actions not releted to a loco
              int buttonAction = additionalButtonActions[i];
              if (buttonAction >= 500) {
                  doDirectAdditionalButtonCommand(i,false);
              }
            }
          }
        } else {
          debug_println("Ignoring Additional Button Press");
        }
      }
      additionalButtonLastRead[i] = additionalButtonRead[i];
    }
  }
}

// *********************************************************************************
//   Battery Test
// *********************************************************************************

void batteryTest_loop() {
  // Read the battery pin
#if USE_BATTERY_TEST
  if(millis()-lastBatteryCheckTime>10000) {
    lastBatteryCheckTime = millis();
    // debug_print("battery pin: "); debug_print(BATTERY_TEST_PIN);
    // debug_print("  battery pin Value: "); debug_println(analogRead(batteryTestPin));  //Reads the analog value on the throttle pin.
    int batteryTestValue = BL.getBatteryChargeLevel();
    
    // debug_print("batteryTestValue: "); debug_println(batteryTestValue); 

    if (batteryTestValue!=lastBatteryTestValue) { 
      lastBatteryTestValue = BL.getBatteryChargeLevel();
      if ( (keypadUseType==KEYPAD_USE_OPERATION) && (!menuIsShowing)) {
        writeOledSpeed();
      }
    }
    if ( (lastBatteryTestValue<USE_BATTERY_SLEEP_AT_PERCENT)  // shutdown if <X% battery
    && (USE_BATTERY_SLEEP_AT_PERCENT > 0) ) {
      deepSleepStart(SLEEP_REASON_BATTERY);
    }
  }
#endif
}

// *********************************************************************************
//  Setup
// *********************************************************************************

void setup() {
  Serial.begin(115200);
  u8g2.begin();
  // i2cSetClock(0,400000);

  clearOledArray(); oledText[0] = appName; oledText[6] = appVersion; oledText[2] = MSG_START;
  writeOledArray(false, false, true, true);

  delay(1000);
  debug_println("Start"); 

  rotaryEncoder.begin();  //initialize rotary encoder
  rotaryEncoder.setup(readEncoderISR);
  //set boundaries and if values should cycle or not 
  rotaryEncoder.setBoundaries(0, 1000, circleValues); //minValue, maxValue, circleValues true|false (when max go to min and vice versa)
  //rotaryEncoder.disableAcceleration(); //acceleration is now enabled by default - disable if you don't need it
  rotaryEncoder.setAcceleration(100); //or set the value - larger number = more acceleration; 0 or 1 means disabled acceleration

  keypad.addEventListener(keypadEvent); // Add an event listener for this keypad
  keypad.setDebounceTime(KEYPAD_DEBOUNCE_TIME);
  keypad.setHoldTime(KEYPAD_HOLD_TIME);

  esp_sleep_enable_ext0_wakeup(GPIO_NUM_13,0); //1 = High, 0 = Low

  keypadUseType = KEYPAD_USE_SELECT_SSID;
  encoderUseType = ENCODER_USE_OPERATION;
  ssidSelectionSource = SSID_CONNECTION_SOURCE_BROWSE;

  initialiseAdditionalButtons();

  resetAllFunctionLabels();
  resetAllFunctionFollow();

  currentSpeed = 0;
  currentDirection = Forward;
  currentSpeedStep = speedStep;

  clearLastPotValues();
  setupPreferences(false);
}

// *********************************************************************************
//  Loop
// *********************************************************************************

void loop() {
  
  if (ssidConnectionState != CONNECTION_STATE_CONNECTED) {
    // connectNetwork();
    ssidsLoop();
    checkForShutdownOnNoResponse();
  } else {  
    if (witConnectionState != CONNECTION_STATE_CONNECTED) {
      witServiceLoop();
    } else {
      wiThrottleProtocol.check();    // parse incoming messages

      setLastServerResponseTime(false);

      if ( (lastServerResponseTime+(heartBeatPeriod*4) < millis()/1000) 
      && (heartbeatCheckEnabled) ) {
        debug_print("Disconnected - Last:");  debug_print(lastServerResponseTime); debug_print(" Current:");  debug_println(millis()/1000);
        reconnect();
      }
    }
  }
  // char key = keypad.getKey();
  keypad.getKey(); 
  if (witConnectionState != CONNECTION_STATE_CONNECTED) {  // only look at the rotary encoder for the WiFi and WiT 
    rotary_loop();
  }
  if (witConnectionState == CONNECTION_STATE_CONNECTED) {
    throttlePot_loop(false);
    reverserPot_loop(); 
    brakePot_loop(); 
    speedAdjust_loop();
  }

  additionalButtonLoop(); 
  
  if (useBatteryTest) { batteryTest_loop(); }

	// debug_println("loop: end" );
}

// *********************************************************************************
//  Key press and Menu
// *********************************************************************************

void doKeyPress(char key, bool pressed) {
  debug_print("doKeyPress(): "); 

  if (pressed)  { //pressed
    switch (keypadUseType) {
      case KEYPAD_USE_OPERATION:
        debug_print("key operation... "); debug_println(key);
        switch (key) {
          case '*':  // menu command
            menuCommand = "";
            if (menuCommandStarted) { // then cancel the menu
              resetMenu();
              writeOledSpeed();
            } else {
              menuCommandStarted = true;
              debug_println("Command started");
              writeOledMenu("");
            }
            break;

          case '#': // end of command
            debug_print("end of command... "); debug_print(key); debug_print ("  :  "); debug_println(menuCommand);
            if ((menuCommandStarted) && (menuCommand.length()>=1)) {
              doMenu();
            } else {
              if (!hashShowsFunctionsInsteadOfKeyDefs) {
                if (!oledDirectCommandsAreBeingDisplayed) {
                  writeOledDirectCommands();
                } else {
                  oledDirectCommandsAreBeingDisplayed = false;
                  writeOledSpeed();
                }
              } else {
                writeOledFunctionList(""); 
              }
            }
            break;

          case '0': case '1': case '2': case '3': case '4': 
          case '5': case '6': case '7': case '8': case '9':
            debug_print("number... "); debug_print(key); debug_print ("  cmd: '"); debug_print(menuCommand); debug_println("'");

            if (menuCommandStarted) { // append to the string

              if ((menuCharsRequired[key-48] == 0) && (menuCommand.length() == 0)) { // menu type is effectively a direct commands from this point
                menuCommand += key;
                doMenu();
              } else {
                if ((menuCharsRequired[menuCommand.substring(0,1).toInt()] == 1) && (menuCommand.length() == 1)) {  // menu type needs only one char
                  menuCommand += key;
                  doMenu();

                } else {  //menu type allows/requires more than one char
                  menuCommand += key;
                  writeOledMenu(menuCommand);
                }
              }
            } else {
              doDirectCommand(key, true);
            }
            break;

          default:  // A, B, C, D
            doDirectCommand(key, true);
            break;
        }
        break;

      case KEYPAD_USE_ENTER_WITHROTTLE_SERVER:
        debug_print("key: Enter wit... "); debug_println(key);
        switch (key){
          case '0': case '1': case '2': case '3': case '4': 
          case '5': case '6': case '7': case '8': case '9':
            witEntryAddChar(key);
            break;
          case '*': // backspace
            witEntryDeleteChar(key);
            break;
          case '#': // end of command
            if (witServerIpAndPortEntered.length() == 17) {
              witConnectionState = CONNECTION_STATE_ENTERED;
            }
            break;
          default:  // do nothing 
            break;
        }

        break;

      case KEYPAD_USE_ENTER_SSID_PASSWORD:
        debug_print("key: Enter password... "); debug_println(key);
        switch (key){
          case '0': case '1': case '2': case '3': case '4': 
          case '5': case '6': case '7': case '8': case '9':
            ssidPasswordAddChar(key);
            break;
          case '*': // backspace
            ssidPasswordDeleteChar(key);
            break;
          case '#': // end of command
              selectedSsidPassword = ssidPasswordEntered;
              encoderUseType = ENCODER_USE_OPERATION;
              keypadUseType = KEYPAD_USE_OPERATION;
              ssidConnectionState = CONNECTION_STATE_SELECTED;
            break;
          default:  // do nothing 
            break;
        }

        break;

      case KEYPAD_USE_SELECT_WITHROTTLE_SERVER:
        debug_print("key: Select wit... "); debug_println(key);
        switch (key){
          case '0': case '1': case '2': case '3': case '4':
            selectWitServer(key - '0');
            break;
          case '#': // show ip address entry screen
            witConnectionState = CONNECTION_STATE_ENTRY_REQUIRED;
            buildWitEntry();
            enterWitServer();
            break;
          default:  // do nothing 
            break;
        }
        break;

      case KEYPAD_USE_SELECT_SSID:
        debug_print("key ssid... "); debug_println(key);
        switch (key){
          case '0': case '1': case '2': case '3': case '4': 
          case '5': case '6': case '7': case '8': case '9':
            selectSsid(key - '0');
            break;
          case '#': // show found SSIds
            ssidConnectionState = CONNECTION_STATE_DISCONNECTED;
            keypadUseType = KEYPAD_USE_SELECT_SSID_FROM_FOUND;
            ssidSelectionSource = SSID_CONNECTION_SOURCE_BROWSE;
            // browseSsids();
            break;
          default:  // do nothing 
            break;
        }
        break;

      case KEYPAD_USE_SELECT_SSID_FROM_FOUND:
        debug_print("key ssid from found... "); debug_println(key);
        switch (key){
          case '0': case '1': case '2': case '3': case '4': 
            selectSsidFromFound(key - '0'+(page*5));
            break;
          case '#': // next page
            if (foundSsidsCount>5) {
              if ( (page+1)*5 < foundSsidsCount ) {
                page++;
              } else {
                page = 0;
              }
              writeOledFoundSSids(""); 
            }
            break;
          case '9': // show in code list of SSIDs
            ssidConnectionState = CONNECTION_STATE_DISCONNECTED;
            keypadUseType = KEYPAD_USE_SELECT_SSID;
            ssidSelectionSource = SSID_CONNECTION_SOURCE_LIST;
            break;
          default:  // do nothing 
            break;
        }
        break;

      case KEYPAD_USE_SELECT_ROSTER:
        debug_print("key Roster... "); debug_println(key);
        switch (key){
          case '0': case '1': case '2': case '3': case '4': 
          case '5': case '6': case '7': case '8': case '9':
            selectRoster((key - '0')+(page*5));
            break;
          case '#':  // next page
            if ( rosterSize > 5 ) {
              if ( (page+1)*5 < rosterSize ) {
                page++;
              } else {
                page = 0;
              }
              writeOledRoster(""); 
            }
            break;
          case '*':  // cancel
            resetMenu();
            writeOledSpeed();
            break;
          default:  // do nothing 
            break;
        }
        break;

      case KEYPAD_USE_SELECT_TURNOUTS_THROW:
      case KEYPAD_USE_SELECT_TURNOUTS_CLOSE:
        debug_print("key turnouts... "); debug_println(key);
        switch (key){
          case '0': case '1': case '2': case '3': case '4': 
          case '5': case '6': case '7': case '8': case '9':
            selectTurnoutList((key - '0')+(page*10), (keypadUseType == KEYPAD_USE_SELECT_TURNOUTS_THROW) ? TurnoutThrow : TurnoutClose);
            break;
          case '#':  // next page
            if ( turnoutListSize > 10 ) {
              if ( (page+1)*10 < turnoutListSize ) {
                page++;
              } else {
                page = 0;
              }
              writeOledTurnoutList("", (keypadUseType == KEYPAD_USE_SELECT_TURNOUTS_THROW) ? TurnoutThrow : TurnoutClose); 
            }
            break;
          case '*':  // cancel
            resetMenu();
            writeOledSpeed();
            break;
          default:  // do nothing 
            break;
        }
        break;

      case KEYPAD_USE_SELECT_ROUTES:
        debug_print("key routes... "); debug_println(key);
        switch (key){
          case '0': case '1': case '2': case '3': case '4': 
          case '5': case '6': case '7': case '8': case '9':
            selectRouteList((key - '0')+(page*10));
            break;
          case '#':  // next page
            if ( routeListSize > 10 ) {
              if ( (page+1)*10 < routeListSize ) {
                page++;
              } else {
                page = 0;
              }
              writeOledRouteList(""); 
            }
            break;
          case '*':  // cancel
            resetMenu();
            writeOledSpeed();
            break;
          default:  // do nothing 
            break;
        }
        break;

      case KEYPAD_USE_SELECT_FUNCTION:
        debug_print("key function... "); debug_println(key);
        switch (key){
          case '0': case '1': case '2': case '3': case '4': 
          case '5': case '6': case '7': case '8': case '9':
            selectFunctionList((key - '0')+(functionPage*10));
            break;
          case '#':  // next page
            if ( (functionPage+1)*10 < MAX_FUNCTIONS ) {
              functionPage++;
              writeOledFunctionList(""); 
            } else {
              functionPage = 0;
              keypadUseType = KEYPAD_USE_OPERATION;
              writeOledDirectCommands();
            }
            break;
          case '*':  // cancel
            resetMenu();
            writeOledSpeed();
            break;
          default:  // do nothing 
            break;
        }
        break;

      case KEYPAD_USE_EDIT_CONSIST:
        debug_print("key Edit Consist... "); debug_println(key);
        switch (key){
          case '0': case '1': case '2': case '3': case '4': 
          case '5': case '6': case '7': case '8': case '9':
            if ( (key-'0') <= wiThrottleProtocol.getNumberOfLocomotives(currentThrottleIndexChar)) {
              selectEditConsistList(key - '0');
            }
            break;
          case '*':  // cancel
            resetMenu();
            writeOledSpeed();
            break;
          default:  // do nothing 
            break;
        }
        break;


      case KEYPAD_USE_POT_VALUES:
        debug_print("Showing Pot values... "); debug_println(key);
        switch (key){
          case '0': case '4': 
          case '5': case '6': case '7':
            // do nothing
            break;
          case '1': 
            lastOledPotValuesState = 1;  // brake
            break;
          case '2': 
            lastOledPotValuesState = 2;  // reverser
            break;
          case '8':
            writePreferences();
            break;
          case '9': 
            recalibatePotValues();
            break;
          case '*':  // cancel
            resetMenu();
            writeOledSpeed();
            break;
          case '#':  // clear last recorded values and the saved preferences
            clearLastPotValues();
            clearPreferences();
            break;
          default:  // do nothing 
            break;
        }
        break;


      case KEYPAD_USE_THROTTLE_POT_VALUES:
        debug_print("Showing Throttle Pot values... "); debug_println(key);
        switch (key){
          case '0': case '1': case '2': case '3': case '4': 
          case '5': case '6': case '7': case '8':
            throttlePotTempValues[(key - '0')] = lastThrottlePotHighValue;
            break;
          case '9': 
            recalibateThrottlePotValues();
            break;
          case '*':  // cancel
            resetMenu();
            writeOledSpeed();
            break;
          case '#':  // next page
            clearLastPotValues();
            break;
          default:  // do nothing 
            break;
        }
        break;


      default:  // do nothing 
        break;
    }

  } else {  // released
    if (keypadUseType == KEYPAD_USE_OPERATION) {
      if ( (!menuCommandStarted) && (key>='0') && (key<='D')) { // only process releases for the numeric keys + A,B,C,D and only if a menu command has not be started
        debug_println("Operation - Process key release");
        doDirectCommand(key, false);
      } else {
        debug_println("Non-Operation - Process key release");
      }
    }
  }

  // debug_println("doKeyPress(): end"); 
}

void doDirectCommand (char key, bool pressed) {
  debug_print("doDirectCommand(): "); debug_println(key);
  int buttonAction = 0 ;
  if (key<=57) {
    buttonAction = buttonActions[(key - '0')];
  } else {
    buttonAction = buttonActions[(key - 55)]; // A, B, C, D
  }
  debug_print("doDirectCommand(): Action: "); debug_println(buttonAction);
  if (buttonAction!=FUNCTION_NULL) {
    if ( (buttonAction>=FUNCTION_0) && (buttonAction<=FUNCTION_31) ) {
      doDirectFunction(buttonAction, pressed);
  } else {
      if (pressed) { // only process these on the key press, not the release
        doDirectAction(buttonAction);
      }
    }
  }
  // debug_println("doDirectCommand(): end"); 
}

void doDirectAdditionalButtonCommand (int buttonIndex, bool pressed) {
  debug_print("doDirectAdditionalButtonCommand(): "); debug_println(buttonIndex);
  int buttonAction = additionalButtonActions[buttonIndex];
  if (buttonAction!=FUNCTION_NULL) {
    if ( (buttonAction>=FUNCTION_0) && (buttonAction<=FUNCTION_31) ) {
      doDirectFunction(buttonAction, pressed);
  } else {
      if (pressed) { // only process these on the key press, not the release
        doDirectAction(buttonAction);
      }
    }
  }
  // debug_println("doDirectAdditionalButtonCommand(): end ");
}

void doDirectAction(int buttonAction) {
  debug_println("doDirectAction(): ");
  switch (buttonAction) {
      // case DIRECTION_FORWARD: {
      //   changeDirection(Forward);
      //   break; 
      // }
      // case DIRECTION_REVERSE: {
      //   changeDirection(Reverse);
      //   break; 
      // }
      // case DIRECTION_TOGGLE: {
      //   toggleDirection();
      //   break; 
      // }
      // case SPEED_STOP: {
      //   speedSet(0);
      //   break; 
      // }
      // case SPEED_UP: {
      //   speedUp(currentSpeedStep);
      //   break; 
      // }
      // case SPEED_DOWN: {
      //   speedDown(currentSpeedStep);
      //   break; 
      // }
      // case SPEED_UP_FAST: {
      //   speedUp(currentSpeedStep*speedStepMultiplier);
      //   break; 
      // }
      // case SPEED_DOWN_FAST: {
      //   speedUp(currentSpeedStep*speedStepMultiplier);
      //   break; 
      // }
      case SPEED_MULTIPLIER: {
        toggleAccelerationDelayTime();
        break; 
      }
      case E_STOP: {
        if (!eStopEngaged) {
          speedEstop();
          eStopEngaged = true;
        } else {
          eStopEngaged = false;
          throttlePot_loop(true);
        }
        break; 
      }
      case E_STOP_CURRENT_LOCO: {
        if (!eStopEngaged) {
          speedEstopCurrentLoco();
          eStopEngaged = true;
        } else {
          eStopEngaged = false;
          throttlePot_loop(true);
        }
        break; 
      }
      case POWER_ON: {
        powerOnOff(PowerOn);
        break; 
      }
      case POWER_OFF: {
        powerOnOff(PowerOff);
        break; 
      }
      case POWER_TOGGLE: {
        powerToggle();
        break; 
      }
      case SHOW_HIDE_BATTERY: {
        batteryShowToggle();
        break; 
      }
      // case NEXT_THROTTLE: {
      //   nextThrottle();
      //   break; 
      // }
      // case SPEED_STOP_THEN_TOGGLE_DIRECTION: {
      //   stopThenToggleDirection();
      //   break; 
      // }
      case CUSTOM_1: {
        wiThrottleProtocol.sendCommand(CUSTOM_COMMAND_1);
        break; 
      }
      case CUSTOM_2: {
        wiThrottleProtocol.sendCommand(CUSTOM_COMMAND_2);
        break; 
      }
      case CUSTOM_3: {
        wiThrottleProtocol.sendCommand(CUSTOM_COMMAND_3);
        break; 
      }
      case CUSTOM_4: {
        wiThrottleProtocol.sendCommand(CUSTOM_COMMAND_4);
        break; 
      }
      case CUSTOM_5: {
        wiThrottleProtocol.sendCommand(CUSTOM_COMMAND_5);
        break; 
      }
      case CUSTOM_6: {
        wiThrottleProtocol.sendCommand(CUSTOM_COMMAND_6);
        break; 
      }
      case CUSTOM_7: {
        wiThrottleProtocol.sendCommand(CUSTOM_COMMAND_7);
        break; 
      }
  }
  // debug_println("doDirectAction(): end");
}

void doMenu() {
  String loco = "";
  String function = "";
  boolean result = false;
  // int index;
  debug_print("Menu: "); debug_println(menuCommand);
  
  switch (menuCommand[0]) {
    case MENU_ITEM_ADD_LOCO: { // select loco
        if (menuCommand.length()>1) {
          loco = menuCommand.substring(1, menuCommand.length());
          loco = getLocoWithLength(loco);
          debug_print("add Loco: "); debug_println(loco);
          wiThrottleProtocol.addLocomotive('0', loco);
          wiThrottleProtocol.getDirection('0', loco);
          wiThrottleProtocol.getSpeed();
          resetFunctionStates();
          writeOledSpeed();
        } else {
          page = 0;
          writeOledRoster("");
        }
        break;
      }
    case MENU_ITEM_DROP_LOCO: { // de-select loco
        loco = menuCommand.substring(1, menuCommand.length());
        if (loco!="") { // a loco is specified
          loco = getLocoWithLength(loco);
          releaseOneLoco(loco);
        } else { //not loco specified so release all
          releaseAllLocos();
        }
        writeOledSpeed();
        break;
      }
    // case MENU_ITEM_TOGGLE_DIRECTION: { // change direction
    //     toggleDirection();
    //     break;
    //   }
     case MENU_ITEM_SPEED_STEP_MULTIPLIER: { // toggle speed step additional Multiplier
        // toggleAdditionalMultiplier();
        toggleAccelerationDelayTime();
        break;
      }
   case MENU_ITEM_THROW_POINT: {  // throw point
        if (menuCommand.length()>1) {
          String turnout = turnoutPrefix + menuCommand.substring(1, menuCommand.length());
          // if (!turnout.equals("")) { // a turnout is specified
            debug_print("throw point: "); debug_println(turnout);
            wiThrottleProtocol.setTurnout(turnout, TurnoutThrow);
          // }
          writeOledSpeed();
        } else {
          page = 0;
          writeOledTurnoutList("", TurnoutThrow);
        }
        break;
      }
    case MENU_ITEM_CLOSE_POINT: {  // close point
        if (menuCommand.length()>1) {
          String turnout = turnoutPrefix + menuCommand.substring(1, menuCommand.length());
          // if (!turnout.equals("")) { // a turnout is specified
            debug_print("close point: "); debug_println(turnout);
            wiThrottleProtocol.setTurnout(turnout, TurnoutClose);
          // }
          writeOledSpeed();
        } else {
          page = 0;
          writeOledTurnoutList("",TurnoutClose);
        }
        break;
      }
    case MENU_ITEM_ROUTE: {  // route
        if (menuCommand.length()>1) {
          String route = routePrefix + menuCommand.substring(1, menuCommand.length());
          // if (!route.equals("")) { // a loco is specified
            debug_print("route: "); debug_println(route);
            wiThrottleProtocol.setRoute(route);
          // }
          writeOledSpeed();
        } else {
          page = 0;
          writeOledRouteList("");
        }
        break;
      }
    case MENU_ITEM_TRACK_POWER: {
        powerToggle();
        break;
      }
    case MENU_ITEM_EXTRAS: { // Extra menu - e.g. disconnect/reconnect/sleep
        char subCommand = menuCommand.charAt(1);
        if (menuCommand.length() > 1) {
          switch (subCommand) {
            case EXTRA_MENU_CHAR_FUNCTION_KEY_TOGGLE: { // toggle showing Def Keys vs Function labels
                hashShowsFunctionsInsteadOfKeyDefs = !hashShowsFunctionsInsteadOfKeyDefs;
                writeOledSpeed();
                break;
              } 
            case EXTRA_MENU_CHAR_EDIT_CONSIST: { // edit consist - loco facings
                writeOledEditConsist();
                break;
              } 
            case EXTRA_MENU_CHAR_POT_VALUES: { // show the potentiometer values
                if(!potValuesInitiallyCleared) { // only automatically clear on first use of this pot screens
                  potValuesInitiallyCleared = true;
                  clearLastPotValues();
                }
                writeOledPotValues();
                break;
              } 
            case EXTRA_MENU_CHAR_THROTTLE_POT_VALUES: { // show the throttle potentiometer values
                if(!potValuesInitiallyCleared) { // only automatically clear on first use of this pot screens
                  potValuesInitiallyCleared = true;
                  clearLastPotValues();
                }
                writeOledThrottlePotValues();
                break;
              } 
            case EXTRA_MENU_CHAR_HEARTBEAT_TOGGLE: { // disable/enable the heartbeat Check
                toggleHeartbeatCheck();
                writeOledSpeed();
                break;
              }
            case EXTRA_MENU_CHAR_DISCONNECT: { // disconnect   
                if (witConnectionState == CONNECTION_STATE_CONNECTED) {
                  witConnectionState = CONNECTION_STATE_DISCONNECTED;
                  disconnectWitServer();
                } else {
                  connectWitServer();
                }
                break;
              }
            case EXTRA_MENU_CHAR_OFF_SLEEP:
            case EXTRA_MENU_CHAR_OFF_SLEEP_HIDDEN: { // sleep/off
                deepSleepStart();
                break;
              }
          }
        } else {
          writeOledSpeed();
        }
        break;
      }
    case MENU_ITEM_FUNCTION: { // function button
        if (menuCommand.length()>1) {
          function = menuCommand.substring(1, menuCommand.length());
          int functionNumber = function.toInt();
          if (function != "") { // a function is specified
            doFunction(functionNumber, true);  // always act like latching i.e. pressed
          }
          writeOledSpeed();
        } else {
          functionPage = 0;
          writeOledFunctionList("");
        }
        break;
      }
  }
  menuCommandStarted = result; 
}

// *********************************************************************************
//  Actions
// *********************************************************************************

void resetMenu() {
  debug_println("resetMenu()");
  page = 0;
  menuCommand = "";
  menuCommandStarted = false;
  if ( (keypadUseType != KEYPAD_USE_SELECT_SSID) 
    && (keypadUseType != KEYPAD_USE_SELECT_WITHROTTLE_SERVER) ) {
    keypadUseType = KEYPAD_USE_OPERATION; 
  }
}

void resetFunctionStates() {
  for (int i=0; i<MAX_FUNCTIONS; i++) {
    functionStates[i] = false;
  }
}

void resetFunctionLabels() {
  debug_println("resetFunctionLabels(): ");
  for (int i=0; i<MAX_FUNCTIONS; i++) {
    functionLabels[i] = "";
  }
  functionPage = 0;
}

void resetAllFunctionLabels() {
  resetFunctionLabels();
}

void resetAllFunctionFollow() {
  functionFollow[0] = CONSIST_FUNCTION_FOLLOW_F0;
  functionFollow[1] = CONSIST_FUNCTION_FOLLOW_F1;
  functionFollow[2] = CONSIST_FUNCTION_FOLLOW_F2;
  for (int j=3; j<MAX_FUNCTIONS; j++) {
    functionFollow[j] = CONSIST_FUNCTION_FOLLOW_OTHER_FUNCTIONS;
  }
}

String getLocoWithLength(String loco) {
  int locoNo = loco.toInt();
  String locoWithLength = "";
  if ( (locoNo > SHORT_DCC_ADDRESS_LIMIT) 
  || ( (locoNo <= SHORT_DCC_ADDRESS_LIMIT) && (loco.charAt(0)=='0') && (!serverType.equals("DCC-EX" ) ) ) 
  ) {
    locoWithLength = "L" + loco;
  } else {
    locoWithLength = "S" + loco;
  }
  return locoWithLength;
}

void speedEstop() {
  eStopEngaged = true;
  currentSpeed = 0;
  targetSpeed = 0;
  wiThrottleProtocol.emergencyStop(getMultiThrottleChar(0));
  debug_println("Speed EStop"); 
  writeOledSpeed();
}

void speedEstopCurrentLoco() {
  eStopEngaged = true;
  targetSpeed = 0;
  currentSpeed = 0;
  wiThrottleProtocol.emergencyStop(getMultiThrottleChar(0));
  debug_println("Speed EStop Curent Loco"); 
  writeOledSpeed();
}

void speedDown(int amt) {
  if (wiThrottleProtocol.getNumberOfLocomotives(getMultiThrottleChar(0)) > 0) {
    int newSpeed = currentSpeed - amt;
    debug_print("Speed Down: "); debug_println(amt);
    speedSet(newSpeed);
  }
}

void speedUp(int amt) {
  if (wiThrottleProtocol.getNumberOfLocomotives(getMultiThrottleChar(0)) > 0) {
    int newSpeed = currentSpeed + amt;
    debug_print("Speed Up: "); debug_println(amt);
    speedSet(newSpeed);
  }
}

void speedSet(int amt) {
  debug_println("setSpeed()");
  if (wiThrottleProtocol.getNumberOfLocomotives('0') > 0) {
    int newSpeed = amt;
    if (newSpeed >126) { newSpeed = 126; }
    if (newSpeed <0) { newSpeed = 0; }
    wiThrottleProtocol.setSpeed('0', newSpeed);
    currentSpeed = newSpeed;
    debug_print("Speed Set: "); debug_println(newSpeed);

    // used to avoid bounce
    lastSpeedSentTime = millis();
    lastSpeedSent = newSpeed;
    // lastDirectionSent = -1;
    lastSpeedThrottleIndex = 0;

    // writeOledSpeed();
    refreshOled();
  }
}

int getDisplaySpeed() {
  if (speedDisplayAsPercent) {
    float speed = currentSpeed;
    speed = speed / 126 *100;
    int iSpeed = speed;
    if (iSpeed-speed >= 0.5) {
      iSpeed = iSpeed + 1;
    }
    return iSpeed;
  } else {
    if (speedDisplayAs0to28) {
      float speed = currentSpeed;
      speed = speed / 126 *28;
      int iSpeed = speed;
      if (iSpeed-speed >= 0.5) {
        iSpeed = iSpeed + 1;
      }
      return iSpeed;
    } else {
      return currentSpeed;
    }
  }
}

void toggleLocoFacing(String loco) {
  debug_print("toggleLocoFacing(): "); debug_println(loco); 
  for(int i=0;i<wiThrottleProtocol.getNumberOfLocomotives('0');i++) {
    if (wiThrottleProtocol.getLocomotiveAtPosition('0', i).equals(loco)) {
      if (wiThrottleProtocol.getDirection('0', loco) == Forward) {
        wiThrottleProtocol.setDirection('0', loco, Reverse);
      } else {
        wiThrottleProtocol.setDirection('0', loco, Forward);
      }
      break;
    }
  } 
}

int getLocoFacing(String loco) {
  int result = Forward;
  for(int i=0;i<wiThrottleProtocol.getNumberOfLocomotives('0');i++) {
    if (wiThrottleProtocol.getLocomotiveAtPosition('0', i).equals(loco)) {
      result = wiThrottleProtocol.getDirection('0', loco);
      break;
    }
  }
  return result;
}

String getDisplayLocoString(int index) {
  String loco = wiThrottleProtocol.getLocomotiveAtPosition('0', index);
  String locoNumber = loco.substring(1);
  if (!wiThrottleProtocol.getLocomotiveAtPosition('0', 0).equals(loco)) { // not the lead loco
    Direction leadLocoDirection 
        = wiThrottleProtocol.getDirection('0', 
                                          wiThrottleProtocol.getLocomotiveAtPosition('0', 0));
    // Direction locoDirection = leadLocoDirection;

    for(int i=0;i<wiThrottleProtocol.getNumberOfLocomotives('0');i++) {
      if (wiThrottleProtocol.getLocomotiveAtPosition('0', i).equals(loco)) {
        if (wiThrottleProtocol.getDirection('0', loco) != leadLocoDirection) {
          locoNumber = locoNumber + DIRECTION_REVERSE_INDICATOR;
        }
        break;
      }
    }
  }
  return locoNumber;
}

void releaseAllLocos() {
  String loco;
  if (wiThrottleProtocol.getNumberOfLocomotives('0')>0) {
    for(int index=wiThrottleProtocol.getNumberOfLocomotives('0')-1;index>=0;index--) {
      loco = wiThrottleProtocol.getLocomotiveAtPosition('0', index);
      wiThrottleProtocol.releaseLocomotive('0', loco);
      writeOledSpeed();  // note the released locos may not be visible
    } 
    resetFunctionLabels();
  }
}

void releaseOneLoco(String loco) {
  debug_print("releaseOneLoco(): "); debug_print(": "); debug_println(loco);
  wiThrottleProtocol.releaseLocomotive('0', loco);
  resetFunctionLabels();
  debug_println("releaseOneLoco(): end"); 
}

// void toggleAdditionalMultiplier() {
//   switch (speedStepCurrentMultiplier) {
//     case 1: 
//       speedStepCurrentMultiplier = speedStepAdditionalMultiplier;
//       break;
//     case speedStepAdditionalMultiplier:
//       speedStepCurrentMultiplier = speedStepAdditionalMultiplier*2;
//       break;
//     case speedStepAdditionalMultiplier*2:
//       speedStepCurrentMultiplier = 1;
//       break;
//   }

//   currentSpeedStep = speedStep * speedStepCurrentMultiplier;
//   writeOledSpeed();
// }

void toggleAccelerationDelayTime() {
  int newIndex = currentAccellerationDelayTimeIndex + 1;
  int noElements = sizeof(accellerationDelayTimes) / sizeof(accellerationDelayTimes[0]);

  if (newIndex<0) {
    newIndex = 0;
  } else if (newIndex>=noElements) {
    newIndex = 0;
  }
  currentAccellerationDelayTimeIndex = newIndex;
  currentAccellerationDelayTime = accellerationDelayTimes[newIndex];
  refreshOled();
}

void toggleHeartbeatCheck() {
  heartbeatCheckEnabled = !heartbeatCheckEnabled;
  debug_print("Heartbeat Check: "); 
  if (heartbeatCheckEnabled) {
    debug_println("Enabled");
  } else {
    debug_println("Disabled");
  }
  writeHeartbeatCheck();
}

void toggleDirection() {
  if (wiThrottleProtocol.getNumberOfLocomotives(getMultiThrottleChar(0)) > 0) {
    changeDirection((currentDirection == Forward) ? Reverse : Forward );
    // writeOledSpeed();
    refreshOled();
  }
}

void changeDirection(Direction direction) {
  String loco; String leadLoco; 
  Direction leadLocoCurrentDirection;
  int locoCount = wiThrottleProtocol.getNumberOfLocomotives('0');

  if (locoCount > 0) {
    currentDirection = direction;
    debug_print("Change direction(): "); debug_println( (direction==Forward) ? "Forward" : "Reverse");

    if (locoCount == 1) {
      // debug_println("Change direction(): one loco");
      wiThrottleProtocol.setDirection('0', direction);  // change all

    } else {
      // debug_println("Change direction(): multiple locos");
      leadLoco = wiThrottleProtocol.getLeadLocomotive('0');
      leadLocoCurrentDirection = wiThrottleProtocol.getDirection('0', leadLoco);

      for (int i=1; i<locoCount; i++) {
        loco = wiThrottleProtocol.getLocomotiveAtPosition('0', i);
        if (wiThrottleProtocol.getDirection('0', loco) == leadLocoCurrentDirection) {
          wiThrottleProtocol.setDirection('0', loco, direction);
        } else {
          if (wiThrottleProtocol.getDirection('0', loco) == Reverse) {
            wiThrottleProtocol.setDirection('0', loco, Forward);
          } else {
            wiThrottleProtocol.setDirection('0', loco, Reverse);
          }
        }
      }
      wiThrottleProtocol.setDirection('0', leadLoco, direction);
    } 
  }
  // writeOledSpeed();
  refreshOled();
  
  // debug_println("Change direction(): end "); 
}

void doDirectFunction(int functionNumber, bool pressed) {
  debug_println("doDirectFunction(): "); 
  if (wiThrottleProtocol.getNumberOfLocomotives('0') > 0) {
    debug_print("direct fn: "); debug_print(functionNumber); debug_println( pressed ? " Pressed" : " Released");
    // wiThrottleProtocol.setFunction('0', functionNumber, pressed);
    doFunctionWhichLocosInConsist(functionNumber, pressed);
    writeOledSpeed(); 
  }
  // debug_print("doDirectFunction(): end"); 
}

void doFunction(int functionNumber, bool pressed) {   // currently ignoring the pressed value
  debug_print("doFunction(): multiThrottleIndex "); debug_println('0');
  if (wiThrottleProtocol.getNumberOfLocomotives('0')>0) {
    // wiThrottleProtocol.setFunction(multiThrottleIndexChar, functionNumber, true);
    doFunctionWhichLocosInConsist(functionNumber, true);
    if (!functionStates[functionNumber]) {
      debug_print("fn: "); debug_print(functionNumber); debug_println(" Pressed");
    } else {
      delay(20);
      // wiThrottleProtocol.setFunction(multiThrottleIndexChar,functionNumber, false);
      doFunctionWhichLocosInConsist(functionNumber, false);
      debug_print("fn: "); debug_print(functionNumber); debug_println(" Released");
    }
    writeOledSpeed(); 
  }
  // debug_println("doFunction(): ");
}

// Work out which locos in a consist should get the function
//
void doFunctionWhichLocosInConsist(int functionNumber, boolean pressed) {
  if (functionFollow[functionNumber]==CONSIST_LEAD_LOCO) {
    wiThrottleProtocol.setFunction('0',functionNumber, pressed);
  } else {  // at the momemnt the only other option in CONSIST_ALL_LOCOS
    wiThrottleProtocol.setFunction('0', "*", functionNumber, pressed);
  }
  debug_print("doFunctionWhichLocosInConsist(): fn: "); debug_print(functionNumber); debug_println(" Released");
}

void powerOnOff(TrackPower powerState) {
  debug_println("powerOnOff()");
  wiThrottleProtocol.setTrackPower(powerState);
  trackPower = powerState;
  writeOledSpeed();
}

void powerToggle() {
  debug_println("PowerToggle()");
  if (trackPower==PowerOn) {
    powerOnOff(PowerOff);
  } else {
    powerOnOff(PowerOn);
  }
}

void batteryShowToggle() {
  debug_println("batteryShowToggle()");
  switch (showBatteryTest) {
    case ICON_ONLY: 
      showBatteryTest = ICON_AND_PERCENT;
      break;
    case ICON_AND_PERCENT: 
      showBatteryTest = NONE;
      break;
    case NONE: 
    default:
      showBatteryTest = ICON_ONLY;
      break;
  }
}


void stopThenToggleDirection() {
  if (wiThrottleProtocol.getNumberOfLocomotives('0')>0) {
    if (currentSpeed != 0) {
      // wiThrottleProtocol.setSpeed(currentThrottleIndexChar, 0);
      speedSet(0);
    } else {
      if (toggleDirectionOnEncoderButtonPressWhenStationary) toggleDirection();
    }
    currentSpeed = 0;
  }
}

void reconnect() {
  clearOledArray(); 
  oledText[0] = appName; oledText[6] = appVersion; 
  oledText[2] = MSG_DISCONNECTED;
  writeOledArray(false, false);
  delay(5000);
  disconnectWitServer();
}

void setLastServerResponseTime(bool force) {
  // debug_print("setLastServerResponseTime "); debug_println((force) ? "True": "False");
  lastServerResponseTime = wiThrottleProtocol.getLastServerResponseTime();
  if ( (lastServerResponseTime==0) || (force) ) lastServerResponseTime = millis() /1000;
  // debug_print("setLastServerResponseTime "); debug_println(lastServerResponseTime);
}

void checkForShutdownOnNoResponse() {
  if (millis()-startWaitForSelection > 240000) {  // 4 minutes
      debug_println("Waited too long for a selection. Shutting down");
      deepSleepStart();
  }
}

void clearLastPotValues() {
  debug_println("clearLastPotValues() ");
  lowestThrottlePotValue = 32768;
  highestThrottlePotValue = -1;
  lowestReverserPotValue = 32768;
  highestReverserPotValue = -1;
  lowestBrakePotValue = 32768;
  highestBrakePotValue = -1;
}

String getSuggestedBrakePotRange() {
  String rslt = String(">") + POT_VALUE_TITLE_BRAKE;
  int brakeRange = highestBrakePotValue - lowestBrakePotValue;
  int previousStep = lowestBrakePotValue;
  int nextStep = 0;
  for (int i=0;i<5;i++) {
    if (i==0) {      
      nextStep = previousStep + brakeRange*0.1;
      rslt = rslt + String(nextStep);
    } else {
      nextStep = previousStep + brakeRange*0.2;
      rslt = rslt + "," + String(nextStep);
    }
    brakePotRecalibratedValues[i] = nextStep;
    previousStep = nextStep;
  }
  return rslt;
}

String getSuggestedReverserPotRange() {
  String rslt = String(">") + POT_VALUE_TITLE_REVERSER;
  int reverserRange = highestReverserPotValue - lowestReverserPotValue;
  int neutralLow = lowestReverserPotValue+ (reverserRange*.33);
  int neutralHigh = lowestReverserPotValue+ (reverserRange*.66);
  rslt = rslt + neutralLow + ", " + neutralHigh ;
  
  reverserPotRecalibratedValues[0] = neutralLow;
  reverserPotRecalibratedValues[1] = neutralHigh;
  return rslt;
}

String getThrottlePotNotchValues(int line) {
  // debug_println("getThrottlePotNotchValues() ");
  String rslt = "";
  int noElements = sizeof(throttlePotTempValues) / sizeof(throttlePotTempValues[0]);
  int start = 0;
  int end = 3;
  if (line==1) {
    start = 3;
    end = 6;
  } else if (line==2) {
    start = 6;
    end = noElements;
  }
  for (int i=start; i<end; i++) {
    if ( (i!=0) && (i!=3) && (i!=6) ) rslt = rslt + ",";
    rslt = rslt + String(throttlePotTempValues[i]);
  }  
  return rslt;
}

String getSuggestedThrottlePotNotchValues(int line) {
  debug_println("getSuggestedThrottlePotNotchValues() ");
  String rslt = "";
  int noElements = sizeof(throttlePotNotchValues) / sizeof(throttlePotNotchValues[0]);
  int start = 0;
  int end = 3;
  if (line==1) {
    start = 3;
    end = 6;
  } else if (line==2) {
    start = 6;
    end = noElements;
  }
  for (int i=0; i<noElements; i++) {
    throttlePotRecalibratedValues[i] = (throttlePotTempValues[i+1] - throttlePotTempValues[i])/ 2 + throttlePotTempValues[i];
    debug_print("i: "); debug_print(i);
    debug_print(" throttlePotTempValues[i]: ");
    debug_print(throttlePotTempValues[i]);
    debug_print(" throttlePotTempValues[i+1]: ");
    debug_print(throttlePotTempValues[i+1]);
    debug_print(" throttlePotRecalibratedValues[i]: ");
    debug_print(throttlePotRecalibratedValues[i]);
    debug_println("");
  }
  
  for (int i=start; i<end; i++) {
    if ( (i!=0) && (i!=3) && (i!=6) ) rslt = rslt + ",";
    rslt = rslt + String(throttlePotRecalibratedValues[i]);
  }
  return rslt;
}

void recalibatePotValues() {
  debug_print("recalibatePotValues() "); debug_println(lastOledPotValuesState);
  int noElements = 0;
  if (lastOledPotValuesState==1) {
    noElements = sizeof(brakePotValues) / sizeof(brakePotValues[0]);
    for (int i=0; i<noElements; i++) {
      brakePotValues[i] = brakePotRecalibratedValues[i];
    }
  } else { // 2
    noElements = sizeof(reverserPotValues) / sizeof(reverserPotValues[0]);;
    for (int i=0; i<noElements; i++) {
      reverserPotValues[i] = reverserPotRecalibratedValues[i];
    }
  }
}

void recalibateThrottlePotValues() {
  debug_println("recalibateThrottlePotValues() ");
  int noElements = sizeof(throttlePotNotchValues) / sizeof(throttlePotNotchValues[0]);
  for (int i=0; i<noElements; i++) {
    throttlePotNotchValues[i] = throttlePotRecalibratedValues[i];
  }
}

String getDots(int howMany) {
  //             123456789_123456789_123456789_123456789_123456789_123456789_
  String dots = "............................................................";
  if (howMany>dots.length()) howMany = dots.length();
  return dots.substring(0,howMany);
}

// *********************************************************************************
//  Select functions
// *********************************************************************************

void selectRoster(int selection) {
  debug_print("selectRoster() "); debug_println(selection);

  if ((selection>=0) && (selection < rosterSize)) {
    String loco = String(rosterLength[selection]) + rosterAddress[selection];
    debug_print("add Loco: "); debug_println(loco);
    wiThrottleProtocol.addLocomotive('0',loco);
    wiThrottleProtocol.getDirection('0',loco);
    wiThrottleProtocol.getSpeed('0');
    resetFunctionStates();
    writeOledSpeed();
    keypadUseType = KEYPAD_USE_OPERATION;
  }
}

void selectTurnoutList(int selection, TurnoutAction action) {
  debug_print("selectTurnoutList() "); debug_println(selection);

  if ((selection>=0) && (selection < turnoutListSize)) {
    String turnout = turnoutListSysName[selection];
    debug_print("Turnout Selected: "); debug_println(turnout);
    wiThrottleProtocol.setTurnout(turnout,action);
    writeOledSpeed();
    keypadUseType = KEYPAD_USE_OPERATION;
  }
}

void selectRouteList(int selection) {
  debug_print("selectRouteList() "); debug_println(selection);

  if ((selection>=0) && (selection < routeListSize)) {
    String route = routeListSysName[selection];
    debug_print("Route Selected: "); debug_println(route);
    wiThrottleProtocol.setRoute(route);
    writeOledSpeed();
    keypadUseType = KEYPAD_USE_OPERATION;
  }
}

void selectFunctionList(int selection) {
  debug_print("selectFunctionList() "); debug_println(selection);

  if ((selection>=0) && (selection < MAX_FUNCTIONS)) {
    String function = functionLabels[selection];
    debug_print("Function Selected: "); debug_println(function);
    doFunction(selection, true);
    writeOledSpeed();
    keypadUseType = KEYPAD_USE_OPERATION;
  }
}

void selectEditConsistList(int selection) {
  debug_print("selectEditConsistList() "); debug_println(selection);

  if (wiThrottleProtocol.getNumberOfLocomotives('0') > 1 ) {
    String loco = wiThrottleProtocol.getLocomotiveAtPosition('0', selection);
    toggleLocoFacing(loco);

    writeOledSpeed();
    keypadUseType = KEYPAD_USE_OPERATION;
  }
}

// *********************************************************************************
//  oLED functions
// *********************************************************************************

void setAppnameForOled() {
  oledText[0] = appName; oledText[6] = appVersion; 
}

void setMenuTextForOled(int menuTextIndex) {
  oledText[5] = menu_text[menuTextIndex];
  if (broadcastMessageText!="") {
    if (millis()-broadcastMessageTime < 10000) {
      oledText[5] = broadcastMessageText;
    } else {
      broadcastMessageText = "";
    }
  }
}

void refreshOled() {
    //  debug_print("refreshOled(): ");
    //  debug_println(lastOledScreen);
  switch (lastOledScreen) {
    case last_oled_screen_speed:
      writeOledSpeed();
      break;
    case last_oled_screen_turnout_list:
      writeOledTurnoutList(lastOledStringParameter, lastOledTurnoutParameter);
      break;
    case last_oled_screen_route_list:
      writeOledRouteList(lastOledStringParameter);
      break;
    case last_oled_screen_function_list:
      writeOledFunctionList(lastOledStringParameter);
      break;
    case last_oled_screen_menu:
      writeOledMenu(lastOledStringParameter);
      break;
    case last_oled_screen_extra_submenu:
      writeOledExtraSubMenu();
      break;
    case last_oled_screen_all_locos:
      writeOledAllLocos(lastOledBooleanParameter);
      break;
    case last_oled_screen_edit_consist:
      writeOledEditConsist();
      break;
    case last_oled_screen_direct_commands:
      writeOledDirectCommands();
      break;
    case last_oled_screen_pot_values:
      writeOledPotValues();
      break;
    case last_oled_screen_throttle_pot_values:
      writeOledThrottlePotValues();
      break;
  }
}


void writeOledFoundSSids(String soFar) {
  menuIsShowing = true;
  keypadUseType = KEYPAD_USE_SELECT_SSID_FROM_FOUND;
  if (soFar == "") { // nothing entered yet
    clearOledArray();
    for (int i=0; i<5 && i<foundSsidsCount; i++) {
      if (foundSsids[(page*5)+i].length()>0) {
        oledText[i] = String(i) + ": " + foundSsids[(page*5)+i] + "   (" + foundSsidRssis[(page*5)+i] + ")" ;
      }
    }
    oledText[5] = "(" + String(page+1) +  ") " + menu_text[menu_select_ssids_from_found];
    writeOledArray(false, false);
  // } else {
  //   int cmd = menuCommand.substring(0, 1).toInt();
  }
}

void writeOledRoster(String soFar) {
  lastOledScreen = last_oled_screen_roster;
  lastOledStringParameter = soFar;

  menuIsShowing = true;
  keypadUseType = KEYPAD_USE_SELECT_ROSTER;
  if (soFar == "") { // nothing entered yet
    clearOledArray();
    for (int i=0; i<5 && i<rosterSize; i++) {
      if (rosterAddress[(page*5)+i] != 0) {
        oledText[i] = String(rosterIndex[i]) + ": " + rosterName[(page*5)+i] + " (" + rosterAddress[(page*5)+i] + ")" ;
      }
    }
    oledText[5] = "(" + String(page+1) +  ") " + menu_text[menu_roster];
    writeOledArray(false, false);
  // } else {
  //   int cmd = menuCommand.substring(0, 1).toInt();
  }
}

void writeOledTurnoutList(String soFar, TurnoutAction action) {
  lastOledScreen = last_oled_screen_turnout_list;
  lastOledStringParameter = soFar;
  lastOledTurnoutParameter = action;

  menuIsShowing = true;
  if (action == TurnoutThrow) {
    keypadUseType = KEYPAD_USE_SELECT_TURNOUTS_THROW;
  } else {
    keypadUseType = KEYPAD_USE_SELECT_TURNOUTS_CLOSE;
  }
  if (soFar == "") { // nothing entered yet
    clearOledArray();
    int j = 0;
    for (int i=0; i<10 && i<turnoutListSize; i++) {
      j = (i<5) ? i : i+1;
      if (turnoutListUserName[(page*10)+i].length()>0) {
        oledText[j] = String(turnoutListIndex[i]) + ": " + turnoutListUserName[(page*10)+i].substring(0,10);
      }
    }
    oledText[5] = "(" + String(page+1) +  ") " + menu_text[menu_turnout_list];
    writeOledArray(false, false);
  // } else {
  //   int cmd = menuCommand.substring(0, 1).toInt();
  }
}

void writeOledRouteList(String soFar) {
  lastOledScreen = last_oled_screen_route_list;
  lastOledStringParameter = soFar;

  menuIsShowing = true;
  keypadUseType = KEYPAD_USE_SELECT_ROUTES;
  if (soFar == "") { // nothing entered yet
    clearOledArray();
    int j = 0;
    for (int i=0; i<10 && i<routeListSize; i++) {
      j = (i<5) ? i : i+1;
      if (routeListUserName[(page*10)+i].length()>0) {
        oledText[j] = String(routeListIndex[i]) + ": " + routeListUserName[(page*10)+i].substring(0,10);
      }
    }
    oledText[5] =  "(" + String(page+1) +  ") " + menu_text[menu_route_list];
    writeOledArray(false, false);
  // } else {
  //   int cmd = menuCommand.substring(0, 1).toInt();
  }
}

void writeOledFunctionList(String soFar) {
  lastOledScreen = last_oled_screen_function_list;
  lastOledStringParameter = soFar;

  menuIsShowing = true;
  keypadUseType = KEYPAD_USE_SELECT_FUNCTION;
  
  if (soFar == "") { // nothing entered yet
    clearOledArray();
    if (wiThrottleProtocol.getNumberOfLocomotives(currentThrottleIndexChar) > 0 ) {
      int j = 0; int k = 0;
      for (int i=0; i<10; i++) {
        k = (functionPage*10) + i;
        if (k < MAX_FUNCTIONS) {
          j = (i<5) ? i : i+1;
          // if (functionLabels[currentThrottleIndex][k].length()>0) {
            oledText[j] = String(i) + ": " 
            + ((k<10) ? functionLabels[k].substring(0,10) : String(k) 
            + "-" + functionLabels[k].substring(0,7)) ;
            
            if (functionStates[k]) {
              oledTextInvert[j] = true;
            }
          // }
        }
      }
      oledText[5] = "(" + String(functionPage) +  ") " + menu_text[menu_function_list];
      // setMenuTextForOled("(" + String(functionPage) +  ") " + menu_function_list);
    } else {
      oledText[0] = MSG_NO_FUNCTIONS;
      // oledText[2] = MSG_THROTTLE_NUMBER + String(currentThrottleIndex+1);
      oledText[3] = MSG_NO_LOCO_SELECTED;
      // oledText[5] = menu_cancel;
      setMenuTextForOled(menu_cancel);
    }
    writeOledArray(false, false);
  // } else {
  //   int cmd = menuCommand.substring(0, 1).toInt();
  }
}

void writeOledEnterPassword() {
  keypadUseType = KEYPAD_USE_ENTER_SSID_PASSWORD;
  encoderUseType = KEYPAD_USE_ENTER_SSID_PASSWORD;
  clearOledArray(); 
  String tempSsidPasswordEntered;
  tempSsidPasswordEntered = ssidPasswordEntered+ssidPasswordCurrentChar;
  if (tempSsidPasswordEntered.length()>12) {
    tempSsidPasswordEntered = "\253"+tempSsidPasswordEntered.substring(tempSsidPasswordEntered.length()-12);
  } else {
    tempSsidPasswordEntered = " "+tempSsidPasswordEntered;
  }
  oledText[0] = MSG_ENTER_PASSWORD;
  oledText[2] = tempSsidPasswordEntered;
  // oledText[5] = menu_enter_ssid_password;
  setMenuTextForOled(menu_enter_ssid_password);
  writeOledArray(false, true);
}

void writeOledMenu(String soFar) {
  lastOledScreen = last_oled_screen_menu;
  lastOledStringParameter = soFar;

  debug_print("writeOledMenu() : "); debug_println(soFar);
  menuIsShowing = true;
  bool drawTopLine = false;
  if (soFar == "") { // nothing entered yet
    clearOledArray();
    int j = 0;
    for (int i=1; i<10; i++) {
      j = (i<6) ? i : i+1;
      oledText[j-1] = String(i) + ": " + menuText[i][0];
    }
    oledText[10] = "0: " + menuText[0][0];
    // oledText[5] = menu_cancel;
    setMenuTextForOled(menu_cancel);
    writeOledArray(false, false);
  } else {
    int cmd = menuCommand.substring(0, 1).toInt();

    clearOledArray();

    oledText[0] = ">> " + menuText[cmd][0] +":"; oledText[6] =  menuCommand.substring(1, menuCommand.length());
    oledText[5] = menuText[cmd][1];

    switch (soFar.charAt(0)) {
      case MENU_ITEM_DROP_LOCO: {
            if (wiThrottleProtocol.getNumberOfLocomotives('0') > 0) {
              writeOledAllLocos(false);
              drawTopLine = true;
            }
          } // fall through
      case MENU_ITEM_FUNCTION: {
      // case MENU_ITEM_TOGGLE_DIRECTION: {
          if (wiThrottleProtocol.getNumberOfLocomotives('0') <= 0 ) {
            // oledText[2] = MSG_THROTTLE_NUMBER + String(1);
            oledText[3] = MSG_NO_LOCO_SELECTED;
            // oledText[5] = menu_cancel;
            setMenuTextForOled(menu_cancel);
          } 
          break;
        }
      case MENU_ITEM_EXTRAS: { // extra menu
          writeOledExtraSubMenu();
          drawTopLine = true;
          break;
        }
    }

    writeOledArray(false, false, true, drawTopLine);
  }
}

void writeOledExtraSubMenu() {
  lastOledScreen = last_oled_screen_extra_submenu;

  int j = 0;
  for (int i=0; i<8; i++) {
    j = (i<4) ? i : i+2;
    oledText[j+1] = (extraSubMenuText[i].length()==0) ? "" : String(i) + ": " + extraSubMenuText[i];
  }
}

void writeOledAllLocos(bool hideLeadLoco) {
  lastOledScreen = last_oled_screen_all_locos;
  lastOledBooleanParameter = hideLeadLoco;

  int startAt = (hideLeadLoco) ? 1 :0;  // for the loco heading menu, we don't want to show the loco 0 (lead) as an option.
  debug_println("writeOledAllLocos(): ");
  String loco;
  int j = 0; int i = 0;
  if (wiThrottleProtocol.getNumberOfLocomotives(currentThrottleIndexChar) > 0) {
    for (int index=0; ((index < wiThrottleProtocol.getNumberOfLocomotives('0')) && (i < 8)); index++) {  //can only show first 8
      j = (i<4) ? i : i+2;
      loco = wiThrottleProtocol.getLocomotiveAtPosition('0', index);
      if (i>=startAt) {
        oledText[j+1] = String(i) + ": " + loco;
        if (wiThrottleProtocol.getDirection('0', loco) == Reverse) {
          oledTextInvert[j+1] = true;
        }
      }
      i++;      
    } 
  }
}

void writeOledEditConsist() {
  lastOledScreen = last_oled_screen_edit_consist;

  menuIsShowing = false;
  clearOledArray();
  debug_println("writeOledEditConsist(): ");
  keypadUseType = KEYPAD_USE_EDIT_CONSIST;
  writeOledAllLocos(true);
  oledText[0] = menuText[11][0];
  oledText[5] = menuText[11][1];
  writeOledArray(false, false);
}

void writeHeartbeatCheck() {
  menuIsShowing = false;
  clearOledArray();
  oledText[0] = menuText[10][0];
  if (heartbeatCheckEnabled) {
    oledText[1] = MSG_HEARTBEAT_CHECK_ENABLED; 
  } else {
    oledText[1] = MSG_HEARTBEAT_CHECK_DISABLED; 
  }
  oledText[5] = menuText[10][1];
  writeOledArray(false, false);
}

void writeOledSpeed() {
  lastOledScreen = last_oled_screen_speed;

  // debug_println("writeOledSpeed() ");
  
  menuIsShowing = false;
  String sLocos = "";
  String sSpeed = "";
  String sDirection = "";

  clearOledArray();
  
  boolean drawTopLine = false;

  if (wiThrottleProtocol.getNumberOfLocomotives('0') > 0 ) {
  
    for (int i=0; i < wiThrottleProtocol.getNumberOfLocomotives('0'); i++) {
      sLocos = sLocos + " " + getDisplayLocoString(i);
    }
    sSpeed = String(getDisplaySpeed());
    sDirection = (currentDirection==Forward) ? DIRECTION_FORWARD_TEXT : DIRECTION_REVERSE_TEXT;
    if ((currentSpeed==0) && (targetDirection==NEUTRAL)) {
      sDirection = DIRECTION_NEUTRAL_TEXT;
    }

    oledText[0] = sLocos; 

    drawTopLine = true;

  } else {
    setAppnameForOled();
    oledText[3] = MSG_NO_LOCO_SELECTED;
    drawTopLine = true;
  }

  if (!hashShowsFunctionsInsteadOfKeyDefs) {
      setMenuTextForOled(menu_menu);
    } else {
    setMenuTextForOled(menu_menu_hash_is_functions);
  }

  writeOledArray(false, false, false, drawTopLine);

  if (wiThrottleProtocol.getNumberOfLocomotives('0') > 0 ) {
    writeOledFunctions();
  }

  // currentAccellerationDelayTimeIndex
  if (currentAccellerationDelayTimeIndex>0) {
    u8g2.setDrawColor(1);
    u8g2.drawLine(0, 34, 2, 32);
    u8g2.drawLine(4, 34, 2, 32);
    u8g2.setFont(FONT_DEFAULT);
    u8g2.drawStr(6, 37, String(currentAccellerationDelayTimeIndex).c_str());
  }

  // brakeCurrentPosition
  if (brakeCurrentPosition>0) {
    u8g2.setDrawColor(1);
    u8g2.drawLine(0, 23, 2, 25);
    u8g2.drawLine(4, 23, 2, 25);
    u8g2.setFont(FONT_DEFAULT);
    u8g2.drawStr(6, 28, String(brakeCurrentPosition).c_str());
  }

  // target speed / throttle position
  if (targetSpeed!=currentSpeed) {
    u8g2.setDrawColor(1);
    u8g2.setFont(FONT_GLYPHS);
    u8g2.drawGlyph(105, 48, glyph_target_speed);
    u8g2.setFont(FONT_DEFAULT);
    String offset = "";
    if (targetSpeed<10) offset = " ";
    if (targetSpeed<100) offset = offset + " ";
    u8g2.drawStr(114, 48, (offset + String(targetSpeed)).c_str());
  }

  // target direction
  u8g2.setDrawColor(1);
  u8g2.setFont(FONT_GLYPHS);
  if (targetDirection==FORWARD) {
    u8g2.drawGlyph(120, 40, glyph_target_direction_forward);
  } else if (targetDirection==NEUTRAL) {
    if (currentSpeed!=0) {
      u8g2.setFont(FONT_DEFAULT);
      u8g2.drawStr(112, 39, String(DIRECTION_NEUTRAL_TEXT).c_str());
      // u8g2.drawGlyph(120, 40, glyph_target_direction_neutral);
    }
  } else {
    u8g2.drawGlyph(120, 40, glyph_target_direction_reverse);
  }
  u8g2.setFont(FONT_DEFAULT);

  // eStop
  if (eStopEngaged) {
    u8g2.setFont(FONT_GLYPHS);
    u8g2.drawGlyph(120, 32, glyph_eStop);
  }

  writeOledBattery();

  // track power
  if (trackPower == PowerOn) {
    u8g2.drawRBox(0,40,9,9,1);
    u8g2.setDrawColor(0);
  }
  u8g2.setFont(FONT_GLYPHS);
  u8g2.drawGlyph(1, 48, glyph_track_power);
  u8g2.setDrawColor(1);

  // heartbeat
  if (!heartbeatCheckEnabled) {
    u8g2.setFont(FONT_GLYPHS);
    u8g2.drawGlyph(13, 49, glyph_heartbeat_off);
    u8g2.setDrawColor(2);
    u8g2.drawLine(13, 48, 20, 41);
    // u8g2.drawLine(13, 48, 21, 40);
    u8g2.setDrawColor(1);
  }

  // direction
  // needed for new function state format
  u8g2.setFont(FONT_DIRECTION); // medium
  u8g2.drawStr(79,36, sDirection.c_str());

  // speed
  const char *cSpeed = sSpeed.c_str();
  // u8g2.setFont(u8g2_font_inb21_mn); // big
  u8g2.setFont(FONT_SPEED); // big
  int width = u8g2.getStrWidth(cSpeed);
  u8g2.drawStr(22+(55-width),45, cSpeed);

  u8g2.sendBuffer();

  // debug_println("writeOledSpeed(): end");
}

void writeOledBattery() {
  if ( (useBatteryTest) && (showBatteryTest!=NONE) && (lastBatteryCheckTime>0)) {
    //int lastBatteryTestValue = random(0,100);
    u8g2.setFont(FONT_GLYPHS);
    u8g2.setDrawColor(1);
    // int x = 13; int y = 28;
    int x = 120; int y = 11;
    // if (useBatteryPercentAsWellAsIcon) x = 102;
    if (showBatteryTest==ICON_AND_PERCENT) x = 102;
    u8g2.drawStr(x, y, String("Z").c_str());
    if (lastBatteryTestValue>10) u8g2.drawLine(x+1, y-6, x+1, y-3);
    if (lastBatteryTestValue>25) u8g2.drawLine(x+2, y-6, x+2, y-3);
    if (lastBatteryTestValue>50) u8g2.drawLine(x+3, y-6, x+3, y-3);
    if (lastBatteryTestValue>75) u8g2.drawLine(x+4, y-6, x+4, y-3);
    if (lastBatteryTestValue>90) u8g2.drawLine(x+5, y-6, x+5, y-3);
    
    // if (useBatteryPercentAsWellAsIcon) {
    if (showBatteryTest==ICON_AND_PERCENT) {
      // x = 13; y = 36;
      x = 112; y = 10;
      u8g2.setFont(FONT_FUNCTION_INDICATORS);
      if(lastBatteryTestValue<5) {
        u8g2.drawStr(x,y, String("LOW").c_str());
      } else {
        u8g2.drawStr(x,y, String(String(lastBatteryTestValue)+"%").c_str());
      }
    }
  }
}

void writeOledFunctions() {
  lastOledScreen = last_oled_screen_speed;

  // debug_println("writeOledFunctions():");
   for (int i=0; i < MAX_FUNCTIONS; i++) {
    if (functionStates[i]) {
      // new function state format
      // u8g2.drawRBox(i*4+12,12+1,5,7,2);
      u8g2.drawRBox(i*4,12+1,5,7,2);
      u8g2.setDrawColor(0);
      u8g2.setFont(FONT_FUNCTION_INDICATORS);   
      // u8g2.drawStr( i*4+1+12, 18+1, String( (i<10) ? i : ((i<20) ? i-10 : i-20)).c_str());
      u8g2.drawStr( i*4+1, 18+1, String( (i<10) ? i : ((i<20) ? i-10 : ((i<30) ? i-20 : i-30))).c_str());
      u8g2.setDrawColor(1);
     }
   }
  // debug_println("writeOledFunctions(): end");
}

void writeOledPotValues() {
  lastOledScreen = last_oled_screen_pot_values;
  keypadUseType = KEYPAD_USE_POT_VALUES;
  menuIsShowing = true;
  writeOledEitherPotValues(false);
}

void writeOledThrottlePotValues() {
  lastOledScreen = last_oled_screen_throttle_pot_values;
  keypadUseType = KEYPAD_USE_THROTTLE_POT_VALUES;
  menuIsShowing = true;
  writeOledEitherPotValues(true);
}

void writeOledEitherPotValues(bool throttleOnly) {
  clearOledArray();
  oledText[1] = POT_VALUE_TITLE_THROTTLE + String(lastThrottlePotValue) + " <=" + String(lastThrottlePotHighValue);
  oledText[7] = ":" + String(throttlePotNotch) + ": " + String(lowestThrottlePotValue) + "-" + String(highestThrottlePotValue);
  if (!throttleOnly) {
    oledText[0] = MENU_ITEM_TEXT_TITLE_POT_VALUES;
    oledText[2] = POT_VALUE_TITLE_REVERSER + String(lastReverserPotValue) + " <=" + String(lastReverserPotHighValue);
    oledText[3] = POT_VALUE_TITLE_BRAKE + String(lastBrakePotValue) + " <=" + String(lastBrakePotHighValue);
    oledText[8] = ":" + String(reverserCurrentPosition) + ": " + String(lowestReverserPotValue) + "-" + String(highestReverserPotValue);
    oledText[9] = ":" + String(brakeCurrentPosition) + ": " + String(lowestBrakePotValue) + "-" + String(highestBrakePotValue);

    if (lastOledPotValuesState==1) {
      int brakeRange = highestBrakePotValue - lowestBrakePotValue;
      if (brakeRange>300) {
        oledText[4] = getSuggestedBrakePotRange();
      } else {
        oledText[4] = MSG_POT_INCOMPLETE;
      }
    } else { // 2
      int reverserRange = highestReverserPotValue - lowestReverserPotValue;
      if (reverserRange>300) {
        oledText[4] = getSuggestedReverserPotRange();
      } else {
        oledText[4] = MSG_POT_INCOMPLETE;
      }
    }
    oledText[5] = menuText[12][1];
    writeOledArray(false, false, true, true, USE_POTS_SMALL_FONTS);

  } else { // trhrottle only

    bool ready = true;
    int noElements = sizeof(throttlePotTempValues) / sizeof(throttlePotTempValues[0]);
    for (int i=1; i<noElements; i++) {
      if (throttlePotTempValues[i]<=throttlePotTempValues[i-1]) { 
        ready = false;
        break;
      }
    }

    oledText[0] = MENU_ITEM_TEXT_TITLE_THROTTLE_POT_VALUES;
    oledText[2] = getThrottlePotNotchValues(0);
    oledText[3] = getThrottlePotNotchValues(1);
    oledText[4] = getThrottlePotNotchValues(2);

    if (ready) {
      oledText[8] = getSuggestedThrottlePotNotchValues(0);
      oledText[9] = getSuggestedThrottlePotNotchValues(1);
      oledText[10] = getSuggestedThrottlePotNotchValues(2);
    } else {
      oledText[9] = MSG_THROTTLE_POT_INCOMPLETE;
    }
    oledText[5] = menuText[14][1];

    writeOledArray(false, false, true, true, USE_THROTTLE_POT_SMALL_FONTS);
  }

}

// * * * * * * * * * * * * * * * * * * * * * * * *

void writeOledArray(bool isThreeColums, bool isPassword) {
  writeOledArray(isThreeColums, isPassword, true, false);
}

void writeOledArray(bool isThreeColums, bool isPassword, bool sendBuffer) {
  writeOledArray(isThreeColums, isPassword, sendBuffer, false);
}

void writeOledArray(bool isThreeColums, bool isPassword, bool sendBuffer, bool drawTopLine) {
  writeOledArray(isThreeColums, isPassword, sendBuffer, drawTopLine, USE_NO_SMALL_FONTS);
}

void writeOledArray(bool isThreeColums, bool isPassword, bool sendBuffer, bool drawTopLine, WhenToUseSmallFonts useSmallFontForContent) {
  // debug_println("Start writeOledArray()");
  u8g2.clearBuffer();					// clear the internal memory

  u8g2.setFont(FONT_DEFAULT); // small
  
  int x=0;
  int y=10;
  int xInc = 64; 
  int max = 12;
  if (isThreeColums) {
    xInc = 42;
    max = 18;
  }

  bool useSmallFonts = false;
  for (int i=0; i < max; i++) {
    const char *cLine1 = oledText[i].c_str();

    useSmallFonts = false;
    if ( (useSmallFontForContent == USE_POTS_SMALL_FONTS) 
          && (((i>0) && (i<4)) || ((i>6) && (i<10))) ) { 
      useSmallFonts = true;
    } else if ( (useSmallFontForContent == USE_THROTTLE_POT_SMALL_FONTS) 
              && (((i>0) && (i<5)) || (i==7)) ) { 
      useSmallFonts = true;
    } 

    if (useSmallFonts) {
      u8g2.setFont(FONT_FUNCTION_INDICATORS); 
    } else {
      u8g2.setFont(FONT_DEFAULT);
    }

    if ((isPassword) && (i==2)) u8g2.setFont(FONT_PASSWORD); 

    if (oledTextInvert[i]) {
      u8g2.drawBox(x,y-8,62,10);
      u8g2.setDrawColor(0);
    }
    u8g2.drawStr(x,y, cLine1);
    u8g2.setDrawColor(1);

    if ((isPassword) && (i==2)) u8g2.setFont(FONT_DEFAULT); 
    y = y + 10;
    if ((i==5) || (i==11)) {
      x = x + xInc;
      y = 10;
    }
  }

  if (drawTopLine) {
    u8g2.drawHLine(0,11,128);
    writeOledBattery();
  }
  u8g2.drawHLine(0,51,128);


  if (sendBuffer) u8g2.sendBuffer();					// transfer internal memory to the display
  // debug_println("writeOledArray(): end ");
}

// * * * * * * * * * * * * * * * * * * * * * * * *

void clearOledArray() {
  for (int i=0; i < 15; i++) {
    oledText[i] = "";
    oledTextInvert[i] = false;
  }
}

void writeOledDirectCommands() {
  lastOledScreen = last_oled_screen_direct_commands;

  oledDirectCommandsAreBeingDisplayed = true;
  clearOledArray();
  oledText[0] = DIRECT_COMMAND_LIST;
  for (int i=0; i < 4; i++) {
    oledText[i+1] = directCommandText[i][0];
  }
  int j = 0;
  for (int i=6; i < 10; i++) {
    oledText[i+1] = directCommandText[j][1];
    j++;
  }
  j=0;
  for (int i=12; i < 16; i++) {
    oledText[i+1] = directCommandText[j][2];
    j++;
  }
  writeOledArray(true, false);
  menuCommandStarted = false;
}

// *********************************************************************************

void deepSleepStart() {
  deepSleepStart(SLEEP_REASON_COMMAND);
}

void deepSleepStart(int shutdownReason) {
  clearOledArray(); 
  setAppnameForOled();
  int delayPeriod = 2000;
  if (shutdownReason==SLEEP_REASON_INACTIVITY) {
    oledText[2] = MSG_AUTO_SLEEP;
    delayPeriod = 10000;
  } else if (shutdownReason==SLEEP_REASON_BATTERY) {
    oledText[2] = MSG_BATTERY_SLEEP;
    delayPeriod = 10000;
  }
  oledText[3] = MSG_START_SLEEP;
  writeOledArray(false, false, true, true);
  delay(delayPeriod);

  u8g2.setPowerSave(1);
  esp_deep_sleep_start();
}
