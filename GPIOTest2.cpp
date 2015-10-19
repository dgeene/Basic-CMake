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

    // experiment - gross test code please ignore
    string gpios [10] = {"13", "6", "25", "22", "17", "16", "12", "5", "24", "27"};
    string keyDown [10] = {"c", "", "", "", "", "", "", "", "", ""};
    string keyUP [10] = {"C", "", "", "", "", "", "", "", "", ""};
    //std::map<string, GPIOClass*> my_gpios;
    Button instances [10]; //this will hold our structs

    for (int i = 0; i < 10; i++) {
        cout << gpios[i] << endl;
        instances[i].buttonName = "my button";
        instances[i].gpio = gpios[i]; 
        instances[i].gpioObject = new GPIOClass(gpios[i]);
        instances[i].gpioObject->export_gpio();
        instances[i].gpioObject->setdir_gpio("in");
        instances[i].keypadDown = "key down"; 
        instances[i].keypadRelease = "key up"; 
    }
    //end experiment

    string inputstate1;


    cout << " gpio pins exported and direction set" << endl;

    
    // start out with all false
    bool currentbuttonstate[10];


    while(1)
    {
        usleep(100000); //sleep .3 seconds
/*
        // my array
        bool buttonpressed[10];
        for( int ii = 0; ii < sizeof(buttonpressed); ii++ ) buttonpressed[ii] = false;

        for every key
             if pressed { buttonpressed[ii] = true; }

        usleep 4000;

        
        for every key
             if (pressed && buttonpressed[ii] == true) { cout << buttonchar; }

*/

        for (int k=0; k<10; k++) {
            Button thisButton = instances[k];
            thisButton.gpioObject->getval_gpio(thisButton.inputState);
            //cout << "current state of gpio pin " << thisButton.gpio << " is: " << thisButton.inputState << endl;
            if (thisButton.inputState == "0") {

                usleep(4000);
                thisButton.gpioObject->getval_gpio(thisButton.inputState); 
                if (thisButton.inputState == "0") {
                    // button was definitely pressed at this point    
                    cout << "Button " << thisButton.gpio << " was pressed." << endl;

                    // this spikes the cpu
                    while(thisButton.inputState == "0") {
                        thisButton.gpioObject->getval_gpio(thisButton.inputState);
                    };
                    
                }
            }
        }


        // allows program to exit gracefully
        if(ctrl_c_pressed) {
            // we should store each gpio object in an array and loop through it
            cout << "Ctrl^C pressed" << endl;
            cout << "unexporting pins and deallocating gpi objects" << endl;

            // attempt to clean up my mess lol
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
