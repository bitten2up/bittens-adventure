#include <iostream>
#include <string>
#include bittendef
// used to toggle indivisal debug settings
class debugsettings{
    public:
        bool debug(){
            #ifdef debug
            debugmode=true; // used for debug logs and leveledit when I add tilemaps
            #else 
            debugmode=false;
            #endif
            return debugmode;
        }
        bool instentwin=false; //win battles instently
        bool noclip=false; // tbh everything is noclip when adding this it is for the future
        bool battletest=false; // replases #define battleTest
        const float version = "0.01"
        string getCodename(){
            if (debugmode){ // used for eastereggs when i add them
                codename ="iced bitten" ;  //define the debug codename;
            }
            else{
                codename = "flaming bitten"
            }
            return codename;
        }
    private:
        bool debugmode;
        string codename;
};
// used to toggle settings not usaly used for debugging
class normalsettings{
    public: 
        bool fullscreen = true;
        string savedata = "null lol";
        bool bittendev = true;
        int whatthehell = 1;