# HandCab  
 ---
##   NOTE/WARNING:  Since the HandCab and software is relatively new you should check back for updates on the HandCab and Read_Pot_Values software/sketches and for any hardware changes.

  ### * 25-01-31 HandCab v0.27 (Added optional linear throttle instead of notches -- Don't install the spring and ball for the detent control)

 ---

A HandCab is a DIY handheld controller that talks to a WiThrottle Server (JMRI, DCC-EX  EX-CommandStation and many others) using the WiThrottle protocol to control DCC model trains.  HandCab uses 3 potentiometers (50K) for the throttle, brake and reverser controls and is modelled after a GP7/9 'barrel' control stand.  

Special thanks to Peter Akers who is responsible for the software on the throttle.  Without it this throttle would never have materialized.

![Alt text](https://github.com/1fatgmc/HandCab/blob/main/24-03-27%20HandCab%20vs%20GP9-1.jpg?raw=true)

---
  ##  Contact me at contactsumner1(at)gmail(dot)com
---
## Video Overview of the throttle in use.

https://www.youtube.com/watch?v=8vLRVYeE0dU

https://youtu.be/oAGohxnNBME

---
## Prerequisites

1. Requires moderate soldering skills.

2. Loading the code (sketch) requires downloading of one of the IDEs, this sketch, the libraries, etc. so some experience with Arduinos is helpful, but not critical.

3. The HandCab Throttle along with most Android and Apple devices running supported throttle apps 'should' (please let me know if it doesn't work with one of the followin) work with any WiThrottle Server such as JMRI, DCC-EX, MRC WiFi, Digitrax LnWi, NCE (WiFi/DCC NCE Cab Bus Interface), TCS CS-105 (Using WiFiTrax WFD-30 or WFD-31)  and others..

4. You should have your decoder in **128 speed step mode**.

---
## Building

You can find a detailed step-by-step build of the throttle on my website:
https://1fatgmc.com/RailRoad/DCC/HandCab-Index.html

---
## Print Files

Print files on thingiverse.com https://www.thingiverse.com/sumner/designs

---
## Parts

Required Components 
(links to where I bought the following & pricing at (https://1fatgmc.com/RailRoad/DCC/page-14.html))
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
 ---
## Wiring Schematic
*Pinouts with Additional 7 Buttons*
![Alt text](https://github.com/1fatgmc/HandCab/blob/main/24-06-16%20Wiring%20Large-Volt%20Meter-1.jpg?raw=true)

 ---
## Loading the code

1. Download the Arduino IDE.  
    * Available from  https://support.arduino.cc/hc/en-us/articles/360019833020-Download-and-install-Arduino-IDE

2. Download the esp32 boards in the Arduino IDE. **(See warning above. Use ESP32 by Espressif Systems Ver 3.0.0 or later)**
    * add the esp32 support with the following instructions:  (See here for detailed instructions:  https://randomnerdtutorials.com/installing-the-esp32-board-in-arduino-ide-windows-instructions/)
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
    * *WiThrottleProtocol.h* - Search for "WiThrottleProtocol" (not "WiThrottle").  Install version 1.1.20 or later if available
    
      Note: <br/> **DO NOT** download these libraries *directly*. Use the *Library Manager*. <br/> **DO NOT** put them in the WiTcontroller folder.
5.  These should have been automatically installed when you downloaded the esp32 boards. YOU SHOULD NOT NEED TO DO ANYTHING SPECIFIC TO GET THESE 
    * WiFi.h - https://github.com/espressif/arduino-esp32/tree/master/libraries/WiFi 
    * ESPmDNS.h - https://github.com/espressif/arduino-esp32/blob/master/libraries/ESPmDNS 

      Note: <br/> **DO NOT** download these libraries *directly*. Use the *Boards Manager*. <br/> **DO NOT** put them in the WiTcontroller folder.

6.   Copy ``config_network_example.h`` to a new file named ``config_network.h``. 
     * Then edit it to include the network ssids you want to use. (Not essential, but entering passwords via the encoder is tedious.)
You can have four saved ones.
     * I have my network and password shown.  Change it to yours unless you are using JMRI also and it might then work.
     * Save the file.
 
7.  Upload the Read_Pot_Values program/sketch.r.
     * **NOTE:** The pos/neg wires on the 'throttle' pot are wired opposite to how they are on the 'brake' & 'reverser' pots (very important)..
     * **NOTE:** When you run the Read_Pot_Values sketch the 'throttle' is off (notch 0) when all the way to the right detent.  The 'brake' & 'reverser' handles should start all the way to the left, opposite of the 'throttle.
     * Connect the ESP32 via USB to your computer.
     * The Read_Pot_Values software/sketch is in the Read_Pot_Values folder.  
       * Open the folder and run  Read_Pot_Values.INO       
       * Select the board type as "WEMOS LOLIN32 Lite" in the Arduino IDE.
         * Go to 'Tools', 'Board', 'esp32' then scroll down to 'WEMOS LOLIN32 Lite' and select it. 
       * Go to 'Tools',  'Port' and select the appropriate port.  More than one will probably show.  Unplug and re-plug to see which one you need. 
     * Click the Upload ==>  
     * If there is nothing on the screen turn the serial monitor on at bottom left of screen or 'Tools', 'Serial Monitor'
     * You will hit lower case 'c' and enter a number of times and will have to follow the instructions on the screen.
     * It will get to the point where it will give you numbers to write down that you will put in the 'config_buttons.h' file next.
     * You might want to go through the program a second time and see if the numbers are close.  
     * They won't be exact but should be within 100 each time you run it.  You can use any set of them for the next step.
 
8.  Copy ``config_buttons_example.h`` to a new file ``config_buttons.h``.  <br/> Open it in either a) The Arduino IDE, b) a plain text editor like Notepad (needs to stay plain text—not formatted).
     * Optionally, edit config_buttons.h to change the mapping of the keypad buttons to specific functions different from what they are (recommend you start what is there).
     * Optionally, edit config_buttons.h to configure the additional pushbuttons to specific functions different from what they are (recommend you start what is there).
     * Enter the 'Throttle' pot definitions.  There are instructions in the 'config_buttons_example.h' file itself.
     * Enter the 'Speed Step' definitions.  There are instructions in the 'config_buttons_example.h' file itself.
     * Enter the 'Brake' pot definitions.  There are instructions in the 'config_buttons_example.h' file itself.
     * Enter the 'Acceleration' definitions.  There are instructions in the 'config_buttons_example.h' file itself.
     * Enter the 'Braking' definitions.  There are instructions in the 'config_buttons_example.h' file itself.
     * Save the file when done.

9.  With the ESP32 still connected run HandCab.INO  
     * Click the ``Upload`` ==>
     * Program should load and come up with the first screen showing that it is browsing for SSIDs.
     * It will find and show any networks in the vicinity.
     * Click on the appropriate one.
     * It should connect if you have the correct network name and password in you 'config_network.h' file. 
     * It will then search for a WiT service and if found the HandCab menu should come up with the version (upper right).
     * Click on '*' for the menu and select a loco and off you go.
     * If there are problems go to the DCC-EX website (https://discord.com/invite/PuPnNMp8Qf) and go to the 'wifi-throttles' section and post.  Peter A, myself (Sumner) or someone will try and help you.

Notes: 
   * HandCab version 0.26 or later requires WiThrottleProtocol version 1.1.25 or later.
   * HandCab version 0.23 or later requires WiThrottleProtocol version 1.1.20 or later.
   * HandCab version 0.22 or later requires WiThrottleProtocol version 1.1.19 or later.
   * HandCab version 0.01 or later requires WiThrottleProtocol version 1.1.14 or later.
   * The *WiFi.h* and *ESPmDNS.h* libraries were automatically installed for me when I installed the esp32 boards, however you may need to install them manually.
   * Later versions of the esp board support are available and do appear to work, but if you have difficulties version 1.0.6 appears to be stable.
   * Later versions of the libraries generally should work, but if you have difficulties use the versions listed above.
   * To get the HandCab sketch I recommend using either the git command line, or the far more friendly 'GitHub Desktop' app.  See instructions above.
   * If you receive and error related to Python, and you are on MacOs 12 and above please edit the platform file, change from python to python3 as follows; preferences->user/path/arduino/packages/hardware/esp32/version/platform.txt and edit the line that looks as follows:tools.gen_esp32part.cmd=python3 "{runtime.platform.path}/tools/gen_esp32part.py"

---

## Using HandCab

**Currently functioning:**
- NOTE:  There are items shown below that are  <strike>striked out</strike>
.  They work on the WiTcontroller software but not the software for the HandCab because of the use of the 'pots' on the HandCab.  One can load the WiTcontroller software (https://github.com/flash62au/WiTcontroller) and use the throttle with the encoder but then the brake and reverser pots are non-functional.  You can select to use the throttle pot.

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
  - Toggle throuigh 3 levels of acceleration delays to change the momentum effect for a loco pulling trains with different loads.
  - Power Track On/Off
  - Disconnect / Reconnect
  - Put ESP32 in deep sleep and restart it- Limited dealing with unexpected disconnects.  It will throw you back to the WiThrottle Server selection screen.
- Boundary between short and long DCC addresses can be configured in config_buttons.h- The controller will automatically shut down if no SSID is selected or entered in 4 minutes (to conserve the battery)

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
  - 3 = Set / Unset a 2 times multiplier for the rotary encoder dial.
  - 4 = Throw turnout/point.  
     - Followed by the turnout/point number, followed by the \# to complete.  e.g. Throw turnout XX12 '\*512\#'  (where XX is a prefix defined in the sketch) 
     - or \# alone to show the list from the server   \# again will show the next page
  - 5 = Close turnout.    
     - Followed by the turnout/point number, followed by \# to complete.  e.g. Close turnout XX12 '\*612\#'  (where XX is a prefix defined in the sketch)
     - or \# alone to show the list from the server
  - 6 = Set Route.    
      - Followed by the Route number, followed by \# to complete.  e.g. to Set route XX:XX:0012 '\*60012\#'  (where \'XX:XX:\' is a prefix defined in the sketch)
      - or \# alone to show the list from the server   \# again will show the next page
  - 7 = Function button. Followed by...
      - the function number, Followed by \# to complete.  e.g. to set function 17 you would press '\*017\#'
      - \# alone, to show the list of functions.
  - 8 = Track Power On/Off.
  - 9 = Extras. Followed by...
      - 0 then \# to toggle the action the the \# key does as a direct action, either to show the direct action key definitions, or the Function labels.  
      - 1 to change the facing of locos in a consist.
      - 2 Show/Recalibrate POT values (see below)
      - 3 to toggle the heartbeat check.
      - 4 Show/Recalibrate throttle POT (see below)
      - 5 128/28 speed step mode toggle (default is 128)
      - 6 Disconnect/Reconnect.  
      - 7 (or 9) to put into deep sleep
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
* 5 = User assigned assignment
* 6 = User assigned assignment
* 7 = User assigned assignment
* 8 = User assigned assignment
* 9 = User assigned assignment

### Allowed assignments for the 0-9 keys:

Note: you need to edit config_buttons.h to alter these assignments   (copy config_buttons_example.h)
- FUNCTION_NULL   - don't do anything
- FUNCTION_0 - FUNCTION_31
- User assigned Function
- User assigned Function
- User assigned Function
- User assigned Function
- User assigned Function
- User assigned Function
- E_STOP   - E Stop all locos on all throttles
- E_STOP_CURRENT_LOCO - E Stop locos on current throttle only
- POWER_TOGGLE
- POWER_ON
- POWER_OFF

---

### Instructions for optional use of a voltage divider to show the battery charge level

TBA

#### USE_BATTERY_TEST

``#define USE_BATTERY_TEST true``

*To enable the battery monitor*, set the following to ``true``. The default is ``false``.

#### BATTERY_TEST_PIN

``#define BATTERY_TEST_PIN 36``

*To set which pin to use.* The default is ``36``.

#### BATTERY_CONVERSION_FACTOR

``#define BATTERY_CONVERSION_FACTOR 1.7``

If the battery does not show 100% when plugged into the charger, you may need to adjust this value. The default is ``1.7``.

    To help work out the correct BATTERY_CONVERSION_FACTOR, you can enable so serial monitor message that will assist.

    In your ``config_buttons.h`` add (or uncomment) these defines:
    
      #define WITCONTROLLER_DEBUG    0
      #define DEBUG_LEVEL   2

    DEBUG_LEVEL must be 2 or greater

    a) Make sure your battery is fully charged.
    b) Upload the code and open the serial monitor. 
    c) Wait. Don't connect.
    You will see lines like...

      BATTERY TestValue: 100 (10003)
      BATTERY lastAnalogReadValue: 2491 (10003)
      BATTERY If Battery full, BATTERY_CONVERSION_FACTOR should be: 1.69 (10014)

    Let it run for a while.
    d) Note one of the recommend values (it will vary a bit) and enter it into the define in your config_buttons.h
    e) Re-upload code and connect to a server
    f) Confirm that the battery reads 100% (repeat if not)
    g) Run the HandCab on battery for few hours and confirm the battery level is droping at an expected rate. (adjust the conversion factor if not.)

