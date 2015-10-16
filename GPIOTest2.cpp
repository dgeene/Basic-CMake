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
    string gpios2 [2] = {"13", "6"};
    string keyDown [10] = {"c", "", "", "", "", "", "", "", "", ""};
    string keyUP [10] = {"C", "", "", "", "", "", "", "", "", ""};
    //std::map<string, GPIOClass*> my_gpios;
    Button instances [10]; //this will hold our structs

    for (int i = 0; i < 2; i++) {
        cout << gpios[i] << endl;
        instances[i] = new Button;
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



    while(1)
    {
        usleep(500000); //sleep .5 seconds

        for (Button* p = &instances[0]; p != &instances[2]; ++p) {
            cout << instances->gpio <<endl;
        }
/*
        gpio17->getval_gpio(inputstate1); // read state of gpio 17
        cout << " current state of gpio pin is " << inputstate1 <<endl;

        gpio22->getval_gpio(inputstate2); // read state of gpio 17
        cout << " current state of gpio pin is " << inputstate1 <<endl;

        if(inputstate1 == "0") // if 0 i.e. button pressed
        {
            cout << "input state is pressed .n Will check input pin state again in 4ms " <<endl;
            usleep(4000);
            cout << "checking again ....." <<endl;
            gpio17->getval_gpio(inputstate1); // checking again to ensure that button is pressed and not just noise

            if(inputstate1 == "0")
            {
                cout << "input pin was definetly pressed. Turning LED on" << endl;
                cout << "waiting until pin is unpressed...." << endl;
                while(inputstate1 == "0") {
                    gpio17->getval_gpio(inputstate1);
                };
                cout << "pin is unpressed" << endl;

            }
            else
                cout << "input pin state us definetly unpressed. That was just noise" << endl;

        }


        if(inputstate2 == "0") // if 0 i.e. button pressed
        {
            cout << "input state is pressed .n Will check input pin state again in 4ms " <<endl;
            usleep(4000);
            cout << "checking again ....." <<endl;
            gpio22->getval_gpio(inputstate2); // checking again to ensure that button is pressed and not just noise

            if(inputstate2 == "0")
            {
                cout << "input pin was definetly pressed. Turning LED on" << endl;
                cout << "waiting until pin is unpressed...." << endl;
                while(inputstate2 == "0") {
                    gpio22->getval_gpio(inputstate2);
                };
                cout << "pin is unpressed" << endl;

            }
            else
                cout << "input pin state us definetly unpressed. That was just noise" << endl;

        }
*/



        // allows program to exit gracefully
        if(ctrl_c_pressed) {
            // we should store each gpio object in an array and loop through it
            cout << "Ctrl^C pressed" << endl;
            cout << "unexporting pins and deallocating gpi objects" << endl;

            // attempt to clean up my mess lol
            for (int j = 0; j < 2; j++) {
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
