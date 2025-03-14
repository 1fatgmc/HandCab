//
// DO NOT alter this file
//
// If you need to alter/override one of the defines in here, 
// do so in config_buttons.h  NOT HERE
//

const String appName = "HandCab";
const String appVersion = "v0.29";

#ifndef DEVICE_NAME
   #define DEVICE_NAME "HandCab"
#endif

#ifndef MENU_TEXT_MENU
   #define MENU_TEXT_MENU                      "* Menu                 # Key Defs"
#endif
#ifndef MENU_TEXT_MENU_HASH_IS_FUNCTIONS
   #define MENU_TEXT_MENU_HASH_IS_FUNCTIONS    "* Menu                          # Fn"
#endif
#ifndef MENU_TEXT_FINISH
   #define MENU_TEXT_FINISH                    "                        # Finish"
#endif
#ifndef MENU_TEXT_CANCEL
   #define MENU_TEXT_CANCEL                    "* Cancel"
#endif
#ifndef MENU_TEXT_SHOW_DIRECT
   #define MENU_TEXT_SHOW_DIRECT               "                  # Show Direct"
#endif
#ifndef MENU_TEXT_ROSTER
   #define MENU_TEXT_ROSTER                    "* Cancel      0-9      #Pg"
#endif
#ifndef MENU_TEXT_TURNOUT_LIST
   #define MENU_TEXT_TURNOUT_LIST              "* Cancel      0-9      #Pg"
#endif
#ifndef MENU_TEXT_ROUTE_LIST
   #define MENU_TEXT_ROUTE_LIST                "* Cancel      0-9      #Pg"
#endif
#ifndef MENU_TEXT_FUNCTION_LIST
   #define MENU_TEXT_FUNCTION_LIST             "* Cancel      0-9      #Pg"
#endif
#ifndef MENU_TEXT_SELECT_WIT_SERVICE
   #define MENU_TEXT_SELECT_WIT_SERVICE        "0-4      # Entry      E.btn OFF"
#endif
#ifndef MENU_TEXT_SELECT_WIT_ENTRY
   #define MENU_TEXT_SELECT_WIT_ENTRY          "0-9   * Back  # Go   E.btn OFF"
#endif
#ifndef MENU_TEXT_SELECT_SSIDS
   #define MENU_TEXT_SELECT_SSIDS              "0-9     # Search      E.btn OFF"
#endif
#ifndef MENU_TEXT_SELECT_SSIDS_FROM_FOUND
   #define MENU_TEXT_SELECT_SSIDS_FROM_FOUND   "0-4    9 List  # Pg   E.btn OFF"
#endif
#ifndef MENU_TEXT_ENTER_SSID_PASSWORD
   #define MENU_TEXT_ENTER_SSID_PASSWORD       "E Chrs  E.btn Slct  # Go  * Bck"
#endif
#ifndef MENU_TEXT_POT_VALUES
   #define MENU_TEXT_POT_VALUES                "* Cancel"
#endif
#ifndef MENU_TEXT_THROTTLE_POT_VALUES
   #define MENU_TEXT_THROTTLE_POT_VALUES       "* Cancel"
#endif

const String menu_text[16] = {
  MENU_TEXT_MENU,
  MENU_TEXT_MENU_HASH_IS_FUNCTIONS,
  MENU_TEXT_FINISH,
  MENU_TEXT_CANCEL,
  MENU_TEXT_SHOW_DIRECT,
  MENU_TEXT_ROSTER,
  MENU_TEXT_TURNOUT_LIST,
  MENU_TEXT_ROUTE_LIST,
  MENU_TEXT_FUNCTION_LIST,
  MENU_TEXT_SELECT_WIT_SERVICE,
  MENU_TEXT_SELECT_WIT_ENTRY,
  MENU_TEXT_SELECT_SSIDS,
  MENU_TEXT_SELECT_SSIDS_FROM_FOUND,
  MENU_TEXT_ENTER_SSID_PASSWORD,
  MENU_TEXT_POT_VALUES,
  MENU_TEXT_THROTTLE_POT_VALUES
};

const int menu_menu =                     0;
const int menu_menu_hash_is_functions =   1;
const int menu_finish =                   2;
const int menu_cancel =                   3;
const int menu_show_direct =              4;
const int menu_roster =                   5;
const int menu_turnout_list =             6;
const int menu_route_list =               7;
const int menu_function_list =            8;
const int menu_select_wit_service =       9;
const int menu_select_wit_entry =        10;
const int menu_select_ssids =            11;
const int menu_select_ssids_from_found = 12;
const int menu_enter_ssid_password =     13;
const int menu_pot_values =              14;
const int menu_throttle_pot_values =     15;

const int last_oled_screen_speed =            0;
const int last_oled_screen_roster =           1;
const int last_oled_screen_turnout_list =     2;
const int last_oled_screen_route_list =       3;
const int last_oled_screen_function_list =    4;
const int last_oled_screen_menu          =    5;
const int last_oled_screen_extra_submenu =    6;
const int last_oled_screen_all_locos =        7;
const int last_oled_screen_edit_consist =     8;
const int last_oled_screen_direct_commands =  9;
const int last_oled_screen_pot_values     =  10;
const int last_oled_screen_throttle_pot_values = 11;

