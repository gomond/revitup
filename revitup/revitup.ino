// revitup.ino
// RF433 adjustable bed controller with safe-mode and hold-to-run relay logic

#include <RF433recv.h>
#include <Arduino.h>
#include "Relay.h"

#define PIN_RFINPUT  2

// Relay objects (pin, isNormallyOpen)
Relay HeadUp(4, false);
Relay HeadDown(5, false);
Relay FootUp(6, false);
Relay FootDown(7, false);

// -----------------------------
// HOLD-TO-RUN RELAY CONTROLLER
// -----------------------------
struct HoldRelay {
    Relay &relay;
    unsigned long lastSeen = 0;
    bool active = false;

    HoldRelay(Relay &r) : relay(r) {}

    void trigger() {
        lastSeen = millis();
        if (!active) {
            relay.turnOn();
            active = true;
        }
    }

    void update(unsigned long timeout) {
        if (active && millis() - lastSeen > timeout) {
            relay.turnOff();
            active = false;
        }
    }

    void forceOff() {
        relay.turnOff();
        active = false;
    }
};

// Create hold-to-run wrappers
HoldRelay HHeadUp(HeadUp);
HoldRelay HHeadDown(HeadDown);
HoldRelay HFootUp(FootUp);
HoldRelay HFootDown(FootDown);

// Safe-mode interlocks
void cancelHeadOpposite(HoldRelay &activeRelay, HoldRelay &oppositeRelay) {
    oppositeRelay.forceOff();
    activeRelay.trigger();
}

void cancelFootOpposite(HoldRelay &activeRelay, HoldRelay &oppositeRelay) {
    oppositeRelay.forceOff();
    activeRelay.trigger();
}

// -----------------------------
// RF CALLBACKS
// -----------------------------
void callback_anycode(const BitVector *recorded) {
    Serial.print(F("Code received: "));
    char *printed_code = recorded->to_str();
    if (printed_code) {
        Serial.print(recorded->get_nb_bits());
        Serial.print(F(" bits: ["));
        Serial.print(printed_code);
        Serial.print(F("]\n"));
        free(printed_code);
    }
}

void callback_head_up(const BitVector *recorded) {
    Serial.println(F("Head Up pressed"));
    cancelHeadOpposite(HHeadUp, HHeadDown);
}

void callback_head_down(const BitVector *recorded) {
    Serial.println(F("Head Down pressed"));
    cancelHeadOpposite(HHeadDown, HHeadUp);
}

void callback_foot_up(const BitVector *recorded) {
    Serial.println(F("Foot Up pressed"));
    cancelFootOpposite(HFootUp, HFootDown);
}

void callback_foot_down(const BitVector *recorded) {
    Serial.println(F("Foot Down pressed"));
    cancelFootOpposite(HFootDown, HFootUp);
}

// Other callbacks (no relay actions)
void callback_flat(const BitVector *recorded) { Serial.println(F("Flat pressed")); }
void callback_G(const BitVector *recorded) { Serial.println(F("G pressed")); }
void callback_lift_1(const BitVector *recorded) { Serial.println(F("Lift 1 pressed")); }
void callback_lift_2(const BitVector *recorded) { Serial.println(F("Lift 2 pressed")); }
void callback_anti_snore(const BitVector *recorded) { Serial.println(F("Anti Snore pressed")); }
void callback_massage_head_plus(const BitVector *recorded) { Serial.println(F("Massage Head + pressed")); }
void callback_massage_head_minus(const BitVector *recorded) { Serial.println(F("Massage Head - pressed")); }
void callback_massage_foot_plus(const BitVector *recorded) { Serial.println(F("Massage Foot + pressed")); }
void callback_massage_foot_minus(const BitVector *recorded) { Serial.println(F("Massage Foot - pressed")); }
void callback_massage_stop(const BitVector *recorded) { Serial.println(F("Massage Stop pressed")); }
void callback_massage_full_body(const BitVector *recorded) { Serial.println(F("Massage Full Body pressed")); }
void callback_massage_1(const BitVector *recorded) { Serial.println(F("Massage Button 1 pressed")); }
void callback_massage_2(const BitVector *recorded) { Serial.println(F("Massage Button 2 pressed")); }
void callback_massage_3(const BitVector *recorded) { Serial.println(F("Massage Button 3 pressed")); }