#### USE_BATTERY_PERCENT_AS_WELL_AS_ICON

``#define USE_BATTERY_PERCENT_AS_WELL_AS_ICON true``

*To show the calculated percentage*, set the following to ``true`` The default is ``false``.

#### USE_BATTERY_SLEEP_AT_PERCENT

``#define USE_BATTERY_SLEEP_AT_PERCENT 3``

*To force the HandCab to go to sleep at a specific level*, set this value. (e.g. to 3 or 5.) A value of less than zero (e.g. -1) will disable the feature. By default it is disabled (-1).

#### Battery Test Notes

* The display of the battery can be temporarily toggled by setting a key or button to ``SHOW_HIDE_BATTERY``.  The display will cycle between none, icon only and icon plus percent value. Note that ``USE_BATTERY_TEST`` must be set to `true` for this to have any effect.

* We recommend including a physical power switch to disconnect the battery as this feature will, slowly, continually drain the battery, even when not being used.

---

### Instructions for temporarily changing the pot values

Normally the pot values are set in ``config_buttons.h``.  
You can get the appropriate values using the **Read_Pot_Values** sketch.

We have found that occasionally (for reasons we don't understand) the values can 'drift'.  If that happens you will notice that you may not be able to get into the lower or higher brake or reverser positions, or the Throttle notch is not the one you expect.

You can use the following feature to temporarily recalibrate the pots and, if you wish, to store the values to non-volatile memory so they will be automatically restored at start-up.

#### To Temporarily Change the Brake values.

1. Open the 'Potentiometer Values' screen via the menu `#` -> `9` -> `2` 

   The values for all three pots are continuously shown on three rows:

   ``actual value`` <= ``averaged value``  :``position``: ``lowest values seen``-``highest value seen``  
   
   Where 'position' is the Throttle Notch (0-8), the Reverser position (0=Fwd 1=Nue 2=Rev) or the Brake position (0-5) based on the currently (temporary or permanent) pot values and the current position of each pot.

2. Move the Brake pot (lever) back and forwards several times

   The proposed values will be shown at the bottom of the screen prefixed with ``>B:``

3. press `9` to temporarily set the Brake Pot values it has discovered to be used for the current session (till you turn the device)

The values shown as a result of this process can be used instead of using the Read_Pot_Values sketch and added into config_buttons.h. The values will be similar.

#### To Temporarily change the Reverser values.

1. Open the 'Potentiometer Values' screen via the menu `#` -> `9` -> `2` 

   The values for all three pots are continuously shown.
2. press `2`

   The proposed values will be shown at the bottom of the screen prefixed with ``>R:``

3. Move the Reverser pot (lever) back and forwards several times
4. press `9` to temporarily set the values it has discovered to be used for the current session (till you turn the device)

The values shown as a result of this process can be used instead of using the **Read_Pot_Values** sketch and added into ``config_buttons.h``. The values will be similar.

#### To Temporarily change the Throttle values.

1. Open the 'Throttle Pot Values' screen via the menu `#` -> `9` -> `4` 
2. Move the Throttle pot (lever) to the first notch (throttle zero)
3. press `0` 
4. Move the Throttle pot (lever) to the second notch (throttle one)
5. press `1` 
... repeat for the remaining positions (2 to 8)

   The proposed values will be shown in the right side of the screen.

6. Press '9' to temporarily set the values it has discovered to be used for the current session (till you turn the device)

The values shown as a result of this process can be used instead of using the **Read_Pot_Values** sketch and added into ``config_buttons.h``. The values will be similar, but may not be identical.

#### Starting again and clearing the non-volatile memory

If you wish to start the temporary recalibration again.

1. Open the 'potentiometer Values' screen via the menu `#` -> `9` -> `2` 
2. Press `#`

    **Note that this also clears all the values from non-volatile memory.**

#### Saving the values to non-volatile memory

1. Open the 'potentiometer Values' screen via the menu `#` -> `9` -> `2` 
2. Press `8` to save the values to non-volatile memory

#### Notes 

  * **Once saved, these 'temporary' values will be automatically restored at device startup.**

  * **<span style="color:red;">If you no longer wish them to be restored at startup you must clear the values from non-volatile memory (see above).</span>**


### Optional linear Throttle Pot.  

i.e. No Notches

If you wish to have no notches for the throttle pot, you must
a) comment out the defines above for THROTTLE_POT_NOTCH_VALUES and THROTTLE_POT_NOTCH_VALUES
 b) uncomment and adjust the lines below as needed in config_buttons.h

