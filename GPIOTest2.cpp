/*
 * Not sure how this can be implemented whether through polling, interrupts, etc
 * The ideal situation
 *  Give a list of GPIO pins to initalize
 *  Loop through an initialize objects for each gpio
 *  Register some sort of event listener for each
 *  Main loop waits for a press "event" on any button
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
    Button instances [10]; //hold our structs

    for (int i = 0; i < 10; i++) {
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
    bool currentbuttonstate[10] = {false};


    while(1)
    {
        usleep(100000); //sleep 100 milliseconds

        // init a temp button state as false
        bool buttonpressed[10] = {false};

        //for every key
        //     if pressed { buttonpressed[ii] = true; }
        for (int k=0; k<10; k++) {
            Button thisButton = instances[k];
            thisButton.gpioObject->getval_gpio(thisButton.inputState);
            if (thisButton.inputState == "0") { // 0 means pressed
                buttonpressed[k] = true;
            }         
        }

        usleep(4000);


        //for every key
        //     if (pressed && buttonpressed[ii] == true) { cout << buttonchar; }
        for (int l=0; l<10; l++) {
            Button thisButton = instances[l];
            thisButton.gpioObject->getval_gpio(thisButton.inputState);
            if (thisButton.inputState == "0" && buttonpressed[l] == true) {
                //press and hold here
                currentbuttonstate[l] = true;
                cout << "Pressed: " << thisButton.buttonName << " Keydown: " << thisButton.keypadDown << endl;
            } else if (thisButton.inputState == "1" && currentbuttonstate[l] == true) {
                cout << "Keyup: " << thisButton.keypadRelease << endl;
                currentbuttonstate[l] = false;
            }

/*
            } else if(currentbuttonstate[l] == true && thisButton.inputState == "1") {
                cout << "Pressed: " << thisButton.buttonName << " Keydown: " << thisButton.keypadDown << endl;
            }
*/

/*
            if (buttonpressed[l] == true) {
                cout << "Pressed: " << thisButton.buttonName << " Keydown: " << thisButton.keypadDown << endl;
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