typedef enum PotDirection {
    REVERSE = 0,
    FORWARD = 1,
    NEUTRAL = 2
} PotDirection;

typedef enum WhenToUseSmallFonts {
    USE_NO_SMALL_FONTS = 0,
    USE_POTS_SMALL_FONTS = 1,
    USE_THROTTLE_POT_SMALL_FONTS = 2
} WhenToUseSmallFonts;

typedef enum ShowBattery {
    NONE = 0,
    ICON_ONLY = 1,
    ICON_AND_PERCENT = 2
} ShowBattery;

#ifndef DIRECT_COMMAND_LIST
  #define DIRECT_COMMAND_LIST            "Direct Commands"
#endif 
#ifndef DIRECTION_FORWARD_TEXT
  #define DIRECTION_FORWARD_TEXT         "Fwd"
#endif 
#ifndef DIRECTION_REVERSE_TEXT
  #define DIRECTION_REVERSE_TEXT         "Rev"
#endif 
#ifndef DIRECTION_NEUTRAL_TEXT
  #define DIRECTION_NEUTRAL_TEXT         "Neu"
#endif 
#ifndef DIRECTION_FORWARD_TEXT_SHORT
  #define DIRECTION_FORWARD_TEXT_SHORT   ">"
#endif 
#ifndef DIRECTION_REVERSE_TEXT_SHORT
  #define DIRECTION_REVERSE_TEXT_SHORT   "<"
#endif 
#ifndef DIRECTION_REVERSE_INDICATOR
  #define DIRECTION_REVERSE_INDICATOR    "'"
#endif 

// const String function_states = "fn ";

#ifndef MSG_START
  #define MSG_START                     "Start"
#endif
#ifndef MSG_BROWSING_FOR_SERVICE
  #define MSG_BROWSING_FOR_SERVICE      "Browsing for WiT services"
#endif
#ifndef MSG_BROWSING_FOR_SSIDS
  #define MSG_BROWSING_FOR_SSIDS        "Browsing for SSIDs"
#endif
#ifndef MSG_NO_SSIDS_FOUND
  #define MSG_NO_SSIDS_FOUND            "No SSIDs found"
#endif
#ifndef MSG_SSIDS_LISTED
  #define MSG_SSIDS_LISTED              "  SSIDs listed"
#endif
#ifndef MSG_SSIDS_FOUND
  #define MSG_SSIDS_FOUND               "    SSIDs found"
#endif
#ifndef MSG_BOUNJOUR_SETUP_FAILED
  #define MSG_BOUNJOUR_SETUP_FAILED     "Unable to setup Listener"
#endif
#ifndef MSG_NO_SERVICES_FOUND
  #define MSG_NO_SERVICES_FOUND         "No services found"
#endif
#ifndef MSG_NO_SERVICES_FOUND_ENTRY_REQUIRED
  #define MSG_NO_SERVICES_FOUND_ENTRY_REQUIRED "Enter witServer IP:Port"
#endif
#ifndef MSG_SERVICES_FOUND
  #define MSG_SERVICES_FOUND            " Service(s) found"
#endif
#ifndef MSG_TRYING_TO_CONNECT
  #define MSG_TRYING_TO_CONNECT         "Trying to Connect"
#endif
#ifndef MSG_CONNECTED
  #define MSG_CONNECTED                 "             Connected"
#endif
#ifndef MSG_CONNECTING
  #define MSG_CONNECTING                "             Connecting..."
#endif

#ifndef MSG_ADDRESS_LABEL
   #define MSG_ADDRESS_LABEL            "IP address: "
#endif
#ifndef MSG_CONNECTION_FAILED
   #define MSG_CONNECTION_FAILED        "Connection failed"
#endif
#ifndef MSG_DISCONNECTED
   #define MSG_DISCONNECTED             "Disconnected"
#endif
#ifndef MSG_AUTO_SLEEP
   #define MSG_AUTO_SLEEP               "Waited too long for Select"
#endif
#ifndef MSG_BATTERY_SLEEP
   #define MSG_BATTERY_SLEEP            "Battery critically low"
#endif
#ifndef MSG_START_SLEEP
   #define MSG_START_SLEEP              "Shutting Down.        E.btn ON"
#endif
#ifndef MSG_THROTTLE_NUMBER
   #define MSG_THROTTLE_NUMBER          "          Throttle #"
#endif
#ifndef MSG_NO_LOCO_SELECTED
   #define MSG_NO_LOCO_SELECTED         "         No Loco selected"
#endif
#ifndef MSG_ENTER_PASSWORD
   #define MSG_ENTER_PASSWORD           "Enter Password"
#endif
#ifndef MSG_GUESSED_EX_CS_WIT_SERVER
   #define MSG_GUESSED_EX_CS_WIT_SERVER "'Guessed' EX-CS WiT server"
