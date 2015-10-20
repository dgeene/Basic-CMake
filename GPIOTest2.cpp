/*
 * Button press detection system with debounce
 * using a mildly complex state machine.
 *
 *
 */
#include <iostream>
#include <map>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "GPIOClass.h"
using namespace std;

void sig_handler(int sig);

bool ctrl_c_pressed = false;

struct Button
{
    string buttonName;
    string gpio;
    GPIOClass* gpioObject;
    string inputState;
    string keypadDown;
    string keypadRelease;
};



int main(void)
{
    const int MAX_BUTTONS=10;

    struct sigaction sig_struct;
    sig_struct.sa_handler = sig_handler;
    sig_struct.sa_flags = 0;
    sigemptyset(&sig_struct.sa_mask);

    if (sigaction(SIGINT, &sig_struct, NULL) == -1) {
        cout << "Problem with sigaction" << endl;
        return -1; 
    }


    string pinName[10]  = {"power", "aux", "1", "2", "3", "4", "5", "6", "vol +", "vol -"};
    string gpios [10]   = {"13", "16", "6", "25", "22", "12", "5", "24", "17", "27"};
    string keyDown [10] = {"c", "m", "g", "f", "b", "k", "j", "i", "a", "e"};
    string keyUp [10]   = {"C", "M", "G", "F", "B", "K", "J", "I", "A", "E"};
    Button instances [MAX_BUTTONS]; //hold our structs

    for (int i = 0; i < MAX_BUTTONS; i++) {
        cout << gpios[i] << endl;
        instances[i].buttonName = pinName[i];
        instances[i].gpio = gpios[i]; 
        instances[i].gpioObject = new GPIOClass(gpios[i]);
        instances[i].gpioObject->export_gpio();
        instances[i].gpioObject->setdir_gpio("in");
        instances[i].keypadDown = keyDown[i]; 
        instances[i].keypadRelease = keyUp[i]; 
    }


    cout << " gpio pins exported and direction set" << endl;
 
    
    // persistent button state outside of loops
    bool buttonstate[MAX_BUTTONS];
    for( int ii = 0; ii < sizeof( buttonstate ); ii++ ) buttonstate[ii] = false;

    while(1)
    {
        usleep(100000); //sleep 100 milliseconds

        // init a temp button state as false
        bool buttonpressed[MAX_BUTTONS];
        for( int ii = 0; ii < sizeof( buttonpressed ); ii++ ) buttonpressed[ii] = false;

        //for every key
        //     if pressed { buttonpressed[ii] = true; }
        for (int k=0; k<MAX_BUTTONS; k++) {
            Button thisButton = instances[k];
            thisButton.gpioObject->getval_gpio(thisButton.inputState);
            if (thisButton.inputState == "0") { // 0 means pressed
                buttonpressed[k] = true;
            }
            else {
                buttonpressed[k] = false;
            }
        }

        usleep(4000);

        //for every key
        //     if (pressed && buttonpressed[ii] == true) { cout << buttonchar; }
        for (int l=0; l<MAX_BUTTONS; l++) {
            Button thisButton = instances[l];
            thisButton.gpioObject->getval_gpio(thisButton.inputState);

            bool stillpressed = false;
            if( thisButton.inputState == "0" && buttonpressed[l] == true ) buttonpressed[l] = true;
            else if( thisButton.inputState == "1" && buttonpressed[l] == false ) buttonpressed[l] = false;
            else buttonpressed[l] = buttonstate[l];

            for( int ii = 0; ii < MAX_BUTTONS; ii++ ) {
                if( buttonpressed[ii] != buttonstate[ii] )
                {
                    buttonstate[ii] = buttonpressed[ii];
                    cout << "X" << endl;
                }
            }

/*
            if (thisButton.inputState == "0" && buttonpressed[l] == true) {
                buttonstate[l] = true;
                //press and hold here
                if( buttonstate[l] == buttonpressed[l] ) {
                    cout << "Pressed: " << thisButton.buttonName << " Keydown: " << thisButton.keypadDown << endl;
                    buttonpressed[l] = buttonstate[l];
                }
            } else if (thisButton.inputState == "1" && buttonstate[l] == true) {
                cout << "Keyup: " << thisButton.keypadRelease << endl;
                buttonstate[l] = false;
            }
*/



        }




        // allows program to exit gracefully
        if(ctrl_c_pressed) {
            cout << "Ctrl^C pressed" << endl;
            cout << "unexporting pins and deallocating gpi objects" << endl;

            // attempt to clean up my mess
            for (int j = 0; j < 10; j++) {
                instances[j].gpioObject->unexport_gpio();
                delete instances[j].gpioObject;
                instances[j].gpioObject = 0;
                // delete instances[j]; // delete expects a pointer?
            }
            break;
        }

    }
    cout << "Exiting....." << endl;
    return 0;
}


void sig_handler(int sig) {
    write(0, "nCtrl^C pressed in sig handler", 32);
    ctrl_c_pressed = true;
}
