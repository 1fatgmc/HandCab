# 1 "C:\\Users\\akers\\AppData\\Local\\Temp\\tmpx1mhji2e"
#include <Arduino.h>
# 1 "C:/Users/akers/Documents/GitHub/HandCab/HandCab.ino"
# 10 "C:/Users/akers/Documents/GitHub/HandCab/HandCab.ino"
#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiThrottleProtocol.h>
#include <AiEsp32RotaryEncoder.h>
#include <Keypad.h>
#include <U8g2lib.h>
#include <string>

#include "config_network.h"
#include "config_buttons.h"
#include "config_keypad_etc.h"

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


TrackPower trackPower = PowerUnknown;
String turnoutPrefix = "";
String routePrefix = "";


bool circleValues = true;
int encoderValue = 0;
int lastEncoderValue = 0;


bool useRotaryEncoderForThrottle = false;
int throttlePotPin = THROTTLE_POT_PIN;
bool throttlePotUseNotches = THROTTLE_POT_USE_NOTCHES;
int throttlePotNotchValues[] = THROTTLE_POT_NOTCH_VALUES;
int throttlePotNotchSpeeds[] = THROTTLE_POT_NOTCH_SPEEDS;
int throttlePotNotch = 0;
int throttlePotTargetSpeed = 0;
int lastThrottlePotValue = 0;
int lastThrottlePotValues[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};