#endif
#ifndef MSG_BYPASS_WIT_SERVER_SEARCH
   #define MSG_BYPASS_WIT_SERVER_SEARCH     "Bypass WiT server search"
#endif
#ifndef MSG_NO_FUNCTIONS
   #define MSG_NO_FUNCTIONS             "Function List - No Functions"
#endif
#ifndef MSG_HEARTBEAT_CHECK_ENABLED
   #define MSG_HEARTBEAT_CHECK_ENABLED  "Heartbeat Check Enabled"
#endif
#ifndef MSG_HEARTBEAT_CHECK_DISABLED
   #define MSG_HEARTBEAT_CHECK_DISABLED "Heartbeat Check Disabled"
#endif
#ifndef MSG_POT_INCOMPLETE
   #define MSG_POT_INCOMPLETE "  Move full range, many times"
#endif
#ifndef MSG_THROTTLE_POT_INCOMPLETE
   #define MSG_THROTTLE_POT_INCOMPLETE "  Set all notches"
#endif

// const String label_locos = "Locos: ";
// const String label_speed = "Speed: ";
const String label_track_power = "TRK";
// const String label_unknown = "?";
// const String label_on = "On" ;
// const String label_off = "Off";

const int glyph_heartbeat_off = 0x00b7;
const int glyph_track_power = 0x00eb;
const int glyph_speed_step = 0x0070;
const int glyph_brake_position = 0x006d;
const int glyph_target_speed = 0x010d;
const int glyph_target_direction_forward = 0x00d8;
const int glyph_target_direction_neutral = 0x00d2;
const int glyph_target_direction_reverse = 0x00d7;
const int glyph_eStop = 0x0079;
const int glyph_128_step_mode = 0x0103;

#define SLEEP_REASON_COMMAND 0
#define SLEEP_REASON_INACTIVITY 1
#define SLEEP_REASON_BATTERY 2

#define KEYPAD_USE_OPERATION 0
#define KEYPAD_USE_SELECT_SSID 1
#define KEYPAD_USE_SELECT_SSID_FROM_FOUND 2
#define KEYPAD_USE_SELECT_WITHROTTLE_SERVER 3
#define KEYPAD_USE_ENTER_WITHROTTLE_SERVER 4
#define KEYPAD_USE_SELECT_ROSTER 5
#define KEYPAD_USE_SELECT_TURNOUTS_THROW 6
#define KEYPAD_USE_SELECT_TURNOUTS_CLOSE 7
#define KEYPAD_USE_SELECT_ROUTES 8
#define KEYPAD_USE_SELECT_FUNCTION 9
#define KEYPAD_USE_ENTER_SSID_PASSWORD 10
#define KEYPAD_USE_EDIT_CONSIST 11
#define KEYPAD_USE_POT_VALUES 12
#define KEYPAD_USE_THROTTLE_POT_VALUES 13

#define ENCODER_USE_OPERATION 0
#define ENCODER_USE_SSID_PASSWORD 1

// used for both wit and ssid
#define CONNECTION_STATE_DISCONNECTED 0
#define CONNECTION_STATE_CONNECTED 1
#define CONNECTION_STATE_SELECTION_REQUIRED 2
#define CONNECTION_STATE_ENTRY_REQUIRED 3
#define CONNECTION_STATE_SELECTED 4
#define CONNECTION_STATE_PASSWORD_ENTRY 5
#define CONNECTION_STATE_ENTERED 6

#define SSID_CONNECTION_SOURCE_LIST 0
#define SSID_CONNECTION_SOURCE_BROWSE 1

#define CMD_FUNCTION 0

#define MAX_LOCOS     10  // maximum number of locos that can be added to the consist

#define MAX_FUNCTIONS 32

#define MENU_ITEM_FUNCTION '0'
#define MENU_ITEM_ADD_LOCO '1'
#define MENU_ITEM_DROP_LOCO '2'
// #define MENU_ITEM_TOGGLE_DIRECTION '3'
#define MENU_ITEM_SPEED_STEP_MULTIPLIER '4'
#define MENU_ITEM_THROW_POINT '5'
#define MENU_ITEM_CLOSE_POINT '6'
#define MENU_ITEM_ROUTE '7'
#define MENU_ITEM_TRACK_POWER '8'
#define MENU_ITEM_EXTRAS '9'


#ifndef MENU_ITEM_TEXT_TITLE_FUNCTION
   #define MENU_ITEM_TEXT_TITLE_FUNCTION               "Function"
#endif
#ifndef MENU_ITEM_TEXT_TITLE_ADD_LOCO
   #define MENU_ITEM_TEXT_TITLE_ADD_LOCO               "Add Loco"
#endif
#ifndef MENU_ITEM_TEXT_TITLE_DROP_LOCO
   #define MENU_ITEM_TEXT_TITLE_DROP_LOCO              "Drop Loco"
#endif
#ifndef MENU_ITEM_TEXT_TITLE_SPEED_STEP_MULTIPLIER
   #define MENU_ITEM_TEXT_TITLE_SPEED_STEP_MULTIPLIER  "X Speed Step"
