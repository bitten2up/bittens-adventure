/**
* this file toggles debugging and release flags for the game
*/

#include <string>
#include <iostream>
using namespace std;
#ifndef bittendef
#define bittendef
// enables debugging features, disable this for releases
#define debug
// enables battle menu test disable this so then you can do other things than the test battle
//#define battleTest //outdated
#endif
int version = 1;
// used to toggle indivisal debug settings
class debugsettings{
    public:
        bool debugmode=true; // used for debug logs and leveledit when I add tilemaps
        bool instentwin=false; //win battles instently
        bool noclip=false; // tbh everything is noclip when adding this it is for the future
        bool battletest=false; // replases #define battleTest
        string versioncodename="iced bitten\nextreamly unstable"; // used for eastereggs when i add them
};