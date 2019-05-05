/********************************************************
* #######################################################
* 
*  Bluetooth Serial - bt-serial.h
*  dekitarpg@gmail.com
* 
* #######################################################
********************************************************/

// Included Libraries:..
#include "bt-serial.h"

/**
* use TOSTRING() function to ensure string value for
* sending to the bluetooth client. 
*/
#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

/**
* Various preprocessor definitions
* Used as example data for sending to client
*/
#define FIRMWARE_VERSION beta

/**
* setup();
* Default Arduino setup function
*/
void setup() {
    // Setup builtin led for toggling
    pinMode(LED_BUILTIN, OUTPUT);
    // setup BTSerial module
    BTSerial::begin(9600,true); 
}

/**
* loop();
* Default Arduino loop function
*/
void loop() {
    // Call update to update bluetooth serial
    // update will return true if a command is
    // ready to be used. This saves having to Call
    // BTSerial::hasCommand(); after BTSerial::update();
    if (BTSerial::update()) {
        // process the available command
        processBTSCommand();
        // Flush the serial buffer
        BTSerial::flush();
    }
}

/**
* processBTSCommand();
* Processes data stored in the most recent bt serial command
*/
void processBTSCommand() {
    // sorts the command data from the buffer string
    // this is required before reading the commands
    BTSCommand comm = BTSerial::getCommand();

    // If command matches disconnection then 
    // return as its not a valid command
    if (strcmp(comm.cn, "+DISC") == 0) {
        Serial.println(""); return;
    }

    // print current command data to serial monitor:..
    Serial.print(comm.cn); Serial.print(" : "); Serial.println(comm.cv);

    // begin checking commands:..
    if (strcmp(comm.cn, "system") == 0) {
        sendStringPairToDevice("version", TOSTRING(FIRMWARE_VERSION));

    } else if (strcmp(comm.cn, "on") == 0) {
        digitalWrite(LED_BUILTIN, HIGH);
        sendStringPairToDevice("led", "on");

    } else if (strcmp(comm.cn, "off") == 0) {
        digitalWrite(LED_BUILTIN, LOW);
        sendStringPairToDevice("led", "off");

    } else if (strcmp(comm.cn, "value") == 0) {
        sendStringPairToDevice("value", comm.cv);
        // convert value to integer:..
        //int value = atoi(comm.cv);
    }
}

/**
* sendStringPairToDevice(k,v);
* Sends k and v strings to device formatted as below
* k:v;
*/
void sendStringPairToDevice(char* k, char* v) {
    BTSerial::print(k);
    BTSerial::print(":");
    BTSerial::print(v);
    BTSerial::println(";");
}

/********************************************************
* #######################################################
* 
*  End of Code
* 
* #######################################################
********************************************************/