#endif
#ifndef MENU_ITEM_TEXT_TITLE_THROW_POINT
   #define MENU_ITEM_TEXT_TITLE_THROW_POINT            "Throw Point"
#endif
#ifndef MENU_ITEM_TEXT_TITLE_CLOSE_POINT
   #define MENU_ITEM_TEXT_TITLE_CLOSE_POINT            "Close Point"
#endif
#ifndef MENU_ITEM_TEXT_TITLE_ROUTE
   #define MENU_ITEM_TEXT_TITLE_ROUTE                  "Route"
#endif
#ifndef MENU_ITEM_TEXT_TITLE_TRACK_POWER
   #define MENU_ITEM_TEXT_TITLE_TRACK_POWER            "Trk Power"
#endif
#ifndef MENU_ITEM_TEXT_TITLE_EXTRAS
   #define MENU_ITEM_TEXT_TITLE_EXTRAS                 "Extras"
#endif
#ifndef MENU_ITEM_TEXT_TITLE_HEARTBEAT
   #define MENU_ITEM_TEXT_TITLE_HEARTBEAT              "Heartbeat"
#endif
#ifndef MENU_ITEM_TEXT_TITLE_EDIT_CONSIST
   #define MENU_ITEM_TEXT_TITLE_EDIT_CONSIST           "Edit Consist Facing"
#endif
#ifndef MENU_ITEM_TEXT_TITLE_POT_VALUES
   #define MENU_ITEM_TEXT_TITLE_POT_VALUES             "Potentiometer Values"
#endif
#ifndef MENU_ITEM_TEXT_TITLE_THROTTLE_POT_VALUES
   #define MENU_ITEM_TEXT_TITLE_THROTTLE_POT_VALUES    "Throttle Pot Values"
#endif

#ifndef MENU_ITEM_TEXT_MENU_FUNCTION
   #define MENU_ITEM_TEXT_MENU_FUNCTION               "no+# Select   * Cancel  # List"
#endif
#ifndef MENU_ITEM_TEXT_MENU_ADD_LOCO
   #define MENU_ITEM_TEXT_MENU_ADD_LOCO               "addr+# Add  * Cancel  # Roster"
#endif
#ifndef MENU_ITEM_TEXT_MENU_DROP_LOCO
   #define MENU_ITEM_TEXT_MENU_DROP_LOCO              "addr+# One   * Cancel    # All"
#endif
// #ifndef MENU_ITEM_TEXT_MENU_TOGGLE_DIRECTION
//    #define MENU_ITEM_TEXT_MENU_TOGGLE_DIRECTION       "# Toggle                      "
// #endif
#ifndef MENU_ITEM_TEXT_MENU_SPEED_STEP_MULTIPLIER
   #define MENU_ITEM_TEXT_MENU_SPEED_STEP_MULTIPLIER  "* Cancel                # Toggle"
#endif
#ifndef MENU_ITEM_TEXT_MENU_THROW_POINT
   #define MENU_ITEM_TEXT_MENU_THROW_POINT            "no+# Throw   * Cancel   # List"
#endif
#ifndef MENU_ITEM_TEXT_MENU_CLOSE_POINT
   #define MENU_ITEM_TEXT_MENU_CLOSE_POINT            "no+# Close   * Cancel   # List"
#endif
#ifndef MENU_ITEM_TEXT_MENU_ROUTE
   #define MENU_ITEM_TEXT_MENU_ROUTE                  "no+# Select  * Cancel   # List"
#endif
#ifndef MENU_ITEM_TEXT_MENU_TRACK_POWER
   #define MENU_ITEM_TEXT_MENU_TRACK_POWER            "* Cancel                # Toggle"
#endif
#ifndef MENU_ITEM_TEXT_MENU_EXTRAS
   #define MENU_ITEM_TEXT_MENU_EXTRAS                 "no Select  * Cancel         "
#endif
#ifndef MENU_ITEM_TEXT_MENU_HEARTBEAT
   #define MENU_ITEM_TEXT_MENU_HEARTBEAT              "* Close                       "
#endif
#ifndef MENU_ITEM_TEXT_MENU_EDIT_CONSIST
   #define MENU_ITEM_TEXT_MENU_EDIT_CONSIST           "no Chng Facing   * Close"
#endif
#ifndef MENU_ITEM_TEXT_MENU_POT_VALUES
   #define MENU_ITEM_TEXT_MENU_POT_VALUES             "* Close 1B 2R  8sav 9set #clr"
#endif
#ifndef MENU_ITEM_TEXT_MENU_THROTTLE_POT_VALUES
   #define MENU_ITEM_TEXT_MENU_THROTTLE_POT_VALUES    "* Close  0-8 set ntch  9 Set"
#endif

