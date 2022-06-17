/**
 * This app turns the ESP32 into a Bluetooth LE keyboard that is intended to act as a dedicated
 * gamepad for the JMRI or any wiThrottle server.

  Instructions:
  - Update WiFi SSIDs and passwords as necessary in config_network.h.
  - Flash the sketch 
 */

#include <WiFi.h>                 // https://github.com/espressif/arduino-esp32/tree/master/libraries/WiFi
#include <ESPmDNS.h>              // https://github.com/espressif/arduino-esp32/blob/master/libraries/ESPmDNS
#include <WiThrottleProtocol.h>   // https://github.com/flash62au/WiThrottleProtocol
#include <AiEsp32RotaryEncoder.h> // https://github.com/igorantolic/ai-esp32-rotary-encoder
#include <Keypad.h>               // https://www.arduinolibraries.info/libraries/keypad
#include <U8g2lib.h>
#include <string>

#include "static.h"
#include "WiTcontroller.h"
#include "actions.h"
#include "config_network.h"
#include "config_keypad_etc.h"   // keypad, encoder, oled display
#include "config.h"

// comment out the following line to stop all console messages
#define DEBUG 1

#ifdef DEBUG
 #define debug_print(...) Serial.print(__VA_ARGS__)
 #define debug_println(...) Serial.println(__VA_ARGS__)
 #define debug_printf(...) Serial.printf(__VA_ARGS__)
#else
 #define debug_print(...)
 #define debug_println(...)
 #define debug_printf(...)
#endif

// *********************************************************************************

void displayUpdateFromWit() {
  if ( (keypadUseType==KEYPAD_USE_OPERATION) && (!menuIsShowing) ) {
    writeOledSpeed();
  }
}

// WiThrottleProtocol Delegate class
class MyDelegate : public WiThrottleProtocolDelegate {
  