note: There MUST be only two values for the two defines

``#define USE_THROTTLE_NOTCHES false``

``#define THROTTLE_POT_NOTCH_VALUES {0000,0000}``

Enter the numbers from the numbers generated by Read_Pot_Values program you run first.  It is only the first and last values you want.

``#define THROTTLE_POT_NOTCH_SPEEDS {0,126}``

Use values between 0-126. These numbers will be the speed step for the beginning, and the end of the pot range. The first MUST always be 0

Examples: 

  #define THROTTLE_POT_NOTCH_VALUES {1111,2837} // Run the Read_Pot-Values program to generate yours.
  
  #define THROTTLE_POT_NOTCH_SPEEDS {0, 126}    // Numbers to left give the locos their full speed range

If the throttle bounces at zero and maximum travel, uncomment and increase this value in config_buttons.h

  ```#define LINEAR_THROTTLE_POT_DEAD_ZONE 5```

---
---

## Change Log

### v0.28
- change to a font that has accented characters

### v0.27
- added optional linear throttle (instead of notches)

### v0.26
- Now requires version 1.1.25 of the WiThrottleProtocol library.
- Add define to enable or disable the heartbeat by default    ``#define HEARTBEAT_ENABLED true``
- New define added for if no response is received from the server in the specified period (milliseconds), shut down. MAX_HEARTBEAT_PERIOD   default is 4 minutes = 240000 