const String menuText[15][3] = {
  {MENU_ITEM_TEXT_TITLE_FUNCTION,              MENU_ITEM_TEXT_MENU_FUNCTION, ""},   //0
  {MENU_ITEM_TEXT_TITLE_ADD_LOCO,              MENU_ITEM_TEXT_MENU_ADD_LOCO, ""},   //1
  {MENU_ITEM_TEXT_TITLE_DROP_LOCO,             MENU_ITEM_TEXT_MENU_DROP_LOCO, ""},   //2
  {"",      "", ""},   //3
  {MENU_ITEM_TEXT_TITLE_SPEED_STEP_MULTIPLIER, MENU_ITEM_TEXT_MENU_SPEED_STEP_MULTIPLIER, ""}, //4     //TODO: remove
  {MENU_ITEM_TEXT_TITLE_THROW_POINT,           MENU_ITEM_TEXT_MENU_THROW_POINT, ""},   //5
  {MENU_ITEM_TEXT_TITLE_CLOSE_POINT,           MENU_ITEM_TEXT_MENU_CLOSE_POINT, ""},   //6
  {MENU_ITEM_TEXT_TITLE_ROUTE,                 MENU_ITEM_TEXT_MENU_ROUTE,       ""},   //7
  {MENU_ITEM_TEXT_TITLE_TRACK_POWER,           MENU_ITEM_TEXT_MENU_TRACK_POWER, ""},   //8 
  {MENU_ITEM_TEXT_TITLE_EXTRAS,                MENU_ITEM_TEXT_MENU_EXTRAS,      ""},   //9

  {MENU_ITEM_TEXT_TITLE_HEARTBEAT,             MENU_ITEM_TEXT_MENU_HEARTBEAT,           ""},   //10
  {MENU_ITEM_TEXT_TITLE_EDIT_CONSIST,          MENU_ITEM_TEXT_MENU_EDIT_CONSIST,        ""},   //11
  {MENU_ITEM_TEXT_TITLE_POT_VALUES,            MENU_ITEM_TEXT_MENU_POT_VALUES,          ""},   //12
  {"",                                         "",                                      ""},   //13
  {MENU_ITEM_TEXT_TITLE_THROTTLE_POT_VALUES,   MENU_ITEM_TEXT_MENU_THROTTLE_POT_VALUES, ""}    //14
};

const int menuCharsRequired[15] = {  // 0=none effectively a direct command / 1=one used for sub menus / 2=one or more
  2,   //0
  2,   //1
  2,   //2
  0,   //3
  0,   //4
  2,   //5
  2,   //6
  2,   //7
  0,   //8
  1,   //9
  2,   //10
  2,   //11
  1,   //12
  1,   //13
  1    //14
};


#ifndef EXTRA_MENU_TEXT_CHAR_FUNCTION_KEY_TOGGLE
   #define EXTRA_MENU_TEXT_CHAR_FUNCTION_KEY_TOGGLE    "Fnc/Key Tgl"
#endif
#ifndef EXTRA_MENU_TEXT_CHAR_EDIT_CONSIST
   #define EXTRA_MENU_TEXT_CHAR_EDIT_CONSIST           "Edt Consist"
#endif
#ifndef EXTRA_MENU_TEXT_CHAR_POT_VALUES
   #define EXTRA_MENU_TEXT_CHAR_POT_VALUES             "POT values"
#endif
#ifndef EXTRA_MENU_TEXT_CHAR_THROTTLE_POT_VALUES
   #define EXTRA_MENU_TEXT_CHAR_THROTTLE_POT_VALUES    "Throttle POT"
#endif
#ifndef EXTRA_MENU_TEXT_CHAR_TBA
   #define EXTRA_MENU_TEXT_CHAR_TBA                    "" 
#endif
#ifndef EXTRA_MENU_TEXT_CHAR_SPEED_STEP_TOGGLE
   #define EXTRA_MENU_TEXT_CHAR_SPEED_STEP_TOGGLE      "Spd Step Tgl" 
#endif
#ifndef EXTRA_MENU_TEXT_CHAR_HEARTBEAT_TOGGLE
   #define EXTRA_MENU_TEXT_CHAR_HEARTBEAT_TOGGLE       "Heartbt Tgl"
#endif
#ifndef EXTRA_MENU_TEXT_CHAR_DISCONNECT
   #define EXTRA_MENU_TEXT_CHAR_DISCONNECT             "Disconnect"
#endif
#ifndef EXTRA_MENU_TEXT_CHAR_OFF_SLEEP
   #define EXTRA_MENU_TEXT_CHAR_OFF_SLEEP              "OFF / Sleep"
#endif

const String extraSubMenuText[10] = { 
   EXTRA_MENU_TEXT_CHAR_FUNCTION_KEY_TOGGLE,      //0
   EXTRA_MENU_TEXT_CHAR_EDIT_CONSIST,             //1
   EXTRA_MENU_TEXT_CHAR_POT_VALUES,               //2
   EXTRA_MENU_TEXT_CHAR_HEARTBEAT_TOGGLE,         //3  
   EXTRA_MENU_TEXT_CHAR_THROTTLE_POT_VALUES,      //4
   EXTRA_MENU_TEXT_CHAR_SPEED_STEP_TOGGLE,        //5
   EXTRA_MENU_TEXT_CHAR_DISCONNECT,               //6
   EXTRA_MENU_TEXT_CHAR_OFF_SLEEP,                //7
   EXTRA_MENU_TEXT_CHAR_TBA,                      //8
   EXTRA_MENU_TEXT_CHAR_TBA};                     //9

