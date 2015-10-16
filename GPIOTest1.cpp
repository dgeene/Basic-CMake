#include <iostream>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "GPIOClass.h"
using namespace std;

void sig_handler(int sig);

bool ctrl_c_pressed = false;

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


    string inputstate;
    GPIOClass* gpio17 = new GPIOClass("17");
    gpio17->export_gpio();

    cout << " gpio pins exported" << endl;

    gpio17->setdir_gpio("in");

    cout << " set gpio directions" << endl;

    while(1)
    {
        usleep(500000); //sleep .5 seconds
        gpio17->getval_gpio(inputstate); // read state of gpio 17
        cout << " current state of gpio pin is " << inputstate <<endl;

        if(inputstate == "0") // if 0 i.e. button pressed
        {
            cout << "input state is pressed .n Will check input pin state again in 4ms " <<endl;
            usleep(4000);
            cout << "checking again ....." <<endl;
            gpio17->getval_gpio(inputstate); // checking again to ensure that button is pressed and not just noise

            if(inputstate == "0")
            {
                cout << "input pin was definetly pressed. Turning LED on" << endl;
                //gpio4->setval_gpio("1");
                cout << "waiting until pin is unpressed...." << endl;
                while(inputstate == "0") {
                    gpio17->getval_gpio(inputstate);
                };
                cout << "pin is unpressed" << endl;

            }
            else
                cout << "input pin state us definetly unpressed. That was just noise" << endl;

        }
        //gpio4->setval_gpio("0");


        // allows program to exit gracefully
        if(ctrl_c_pressed) {
            cout << "Ctrl^C pressed" << endl;
            cout << "unexporting pins" << endl;
            gpio17->unexport_gpio();
            //gpio4->unexport_gpio();
            cout << "deallocating GPIO objects" << endl;
            delete gpio17;
            gpio17 = 0;
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