### v0.25
- Added code to help work out the BATTERY_CONVERSION_FACTOR

### v0.24
- Removed THROTTLE_POT_USE_NOTCHES references (left over from WiTcontroller)
- Added an option to toggle the Speed Step Mode 128/28 steps
  Now requires version 1.1.21 of the WiThrottleProtocol library.

### v0.22
- Fix for the EStop not working correctly
- Added support for the additional logging options in the WiThrottleProtocol library.  
  Now requires version 1.1.19 of the WiThrottleProtocol library.

### v0.21
- Added alternate calculation for the THROTTLE_POT_NOTCH_VALUES in Read_Pot_Values2

### v0.20
- fixed bug where it would not reliably go into notch 8
- Created a new version of Read_Pot_Values (Read_Pot_Values2).  
  Calculations are the same for now, code is simpler

### v0.19
- Added action ``SHOW_HIDE_BATTERY`` which can be assigned to any key or button
- Battery shows on most screens now

### v0.18
- Bug fix for the ability to store the 'temporary' pot values to non-volatile memory
- Added the ability override the conversion factor used to calculate the pattery level ``#define BATTERY_CONVERSION_FACTOR``. Default is 1.7
- Moved the battery indicator to the top right corner
- Brake and Accelleration indicators are hidden if zero

### v0.17
- Add the ability to store the 'temporary' pot values to non-volatile memory, and automatically restore them at startup.
- eStop bug fix
- change platfom.ini to specifically wemos_d1_mini32 (for VCS users only)

### v0.16
- Additional work to smooth out the pot values
- UI changes the pot value screens

### v0.15
- UI improvements for the Throttle Pot (temporary) recalibration

### v0.14
- Added dynamic (temporary) recalibration of the Reverser and Brake pots to the *92 menu  
- Added dynamic (temporary) recalibration of the 8 throttle pot notches to the *94 menu  

### v0.13
- automated fix the latest versions of the ESP32 Board Library (3.0.0 and later) having renamed an attribute. The code now automatically adjusts for this.  

## v0.12
- manual fix the latest versions of the ESP32 Board Library (3.0.0 and later) having renamed an attribute.

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