#define EXTRA_MENU_CHAR_FUNCTION_KEY_TOGGLE    '0'
#define EXTRA_MENU_CHAR_EDIT_CONSIST           '1'
#define EXTRA_MENU_CHAR_POT_VALUES             '2'
#define EXTRA_MENU_CHAR_HEARTBEAT_TOGGLE       '3'
#define EXTRA_MENU_CHAR_THROTTLE_POT_VALUES    '4'
#define EXTRA_MENU_CHAR_SPEED_STEP_TOGGLE      '5'
#define EXTRA_MENU_CHAR_DISCONNECT             '6'
#define EXTRA_MENU_CHAR_OFF_SLEEP              '7'
#define EXTRA_MENU_CHAR_OFF_SLEEP_HIDDEN       '9'  // doesn't appear in the menu

// if defined in config_buttons.h these values will be overwritten
//
// speed increase for each click of the encoder 
#ifdef SPEED_STEP
  const int speedStep = SPEED_STEP;
#else
  const int speedStep = 4;
#endif
#ifdef SPEED_STEP_MULTIPLIER
  const int speedStepMultiplier = SPEED_STEP_MULTIPLIER;  // for 'fast' speed steps
#else
  const int speedStepMultiplier = 3;  // for 'fast' speed steps
#endif
// Additional multiplier.  If the multiplier is enabled from the menu, each rotation of the encoder becomes the speedStep * the AdditionalMultiplier
#ifdef SPEED_STEP_MULTIPLIER
  const int speedStepAdditionalMultiplier = SPEED_STEP_MULTIPLIER;
#else
  const int speedStepAdditionalMultiplier = 2;
#endif

#ifdef  DISPLAY_SPEED_AS_PERCENT
  const bool speedDisplayAsPercent = DISPLAY_SPEED_AS_PERCENT;
#else
  const bool speedDisplayAsPercent = false;
#endif

#ifdef  DISPLAY_SPEED_AS_0_TO_28
  const bool speedDisplayAs0to28 = DISPLAY_SPEED_AS_0_TO_28;
#else
  const bool speedDisplayAs0to28 = false;
#endif

String witServerIpAndPortEntryMask = "###.###.###.###:#####";

#ifndef DEFAULT_IP_AND_PORT 
  #define DEFAULT_IP_AND_PORT ""
#endif

#ifndef SSID_CONNECTION_TIMEOUT 
  #define SSID_CONNECTION_TIMEOUT 10000
#endif

const char ssidPasswordBlankChar = 164;


#ifndef SHORT_DCC_ADDRESS_LIMIT
  #define SHORT_DCC_ADDRESS_LIMIT 127
#endif

// *******************************************************************************************************************

#define FONT_DEFAULT u8g2_font_NokiaSmallPlain_te
#define FONT_FUNCTION_INDICATORS u8g2_font_tiny_simon_tr
#define FONT_THROTTLE_NUMBER u8g2_font_neuecraft_tr
#define FONT_PASSWORD u8g2_font_9x15_tf
#define FONT_SPEED u8g2_font_profont29_mr
// #define FONT_DIRECTION u8g2_font_profont17_tr
#define FONT_DIRECTION u8g2_font_neuecraft_tr
#define FONT_GLYPHS u8g2_font_open_iconic_all_1x_t

// *******************************************************************************************************************

# define DCC_EX_TURNOUT_PREFIX ""
# define DCC_EX_ROUTE_PREFIX "R"

#define CONSIST_LEAD_LOCO 0
#define CONSIST_ALL_LOCOS 1

// *******************************************************************************************************************

#ifndef HANDCAB_DEBUG 
  #define HANDCAB_DEBUG    0
#endif

#ifndef WITHROTTLE_PROTOCOL_DEBUG
  #define WITHROTTLE_PROTOCOL_DEBUG   1
#endif
#ifndef DEBUG_LEVEL
  #define DEBUG_LEVEL   1
#endif

// *******************************************************************************************************************

#ifndef AUTO_CONNECT_TO_FIRST_DEFINED_SERVER
  #define AUTO_CONNECT_TO_FIRST_DEFINED_SERVER false
#endif

#ifndef AUTO_CONNECT_TO_FIRST_WITHROTTLE_SERVER
  #define AUTO_CONNECT_TO_FIRST_WITHROTTLE_SERVER true
#endif

#ifndef OUTBOUND_COMMANDS_MINIMUM_DELAY
  #define OUTBOUND_COMMANDS_MINIMUM_DELAY 50
#endif

#ifndef SSID_NAME_FOR_COMMANDS_NEED_LEADING_CR_LF
  #define SSID_NAME_FOR_COMMANDS_NEED_LEADING_CR_LF "wftrx_"
