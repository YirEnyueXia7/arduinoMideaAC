#include <Arduino.h>
#include <mideaAC.h>

acSerial s1;

// AC connection on Serial
// debug output on Serial1

#define USE_SERIAL Serial1

void ACevent(ac_status_t * status) {
    USE_SERIAL.printf("[ACevent] ----------------------------\n");
    s1.print_status(status);
}

void setup() {
    USE_SERIAL.begin(115200);
    USE_SERIAL.setDebugOutput(true);

    USE_SERIAL.println();
    USE_SERIAL.println();
    USE_SERIAL.println();

    for(uint8_t t = 4; t > 0; t--) {
        USE_SERIAL.printf("[SETUP] BOOT WAIT %d...\n", t);
        USE_SERIAL.flush();
        delay(1000);
    }

    // connect Serial
    Serial.begin(9600);
    s1.begin((Stream *)&Serial, "Serial");
    s1.send_getSN();

    s1.onStatusEvent(ACevent);

    // enable AC
    // send_conf_h(on, soll, fan, mode, lamelle, turbo, eco)
    s1.send_conf_h(true, 21, 1, acModeCool, false, false, false);
}

unsigned long statusMillis = 0;

void loop() {
    s1.loop();
    unsigned long currentMillis = millis();
    // get status every 5 sec
    if(currentMillis - statusMillis >= 5000) {
        statusMillis = currentMillis;
        s1.send_status(true, true);
        s1.request_status();
    }
}