// RF manager
RF_manager rf(PIN_RFINPUT);

// -----------------------------
// SETUP
// -----------------------------
void setup() {
    pinMode(PIN_RFINPUT, INPUT);
    Serial.begin(115200);

    HeadUp.begin();
    HeadDown.begin();
    FootUp.begin();
    FootDown.begin();

    // 40-bit receiver registration
    rf.register_Receiver(
        RFMOD_TRIBIT_INVERTED,
        19468,
        4968,
        5512,
        1018,
        506,
        1018,
        0,
        0,
        968,
        19468,
        40
    );

    rf.register_callback(callback_head_down, 500,
        new BitVector(40, 5, 0x78, 0x10, 0xd6, 0xbe, 0xbe));

    // 32-bit receiver registration
    rf.register_Receiver(
        RFMOD_TRIBIT_INVERTED,
        5428,
        0,
        0,
        1014,
        514,
        1014,
        0,
        0,
        440,
        5428,
        32
    );

    rf.register_callback(callback_head_up, 500,
        new BitVector(32, 4, 0x78, 0x10, 0xd7, 0xbf));

    rf.register_callback(callback_flat, 500,
        new BitVector(32, 4, 0x78, 0x10, 0xce, 0xa6));

    rf.register_callback(callback_foot_up, 500,
        new BitVector(32, 4, 0x78, 0x10, 0xd5, 0xbd));

    rf.register_callback(callback_foot_down, 500,
        new BitVector(32, 4, 0x78, 0x10, 0xd4, 0xbc));

    rf.register_callback(callback_G, 500,
        new BitVector(32, 4, 0x78, 0x10, 0xb8, 0xd0));

    rf.register_callback(callback_lift_1, 500,
        new BitVector(32, 4, 0x78, 0x10, 0xc3, 0xab));

    rf.register_callback(callback_lift_2, 500,
        new BitVector(32, 4, 0x78, 0x10, 0xc1, 0xa9));

    rf.register_callback(callback_anti_snore, 500,
        new BitVector(32, 4, 0x78, 0x10, 0xb7, 0xdf));

    rf.register_callback(callback_massage_head_plus, 500,
        new BitVector(32, 4, 0x78, 0x10, 0xd3, 0xbb));

    rf.register_callback(callback_massage_head_minus, 500,
        new BitVector(32, 4, 0x78, 0x10, 0xd2, 0xba));

    rf.register_callback(callback_massage_foot_minus, 500,
        new BitVector(32, 4, 0x78, 0x10, 0xd0, 0xb8));

    rf.register_callback(callback_massage_foot_plus, 500,
        new BitVector(32, 4, 0x78, 0x10, 0xd1, 0xb9));

    rf.register_callback(callback_massage_stop, 500,
        new BitVector(32, 4, 0x78, 0x10, 0xcc, 0xa4));

    rf.register_callback(callback_massage_full_body, 500,
        new BitVector(32, 4, 0x78, 0x10, 0xcb, 0xa3));

    rf.register_callback(callback_massage_1, 500,
        new BitVector(32, 4, 0x78, 0x10, 0xca, 0xa2));

    rf.register_callback(callback_massage_2, 500,
        new BitVector(32, 4, 0x78, 0x10, 0xc9, 0xa1));

    rf.register_callback(callback_massage_3, 500,
        new BitVector(32, 4, 0x78, 0x10, 0xc8, 0xa0));

    Serial.println(F("Waiting for signal"));
    rf.activate_interrupts_handler();
}

// -----------------------------
// MAIN LOOP
// -----------------------------
void loop() {
    rf.do_events();

    const unsigned long timeout = 200; // ms after last packet

    HHeadUp.update(timeout);
    HHeadDown.update(timeout);
    HFootUp.update(timeout);
    HFootDown.update(timeout);
}