#endif

#ifndef ROTARY_ENCODER_DEBOUNCE_TIME
  #define ROTARY_ENCODER_DEBOUNCE_TIME 200
#endif

// *******************************************************************************************************************

#ifndef F0_LATCHING
  #define F0_LATCHING true
#endif

#ifndef F1_LATCHING
  #define F1_LATCHING false
#endif

#ifndef F2_LATCHING
  #define F2_LATCHING false
#endif

#ifndef F0_LABEL
  #define F0_LABEL "Light"
#endif

#ifndef F1_LABEL
  #define F1_LABEL "Bell"
#endif

#ifndef F2_LABEL
  #define F2_LABEL "Horn"
#endif

// *******************************************************************************************************************

#ifndef ROTARY_ENCODER_DEBOUNCE_TIME
  #define ROTARY_ENCODER_DEBOUNCE_TIME 200
#endif

#ifndef SEARCH_ROSTER_ON_ENTRY_OF_DCC_ADDRESS
  #define SEARCH_ROSTER_ON_ENTRY_OF_DCC_ADDRESS false
#endif

// *******************************************************************************************************************
// consists follow functions
#ifndef CONSIST_FUNCTION_FOLLOW_F0
    #define CONSIST_FUNCTION_FOLLOW_F0                  CONSIST_ALL_LOCOS            // lights
#endif
#ifndef CONSIST_FUNCTION_FOLLOW_F1
    #define CONSIST_FUNCTION_FOLLOW_F1                  CONSIST_LEAD_LOCO            // bell
#endif
#ifndef CONSIST_FUNCTION_FOLLOW_F2
    #define CONSIST_FUNCTION_FOLLOW_F2                  CONSIST_LEAD_LOCO            // horn
#endif
#ifndef CONSIST_FUNCTION_FOLLOW_OTHER_FUNCTIONS
    #define CONSIST_FUNCTION_FOLLOW_OTHER_FUNCTIONS     CONSIST_LEAD_LOCO
#endif

// *******************************************************************************************************************
// other options
#ifndef HASH_SHOWS_FUNCTIONS_INSTEAD_OF_KEY_DEFS
    #define HASH_SHOWS_FUNCTIONS_INSTEAD_OF_KEY_DEFS false  // default if not defined in config_buttons.h
#endif

#ifndef ENCODER_BUTTON_ACTION
    #define ENCODER_BUTTON_ACTION E_STOP  // default if not defined in config_buttons.h
#endif

// *******************************************************************************************************************
// OLED


#ifdef U8X8_HAVE_HW_SPI
    #include <SPI.h>                       // add to include path [Arduino install]\hardware\arduino\avr\libraries\SPI\src
#endif
#ifdef U8X8_HAVE_HW_I2C
    #include <Wire.h>                      // add to include path [Arduino install]\hardware\arduino\avr\libraries\Wire\src
#endif

// U8g2 Constructor List (Frame Buffer)
// you can overide this in config_buttons.h     DO NOT CHANGE IT HERE
#ifndef OLED_TYPE

    U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE, /* clock=*/ 22, /* data=*/ 23);
#else
    OLED_TYPE
#endif

// *******************************************************************************************************************
// additional / optional commands
//  these can be any legigitmate WiThrottle protocol command
// refer to https://www.jmri.org/help/en/package/jmri/jmrit/withrottle/Protocol.shtml

#ifndef CUSTOM_COMMAND_1
   #define CUSTOM_COMMAND_1 ""
#endif 
#ifndef CUSTOM_COMMAND_2
   #define CUSTOM_COMMAND_2 ""
#endif 
#ifndef CUSTOM_COMMAND_3
   #define CUSTOM_COMMAND_3 ""
#endif 
#ifndef CUSTOM_COMMAND_4
   #define CUSTOM_COMMAND_4 ""
#endif 
#ifndef CUSTOM_COMMAND_5
   #define CUSTOM_COMMAND_5 ""
#endif 
#ifndef CUSTOM_COMMAND_6
   #define CUSTOM_COMMAND_6 ""
#endif 
#ifndef CUSTOM_COMMAND_7
   #define CUSTOM_COMMAND_7 ""
#endif 



// *******************************************************************************************************************
// default direct functions

#ifndef CHOSEN_KEYPAD_0_FUNCTION
   #define CHOSEN_KEYPAD_0_FUNCTION     FUNCTION_0
#endif
#ifndef CHOSEN_KEYPAD_1_FUNCTION
   #define CHOSEN_KEYPAD_1_FUNCTION     FUNCTION_1
#endif
#ifndef CHOSEN_KEYPAD_2_FUNCTION
   #define CHOSEN_KEYPAD_2_FUNCTION     FUNCTION_2
#endif
#ifndef CHOSEN_KEYPAD_3_FUNCTION
   #define CHOSEN_KEYPAD_3_FUNCTION     FUNCTION_3
#endif
#ifndef CHOSEN_KEYPAD_4_FUNCTION
   #define CHOSEN_KEYPAD_4_FUNCTION     FUNCTION_4
