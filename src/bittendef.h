/**
* this file toggles debugging and release flags for the game
*/
#ifndef bittendef
#define bittendef
// enables debugging features, disable this for releases
#define debug
//#define debugsprites // cuts fps to 15fps, sense i animate at 15fps then add extra frames
// files
#define SETTINGS_FILE "assets/settings.cfg"
// enables battle menu test disable this so then you can do other things than the test battle
//#define battleTest //outdated
// this is a really nesh flag, it is used in my notepad++ debug compiles
//#define notepadplusplusDebug

    // TODO make this editable in an settings menu
#define SCREENWIDTH 800
#define SCREENHEIGHT 450
////////////////////////////////////////////////////////////
// settings storage
////////////////////////////////////////////////////////////
typedef enum {
    MUSIC    = 0
} SettingsData;
#endif