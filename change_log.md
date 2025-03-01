# Change Log

### v0.29
- Added optional define for ``COAST_DELAY_TIMES`` separate from the ``ACCELLERATION_DELAY_TIMES``
- Removed #define for ``WITCONTROLLER_DEBUG`` and replaced it with ``HABDCAB_DEBUG``
- Reworking of the DEBUG_LEVEL code to reduce memory usage if not enabled

### v0.28
- change to a font that has accented characters

### v0.27
- added optional linear throttle (instead of notches). Don't install the spring and ball for the detent control

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