  public:
    void receivedVersion(String version) {    
      debug_printf("Received Version: %s\n",version); 
    }
    void receivedServerDescription(String description) {
      debug_print("Received Description: ");
      debug_println(description);
    }
    void receivedSpeed(int speed) {             // Vnnn
      debug_print("Received Speed: "); debug_println(speed); 
      if (currentSpeed != speed) {
        currentSpeed = speed;
        displayUpdateFromWit();
      }
    }
    void receivedDirection(Direction dir) {     // R{0,1}
      debug_print("Received Direction: "); debug_println(dir); 
      if (currentDirection != dir) {
        currentDirection = dir;
        displayUpdateFromWit();
      }
    }
    void receivedFunctionState(uint8_t func, bool state) { 
      debug_print("Received Fn: "); debug_print(func); debug_print(" State: "); debug_println( (state) ? "True" : "False" );
      if (functionStates[func] != state) {
        functionStates[func] = state;
        displayUpdateFromWit();
      }
    }
    void receivedTrackPower(TrackPower state) { 
      debug_print("Received TrackPower: "); debug_println(state);
      if (trackPower != state) {
        trackPower = state;
        displayUpdateFromWit();
      }
    }
    void receivedRosterEntries(int size) {
      debug_print("Received Roster Entries. Size: "); debug_println(size);
      rosterSize = size;
    }
    void receivedRosterEntry(int index, String name, int address, char length) {
      if (index < 10) {
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
      if (index < 10) {
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
      if (index < 10) {
        routeListIndex[index] = index; 
        routeListSysName[index] = sysName; 
        routeListUserName[index] = userName;
        routeListState[index] = state;
      }
    }
};

WiFiClient client;
WiThrottleProtocol wiThrottleProtocol;
MyDelegate myDelegate;
int deviceId = random(1000,9999);

// *********************************************************************************

void ssidsLoop() {
  keypadUseType = KEYPAD_USE_SELECT_SSID;

  if (ssidConnectionState == CONNECTION_STATE_DISCONNECTED) {
    browseSsids(); 
  }
  
  if (ssidConnectionState == CONNECTION_STATE_SELECTED) {
    connectSsid();
  }
}

void browseSsids(){
  debug_println("browseSsids()");

  clearOledArray(); 
  setAppnameForOled(); 
  writeOledArray(false);

  if (maxSsids == 0) {
    oledText[1] = msg_no_ssids_found;
    writeOledArray(false);
    debug_println(oledText[1]);
  
  } else {
    debug_print(maxSsids);  debug_println(msg_ssids_found);
    clearOledArray(); oledText[1] = msg_ssids_found;

    for (int i = 0; i < maxSsids; ++i) {
      debug_print(i+1); debug_print(": "); debug_println(ssids[i]);
      if (i<5) { 
        oledText[i] = String(i+1) + ": " + ssids[i];
      } else {
        if (i<10) {  // only have room for 10
          oledText[i+1] = String(i+1) + ": " + ssids[i];
        }
      }
    }

    if (maxSsids > 0) {
      oledText[5] = menu_select_ssids;
    }
    writeOledArray(false);

    if (maxSsids == 1) {
      selectedSsid = ssids[0];
      selectedSsidPassword = passwords[0];
      ssidConnectionState = CONNECTION_STATE_SELECTED;

      turnoutPrefix = turnoutPrefixes[0];
      routePrefix = routePrefixes[0];
      
    } else {
      ssidConnectionState = CONNECTION_STATE_SELECTION_REQUIRED;
    }
  }
}

void selectSsid(int selection) {
  debug_print("selectSsid() "); debug_println(selection);

  int correctedSelection = selection - 1; 
  if ((correctedSelection>=0) && (correctedSelection < maxSsids)) {
    ssidConnectionState = CONNECTION_STATE_SELECTED;
    selectedSsid = ssids[correctedSelection];
    selectedSsidPassword = passwords[correctedSelection];
    
    turnoutPrefix = turnoutPrefixes[correctedSelection];
    routePrefix = routePrefixes[correctedSelection];
  }
}

void connectSsid() {
  debug_println("Connecting to ssid...");
  clearOledArray(); 
  oledText[0] = appName; oledText[6] = appVersion; 
  oledText[1] = selectedSsid; oledText[2] + "connecting...";
  writeOledArray(false);

  double startTime = millis();
  double nowTime = startTime;

  const char *cSsid = selectedSsid.c_str();
  const char *cPassword = selectedSsidPassword.c_str();

  if (cSsid!="") {
    debug_print("Trying Network "); debug_println(cSsid);
    clearOledArray(); 
    setAppnameForOled(); 
    oledText[1] = selectedSsid; oledText[2] =  msg_trying_to_connect;
    writeOledArray(false);

    WiFi.begin(cSsid, cPassword); 

    while ( (WiFi.status() != WL_CONNECTED) 
      && ((nowTime-startTime) <= 10000) ) { // try for 10 seconds
      debug_print("Trying Network ... Checking status "); debug_println(cSsid);
      delay(250);
      debug_print(".");
      nowTime = millis();
    }

    debug_println("");
    if (WiFi.status() == WL_CONNECTED) {
      debug_print("Connected. IP address: "); debug_println(WiFi.localIP());
      oledText[2] = msg_connected; 
      oledText[3] = msg_address_label + String(WiFi.localIP());
      writeOledArray(false);
      // ssidConnected = true;
      ssidConnectionState = CONNECTION_STATE_CONNECTED;
      keypadUseType = KEYPAD_USE_SELECT_WITHROTTLE_SERVER;

      // setup the bonjour listener
      if (!MDNS.begin("ESP32_Browser")) {
        debug_println("Error setting up MDNS responder!");
        oledText[2] = msg_bounjour_setup_failed;
        writeOledArray(false);
        delay(2000);
        ssidConnectionState = CONNECTION_STATE_DISCONNECTED;
      }

    } else {
      debug_print(msg_connection_failed);
      oledText[2] = msg_connection_failed;
      writeOledArray(false);
      delay(2000);
      
      WiFi.disconnect();      
      ssidConnectionState = CONNECTION_STATE_DISCONNECTED;
    }
  }
}

// *********************************************************************************

void witService() {
  keypadUseType = KEYPAD_USE_SELECT_WITHROTTLE_SERVER;

  if (witConnectionState == CONNECTION_STATE_DISCONNECTED) {
    browseWitService(); 
  }
  
  if (witConnectionState == CONNECTION_STATE_SELECTED) {
    connectWitServer();
  }
}

void browseWitService(){
  debug_println("browseWitService()");

  const char * service = "withrottle";
  const char * proto= "tcp";

  debug_printf("Browsing for service _%s._%s.local. on %s ... ", service, proto, selectedSsid);
  clearOledArray(); 
  oledText[0] = appName; oledText[6] = appVersion; 
  oledText[1] = selectedSsid;   oledText[2] = msg_browsing_for_service;
  writeOledArray(false);

  noOfWitServices = MDNS.queryService(service, proto);
  if (noOfWitServices == 0) {
    oledText[1] = msg_no_services_found;
    writeOledArray(false);
    debug_println(oledText[1]);
    delay(5000);
    witConnectionState = CONNECTION_STATE_DISCONNECTED;
    ssidConnectionState = CONNECTION_STATE_DISCONNECTED;
  
  } else {
    debug_print(noOfWitServices);  debug_println(msg_services_found);
    clearOledArray(); oledText[1] = msg_services_found;

    for (int i = 0; i < noOfWitServices; ++i) {
      // Print details for each service found
      debug_print("  "); debug_print(i+1); debug_print(": "); debug_print(MDNS.hostname(i));
      debug_print(" ("); debug_print(MDNS.IP(i)); debug_print(":"); debug_print(MDNS.port(i)); debug_println(")");
      if (i<5) {  // only have room for 5
        oledText[i] = String(i+1) + ": " + MDNS.IP(i).toString() + ":" + String(MDNS.port(i));
      }
    }

    if (noOfWitServices > 0) {
      oledText[5] = menu_select_wit_service;
    }
    writeOledArray(false);

    if (noOfWitServices == 1) {
      selectedWitServerIP = MDNS.IP(0);
      selectedWitServerPort = MDNS.port(0);
      witConnectionState = CONNECTION_STATE_SELECTED;
    } else {
      witConnectionState = CONNECTION_STATE_SELECTION_REQUIRED;
    }
  }
}

void selectWitServer(int selection) {
  debug_print("selectWitServer() "); debug_println(selection);

  int correctedCollection = selection - 1; 
  if ((correctedCollection>=0) && (correctedCollection < noOfWitServices)) {
    witConnectionState = CONNECTION_STATE_SELECTED;
    selectedWitServerIP = MDNS.IP(correctedCollection);
    selectedWitServerPort = MDNS.port(correctedCollection);
    keypadUseType = KEYPAD_USE_OPERATION;
  }
}

void connectWitServer() {
  // Pass the delegate instance to wiThrottleProtocol
  wiThrottleProtocol.setDelegate(&myDelegate);
  // Uncomment for logging on Serial
  // wiThrottleProtocol.setLogStream(&Serial);

  debug_println("Connecting to the server...");
  clearOledArray(); 
  setAppnameForOled(); 
  oledText[1] = selectedWitServerIP.toString() + " " + String(selectedWitServerPort); oledText[2] + "connecting...";
  writeOledArray(false);

  if (!client.connect(selectedWitServerIP, selectedWitServerPort)) {
    debug_println(msg_connection_failed);
    oledText[3] = msg_connection_failed;
    writeOledArray(false);
    delay(5000);
    
    witConnectionState = CONNECTION_STATE_DISCONNECTED;
    ssidConnectionState = CONNECTION_STATE_DISCONNECTED;
  } else {
    debug_print("Connected to server: ");   debug_println(selectedWitServerIP); debug_println(selectedWitServerPort);

    // Pass the communication to WiThrottle
    wiThrottleProtocol.connect(&client);
    debug_println("WiThrottle connected");

    wiThrottleProtocol.setDeviceName(deviceName);  
    wiThrottleProtocol.setDeviceID(String(deviceId));  

    witConnectionState = CONNECTION_STATE_CONNECTED;

    oledText[2] = msg_connected;
    oledText[5] = menu_menu;
    writeOledArray(false);

    keypadUseType = KEYPAD_USE_OPERATION;
  }
}

void disconnectWitServer() {
  releaseAllLocos();
  wiThrottleProtocol.disconnect();
  debug_println("Disconnected from wiThrottle server\n");
  clearOledArray(); oledText[0] = msg_disconnected;
  writeOledArray(false);
  witConnectionState = CONNECTION_STATE_DISCONNECTED;
}

// *********************************************************************************

AiEsp32RotaryEncoder rotaryEncoder = AiEsp32RotaryEncoder(ROTARY_ENCODER_A_PIN, ROTARY_ENCODER_B_PIN, ROTARY_ENCODER_BUTTON_PIN, ROTARY_ENCODER_VCC_PIN, ROTARY_ENCODER_STEPS);
void IRAM_ATTR readEncoderISR() {
  rotaryEncoder.readEncoder_ISR();
}

void rotary_onButtonClick() {
  if ( (keypadUseType!=KEYPAD_USE_SELECT_WITHROTTLE_SERVER)
       && (keypadUseType!=KEYPAD_USE_SELECT_SSID) ) {
    static unsigned long lastTimePressed = 0;
    if (millis() - lastTimePressed < encoderDebounceTime) {   //ignore multiple press in that time milliseconds
      debug_println("encoder button debounce");
      return;
    }
    lastTimePressed = millis();
    if (wiThrottleProtocol.getNumberOfLocomotives()>0) {
      if (currentSpeed!=0) {
        wiThrottleProtocol.setSpeed(0);
      } else {
        if (toggleDirectionOnEncoderButtonPressWhenStationary) toggleDirection();
      }
      currentSpeed = 0;
    }
    debug_println("encoder button pressed");
    writeOledSpeed();
  }  else {
    deepSleepStart();
  }
}

void rotary_loop() {
  if (rotaryEncoder.encoderChanged()) {   //don't print anything unless value changed
    encoderValue = rotaryEncoder.readEncoder();
     debug_print("Encoder From: "); debug_print(lastEncoderValue);  debug_print(" to: "); debug_println(encoderValue);
    if (abs(encoderValue-lastEncoderValue) > 800) { // must have passed through zero
      if (encoderValue > 800) {
        lastEncoderValue = 1001; 
      } else {
        lastEncoderValue = 0; 
      }
      debug_print("Corrected Encoder From: "); debug_print(lastEncoderValue); debug_print(" to: "); debug_println(encoderValue);
    }
    if (wiThrottleProtocol.getNumberOfLocomotives()>0) {
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
    lastEncoderValue = encoderValue;
  }
  
  if (rotaryEncoder.isEncoderButtonClicked()) {
    rotary_onButtonClick();
  }
}

void encoderSpeedChange(boolean rotationIsClockwise, int speedChange) {
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
 
void keypadEvent(KeypadEvent key){
  switch (keypad.getState()){
  case PRESSED:
    debug_print("Button "); debug_print(String(key - '0')); debug_println(" pushed.");
    doKeyPress(key, true);
    break;
  case RELEASED:
    doKeyPress(key, false);
    debug_print("Button "); debug_print(String(key - '0')); debug_println(" released.");
    break;
//    case HOLD:
//        break;
  }
}

// *********************************************************************************
// *********************************************************************************

void setup() {
  Serial.begin(115200);
  u8g2.begin();

  clearOledArray(); oledText[0] = appName; oledText[6] = appVersion; oledText[2] = msg_start;
  writeOledArray(false);

  delay(1000);
  debug_println("Start"); 

  rotaryEncoder.begin();  //initialize rotary encoder
  rotaryEncoder.setup(readEncoderISR);
  //set boundaries and if values should cycle or not 
  rotaryEncoder.setBoundaries(0, 1000, circleValues); //minValue, maxValue, circleValues true|false (when max go to min and vice versa)
  //rotaryEncoder.disableAcceleration(); //acceleration is now enabled by default - disable if you dont need it
  rotaryEncoder.setAcceleration(100); //or set the value - larger number = more accelearation; 0 or 1 means disabled acceleration

  keypad.addEventListener(keypadEvent); // Add an event listener for this keypad
  keypad.setDebounceTime(keypadDebounceTime);

  esp_sleep_enable_ext0_wakeup(GPIO_NUM_13,0); //1 = High, 0 = Low

  keypadUseType = KEYPAD_USE_SELECT_SSID;
}

void loop() {
  
  // if (!ssidConnected) {
  if (ssidConnectionState != CONNECTION_STATE_CONNECTED) {
    // connectNetwork();
    ssidsLoop();
  } else {  
    if (witConnectionState != CONNECTION_STATE_CONNECTED) {
      witService();
    } else {
      wiThrottleProtocol.check();    // parse incoming messages
    }
  }
  char key = keypad.getKey();
  rotary_loop();

  // delay(5);
}

// *********************************************************************************
// *********************************************************************************

void doKeyPress(char key, boolean pressed) {
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
            if ((menuCommandStarted) && (menuCommand.length()>=1)) {
              doMenu();
            } else {
              writeOledDirectCommands();
            }
            break;

          case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
            if (menuCommandStarted) { // appeand to the string
              menuCommand += key;
              writeOledMenu(menuCommand);
            } else {
              doDirectCommand(key, true);
            }
            break;

          default:  //A, B, C, D
            doDirectCommand(key, true);
            break;
        }
        break;

      case KEYPAD_USE_SELECT_WITHROTTLE_SERVER:
        debug_print("key wit... "); debug_println(key);
        switch (key){
          case '1': case '2': case '3': case '4': case '5':
            selectWitServer(key - '0');
            break;
          default:  // do nothing 
            break;
        }
        break;

      case KEYPAD_USE_SELECT_SSID:
        debug_print("key ssid... "); debug_println(key);
        switch (key){
          case '1': case '2': case '3': case '4': case '5':
            selectSsid(key - '0');
            break;
          default:  // do nothing 
            break;
        }
        break;

      case KEYPAD_USE_SELECT_ROSTER:
      case KEYPAD_USE_SELECT_TURNOUTS_THROW:
      case KEYPAD_USE_SELECT_TURNOUTS_CLOSE:
      case KEYPAD_USE_SELECT_ROUTES:
        debug_print("key Roster/Rurnouts/Routes... "); debug_println(key);
        switch (key){
          case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
            
            switch (keypadUseType) {
              case KEYPAD_USE_SELECT_ROSTER:
                selectRoster(key - '0');
                break;
              case KEYPAD_USE_SELECT_TURNOUTS_THROW:
                selectTurnoutList(key - '0', TurnoutThrow);
                break;
              case KEYPAD_USE_SELECT_TURNOUTS_CLOSE:
                selectTurnoutList(key - '0', TurnoutClose);
                break;
              case KEYPAD_USE_SELECT_ROUTES:
                selectRouteList(key - '0');
                break;
            }
            break;
          
          case '*':  // cancel
          case '#':  // cancel  todo: change to next page
            resetMenu();
            writeOledSpeed();
            break;

          default:  // do nothing 
            break;
        }
        break;
    }

  } // else {  // released
    // do nothing

    // if (keypadUseType == KEYPAD_USE_OPERATION) {
    //   if ( (!menuCommandStarted) && (key>='0') && (key<='D')) { // only process releases for the numeric keys + A,B,C,D and only if a menu command has not be started
    //     debug_println("Operation - Process key release");
    //     doDirectCommand(key, false);
    //   } else {
    //     debug_println("Non-Operation - Process key release");
    //     if (keypadUseNonOperationComplete) {   //finished processing the menu etc.
    //       resetMenu();
    //     }
    //   }
    // } // else {  // keypadUser type = KEYPAD_USE_SELECT_WITHROTTLE_SERVER
    // }
  // }
}

void doDirectCommand (char key, boolean pressed) {
  debug_print("Direct command: "); debug_println(key);
  int buttonAction = buttonActions[(key - '0')];
  if (buttonAction!=FUNCTION_NULL) {
    if ( (buttonAction>=FUNCTION_0) && (buttonAction<=FUNCTION_28) ) {
      doFunction(buttonAction, pressed);

    } else {
      if (pressed) { // only process these on the key press, not the release
        switch (buttonAction) {
            case DIRECTION_FORWARD: {
              changeDirection(Forward);
              break; 
            }
            case DIRECTION_REVERSE: {
              changeDirection(Reverse);
              break; 
            }
            case DIRECTION_TOGGLE: {
              toggleDirection();
              break; 
            }
            case SPEED_UP: {
              speedUp(currentSpeedStep);
              break; 
            }
            case SPEED_DOWN: {
              speedDown(currentSpeedStep);
              break; 
            }
            case SPEED_UP_FAST: {
              speedUp(currentSpeedStep*speedStepMultiplier);
              break; 
            }
            case SPEED_DOWN_FAST: {
              speedUp(currentSpeedStep*speedStepMultiplier);
              break; 
            }
            case E_STOP: {
              speedEstop();
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
        }
      }
    }
  }
}

void doMenu() {
  String loco = "";
  String function = "";
  boolean result = false;
  debug_print("Menu: "); debug_println(menuCommand);
  
  switch (menuCommand[0]) {
    case '1': { // select loco
        if (menuCommand.length()>1) {
          loco = menuCommand.substring(1, menuCommand.length());
          loco = getLocoWithLength(loco);
          debug_print("add Loco: "); debug_println(loco);
          wiThrottleProtocol.addLocomotive(loco);
          resetFunctionStates();
          writeOledSpeed();
        } else {
          writeOledRoster("");
        }
        break;
      }
    case '2': { // de-select loco
        loco = menuCommand.substring(1, menuCommand.length());
        if (loco!="") { // a loco is specified
          loco = getLocoWithLength(loco);
          debug_print("release Loco: "); debug_println(loco);
          wiThrottleProtocol.releaseLocomotive(loco);
        } else { //not loco specified so release all
          releaseAllLocos();
        }
        writeOledSpeed();
        break;
      }
    case '3': { // change direction
        toggleDirection();
        break;
      }
     case '4': { // toggle additional Multiplier
        toggleAdditionalMultiplier();
        break;
      }
   case '5': {  // throw turnout
        if (menuCommand.length()>1) {
          String turnout = turnoutPrefix + menuCommand.substring(1, menuCommand.length());
          // if (!turnout.equals("")) { // a turnout is specified
            debug_print("throw turnout: "); debug_println(turnout);
            wiThrottleProtocol.setTurnout(turnout, TurnoutThrow);
          // }
          writeOledSpeed();
        } else {
          writeOledTurnoutList("", TurnoutThrow);
        }
        break;
      }
    case '6': {  // close turnout
        if (menuCommand.length()>1) {
          String turnout = turnoutPrefix + menuCommand.substring(1, menuCommand.length());
          // if (!turnout.equals("")) { // a turnout is specified
            debug_print("close turnout: "); debug_println(turnout);
            wiThrottleProtocol.setTurnout(turnout, TurnoutClose);
          // }
          writeOledSpeed();
        } else {
          writeOledTurnoutList("",TurnoutClose);
        }
        break;
      }
    case '7': {  // route
        if (menuCommand.length()>1) {
          String route = routePrefix + menuCommand.substring(1, menuCommand.length());
          // if (!route.equals("")) { // a loco is specified
            debug_print("route: "); debug_println(route);
            wiThrottleProtocol.setRoute(route);
          // }
          writeOledSpeed();
        } else {
          writeOledRouteList("");
        }
        break;
      }
    case '8': {
        powerToggle();
        debug_println("Power toggle");
        writeOledSpeed();
        break;
      }
    case '9': { // disconnect/reconnect/sleep
        String subcommand = menuCommand.substring(1, menuCommand.length());
        if (subcommand.equals("")) { // no subcommand is specified   
          if (witConnectionState == CONNECTION_STATE_CONNECTED) {
            witConnectionState == CONNECTION_STATE_DISCONNECTED;
            disconnectWitServer();
          } else {
            connectWitServer();
          }
        } else { // subcommand
          if (subcommand.equals("9")) { // sleep
            deepSleepStart();
          }
        }
        break;
      }
    case '0': { // function button
        if (menuCommand.length()>1) {
          function = menuCommand.substring(1, menuCommand.length());
          int functionNumber = function.toInt();
          if (function != "") { // a function is specified
            doFunction(functionNumber, true);  // always act like latching i.e. pressed
          }
        }
        writeOledSpeed();
        break;
      }
  }
  menuCommandStarted = result; 
}

// *********************************************************************************

void resetMenu() {
  debug_println("resetMenu()");
  menuCommand = "";
  menuCommandStarted = false;
  if ( (keypadUseType != KEYPAD_USE_SELECT_SSID) 
    && (keypadUseType != KEYPAD_USE_SELECT_WITHROTTLE_SERVER) ) {
    keypadUseType = KEYPAD_USE_OPERATION; 
  }
 }

 void resetFunctionStates() {
   for (int i=0; i<28; i++) {
     functionStates[i] = false;
   }
 }

String getLocoWithLength(String loco) {
  int locoNo = menuCommand.toInt() + 0;
  if (locoNo <= 127) {
    loco = "S" + loco;
  } else {
    loco = "L" + loco;
  }
  return loco;
}

void speedEstop() {
  wiThrottleProtocol.emergencyStop();
  currentSpeed = 0;
  debug_println("Speed EStop"); 
  writeOledSpeed();
}

void speedDown(int amt) {
  if (wiThrottleProtocol.getNumberOfLocomotives()>0) {
    int newSpeed = currentSpeed - amt;
    debug_print("Speed Down: "); debug_println(amt);
    speedSet(newSpeed);
  }
}

void speedUp(int amt) {
  if (wiThrottleProtocol.getNumberOfLocomotives()>0) {
    int newSpeed = currentSpeed + amt;
    debug_print("Speed Up: "); debug_println(amt);
    speedSet(newSpeed);
  }
}

void speedSet(int amt) {
  if (wiThrottleProtocol.getNumberOfLocomotives()>0) {
    int newSpeed = amt;
    if (newSpeed >128) { newSpeed = 126; }
    if (newSpeed <0) { newSpeed = 0; }
    wiThrottleProtocol.setSpeed(newSpeed);
    currentSpeed = newSpeed;
    debug_print("Speed Set: "); debug_println(newSpeed);
    writeOledSpeed();
  }
}

void releaseAllLocos() {
  if (wiThrottleProtocol.getNumberOfLocomotives()>0) {
    for(int index=0;index<wiThrottleProtocol.getNumberOfLocomotives();index++) {
      wiThrottleProtocol.releaseLocomotive(wiThrottleProtocol.getLocomotiveAtPosition(index));
      writeOledSpeed();
    } 
  }
}

void toggleAdditionalMultiplier() {
  if (speedStep != currentSpeedStep) {
    currentSpeedStep = speedStep;
  } else {
    currentSpeedStep = speedStep * speedStepAdditionalMultiplier;
    writeOledSpeed();
  }
}

void toggleDirection() {
  if (wiThrottleProtocol.getNumberOfLocomotives()>0) {
  //   Direction direction = Reverse;
  //   if (currentDirection == Reverse) {
  //     direction = Forward;
  //   }
    changeDirection( (currentDirection == Forward) ? Reverse : Forward );
    writeOledSpeed();
  }
}

void changeDirection(Direction direction) {
  if (wiThrottleProtocol.getNumberOfLocomotives()>0) {
    wiThrottleProtocol.setDirection(direction);
    currentDirection = direction;
    debug_print("Change direction: "); debug_println( (direction==Forward) ? "Forward" : "Reverse");
    writeOledSpeed(); 
  }
}

void doFunction(int functionNumber, boolean pressed) {
  if (wiThrottleProtocol.getNumberOfLocomotives()>0) {
    wiThrottleProtocol.setFunction(functionNumber, pressed );
    debug_print("fn: "); debug_print(functionNumber); debug_println( (pressed) ? " Pressed" : " Released");
  }
}

void powerOnOff(TrackPower powerState) {
  wiThrottleProtocol.setTrackPower(powerState);
  trackPower = powerState;
}

void powerToggle() {
  if (trackPower==PowerOn) {
    powerOnOff(PowerOff);
  } else {
    powerOnOff(PowerOn);
  }
}

// *********************************************************************************

void selectRoster(int selection) {
  debug_print("selectRoster() "); debug_println(selection);

  if ((selection>=0) && (selection < rosterSize)) {
    String loco = String(rosterLength[selection]) + rosterAddress[selection];
    debug_print("add Loco: "); debug_println(loco);
    wiThrottleProtocol.addLocomotive(loco);
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

// *********************************************************************************

void setAppnameForOled() {
  oledText[0] = appName; oledText[6] = appVersion; 
}

void writeOledRoster(String soFar) {
  menuIsShowing = true;
  keypadUseType = KEYPAD_USE_SELECT_ROSTER;
  if (soFar == "") { // nothing entered yet
    clearOledArray();
    int j = 0;
    for (int i=0; i<10 && i<rosterSize; i++) {
      j = (i<5) ? j=i : j = i+1;
      oledText[j] = String(rosterIndex[i]) + ": " + rosterName[i].substring(0,10);
    }
    oledText[5] = menu_roster;
    writeOledArray(false);
  } else {
    int cmd = menuCommand.substring(0, 1).toInt();
  }
}

void writeOledTurnoutList(String soFar, TurnoutAction action) {
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
      j = (i<5) ? j=i : j = i+1;
      oledText[j] = String(turnoutListIndex[i]) + ": " + turnoutListUserName[i].substring(0,10);
    }
    oledText[5] = menu_turnout_list;
    writeOledArray(false);
  } else {
    int cmd = menuCommand.substring(0, 1).toInt();
  }
}

void writeOledRouteList(String soFar) {
  menuIsShowing = true;
  keypadUseType = KEYPAD_USE_SELECT_ROUTES;
  if (soFar == "") { // nothing entered yet
    clearOledArray();
    int j = 0;
    for (int i=0; i<10 && i<routeListSize; i++) {
      j = (i<5) ? j=i : j = i+1;
      oledText[j] = String(routeListIndex[i]) + ": " + routeListUserName[i].substring(0,10);
    }
    oledText[5] = menu_route_list;
    writeOledArray(false);
  } else {
    int cmd = menuCommand.substring(0, 1).toInt();
  }
}

void writeOledMenu(String soFar) {
  menuIsShowing = true;
  if (soFar == "") { // nothing entered yet
    clearOledArray();
    int j = 0;
    for (int i=1; i<10; i++) {
      j = (i<6) ? j=i : j = i+1;
      oledText[j-1] = String(i) + ": " + menuText[i][0];
    }
    oledText[10] = "0: " + menuText[0][0];
    oledText[11] = menu_cancel;
    writeOledArray(false);
  } else {
    int cmd = menuCommand.substring(0, 1).toInt();

    clearOledArray();

    oledText[0] = "Menu: " + menuText[cmd][0]; oledText[1] =  menuCommand.substring(1, menuCommand.length());
    oledText[5] = menuText[cmd][1];
    writeOledArray(false);
  }
}

void writeOledSpeed() {
  menuIsShowing = false;
  String sLocos = "";
  String sSpeed = "";
  String sDirection = "";

  clearOledArray();
  
  boolean drawTopLine = false;

  if (wiThrottleProtocol.getNumberOfLocomotives() > 0 ) {
    // oledText[0] = label_locos; oledText[2] = label_speed;
  
    for (int i=0; i < wiThrottleProtocol.getNumberOfLocomotives(); i++) {
      sLocos = sLocos + " " + wiThrottleProtocol.getLocomotiveAtPosition(i);
    }
    sSpeed = String(currentSpeed);
    sDirection = (currentDirection==Forward) ? direction_forward : direction_reverse;

    oledText[0] = sLocos; oledText[7] = sDirection;

    drawTopLine = true;

  } else {
    setAppnameForOled();
    oledText[2] = msg_no_loco_selected;
  }

  if (speedStep != currentSpeedStep) {
    oledText[3] = "X " + String(speedStepAdditionalMultiplier);
  }

  oledText[5] = menu_menu;
  writeOledArray(false, false, drawTopLine);

  if (wiThrottleProtocol.getNumberOfLocomotives() > 0 ) {
    writeOledFunctions();
  }

  if (trackPower == PowerOn) {
    u8g2.drawBox(0,41,15,8);
    u8g2.setDrawColor(1);
    u8g2.setDrawColor(0);
  }
  u8g2.setFont(u8g2_font_profont10_tf);
  u8g2.drawStr(0, 48, label_track_power.c_str());
  u8g2.setDrawColor(1);


  const char *cSpeed = sSpeed.c_str();
  u8g2.setFont(u8g2_font_inb21_mn); // big
  int width = u8g2.getStrWidth(cSpeed);
  u8g2.drawStr(35+(55-width),45, cSpeed);
  u8g2.sendBuffer();
}

void writeOledFunctions() {
   int x = 109;
   for (int i=0; i < 8; i++) {
     if (functionStates[i]) {
      //  debug_print("Fn On "); debug_println(i);
      int y = (i+2)*10-8;
      if (i>=4) { 
        x = 119; 
        y = (i-2)*10-8;
      }

      u8g2.drawBox(x,y,8,8);
      u8g2.setDrawColor(0);
      u8g2.setFont(u8g2_font_profont10_tf);
      u8g2.drawStr( x+2, y+7, String(i).c_str());
      u8g2.setDrawColor(1);
    //  } else {
    //    debug_print("Fn Off "); debug_println(i);
     }
   }
}

void writeOledArray(boolean isThreeColums) {
  writeOledArray(isThreeColums, true, false);
}

void writeOledArray(boolean isThreeColums, boolean sendBuffer) {
  writeOledArray(isThreeColums, sendBuffer, false);
}

void writeOledArray(boolean isThreeColums, boolean sendBuffer, boolean drawTopLine) {

  u8g2.clearBuffer();					// clear the internal memory

  // u8g2.setFont(u8g2_font_ncenB08_tr);	// serif bold
  // u8g2.setFont(u8g2_font_helvB08_te);	// helv bold
  // u8g2.setFont(u8g2_font_helvB08_tf);	// helv
  u8g2.setFont(u8g2_font_NokiaSmallPlain_tf); // small
  
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
    u8g2.drawStr(x,y, cLine1);
    y = y + 10;
    if ((i==5) || (i==11)) {
      x=x+xInc;
      y=10;
    }
  }
  if (drawTopLine) u8g2.drawHLine(0,11,128);
  u8g2.drawHLine(0,51,128);

  if (sendBuffer) u8g2.sendBuffer();					// transfer internal memory to the display
}

void clearOledArray() {
  for (int i=0; i < 15; i++) {
    oledText[i] = "";
  }
}

void writeOledDirectCommands() {
  clearOledArray();
  oledText[0] = direct_command_list;
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
  writeOledArray(true);
  menuCommandStarted = false;
}

// *********************************************************************************

void deepSleepStart() {
  clearOledArray(); 
  setAppnameForOled();
  oledText[2] = msg_start_sleep;
  writeOledArray(false);
  delay(2000);

  u8g2.setPowerSave(1);
  esp_deep_sleep_start();
}