int reverserPotPin = REVERSER_POT_PIN;
int reverserPotValues[] = REVERSER_POT_VALUES;
int lastReverserPotValue = 0;
int lastReverserPotValues[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
int reverserCurrentPosition = REVERSER_POSITION_NEUTRAL;


int brakePotPin = BRAKE_POT_PIN;
int brakePotValues[] = BRAKE_POT_VALUES;
int lastBrakePotValue = 0;
int lastBrakePotValues[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
int brakeCurrentPosition = 0;


int brakeDelayTimes[] = BRAKE_DELAY_TIMES;
int currentBrakeDelayTime = 0;
int accellerationDelayTimes[] = ACCELLERATION_DELAY_TIMES;
int currentAccellerationDelayTime = accellerationDelayTimes[0];
int currentAccellerationDelayTimeIndex = 0;
int targetSpeed = 0;
PotDirection targetDirection = FORWARD;
double startMomentumTimerMillis = -1;



String selectedSsid = "";
String selectedSsidPassword = "";
int ssidConnectionState = CONNECTION_STATE_DISCONNECTED;


String ssidPasswordEntered = "";
boolean ssidPasswordChanged = true;
char ssidPasswordCurrentChar = ssidPasswordBlankChar;

IPAddress selectedWitServerIP;
int selectedWitServerPort = 0;
String selectedWitServerName ="";
int noOfWitServices = 0;
int witConnectionState = CONNECTION_STATE_DISCONNECTED;
String serverType = "";


IPAddress foundWitServersIPs[maxFoundWitServers];
int foundWitServersPorts[maxFoundWitServers];
String foundWitServersNames[maxFoundWitServers];
int foundWitServersCount = 0;
bool autoConnectToFirstDefinedServer = AUTO_CONNECT_TO_FIRST_DEFINED_SERVER;
bool autoConnectToFirstWiThrottleServer = AUTO_CONNECT_TO_FIRST_WITHROTTLE_SERVER;
int outboundCmdsMininumDelay = OUTBOUND_COMMANDS_MINIMUM_DELAY;
bool commandsNeedLeadingCrLf = false;


String foundSsids[maxFoundSsids];
long foundSsidRssis[maxFoundSsids];
boolean foundSsidsOpen[maxFoundSsids];
int foundSsidsCount = 0;
int ssidSelectionSource;
double startWaitForSelection;


String witServerIpAndPortConstructed = "###.###.###.###:#####";
String witServerIpAndPortEntered = DEFAULT_IP_AND_PORT;
boolean witServerIpAndPortChanged = true;


int rosterSize = 0;
int rosterIndex[maxRoster];
String rosterName[maxRoster];
int rosterAddress[maxRoster];
char rosterLength[maxRoster];

int page = 0;
int functionPage = 0;


String broadcastMessageText = "";
long broadcastMessageTime = 0;


int lastOledScreen = 0;
String lastOledStringParameter = "";
int lastOledIntParameter = 0;
boolean lastOledBooleanParameter = false;
TurnoutAction lastOledTurnoutParameter = TurnoutToggle;


int turnoutListSize = 0;
int turnoutListIndex[maxTurnoutList];
String turnoutListSysName[maxTurnoutList];
String turnoutListUserName[maxTurnoutList];
int turnoutListState[maxTurnoutList];


int routeListSize = 0;
int routeListIndex[maxRouteList];
String routeListSysName[maxRouteList];
String routeListUserName[maxRouteList];
int routeListState[maxRouteList];


boolean functionStates[MAX_FUNCTIONS];


String functionLabels[MAX_FUNCTIONS];


int functionFollow[MAX_FUNCTIONS];


int currentSpeedStep;


int currentThrottleIndex = 0;
char currentThrottleIndexChar = '0';

int heartBeatPeriod = 10;
long lastServerResponseTime;
boolean heartbeatCheckEnabled = true;


long lastSpeedSentTime = 0;
int lastSpeedSent = 0;

int lastSpeedThrottleIndex = 0;




const char* deviceName = DEVICE_NAME;

static unsigned long rotaryEncoderButtonLastTimePressed = 0;
const int rotaryEncoderButtonEncoderDebounceTime = ROTARY_ENCODER_DEBOUNCE_TIME;

const boolean encoderRotationClockwiseIsIncreaseSpeed = ENCODER_ROTATION_CLOCKWISE_IS_INCREASE_SPEED;







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


int additionalButtonActions[MAX_ADDITIONAL_BUTTONS] = {
                          CHOSEN_ADDITIONAL_BUTTON_0_FUNCTION,
                          CHOSEN_ADDITIONAL_BUTTON_1_FUNCTION,
                          CHOSEN_ADDITIONAL_BUTTON_2_FUNCTION,
                          CHOSEN_ADDITIONAL_BUTTON_3_FUNCTION,
                          CHOSEN_ADDITIONAL_BUTTON_4_FUNCTION,
                          CHOSEN_ADDITIONAL_BUTTON_5_FUNCTION,
                          CHOSEN_ADDITIONAL_BUTTON_6_FUNCTION
};
unsigned long lastAdditionalButtonDebounceTime[MAX_ADDITIONAL_BUTTONS] = {0,0,0,0,0,0,0};
unsigned long additionalButtonDebounceDelay = ADDITIONAL_BUTTON_DEBOUNCE_DELAY;
boolean additionalButtonRead[MAX_ADDITIONAL_BUTTONS];
boolean additionalButtonLastRead[MAX_ADDITIONAL_BUTTONS];
void displayUpdateFromWit();
int getMultiThrottleIndex(char multiThrottle);
char getMultiThrottleChar(int multiThrottleIndex);
void ssidsLoop();
void browseSsids();
void selectSsidFromFound(int selection);
void getSsidPasswordAndWitIpForFound();
void enterSsidPassword();
void showListOfSsids();
void selectSsid(int selection);
void connectSsid();
void witServiceLoop();
void browseWitService();
void selectWitServer(int selection);
void connectWitServer();
void enterWitServer();
void disconnectWitServer();
void witEntryAddChar(char key);
void witEntryDeleteChar(char key);
void ssidPasswordAddChar(char key);
void ssidPasswordDeleteChar(char key);
void buildWitEntry();
void IRAM_ATTR readEncoderISR();
void rotary_onButtonClick();
void rotary_loop();
void encoderSpeedChange(bool rotationIsClockwise, int speedChange);
void throttlePot_loop(bool forceRead);
void reverserPot_loop();
void brakePot_loop();
void speedAdjust_loop();
void targetSpeedOverride();
void keypadEvent(KeypadEvent key);
void initialiseAdditionalButtons();
void additionalButtonLoop();
void setup();
void loop();
void doKeyPress(char key, bool pressed);
void doDirectCommand (char key, bool pressed);
void doDirectAdditionalButtonCommand (int buttonIndex, bool pressed);
void doDirectAction(int buttonAction);
void doMenu();
void resetMenu();
void resetFunctionStates();
void resetFunctionLabels();
void resetAllFunctionLabels();
void resetAllFunctionFollow();
String getLocoWithLength(String loco);
void speedEstop();
void speedEstopCurrentLoco();
void speedDown(int amt);
void speedUp(int amt);
void speedSet(int amt);
int getDisplaySpeed();
void toggleLocoFacing(String loco);
int getLocoFacing(String loco);
String getDisplayLocoString(int index);
void releaseAllLocos();
void releaseOneLoco(String loco);
void toggleAccelerationDelayTime();
void toggleHeartbeatCheck();
void toggleDirection();
void changeDirection(Direction direction);
void doDirectFunction(int functionNumber, bool pressed);
void doFunction(int functionNumber, bool pressed);
void doFunctionWhichLocosInConsist(int functionNumber, boolean pressed);
void powerOnOff(TrackPower powerState);
void powerToggle();
void stopThenToggleDirection();
void reconnect();
void setLastServerResponseTime(bool force);
void checkForShutdownOnNoResponse();
void selectRoster(int selection);
void selectTurnoutList(int selection, TurnoutAction action);
void selectRouteList(int selection);
void selectFunctionList(int selection);
void selectEditConsistList(int selection);
void setAppnameForOled();
void setMenuTextForOled(int menuTextIndex);
void refreshOled();
void writeOledFoundSSids(String soFar);
void writeOledRoster(String soFar);
void writeOledTurnoutList(String soFar, TurnoutAction action);
void writeOledRouteList(String soFar);
void writeOledFunctionList(String soFar);
void writeOledEnterPassword();
void writeOledMenu(String soFar);
void writeOledExtraSubMenu();
void writeOledAllLocos(bool hideLeadLoco);
void writeOledEditConsist();
void writeHeartbeatCheck();
void writeOledSpeed();
void writeOledFunctions();
void writeOledPotValues();
void writeOledArray(bool isThreeColums, bool isPassword);
void writeOledArray(bool isThreeColums, bool isPassword, bool sendBuffer);
void writeOledArray(bool isThreeColums, bool isPassword, bool sendBuffer, bool drawTopLine);
void clearOledArray();
void writeOledDirectCommands();
void deepSleepStart();
void deepSleepStart(bool autoShutdown);
#line 289 "C:/Users/akers/Documents/GitHub/HandCab/HandCab.ino"
void displayUpdateFromWit() {
  debug_print("displayUpdateFromWit(): keyapdeUseType "); debug_print(keypadUseType);
  debug_print(" menuIsShowing "); debug_print(menuIsShowing);
  debug_println("");
  if ( (keypadUseType==KEYPAD_USE_OPERATION) && (!menuIsShowing) ) {
    writeOledSpeed();
  }
}


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
    void receivedSpeedMultiThrottle(char multiThrottle, int speed) {
      debug_print("Received Speed: ("); debug_print(millis()); debug_print(") throttle: "); debug_print(multiThrottle); debug_print(" speed: "); debug_println(speed);
      int multiThrottleIndex = getMultiThrottleIndex(multiThrottle);

      if (currentSpeed != speed) {


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
    void receivedDirectionMultiThrottle(char multiThrottle, Direction dir) {
      debug_print("Received Direction: "); debug_println(dir);


      if (currentDirection != dir) {
        currentDirection = dir;
        displayUpdateFromWit();
      }
    }
    void receivedFunctionStateMultiThrottle(char multiThrottle, uint8_t func, bool state) {
      debug_print("Received Fn: "); debug_print(func); debug_print(" State: "); debug_println( (state) ? "True" : "False" );


      if (functionStates[func] != state) {
        functionStates[func] = state;
        displayUpdateFromWit();
      }
    }
    void receivedRosterFunctionListMultiThrottle(char multiThrottle, String functions[MAX_FUNCTIONS]) {
      debug_println("Received Fn List: ");


      for(int i = 0; i < MAX_FUNCTIONS; i++) {
        functionLabels[i] = functions[i];
        debug_print(" Function: "); debug_print(i); debug_print(" - "); debug_println( functions[i] );
      }
    }
    void receivedTrackPower(TrackPower state) {
      debug_print("Received TrackPower: "); debug_println(state);
      if (trackPower != state) {
        trackPower = state;
        displayUpdateFromWit();
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

void browseSsids() {
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

  int numSsids = WiFi.scanNetworks();
  while ( (numSsids == -1)
    && ((nowTime-startTime) <= 10000) ) {
    delay(250);
    debug_print(".");
    nowTime = millis();
  }

  startWaitForSelection = millis();

  foundSsidsCount = 0;
  if (numSsids == -1) {
    debug_println("Couldn't get a wifi connection");

  } else {
    for (int thisSsid = 0; thisSsid < numSsids; thisSsid++) {

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
  if (ssidPasswordChanged) {
    debug_println("enterSsidPassword()");
    writeOledEnterPassword();
    ssidPasswordChanged = false;
  }
}
void showListOfSsids() {
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
    debug_print(maxSsids); debug_println(MSG_SSIDS_LISTED);
    clearOledArray(); oledText[10] = MSG_SSIDS_LISTED;

    for (int i = 0; i < maxSsids; ++i) {
      debug_print(i+1); debug_print(": "); debug_println(ssids[i]);
      int j = i;
      if (i>=5) {
        j=i+1;
      }
      if (i<=10) {
        oledText[j] = String(i) + ": ";
        if (ssids[i].length()<9) {
          oledText[j] = oledText[j] + ssids[i];
        } else {
          oledText[j] = oledText[j] + ssids[i].substring(0,9) + "..";
        }
      }
    }

    if (maxSsids > 0) {

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
    for (int i = 0; i < 3; ++i) {
      oledText[1] = selectedSsid; oledText[2] = String(MSG_TRYING_TO_CONNECT) + " (" + String(i) + ")";
      writeOledArray(false, false, true, true);

      nowTime = startTime;
      WiFi.begin(cSsid, cPassword);

      debug_print("Trying Network ... Checking status "); debug_print(cSsid); debug_print(" :"); debug_print(cPassword); debug_println(":");
      while ( (WiFi.status() != WL_CONNECTED)
        && ((nowTime-startTime) <= SSID_CONNECTION_TIMEOUT) ) {
        delay(250);
        debug_print(".");
        nowTime = millis();
      }

      if (WiFi.status() == WL_CONNECTED) {
        if (selectedSsid.indexOf(SSID_NAME_FOR_COMMANDS_NEED_LEADING_CR_LF)>=0) {
          commandsNeedLeadingCrLf = true;
          debug_print(SSID_NAME_FOR_COMMANDS_NEED_LEADING_CR_LF); debug_println(" - Commands need to be sent twice");
        }

        break;
      } else {
        debug_println("");
      }
    }

    debug_println("");
    if (WiFi.status() == WL_CONNECTED) {
      debug_print("Connected. IP address: "); debug_println(WiFi.localIP());
      oledText[2] = MSG_CONNECTED;
      oledText[3] = MSG_ADDRESS_LABEL + String(WiFi.localIP());
      writeOledArray(false, false, true, true);

      ssidConnectionState = CONNECTION_STATE_CONNECTED;
      keypadUseType = KEYPAD_USE_SELECT_WITHROTTLE_SERVER;


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
  oledText[1] = selectedSsid; oledText[2] = MSG_BROWSING_FOR_SERVICE;
  writeOledArray(false, false, true, true);

  noOfWitServices = 0;
  if ( (selectedSsid.substring(0,6) == "DCCEX_") && (selectedSsid.length()==12) ) {
    debug_println(MSG_BYPASS_WIT_SERVER_SEARCH);
    oledText[1] = MSG_BYPASS_WIT_SERVER_SEARCH;
    writeOledArray(false, false, true, true);
    delay(500);
  } else {
    while ( (noOfWitServices == 0)
      && ((nowTime-startTime) <= 5000) ) {
      noOfWitServices = MDNS.queryService(service, proto);
      if (noOfWitServices == 0 ) {
        delay(500);
        debug_print(".");
      }
      nowTime = millis();
    }
    debug_println("");
  }


  foundWitServersCount = noOfWitServices;
  if (noOfWitServices > 0) {
    for (int i = 0; ((i < noOfWitServices) && (i<maxFoundWitServers)); ++i) {
      foundWitServersNames[i] = MDNS.hostname(i);
      foundWitServersIPs[i] = MDNS.IP(i);
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
    debug_print(noOfWitServices); debug_println(MSG_SERVICES_FOUND);
    clearOledArray(); oledText[1] = MSG_SERVICES_FOUND;

    for (int i = 0; i < foundWitServersCount; ++i) {

      debug_print("  "); debug_print(i); debug_print(": '"); debug_print(foundWitServersNames[i]);
      debug_print("' ("); debug_print(foundWitServersIPs[i]); debug_print(":"); debug_print(foundWitServersPorts[i]); debug_println(")");
      if (i<5) {
        String truncatedIp = ".." + foundWitServersIPs[i].toString().substring(foundWitServersIPs[i].toString().lastIndexOf("."));
        oledText[i] = String(i) + ": " + truncatedIp + ":" + String(foundWitServersPorts[i]) + " " + foundWitServersNames[i];
      }
    }

    if (foundWitServersCount > 0) {

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

  wiThrottleProtocol.setDelegate(&myDelegate);
#if WITHROTTLE_PROTOCOL_DEBUG == 0
  wiThrottleProtocol.setLogStream(&Serial);
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
    debug_print("Connected to server: "); debug_println(selectedWitServerIP); debug_println(selectedWitServerPort);


    wiThrottleProtocol.connect(&client, outboundCmdsMininumDelay);
    debug_println("WiThrottle connected");

    wiThrottleProtocol.setDeviceName(deviceName);
    wiThrottleProtocol.setDeviceID(String(deviceId));
    wiThrottleProtocol.setCommandsNeedLeadingCrLf(commandsNeedLeadingCrLf);

    witConnectionState = CONNECTION_STATE_CONNECTED;
    setLastServerResponseTime(true);

    oledText[3] = MSG_CONNECTED;
    if (!hashShowsFunctionsInsteadOfKeyDefs) {

      setMenuTextForOled(menu_menu);
    } else {

      setMenuTextForOled(menu_menu_hash_is_functions);
    }
    writeOledArray(false, false, true, true);

    keypadUseType = KEYPAD_USE_OPERATION;
  }
}

void enterWitServer() {
  keypadUseType = KEYPAD_USE_ENTER_WITHROTTLE_SERVER;
  if (witServerIpAndPortChanged) {
    debug_println("enterWitServer()");
    clearOledArray();
    setAppnameForOled();
    oledText[1] = MSG_NO_SERVICES_FOUND_ENTRY_REQUIRED;
    oledText[3] = witServerIpAndPortConstructed;

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

      if ( (millis() - rotaryEncoderButtonLastTimePressed) < rotaryEncoderButtonEncoderDebounceTime) {
        debug_println("encoder button debounce");
        return;
      }
      rotaryEncoderButtonLastTimePressed = millis();

      doDirectAction(encoderButtonAction);

      debug_println("encoder button pressed");
      writeOledSpeed();
    } else {
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
  if (rotaryEncoder.encoderChanged()) {

    encoderValue = rotaryEncoder.readEncoder();
    debug_print("Encoder From: "); debug_print(lastEncoderValue); debug_print(" to: "); debug_println(encoderValue);

    if ( (millis() - rotaryEncoderButtonLastTimePressed) < rotaryEncoderButtonEncoderDebounceTime) {
      debug_println("encoder button debounce - in Rotary_loop()");
      return;
    }

    if (abs(encoderValue-lastEncoderValue) > 800) {
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
    } else {
        if (encoderValue > lastEncoderValue) {
          if (ssidPasswordCurrentChar==ssidPasswordBlankChar) {
            ssidPasswordCurrentChar = 66;
          } else {
            ssidPasswordCurrentChar = ssidPasswordCurrentChar - 1;
            if ((ssidPasswordCurrentChar < 32) ||(ssidPasswordCurrentChar > 126) ) {
              ssidPasswordCurrentChar = 126;
            }
          }
        } else {
          if (ssidPasswordCurrentChar==ssidPasswordBlankChar) {
            ssidPasswordCurrentChar = 64;
          } else {
            ssidPasswordCurrentChar = ssidPasswordCurrentChar + 1;
            if (ssidPasswordCurrentChar > 126) {
              ssidPasswordCurrentChar = 32;
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





void throttlePot_loop(bool forceRead) {

  int currentThrottlePotNotch = throttlePotNotch;
  int potValue = ( analogRead(throttlePotPin) );


  int noElements = sizeof(lastThrottlePotValues) / sizeof(lastThrottlePotValues[0]);
  int avgPotValue = 0;
  for (int i=1; i<noElements; i++) {
    lastThrottlePotValues[i-1] = lastThrottlePotValues[i];
    avgPotValue = avgPotValue + lastThrottlePotValues[i-1];
  }
  lastThrottlePotValues[noElements-1] = potValue;
  avgPotValue = (avgPotValue + potValue) / noElements;



  if ( (avgPotValue!=lastThrottlePotValue)
  || (forceRead) ) {
    lastThrottlePotValue = avgPotValue;
    noElements = sizeof(throttlePotNotchValues) / sizeof(throttlePotNotchValues[0]);


      throttlePotNotch = 0;
      for (int i=0; i<noElements; i++) {
        if (avgPotValue < throttlePotNotchValues[i]) {
          throttlePotTargetSpeed = throttlePotNotchSpeeds[i];
          throttlePotNotch = i;
          break;
        }
      }
      if ( (throttlePotNotch!=currentThrottlePotNotch)
      || (forceRead) ) {
            targetSpeed = throttlePotTargetSpeed;
            targetSpeedAndDirectionOverride();
            if (!forceRead) startMomentumTimerMillis = millis();
      }
# 1178 "C:/Users/akers/Documents/GitHub/HandCab/HandCab.ino"
    refreshOled();
  }
}





void reverserPot_loop() {
  int lastReverserPosition = reverserCurrentPosition;
  int potValue = ( analogRead(reverserPotPin) );


  int noElements = sizeof(lastReverserPotValues) / sizeof(lastReverserPotValues[0]);
  int avgPotValue = 0;
  for (int i=1; i<noElements; i++) {
    lastReverserPotValues[i-1] = lastReverserPotValues[i];
    avgPotValue = avgPotValue + lastReverserPotValues[i-1];
  }
  lastReverserPotValues[noElements-1] = potValue;
  avgPotValue = (avgPotValue + potValue) / noElements;


  if (avgPotValue!=lastReverserPotValue) {

    lastReverserPotValue = avgPotValue;

    if (lastReverserPotValue < reverserPotValues[0]) {
      reverserCurrentPosition = REVERSER_POSITION_FORWARD;
    } else if (lastReverserPotValue < reverserPotValues[1]) {
      reverserCurrentPosition = REVERSER_POSITION_NEUTRAL;
    } else {
      reverserCurrentPosition = REVERSER_POSITION_REVERSE;
    }

    if (lastReverserPosition != reverserCurrentPosition) {
      if (reverserCurrentPosition==REVERSER_POSITION_FORWARD) {
        debug_println("Reverser - Forward");

        targetDirection = FORWARD;
      } else if (reverserCurrentPosition==REVERSER_POSITION_REVERSE) {
        debug_println("Reverser - Reverse");

        targetDirection = REVERSE;
      } else {
        debug_println("Reverser - Neutral");
        targetDirection = NEUTRAL;
      }
      throttlePot_loop(true);
      targetSpeedOverride();
    }
  }
  refreshOled();
}





void brakePot_loop() {
  int potValue = ( analogRead(brakePotPin) );


  int noElements = sizeof(lastBrakePotValues) / sizeof(lastBrakePotValues[0]);
  int avgPotValue = 0;
  for (int i=1; i<noElements; i++) {
    lastBrakePotValues[i-1] = lastBrakePotValues[i];
    avgPotValue = avgPotValue + lastBrakePotValues[i-1];
  }
  lastBrakePotValues[noElements-1] = potValue;
  avgPotValue = (avgPotValue + potValue) / noElements;


  if (avgPotValue!=lastBrakePotValue) {

    lastBrakePotValue = avgPotValue;

    noElements = sizeof(brakeDelayTimes) / sizeof(brakeDelayTimes[0]);
    currentBrakeDelayTime = 0;
    for (int i=0; i<noElements; i++) {
      if (avgPotValue < brakePotValues[i]) {
        brakeCurrentPosition = i;
        currentBrakeDelayTime = brakeDelayTimes[i];
        throttlePot_loop(true);
        targetSpeedOverride();
        break;
      }
    }

    refreshOled();
  }
}






void speedAdjust_loop() {
  if (wiThrottleProtocol.getNumberOfLocomotives(getMultiThrottleChar(0)) > 0) {
    int changeAmount = 0;
    if (currentSpeed!=targetSpeed) {
      debug_print("speedAdjust_loop() target: "); debug_print(targetSpeed);
      debug_print(" current: "); debug_println(currentSpeed);
      if (currentSpeed>targetSpeed) {
        if (millis() - startMomentumTimerMillis >= currentBrakeDelayTime) {
          changeAmount = -1 * DCC_SPEED_CHANGE_AMOUNT;
          if (currentSpeed-changeAmount < targetSpeed) changeAmount = targetSpeed-currentSpeed;
        }
      }
      else {
        if (millis() - startMomentumTimerMillis >= currentAccellerationDelayTime) {
          startMomentumTimerMillis = millis();
          changeAmount = 1 * DCC_SPEED_CHANGE_AMOUNT;
          if (currentSpeed+changeAmount > targetSpeed) changeAmount = currentSpeed-targetSpeed;
        }
      }
      if (changeAmount!=0) {
        startMomentumTimerMillis = millis();
        speedSet(currentSpeed + changeAmount);
      }
    }
  } else {
    startMomentumTimerMillis = -1;
  }
}




void targetSpeedOverride() {


  if (brakeCurrentPosition>0) {
    targetSpeed = 0;
  }


  if (reverserCurrentPosition==REVERSER_POSITION_FORWARD) {
    if (currentSpeed==0) {
      changeDirection(Forward);
      return;
    }

  } else if (reverserCurrentPosition==REVERSER_POSITION_NEUTRAL) {
    targetSpeed = 0;
    return;

  } else {
    if (currentSpeed==0) {
      changeDirection(Reverse);
      return;
    }

  }
}





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

      if (additionalButtonLastRead[i] != buttonRead) {
        if ((millis() - lastAdditionalButtonDebounceTime[i]) > additionalButtonDebounceDelay) {
          lastAdditionalButtonDebounceTime[i] = millis();
          additionalButtonRead[i] = buttonRead;

          if ( ((additionalButtonType[i] == INPUT_PULLUP) && (additionalButtonRead[i] == LOW))
              || ((additionalButtonType[i] == INPUT) && (additionalButtonRead[i] == HIGH)) ) {
            debug_print("Additional Button Pressed: "); debug_print(i); debug_print(" pin:"); debug_print(additionalButtonPin[i]); debug_print(" action:"); debug_println(additionalButtonActions[i]);
            if (wiThrottleProtocol.getNumberOfLocomotives(currentThrottleIndexChar) > 0) {
              doDirectAdditionalButtonCommand(i,true);
            } else {
              int buttonAction = additionalButtonActions[i];
              if (buttonAction >= 500) {
                  doDirectAdditionalButtonCommand(i,true);
              }
            }
          } else {
            debug_print("Additional Button Released: "); debug_print(i); debug_print(" pin:"); debug_print(additionalButtonPin[i]); debug_print(" action:"); debug_println(additionalButtonActions[i]);
            if (wiThrottleProtocol.getNumberOfLocomotives(currentThrottleIndexChar) > 0) {
              doDirectAdditionalButtonCommand(i,false);
            } else {
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





void setup() {
  Serial.begin(115200);
  u8g2.begin();


  clearOledArray(); oledText[0] = appName; oledText[6] = appVersion; oledText[2] = MSG_START;
  writeOledArray(false, false, true, true);

  delay(1000);
  debug_println("Start");

  rotaryEncoder.begin();
  rotaryEncoder.setup(readEncoderISR);

  rotaryEncoder.setBoundaries(0, 1000, circleValues);

  rotaryEncoder.setAcceleration(100);

  keypad.addEventListener(keypadEvent);
  keypad.setDebounceTime(KEYPAD_DEBOUNCE_TIME);
  keypad.setHoldTime(KEYPAD_HOLD_TIME);

  esp_sleep_enable_ext0_wakeup(GPIO_NUM_13,0);

  keypadUseType = KEYPAD_USE_SELECT_SSID;
  encoderUseType = ENCODER_USE_OPERATION;
  ssidSelectionSource = SSID_CONNECTION_SOURCE_BROWSE;

  initialiseAdditionalButtons();

  resetAllFunctionLabels();
  resetAllFunctionFollow();

  currentSpeed = 0;
  currentDirection = Forward;
  currentSpeedStep = speedStep;
}





void loop() {

  if (ssidConnectionState != CONNECTION_STATE_CONNECTED) {

    ssidsLoop();
    checkForShutdownOnNoResponse();
  } else {
    if (witConnectionState != CONNECTION_STATE_CONNECTED) {
      witServiceLoop();
    } else {
      wiThrottleProtocol.check();

      setLastServerResponseTime(false);

      if ( (lastServerResponseTime+(heartBeatPeriod*4) < millis()/1000)
      && (heartbeatCheckEnabled) ) {
        debug_print("Disconnected - Last:"); debug_print(lastServerResponseTime); debug_print(" Current:"); debug_println(millis()/1000);
        reconnect();
      }
    }
  }

  keypad.getKey();
  if (witConnectionState != CONNECTION_STATE_CONNECTED) {
    rotary_loop();
  }
  if (witConnectionState == CONNECTION_STATE_CONNECTED) {
    throttlePot_loop(false);
    reverserPot_loop();
    brakePot_loop();
    speedAdjust_loop();
  }

  additionalButtonLoop();


}





void doKeyPress(char key, bool pressed) {
  debug_print("doKeyPress(): ");

  if (pressed) {
    switch (keypadUseType) {
      case KEYPAD_USE_OPERATION:
        debug_print("key operation... "); debug_println(key);
        switch (key) {
          case '*':
            menuCommand = "";
            if (menuCommandStarted) {
              resetMenu();
              writeOledSpeed();
            } else {
              menuCommandStarted = true;
              debug_println("Command started");
              writeOledMenu("");
            }
            break;

          case '#':
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

            if (menuCommandStarted) {

              if ((menuCharsRequired[key-48] == 0) && (menuCommand.length() == 0)) {
                menuCommand += key;
                doMenu();
              } else {
                if ((menuCharsRequired[menuCommand.substring(0,1).toInt()] == 1) && (menuCommand.length() == 1)) {
                  menuCommand += key;
                  doMenu();

                } else {
                  menuCommand += key;
                  writeOledMenu(menuCommand);
                }
              }
            } else {
              doDirectCommand(key, true);
            }
            break;

          default:
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
          case '*':
            witEntryDeleteChar(key);
            break;
          case '#':
            if (witServerIpAndPortEntered.length() == 17) {
              witConnectionState = CONNECTION_STATE_ENTERED;
            }
            break;
          default:
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
          case '*':
            ssidPasswordDeleteChar(key);
            break;
          case '#':
              selectedSsidPassword = ssidPasswordEntered;
              encoderUseType = ENCODER_USE_OPERATION;
              keypadUseType = KEYPAD_USE_OPERATION;
              ssidConnectionState = CONNECTION_STATE_SELECTED;
            break;
          default:
            break;
        }

        break;

      case KEYPAD_USE_SELECT_WITHROTTLE_SERVER:
        debug_print("key: Select wit... "); debug_println(key);
        switch (key){
          case '0': case '1': case '2': case '3': case '4':
            selectWitServer(key - '0');
            break;
          case '#':
            witConnectionState = CONNECTION_STATE_ENTRY_REQUIRED;
            buildWitEntry();
            enterWitServer();
            break;
          default:
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
          case '#':
            ssidConnectionState = CONNECTION_STATE_DISCONNECTED;
            keypadUseType = KEYPAD_USE_SELECT_SSID_FROM_FOUND;
            ssidSelectionSource = SSID_CONNECTION_SOURCE_BROWSE;

            break;
          default:
            break;
        }
        break;

      case KEYPAD_USE_SELECT_SSID_FROM_FOUND:
        debug_print("key ssid from found... "); debug_println(key);
        switch (key){
          case '0': case '1': case '2': case '3': case '4':
            selectSsidFromFound(key - '0'+(page*5));
            break;
          case '#':
            if (foundSsidsCount>5) {
              if ( (page+1)*5 < foundSsidsCount ) {
                page++;
              } else {
                page = 0;
              }
              writeOledFoundSSids("");
            }
            break;
          case '9':
            ssidConnectionState = CONNECTION_STATE_DISCONNECTED;
            keypadUseType = KEYPAD_USE_SELECT_SSID;
            ssidSelectionSource = SSID_CONNECTION_SOURCE_LIST;
            break;
          default:
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
          case '#':
            if ( rosterSize > 5 ) {
              if ( (page+1)*5 < rosterSize ) {
                page++;
              } else {
                page = 0;
              }
              writeOledRoster("");
            }
            break;
          case '*':
            resetMenu();
            writeOledSpeed();
            break;
          default:
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
          case '#':
            if ( turnoutListSize > 10 ) {
              if ( (page+1)*10 < turnoutListSize ) {
                page++;
              } else {
                page = 0;
              }
              writeOledTurnoutList("", (keypadUseType == KEYPAD_USE_SELECT_TURNOUTS_THROW) ? TurnoutThrow : TurnoutClose);
            }
            break;
          case '*':
            resetMenu();
            writeOledSpeed();
            break;
          default:
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
          case '#':
            if ( routeListSize > 10 ) {
              if ( (page+1)*10 < routeListSize ) {
                page++;
              } else {
                page = 0;
              }
              writeOledRouteList("");
            }
            break;
          case '*':
            resetMenu();
            writeOledSpeed();
            break;
          default:
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
          case '#':
            if ( (functionPage+1)*10 < MAX_FUNCTIONS ) {
              functionPage++;
              writeOledFunctionList("");
            } else {
              functionPage = 0;
              keypadUseType = KEYPAD_USE_OPERATION;
              writeOledDirectCommands();
            }
            break;
          case '*':
            resetMenu();
            writeOledSpeed();
            break;
          default:
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
          case '*':
            resetMenu();
            writeOledSpeed();
            break;
          default:
            break;
        }
        break;


      case KEYPAD_USE_POT_VALUES:
        debug_print("Showing Pot values... "); debug_println(key);
        switch (key){
          case '0': case '1': case '2': case '3': case '4':
          case '5': case '6': case '7': case '8': case '9':

            break;
          case '*':
            resetMenu();
            writeOledSpeed();
            break;
          default:
            break;
        }
        break;


      default:
        break;
    }

  } else {
    if (keypadUseType == KEYPAD_USE_OPERATION) {
      if ( (!menuCommandStarted) && (key>='0') && (key<='D')) {
        debug_println("Operation - Process key release");
        doDirectCommand(key, false);
      } else {
        debug_println("Non-Operation - Process key release");
      }
    }
  }


}

void doDirectCommand (char key, bool pressed) {
  debug_print("doDirectCommand(): "); debug_println(key);
  int buttonAction = 0 ;
  if (key<=57) {
    buttonAction = buttonActions[(key - '0')];
  } else {
    buttonAction = buttonActions[(key - 55)];
  }
  debug_print("doDirectCommand(): Action: "); debug_println(buttonAction);
  if (buttonAction!=FUNCTION_NULL) {
    if ( (buttonAction>=FUNCTION_0) && (buttonAction<=FUNCTION_31) ) {
      doDirectFunction(buttonAction, pressed);
  } else {
      if (pressed) {
        doDirectAction(buttonAction);
      }
    }
  }

}

void doDirectAdditionalButtonCommand (int buttonIndex, bool pressed) {
  debug_print("doDirectAdditionalButtonCommand(): "); debug_println(buttonIndex);
  int buttonAction = additionalButtonActions[buttonIndex];
  if (buttonAction!=FUNCTION_NULL) {
    if ( (buttonAction>=FUNCTION_0) && (buttonAction<=FUNCTION_31) ) {
      doDirectFunction(buttonAction, pressed);
  } else {
      if (pressed) {
        doDirectAction(buttonAction);
      }
    }
  }

}

void doDirectAction(int buttonAction) {
  debug_println("doDirectAction(): ");
  switch (buttonAction) {
# 1913 "C:/Users/akers/Documents/GitHub/HandCab/HandCab.ino"
      case SPEED_MULTIPLIER: {
        toggleAccelerationDelayTime();
        break;
      }
      case E_STOP: {
        speedEstop();
        break;
      }
      case E_STOP_CURRENT_LOCO: {
        speedEstopCurrentLoco();
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
# 1945 "C:/Users/akers/Documents/GitHub/HandCab/HandCab.ino"
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

}

void doMenu() {
  String loco = "";
  String function = "";
  boolean result = false;

  debug_print("Menu: "); debug_println(menuCommand);

  switch (menuCommand[0]) {
    case MENU_ITEM_ADD_LOCO: {
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
    case MENU_ITEM_DROP_LOCO: {
        loco = menuCommand.substring(1, menuCommand.length());
        if (loco!="") {
          loco = getLocoWithLength(loco);
          releaseOneLoco(loco);
        } else {
          releaseAllLocos();
        }
        writeOledSpeed();
        break;
      }




     case MENU_ITEM_SPEED_STEP_MULTIPLIER: {

        toggleAccelerationDelayTime();
        break;
      }
   case MENU_ITEM_THROW_POINT: {
        if (menuCommand.length()>1) {
          String turnout = turnoutPrefix + menuCommand.substring(1, menuCommand.length());

            debug_print("throw point: "); debug_println(turnout);
            wiThrottleProtocol.setTurnout(turnout, TurnoutThrow);

          writeOledSpeed();
        } else {
          page = 0;
          writeOledTurnoutList("", TurnoutThrow);
        }
        break;
      }
    case MENU_ITEM_CLOSE_POINT: {
        if (menuCommand.length()>1) {
          String turnout = turnoutPrefix + menuCommand.substring(1, menuCommand.length());

            debug_print("close point: "); debug_println(turnout);
            wiThrottleProtocol.setTurnout(turnout, TurnoutClose);

          writeOledSpeed();
        } else {
          page = 0;
          writeOledTurnoutList("",TurnoutClose);
        }
        break;
      }
    case MENU_ITEM_ROUTE: {
        if (menuCommand.length()>1) {
          String route = routePrefix + menuCommand.substring(1, menuCommand.length());

            debug_print("route: "); debug_println(route);
            wiThrottleProtocol.setRoute(route);

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
    case MENU_ITEM_EXTRAS: {
        char subCommand = menuCommand.charAt(1);
        if (menuCommand.length() > 1) {
          switch (subCommand) {
            case EXTRA_MENU_CHAR_FUNCTION_KEY_TOGGLE: {
                hashShowsFunctionsInsteadOfKeyDefs = !hashShowsFunctionsInsteadOfKeyDefs;
                writeOledSpeed();
                break;
              }
            case EXTRA_MENU_CHAR_EDIT_CONSIST: {
                writeOledEditConsist();
                break;
              }
            case EXTRA_MENU_CHAR_POT_VALUES: {
                writeOledPotValues();
                break;
              }
            case EXTRA_MENU_CHAR_HEARTBEAT_TOGGLE: {
                toggleHeartbeatCheck();
                writeOledSpeed();
                break;
              }
            case EXTRA_MENU_CHAR_DISCONNECT: {
                if (witConnectionState == CONNECTION_STATE_CONNECTED) {
                  witConnectionState = CONNECTION_STATE_DISCONNECTED;
                  disconnectWitServer();
                } else {
                  connectWitServer();
                }
                break;
              }
            case EXTRA_MENU_CHAR_OFF_SLEEP:
            case EXTRA_MENU_CHAR_OFF_SLEEP_HIDDEN: {
                deepSleepStart();
                break;
              }
          }
        } else {
          writeOledSpeed();
        }
        break;
      }
    case MENU_ITEM_FUNCTION: {
        if (menuCommand.length()>1) {
          function = menuCommand.substring(1, menuCommand.length());
          int functionNumber = function.toInt();
          if (function != "") {
            doFunction(functionNumber, true);
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
  wiThrottleProtocol.emergencyStop();
  currentSpeed = 0;
  debug_println("Speed EStop");
  writeOledSpeed();
}

void speedEstopCurrentLoco() {
  wiThrottleProtocol.emergencyStop();
  currentSpeed = 0;
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


    lastSpeedSentTime = millis();
    lastSpeedSent = newSpeed;

    lastSpeedThrottleIndex = 0;


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
  if (!wiThrottleProtocol.getLocomotiveAtPosition('0', 0).equals(loco)) {
    Direction leadLocoDirection
        = wiThrottleProtocol.getDirection('0',
                                          wiThrottleProtocol.getLocomotiveAtPosition('0', 0));


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
      writeOledSpeed();
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
# 2339 "C:/Users/akers/Documents/GitHub/HandCab/HandCab.ino"
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

      wiThrottleProtocol.setDirection('0', direction);

    } else {

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

  refreshOled();


}

void doDirectFunction(int functionNumber, bool pressed) {
  debug_println("doDirectFunction(): ");
  if (wiThrottleProtocol.getNumberOfLocomotives('0') > 0) {
    debug_print("direct fn: "); debug_print(functionNumber); debug_println( pressed ? " Pressed" : " Released");

    doFunctionWhichLocosInConsist(functionNumber, pressed);
    writeOledSpeed();
  }

}

void doFunction(int functionNumber, bool pressed) {
  debug_print("doFunction(): multiThrottleIndex "); debug_println('0');
  if (wiThrottleProtocol.getNumberOfLocomotives('0')>0) {

    doFunctionWhichLocosInConsist(functionNumber, true);
    if (!functionStates[functionNumber]) {
      debug_print("fn: "); debug_print(functionNumber); debug_println(" Pressed");
    } else {
      delay(20);

      doFunctionWhichLocosInConsist(functionNumber, false);
      debug_print("fn: "); debug_print(functionNumber); debug_println(" Released");
    }
    writeOledSpeed();
  }

}



void doFunctionWhichLocosInConsist(int functionNumber, boolean pressed) {
  if (functionFollow[functionNumber]==CONSIST_LEAD_LOCO) {
    wiThrottleProtocol.setFunction('0',functionNumber, pressed);
  } else {
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


void stopThenToggleDirection() {
  if (wiThrottleProtocol.getNumberOfLocomotives('0')>0) {
    if (currentSpeed != 0) {

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

  lastServerResponseTime = wiThrottleProtocol.getLastServerResponseTime();
  if ( (lastServerResponseTime==0) || (force) ) lastServerResponseTime = millis() /1000;

}

void checkForShutdownOnNoResponse() {
  if (millis()-startWaitForSelection > 240000) {
      debug_println("Waited too long for a selection. Shutting down");
      deepSleepStart();
  }
}





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
  }
}


void writeOledFoundSSids(String soFar) {
  menuIsShowing = true;
  keypadUseType = KEYPAD_USE_SELECT_SSID_FROM_FOUND;
  if (soFar == "") {
    clearOledArray();
    for (int i=0; i<5 && i<foundSsidsCount; i++) {
      if (foundSsids[(page*5)+i].length()>0) {
        oledText[i] = String(i) + ": " + foundSsids[(page*5)+i] + "   (" + foundSsidRssis[(page*5)+i] + ")" ;
      }
    }
    oledText[5] = "(" + String(page+1) + ") " + menu_text[menu_select_ssids_from_found];
    writeOledArray(false, false);


  }
}

void writeOledRoster(String soFar) {
  lastOledScreen = last_oled_screen_roster;
  lastOledStringParameter = soFar;

  menuIsShowing = true;
  keypadUseType = KEYPAD_USE_SELECT_ROSTER;
  if (soFar == "") {
    clearOledArray();
    for (int i=0; i<5 && i<rosterSize; i++) {
      if (rosterAddress[(page*5)+i] != 0) {
        oledText[i] = String(rosterIndex[i]) + ": " + rosterName[(page*5)+i] + " (" + rosterAddress[(page*5)+i] + ")" ;
      }
    }
    oledText[5] = "(" + String(page+1) + ") " + menu_text[menu_roster];
    writeOledArray(false, false);


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
  if (soFar == "") {
    clearOledArray();
    int j = 0;
    for (int i=0; i<10 && i<turnoutListSize; i++) {
      j = (i<5) ? i : i+1;
      if (turnoutListUserName[(page*10)+i].length()>0) {
        oledText[j] = String(turnoutListIndex[i]) + ": " + turnoutListUserName[(page*10)+i].substring(0,10);
      }
    }
    oledText[5] = "(" + String(page+1) + ") " + menu_text[menu_turnout_list];
    writeOledArray(false, false);


  }
}

void writeOledRouteList(String soFar) {
  lastOledScreen = last_oled_screen_route_list;
  lastOledStringParameter = soFar;

  menuIsShowing = true;
  keypadUseType = KEYPAD_USE_SELECT_ROUTES;
  if (soFar == "") {
    clearOledArray();
    int j = 0;
    for (int i=0; i<10 && i<routeListSize; i++) {
      j = (i<5) ? i : i+1;
      if (routeListUserName[(page*10)+i].length()>0) {
        oledText[j] = String(routeListIndex[i]) + ": " + routeListUserName[(page*10)+i].substring(0,10);
      }
    }
    oledText[5] = "(" + String(page+1) + ") " + menu_text[menu_route_list];
    writeOledArray(false, false);


  }
}

void writeOledFunctionList(String soFar) {
  lastOledScreen = last_oled_screen_function_list;
  lastOledStringParameter = soFar;

  menuIsShowing = true;
  keypadUseType = KEYPAD_USE_SELECT_FUNCTION;

  if (soFar == "") {
    clearOledArray();
    if (wiThrottleProtocol.getNumberOfLocomotives(currentThrottleIndexChar) > 0 ) {
      int j = 0; int k = 0;
      for (int i=0; i<10; i++) {
        k = (functionPage*10) + i;
        if (k < MAX_FUNCTIONS) {
          j = (i<5) ? i : i+1;

            oledText[j] = String(i) + ": "
            + ((k<10) ? functionLabels[k].substring(0,10) : String(k)
            + "-" + functionLabels[k].substring(0,7)) ;

            if (functionStates[k]) {
              oledTextInvert[j] = true;
            }

        }
      }
      oledText[5] = "(" + String(functionPage) + ") " + menu_text[menu_function_list];

    } else {
      oledText[0] = MSG_NO_FUNCTIONS;

      oledText[3] = MSG_NO_LOCO_SELECTED;

      setMenuTextForOled(menu_cancel);
    }
    writeOledArray(false, false);


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

  setMenuTextForOled(menu_enter_ssid_password);
  writeOledArray(false, true);
}

void writeOledMenu(String soFar) {
  lastOledScreen = last_oled_screen_menu;
  lastOledStringParameter = soFar;

  debug_print("writeOledMenu() : "); debug_println(soFar);
  menuIsShowing = true;
  bool drawTopLine = false;
  if (soFar == "") {
    clearOledArray();
    int j = 0;
    for (int i=1; i<10; i++) {
      j = (i<6) ? i : i+1;
      oledText[j-1] = String(i) + ": " + menuText[i][0];
    }
    oledText[10] = "0: " + menuText[0][0];

    setMenuTextForOled(menu_cancel);
    writeOledArray(false, false);
  } else {
    int cmd = menuCommand.substring(0, 1).toInt();

    clearOledArray();

    oledText[0] = ">> " + menuText[cmd][0] +":"; oledText[6] = menuCommand.substring(1, menuCommand.length());
    oledText[5] = menuText[cmd][1];

    switch (soFar.charAt(0)) {
      case MENU_ITEM_DROP_LOCO: {
            if (wiThrottleProtocol.getNumberOfLocomotives('0') > 0) {
              writeOledAllLocos(false);
              drawTopLine = true;
            }
          }
      case MENU_ITEM_FUNCTION: {

          if (wiThrottleProtocol.getNumberOfLocomotives('0') <= 0 ) {

            oledText[3] = MSG_NO_LOCO_SELECTED;

            setMenuTextForOled(menu_cancel);
          }
          break;
        }
      case MENU_ITEM_EXTRAS: {
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

  int startAt = (hideLeadLoco) ? 1 :0;
  debug_println("writeOledAllLocos(): ");
  String loco;
  int j = 0; int i = 0;
  if (wiThrottleProtocol.getNumberOfLocomotives(currentThrottleIndexChar) > 0) {
    for (int index=0; ((index < wiThrottleProtocol.getNumberOfLocomotives('0')) && (i < 8)); index++) {
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


  u8g2.setDrawColor(1);
  u8g2.setFont(FONT_GLYPHS);
  u8g2.drawGlyph(0, 38, glyph_speed_step);
  u8g2.setFont(FONT_DEFAULT);
  u8g2.drawStr(9, 37, String(currentAccellerationDelayTimeIndex).c_str());


  u8g2.setDrawColor(1);
  u8g2.setFont(FONT_GLYPHS);
  u8g2.drawGlyph(0, 28, glyph_brake_position);
  u8g2.setFont(FONT_DEFAULT);
  u8g2.drawStr(9, 27, String(brakeCurrentPosition).c_str());


  if (targetSpeed!=currentSpeed) {
    u8g2.setDrawColor(1);
    u8g2.setFont(FONT_GLYPHS);
    u8g2.drawGlyph(104, 48, glyph_target_speed);
    u8g2.setFont(FONT_DEFAULT);
    u8g2.drawStr(113, 48, String(targetSpeed).c_str());
  }


  u8g2.setDrawColor(1);
  u8g2.setFont(FONT_GLYPHS);
  if (targetDirection==FORWARD) {
    u8g2.drawGlyph(120, 40, glyph_target_direction_forward);
  } else if (targetDirection==NEUTRAL) {
    u8g2.drawGlyph(120, 40, glyph_target_direction_neutral);
  } else {
    u8g2.drawGlyph(120, 40, glyph_target_direction_reverse);
  }
  u8g2.setFont(FONT_DEFAULT);


  if (trackPower == PowerOn) {
    u8g2.drawRBox(0,40,9,9,1);
    u8g2.setDrawColor(0);
  }
  u8g2.setFont(FONT_GLYPHS);
  u8g2.drawGlyph(1, 48, glyph_track_power);
  u8g2.setDrawColor(1);


  if (!heartbeatCheckEnabled) {
    u8g2.setFont(FONT_GLYPHS);
    u8g2.drawGlyph(13, 49, glyph_heartbeat_off);
    u8g2.setDrawColor(2);
    u8g2.drawLine(13, 48, 20, 41);

    u8g2.setDrawColor(1);
  }



  u8g2.setFont(FONT_DIRECTION);
  u8g2.drawStr(79,36, sDirection.c_str());


  const char *cSpeed = sSpeed.c_str();

  u8g2.setFont(FONT_SPEED);
  int width = u8g2.getStrWidth(cSpeed);
  u8g2.drawStr(22+(55-width),45, cSpeed);

  u8g2.sendBuffer();


}

void writeOledFunctions() {
  lastOledScreen = last_oled_screen_speed;


   for (int i=0; i < MAX_FUNCTIONS; i++) {
    if (functionStates[i]) {

      u8g2.drawRBox(i*4+12,12+1,5,7,2);
      u8g2.setDrawColor(0);
      u8g2.setFont(FONT_FUNCTION_INDICATORS);
      u8g2.drawStr( i*4+1+12, 18+1, String( (i<10) ? i : ((i<20) ? i-10 : i-20)).c_str());
      u8g2.setDrawColor(1);
     }
   }

}


void writeOledPotValues() {
  lastOledScreen = last_oled_screen_pot_values;
  keypadUseType = KEYPAD_USE_POT_VALUES;
  menuIsShowing = true;

  clearOledArray();
  oledText[0] = MENU_ITEM_TEXT_TITLE_POT_VALUES;
  oledText[1] = POT_VALUE_TITLE_THROTTLE;
  oledText[2] = POT_VALUE_TITLE_REVERSER;
  oledText[3] = POT_VALUE_TITLE_BRAKE;
  oledText[7] = lastThrottlePotValue;
  oledText[8] = lastReverserPotValue;
  oledText[9] = lastBrakePotValue;
  oledText[5] = menuText[12][1];

  writeOledArray(false, false, true, true);
}



void writeOledArray(bool isThreeColums, bool isPassword) {
  writeOledArray(isThreeColums, isPassword, true, false);
}

void writeOledArray(bool isThreeColums, bool isPassword, bool sendBuffer) {
  writeOledArray(isThreeColums, isPassword, sendBuffer, false);
}

void writeOledArray(bool isThreeColums, bool isPassword, bool sendBuffer, bool drawTopLine) {

  u8g2.clearBuffer();

  u8g2.setFont(FONT_DEFAULT);

  int x=0;
  int y=10;
  int xInc = 64;
  int max = 12;
  if (isThreeColums) {
    xInc = 42;
    max = 18;
  }

  for (int i=0; i < max; i++) {
    const char *cLine1 = oledText[i].c_str();
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

  if (drawTopLine) u8g2.drawHLine(0,11,128);
  u8g2.drawHLine(0,51,128);

  if (sendBuffer) u8g2.sendBuffer();

}



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



void deepSleepStart() {
  deepSleepStart(false);
}

void deepSleepStart(bool autoShutdown) {
  clearOledArray();
  setAppnameForOled();
  int delayPeriod = 2000;
  if (autoShutdown) {
    oledText[2] = MSG_AUTO_SLEEP;
    delayPeriod = 10000;
  }
  oledText[3] = MSG_START_SLEEP;
  writeOledArray(false, false, true, true);
  delay(delayPeriod);

  u8g2.setPowerSave(1);
  esp_deep_sleep_start();
}