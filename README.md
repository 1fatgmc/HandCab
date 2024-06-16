# HandCab

NOTE:  The software shown here at the moment is not finalized or complete.  Please don't try and use any of it until this notice is gone.

A HandCab is a DIY handheld controller that talks to a WiThrottle Server (JMRI, DCC-EX  EX-CommandStation and many others) using the WiThrottle protocol to control DCC model trains.  HandCab uses 3 potentiometers (50K) for the throttle, brake and reverser controls and is modelled after a GP7/9 'barrel' control stand.

![Alt text](https://github.com/1fatgmc/HandCab/blob/main/24-03-27%20HandCab%20vs%20GP9-1.jpg?raw=true)

---

## Video Overview of the throttle in use.

https://www.youtube.com/watch?v=8vLRVYeE0dU

## Prerequisites

1. Requires moderate soldering skills.

2. Loading the code (sketch) requires downloading of one of the IDEs, this sketch, the libraries, etc. so some experience with Arduinos is helpful, but not critical.

3. The HandCab Throttle along with most Android and Apple devices running supported throttle apps will work with any WiThrottle Server such as JMRI, DCC-EX, MRC WiFi, Digitrax LnWi, NCE (WiFi/DCC NCE Cab Bus Interface), TCS CS-105 (Using WiFiTrax WFD-30 or WFD-31)  and others.

---
## Building

You can find a detailed build of the throttle on my website:
https://1fatgmc.com/RailRoad/DCC/HandCab-Index.html

## Parts

Required Components 
(for links to where I bought the following goto (https://1fatgmc.com/RailRoad/DCC/page-14.html)
* 1 - ESP32 LOLIN32 Lite.
* 1 - 3x4 Keypad.
* 1 - 1.3” OlED Display.
* 1 - Encoder.
* 3 – 50K potentiometers.
* 2 – On/Off Pushbutton switches.
* 1 – Momentary Limit Switch.
* 7 – SPST Momentary Pushbutton switches.
* 1 – Battery Selector DPDT c/o switch if have 2 batteries.
* 1- 3 stage Adafruit 4410 USB charger (optional but I like having a better charger than the ESP32 one.
* 1 or 2 Batteries …... (I like 2 so I can change if needed).
* 1 or 2 or No JST connectors.
 
*Pinouts with Additional 7 Buttons*
![Alt text](https://github.com/1fatgmc/HandCab/blob/main/24-06-02%20Schematic-1.jpg?raw=true)

## Loading the code

 **WARNING!**

    The latest versions of the ESP32 Board Library (3.0.0 and later) have renamed an attribute. 
    The HandCab & WiTcontroller code has been modified to reflect this.  If you are using an 
    earlier version then you need to include the define ``#define USING_OLDER_ESPMDNS true`` 
    in ``config_buttons.h``.
    NOTE: The symptom of this problem is if you receive an error like the following when you try to 
    build/compile ``...\static.h:761:42: error: 'class MDNSResponder' has no member named 'address'
    #define ESPMDNS_IP_ATTRIBUTE_NAME MDNS.address(i)`` 

1. Download the Arduino IDE.  
    * Available from  https://support.arduino.cc/hc/en-us/articles/360019833020-Download-and-install-Arduino-IDE
2. Download the esp32 boards in the Arduino IDE.
    * add the esp322 support with the following instructions:  (See here for detailed instructions:  https://randomnerdtutorials.com/installing-the-esp32-board-in-arduino-ide-windows-instructions/)
        * In the Arduino IDE, go to *File* > *Preferences*
        * Enter the following into the 'Additional Board Manager URLs' field:  https://dl.espressif.com/dl/package_esp32_index.json
    * Then Use the *Boards Manager* in the *Arduino IDE* to install the esp32 board support
        * *Tools* > *Board* > *Boards Manager*
        * Search for "esp32" by Expressive Systems.  Install version 2.0.11
3. Download or clone *this* repository. (Note: if you 'clone' initially, it is easier to receive updates to the code by doing a 'fetch' subsequently.  See Notes below.)
    * Clone - **First Time**
       * Install *GitHub Desktop* from https://desktop.github.com/
       * Create a free account on GitHub and authorise the app to allow it to connect to GitHub
       * Select *file* -> *Clone Repository* - or 'Clone an repository from the internet' from the welcome page then select the 'URL' tab
       * Enter *https://github.com/1fatgmc/HandCab* as the URL
       * Select a local folder to install it.  The default folder for the Arduino usually looks like "...username\Documents\Arduino\". (This is a good but not essential place to put it.)
       * Click *Clone*
       * **Subsequently**  (Anytime after the first 'clone')
         * click *Fetch Origin* and any changes to the code will be bought down to your PC, but you config_buttons.h and config_network.h will not be touched.
    * Download 
       * Open *https://github.com/1fatgmc/HandCab*
       * Click the green "Code" button and select download zip
       * Extract the zip file to a local folder.  The default folder for the Arduino usually looks like "...username\Documents\Arduino\". This is a good but not essential place to put it.
4. Load the needed libraries to your PC. These can loaded from the *Library Manager* in the *Arduino IDE*.
    * *U8g2lib.h* -  Search for "U8g2".   Install version 2.34.22
    * *AiEsp32RotaryEncoder.h* - search for "Ai Esp32 Rotary Encoder".  Install Version 1.6
    * *Keypad.h* - Search for "Keypad" by Mark Stanley.   Install version 3.1.1
    * *WiThrottleProtocol.h* - Search for "WiThrottleProtocol" (not "WiThrottle").  Install version 1.0.3 or later if available
5. These should have been automatically installed when you downloaded the esp32 boards.  *YOU SHOULD NOT NEED TO DO ANYTHING SPECIFIC TO GET THESE*
    * *WiFi.h*  - https://github.com/espressif/arduino-esp32/tree/master/libraries/WiFi
    * *ESPmDNS.h* - https://github.com/espressif/arduino-esp32/blob/master/libraries/ESPmDNS
6. Copy '**config_network_example.h**' to a new file to '**config_network.h**'.
    * Then edit it to include the network ssids you want to use.  (Not essential, but entering passwords via the encoder is tedious.)
7. Copy '**config_buttons_example.h**' to a new file '**config_buttons.h**'.
    * Optionally, edit this to change the mapping of the keypad buttons to specific functions.
    * Optionally, edit this to configure the additional buttons (if you have included them) to specific functions.
    * Optionally, edit this to change if you want the function buttons to display when you press #, instead of the default of showing the Key Definitions
8. Upload the sketch.  
    * Select the board type as "WEMOS LOLIN32 Lite" in the *Arduino IDE*.
    * Connect the board via USB and select the appropriate port in the *Arduino IDE*.
    * Click *Upload* 

Notes: 
   * HandCab version 0.01 or later requires WiThrottleProtocol version 1.1.14 or later.
   * The *WiFi.h* and *ESPmDNS.h* libraries were automatically installed for me when I installed the esp32 boards, however you may need to install them manually.
   * Later versions of the esp board support are available and do appear to work, but if you have difficulties version 1.0.6 appears to be stable.
   * Later versions of the libraries generally should work, but if you have difficulties use the versions listed above.
   * To get the HandCab sketch I recommend using either the git command line, or the far more friendly 'GitHub Desktop' app.  See instructions above.
   * If you receive and error related to Python, and you are on MacOs 12 and above please edit the platform file, change from python to python3 as follows; preferences->user/path/arduino/packages/hardware/esp32/version/platform.txt and edit the line that looks as follows:tools.gen_esp32part.cmd=python3 "{runtime.platform.path}/tools/gen_esp32part.py"

---

## Using HandCab

**Currently functioning:**
- Provides a list of discovered SSIDs with the ability to choose one. When you select one:
  - if it is one in your specified list (in the sketch), it will use that specified password 
  - if it is a DCC++EX WiFi Command Station in access Point mode, it will guess the password
  - otherwise it will ask to enter the password (Use the rotary encoder to choose each character and the encoder button to select it.  * = backspace.  # = enter the password.) 
- Optionally provides a list of SSIDs with the specified passwords (in the sketch) to choose from
- Auto-connects to the first found WiThrottle Protocol Server if only one found, otherwise 
  - Asks which to connect to.
  - If none found will ask to enter the IP Address and Port
  - Guesses the WiThrottle IP address and Port for DCC++EX WiFi Access Point mode Command Stations
  - optionally can add a #define (a preference) to disable this auto connect feature
- Rudimentary on-the-fly consists
- Assign commands directly to the 1-9 buttons (in the sketch) (see list below)
  - This is done in config_button.h
  - Latching / non-latching for the function is provided by the roster entry of wiThrottle server
- Optionally use a potentiometer (pot) instead of the rotary encoder
- Optional ability to assign commands directly to the 1-7 additional buttons (in the sketch) (see list below)
  - These are defined config_button.h
- Command menu (see below for full list) including:
  - Able to select and deselect locos 
    - by their DCC address, via the keypad
      - On NCE systems, a leading zero (0) will force a long address
    - from the first 50 locos in the roster
  - Able to select multiple locos to create a consist
    - Able to change the facing of the additional locos in the consists (via the 'extra' menu after selection)
  - Able to activate any function (0-31)
    - Showing of the roster function labels (from the WiThrottle server if provided)
    - Quick access to the functions by pressing #. Temporarily enabled via the Extras menu (or permanently enabled in config_button.h)
    - Limited ability to configure which functions are sent to the first or all locos in a consist (defined in config_button.h)
  - Able to throw/close turnouts/points
    - from the address
    - from the first 50 turnouts/points in the server list
  - Able to activate routes
    - from their address
    - from the first 50 routes in the server list
  - Set/unset a multiplier for the rotary encoder
  - Power Track On/Off
  - Disconnect / Reconnect
  - Put ESP32 in deep sleep and restart it
- <strike>Have up to 6 throttles, each with an unlimited number of locos in consist. Default is 2 throttles, which can be increased or decreased temporarily via the Extras menu (or permanently enabled in config_button.h)</strike>
- Limited dealing with unexpected disconnects.  It will throw you back to the WiThrottle Server selection screen.
- Boundary between short and long DCC addresses can be configured in config_buttons.h
- <strike>The default speed step (per encoder click) can be configured in config_buttons.h</strike>
- The controller will automatically shut down if no SSID is selected or entered in 4 minutes (to conserve the battery)

**ToDo:**
- Deal with unexpected disconnects better
  - automatic attempt to reconnect
- Keep a list of IP addresses and ports if mDNS doesn't provide any
- Remember (for the current session only) recently selected locos

### Command menu:
- 0-9 keys = pressing these directly will do whatever you has been preset in the sketch for them to do  (see \# below)
- \* = Menu:  The button press following the \* is the actual command:
  - 1 = Add loco.  
     - Followed by the loco number, followed by \# to complete.  e.g. to select loco 99 you would press '\*199\#'
     - or \# alone to show the roster   \# again will show the next page
  - 2 = release loco:
     - Followed by the loco number, followed by \# to release an individual loco.  e.g. to deselect the loco 99 you would press '\*299\#'
     - Otherwise followed directly by \#  to release all e.g. '\*2\#'
  - 3 = <strike>Toggle direction.</strike>
  - 4 = Set / Unset a 2 times multiplier for the rotary encoder dial.
  - 5 = Throw turnout/point.  
     - Followed by the turnout/point number, followed by the \# to complete.  e.g. Throw turnout XX12 '\*512\#'  (where XX is a prefix defined in the sketch) 
     - or \# alone to show the list from the server   \# again will show the next page
  - 6 = Close turnout.    
     - Followed by the turnout/point number, followed by \# to complete.  e.g. Close turnout XX12 '\*612\#'  (where XX is a prefix defined in the sketch)
     - or \# alone to show the list from the server
  - 7 = Set Route.    
      - Followed by the Route number, followed by \# to complete.  e.g. to Set route XX:XX:0012 '\*60012\#'  (where \'XX:XX:\' is a prefix defined in the sketch)
      - or \# alone to show the list from the server   \# again will show the next page
  - 0 = Function button. Followed by...
      - the function number, Followed by \# to complete.  e.g. to set function 17 you would press '\*017\#'
      - \# alone, to show the list of functions.
  - 8 = Track Power On/Off.
  - 9 = Extras. Followed by...
      - 0 then \# to toggle the action the the \# key does as a direct action, either to show the direct action key definitions, or the Function labels.  
      - 1 to change the facing of locos in a consist.
      - 3 to toggle the heartbeat check.
      - <strike>4 to increase the number of available throttle (up to 6)</strike>
      - <strike>5 to decrease the number of available throttle (down to 1)</strike>
      - 6 then \# to Disconnect/Reconnect.  
      - 7 (or 9) then \# to put into deep sleep
Pressing '\*' again before the '\#' will terminate the current command (but not start a new command)
 - \# = Pressing # alone will show the function the the numbered keys (0-9) perform, outside the menu.
       Optionally, you can configure it so that the the Function labels from the roster show 

Pressing the Encoder button while the ESP32 is in Deep Sleep will revive it.


### Default number key assignments (0-9)  (outside the menu)

* 0 = FUNCTION_0 (DCC Lights)
* 1 = FUNCTION_1 (DCC Bell)
* 2 = FUNCTION_3 (DCC Horn/Whistle)
* 3 = FUNCTION_3
* 4 = FUNCTION_4
* 5 = <strike>NEXT_THROTLE</strike>
* 6 = <strike>SPEED_MULTIPLIER</strike>
* 7 = <strike>DIRECTION_REVERSE</strike>
* 8 = <strike>SPEED_STOP</strike>
* 9 = <strike>DIRECTION_FORWARD</strike>

### Allowed assignments for the 0-9 keys:

Note: you need to edit config_buttons.h to alter these assignments   (copy config_buttons_example.h)
- FUNCTION_NULL   - don't do anything
- FUNCTION_0 - FUNCTION_31
- <strike>SPEED_STOP</strike>
- <strike>SPEED_UP</strike>
- <strike>SPEED_DOWN</strike>
- <strike>SPEED_UP_FAST</strike>
- <strike>SPEED_DOWN_FAST</strike>
- <strike>SPEED_MULTIPLIER</strike>
- E_STOP   - E Stop all locos on all throttles
- E_STOP_CURRENT_LOCO - E Stop locos on current throttle only
- POWER_TOGGLE
- POWER_ON
- POWER_OFF

### instructions for the potentiometers (pot) for the throttle

config_buttons.h can include the following optional defines:

  * \#define THROTTLE_POT_PIN 39
  * \#define THROTTLE_POT_USE_NOTCHES true 
  * \#define THROTTLE_POT_NOTCH_VALUES {1,585,1170,1755,2340,2925,3510,4094}
  * \#define THROTTLE_POT_NOTCH_SPEEDS {0,18,36,54,72,90,108,127} 

  You must specify the PIN to be used.  Currently PINs 34, 35 and 39 are the only ones that cannot be used by the app for other purposes, so these are the safest to use.  this should be connected to the centre pin of the pot. The 3v and GND should be connected to the outer pins of the pot.

    Note that eventually the app will upgraded to support a brake and reverser pots.  These will use pins 34 and 35 by default.

  The pot can be set to have 8 defined 'notches' (the default) or just a linear value.

  If you want to have the 8 notches:
  
  a) You must define the values the pot will send at each of 8 points - ``THROTTLE_POT_NOTCH_VALUES``.  Note that you should avoid the value zero (0) for notch zero.  Use at least 1 instead.

    The example values above are usable for a 50k ohm pot but any value pot can be used. Just adjust the numbers.

  b) You must define what speed should be sent for each notch - ``THROTTLE_POT_NOTCH_SPEEDS``

<strike>  If you want a linear speed instead of notches:

  a) You must define the values the pot will send at at zero throw and full throw in the first and last of the 8 values in ``THROTTLE_POT_NOTCH_VALUES``.  The other values will be ignored but you still need to include 8 values.  (They can be zero.)  Note that you should avoid the value zero (0) for notch zero.  Use at least 1 instead.
</strike>

  Sumner Patterson is developing an app to help find the appropriate pot values for ``THROTTLE_POT_NOTCH_VALUES``.

### Instructions for optional use of a voltage divider to show the battery charge level

TBA

Recommend adding a physical power switch as this will continually drain the battery, even when not being used.

*Pinouts for Optional Battery Monitor*
![Assembly diagram - Optional Battery Monitor](https://github.com/flash62au/WiTcontroller/blob/main/WiTcontroller%20-%20Optional%20battery%20monitor.png)

---
---

## Change Log

### v0.11
- add support for the Battery test from WiTController

### v0.10
- change target direction indicator

### v0.09
- bug fix brake position 6 [5] 

### v0.08
- added eStop toggle

### v0.07
- bug fix for brake last position
- reverse reverser

### v0.06
- revised throttle processing

### v0.05
- change to need one less entry in BRAKE_POT_VALUES than BRAKE_DELAY_TIMES

### v0.04
- added support for brake overriding the throttle setting.
- added support for neutral overriding the throttle setting.

### v0.03
- rudimentary momentum added
- support the acceleration multiplier   default to keypad-6

### v0.02
- added pot test. menu 9 -> 2 

### v0.02
- basic support of the reverser pot

### v0.01 
- setup