#endif
#ifndef CHOSEN_KEYPAD_5_FUNCTION
   #define CHOSEN_KEYPAD_5_FUNCTION     FUNCTION_5
#endif
#ifndef CHOSEN_KEYPAD_6_FUNCTION
   #define CHOSEN_KEYPAD_6_FUNCTION     SPEED_MULTIPLIER
#endif
#ifndef CHOSEN_KEYPAD_7_FUNCTION
   #define CHOSEN_KEYPAD_7_FUNCTION     FUNCTION_7
#endif
#ifndef CHOSEN_KEYPAD_8_FUNCTION
   #define CHOSEN_KEYPAD_8_FUNCTION     E_STOP
#endif
#ifndef CHOSEN_KEYPAD_9_FUNCTION
   #define CHOSEN_KEYPAD_9_FUNCTION     FUNCTION_9
#endif
#ifndef CHOSEN_KEYPAD_A_FUNCTION
   #define CHOSEN_KEYPAD_A_FUNCTION     CUSTOM_1
#endif
#ifndef CHOSEN_KEYPAD_B_FUNCTION
   #define CHOSEN_KEYPAD_B_FUNCTION     CUSTOM_2
#endif
#ifndef CHOSEN_KEYPAD_C_FUNCTION
   #define CHOSEN_KEYPAD_C_FUNCTION     CUSTOM_3
#endif
#ifndef CHOSEN_KEYPAD_D_FUNCTION
   #define CHOSEN_KEYPAD_D_FUNCTION     CUSTOM_4
#endif

// ***************************************************
// Potentiometers

#ifndef THROTTLE_POT_PIN
   #define THROTTLE_POT_PIN 39
#endif
#ifndef THROTTLE_POT_NOTCH_VALUES   
   #define THROTTLE_POT_NOTCH_VALUES {20,585,1170,1755,2340,2925,3510,4000}
#endif
#ifndef THROTTLE_POT_NOTCH_SPEEDS
   #define THROTTLE_POT_NOTCH_SPEEDS {4, 10,  15,  25,  35,  45, 55, 65}
#endif
#ifndef USE_THROTTLE_NOTCHES
   #define USE_THROTTLE_NOTCHES true
#endif
#ifndef LINEAR_THROTTLE_POT_DEAD_ZONE
   #define LINEAR_THROTTLE_POT_DEAD_ZONE 5
#endif

#ifndef REVERSER_POT_PIN
   #define REVERSER_POT_PIN 36
#endif
#ifndef REVERSER_VALUES
   #define REVERSER_VALUES {981,2141}; 
#endif

#define REVERSER_POSITION_FORWARD 0
#define REVERSER_POSITION_NEUTRAL 1
#define REVERSER_POSITION_REVERSE 2

#define POT_VALUE_TITLE_THROTTLE "T:"
#define POT_VALUE_TITLE_REVERSER "R:"
#define POT_VALUE_TITLE_BRAKE    "B:"

#ifndef BRAKE_POT_VALUES
   #define BRAKE_POT_VALUES  {720, 1365, 2048, 2731, 3413};
#endif
#ifndef BRAKE_DELAY_TIMES
   #define BRAKE_DELAY_TIMES {500,200,125,50,25,1};
#endif
#ifndef ACCELLERATION_DELAY_TIMES
   #define ACCELLERATION_DELAY_TIMES {182,415,600}; 
#endif
// #ifndef COAST_DELAY_TIMES
// note this is defined in HandCab.ino - Not here
// #endif
#ifndef DCC_SPEED_CHANGE_AMOUNT
   #define DCC_SPEED_CHANGE_AMOUNT 1
#endif

// ***************************************************
// Battery test

#ifndef USE_BATTERY_TEST
   #define USE_BATTERY_TEST false
#endif
#ifndef BATTERY_TEST_PIN
   #define BATTERY_TEST_PIN 36
#endif
#ifndef BATTERY_CONVERSION_FACTOR
   #define BATTERY_CONVERSION_FACTOR 1.7
#endif
#ifndef USE_BATTERY_PERCENT_AS_WELL_AS_ICON
   #define USE_BATTERY_PERCENT_AS_WELL_AS_ICON false
#endif
#ifndef USE_BATTERY_SLEEP_AT_PERCENT
   #define USE_BATTERY_SLEEP_AT_PERCENT -1
#endif

// ***************************************************
//  ESPmDNS problem

#if ESP_IDF_VERSION_MAJOR < 5
  #define ESPMDNS_IP_ATTRIBUTE_NAME MDNS.IP(i)
#else
  #define ESPMDNS_IP_ATTRIBUTE_NAME MDNS.address(i)
#endif

// ***************************************************
// Heartbeat 

// max period 
#ifndef MAX_HEARTBEAT_PERIOD
   #define MAX_HEARTBEAT_PERIOD 240000
#endif

#ifndef HEARTBEAT_ENABLED
   #define HEARTBEAT_ENABLED true
#endif
