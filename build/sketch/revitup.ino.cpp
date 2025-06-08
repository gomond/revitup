#line 1 "C:\\Users\\Greg\\iCloudDrive\\Documents\\Arduino\\revitup\\revitup.ino"
// Example sketch that comes along with RF433recv library.
// Implements what is needed to respond to a 2-button telecommand: implement
// receiver and callback functions.
// The receiver timings and receiver codes can be worked out using RF433any
// library.

/*
  Copyright 2021 Sébastien Millet

  `RF433recv' is free software: you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as
  published by the Free Software Foundation, either version 3 of the
  License, or (at your option) any later version.

  `RF433recv' is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this program. If not, see
  <https://www.gnu.org/licenses>.
*/

//
// Schematic: Radio Frequencies RECEIVER plugged on D2
//

#include "RF433recv.h"

#include <Arduino.h>
//#include <ESP32Encoder.h>

#define CLK 13 // CLK ENCODER 
#define DT 15 // DT ENCODER 
//#define foot_CLK 16 // CLK ENCODER 
//#define foot_DT 17 // DT ENCODER 
 
//ESP32Encoder encoder_head;
//ESP32Encoder encoder_foot;

#define PIN_RFINPUT  2
    // Specifying the interrupt number is optional, you can leave it to the
    // constructor to work it out.
//#define INT_RFINPUT  2
//Relay HeadUp(4, false); // constructor receives (pin, isNormallyOpen) true = Normally Open, false = Normally Closed
//Relay HeadDown(5,false);
//Relay FootUp(6,false);
//Relay FootDown(7,false);

uint8_t OUTPUT_PIN_1, OUTPUT_PIN_2; 

bool ButtonPressed = false;

#line 55 "C:\\Users\\Greg\\iCloudDrive\\Documents\\Arduino\\revitup\\revitup.ino"
void callback_anycode(const BitVector *recorded);
#line 69 "C:\\Users\\Greg\\iCloudDrive\\Documents\\Arduino\\revitup\\revitup.ino"
void callback_head_up(const BitVector *recorded);
#line 75 "C:\\Users\\Greg\\iCloudDrive\\Documents\\Arduino\\revitup\\revitup.ino"
void callback_button_released(const BitVector *recorded);
#line 83 "C:\\Users\\Greg\\iCloudDrive\\Documents\\Arduino\\revitup\\revitup.ino"
void callback_head_down(const BitVector *recorded);
#line 90 "C:\\Users\\Greg\\iCloudDrive\\Documents\\Arduino\\revitup\\revitup.ino"
void callback_flat(const BitVector *recorded);
#line 95 "C:\\Users\\Greg\\iCloudDrive\\Documents\\Arduino\\revitup\\revitup.ino"
void callback_foot_down(const BitVector *recorded);
#line 102 "C:\\Users\\Greg\\iCloudDrive\\Documents\\Arduino\\revitup\\revitup.ino"
void callback_foot_up(const BitVector *recorded);
#line 109 "C:\\Users\\Greg\\iCloudDrive\\Documents\\Arduino\\revitup\\revitup.ino"
void callback_G(const BitVector *recorded);
#line 113 "C:\\Users\\Greg\\iCloudDrive\\Documents\\Arduino\\revitup\\revitup.ino"
void callback_lift_1(const BitVector *recorded);
#line 117 "C:\\Users\\Greg\\iCloudDrive\\Documents\\Arduino\\revitup\\revitup.ino"
void callback_lift_2(const BitVector *recorded);
#line 121 "C:\\Users\\Greg\\iCloudDrive\\Documents\\Arduino\\revitup\\revitup.ino"
void callback_anti_snore(const BitVector *recorded);
#line 125 "C:\\Users\\Greg\\iCloudDrive\\Documents\\Arduino\\revitup\\revitup.ino"
void callback_massage_head_plus(const BitVector *recorded);
#line 129 "C:\\Users\\Greg\\iCloudDrive\\Documents\\Arduino\\revitup\\revitup.ino"
void callback_massage_head_minus(const BitVector *recorded);
#line 133 "C:\\Users\\Greg\\iCloudDrive\\Documents\\Arduino\\revitup\\revitup.ino"
void callback_massage_foot_plus(const BitVector *recorded);
#line 137 "C:\\Users\\Greg\\iCloudDrive\\Documents\\Arduino\\revitup\\revitup.ino"
void callback_massage_foot_minus(const BitVector *recorded);
#line 141 "C:\\Users\\Greg\\iCloudDrive\\Documents\\Arduino\\revitup\\revitup.ino"
void callback_massage_stop(const BitVector *recorded);
#line 146 "C:\\Users\\Greg\\iCloudDrive\\Documents\\Arduino\\revitup\\revitup.ino"
void callback_massage_full_body(const BitVector *recorded);
#line 150 "C:\\Users\\Greg\\iCloudDrive\\Documents\\Arduino\\revitup\\revitup.ino"
void callback_massage_1(const BitVector *recorded);
#line 154 "C:\\Users\\Greg\\iCloudDrive\\Documents\\Arduino\\revitup\\revitup.ino"
void callback_massage_2(const BitVector *recorded);
#line 158 "C:\\Users\\Greg\\iCloudDrive\\Documents\\Arduino\\revitup\\revitup.ino"
void callback_massage_3(const BitVector *recorded);
#line 172 "C:\\Users\\Greg\\iCloudDrive\\Documents\\Arduino\\revitup\\revitup.ino"
void setup();
#line 284 "C:\\Users\\Greg\\iCloudDrive\\Documents\\Arduino\\revitup\\revitup.ino"
void loop();
#line 55 "C:\\Users\\Greg\\iCloudDrive\\Documents\\Arduino\\revitup\\revitup.ino"
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
    Serial.print(F("Head Up pressed\n"));
    OUTPUT_PIN_1 = 4; // Set the pin for Head Up
    OUTPUT_PIN_2 = 5; // Set the pin for Head Down
    ButtonPressed = true;
}
void callback_button_released(const BitVector *recorded) {
    Serial.print(F("Button released\n"));
    ButtonPressed = false;
}


// Callback functions for the other buttons

void callback_head_down(const BitVector *recorded) {
    Serial.print(F("Head Down pressed\n")); 
    OUTPUT_PIN_1 = 5; // Set the pin for Head Up
    OUTPUT_PIN_2 = 4; // Set the pin for Head Down
    ButtonPressed = true;
}

void callback_flat(const BitVector *recorded) {
    Serial.print(F("Flat pressed\n"));
    ButtonPressed = true;
}

void callback_foot_down(const BitVector *recorded) {
    Serial.print(F("Foot Down pressed\n"));
    OUTPUT_PIN_1 = 7; // Set the pin for Foot Down
    OUTPUT_PIN_2 = 6; // Set the pin for Foot Up
    ButtonPressed = true;
    }

void callback_foot_up(const BitVector *recorded) {
    Serial.print(F("Foot Up pressed\n"));
    OUTPUT_PIN_1 = 6; // Set the pin for Foot Up
    OUTPUT_PIN_2 = 7; // Set the pin for Foot Down
    ButtonPressed = true;
}

void callback_G(const BitVector *recorded) {
    Serial.print(F("G pressed\n"));
}

void callback_lift_1(const BitVector *recorded) {
    Serial.print(F("Lift 1 pressed\n"));
}

void callback_lift_2(const BitVector *recorded) {
    Serial.print(F("Lift 2 pressed\n"));
}

void callback_anti_snore(const BitVector *recorded) {
    Serial.print(F("Anti Snore pressed\n"));
}

void callback_massage_head_plus(const BitVector *recorded) {
    Serial.print(F("Massage Head + pressed\n"));
}

void callback_massage_head_minus(const BitVector *recorded) {
    Serial.print(F("Massage Head - pressed\n"));
}

void callback_massage_foot_plus(const BitVector *recorded) {
    Serial.print(F("Massage Foot + pressed\n"));
}

void callback_massage_foot_minus(const BitVector *recorded) {
    Serial.print(F("Massage Foot - pressed\n"));
}

void callback_massage_stop(const BitVector *recorded) {
    Serial.print(F("Massage Stop pressed\n"));
}


void callback_massage_full_body(const BitVector *recorded) {
    Serial.print(F("Massage Full Body pressed\n"));
}

void callback_massage_1(const BitVector *recorded) {
    Serial.print(F("Massage Button 1 pressed\n"));
}

void callback_massage_2(const BitVector *recorded) {
    Serial.print(F("Massage Button 2 pressed\n"));
}

void callback_massage_3(const BitVector *recorded) {
    Serial.print(F("Massage Button 3 pressed\n"));
}





//RF_manager rf(PIN_RFINPUT, INT_RFINPUT);
    // Second parameter is optional. Could also be:
RF_manager rf(PIN_RFINPUT);



void setup() {
    pinMode(PIN_RFINPUT, INPUT);
    //pinMode(OUTPUT_PIN, OUTPUT); // Set OUTPUT_PIN as an output pin

    //encoder_head.attachHalfQuad ( DT, CLK );
    //encoder_head.setCount ( 0 );
    
    Serial.begin(115200);

        // Replace the below code with your telecommand timings.
        // See RF433any (https://github.com/sebmillet/RF433any) as to how to
        // work out these timings. In particular, you can use example:
        //   examples/01_main/01_main.ino
        // to output exactly what is needed to call register_Receiver().
        // This example complete URL is:
        //   https://github.com/sebmillet/RF433any/blob/main/examples/01_main/01_main.ino

   // Data: 78 10 d7 bf

Serial.print(F("Im Here Setup\n")); 

// 40 Bit registration

rf.register_Receiver(
  RFMOD_TRIBIT_INVERTED,  // mod
  19468,                  // initseq
  4968,                   // lo_prefix
  5512,                   // hi_prefix
  1018,                   // first_lo_ign
  506,                    // lo_short
  1018,                   // lo_long
  0,                      // hi_short (0 => take lo_short)
  0,                      // hi_long  (0 => take lo_long)
  968,                    // lo_last
  19468,                  // sep
  40                      // nb_bits
);
rf.register_callback(callback_head_down, 500,
                     new BitVector(40, 5, 0x78, 0x10, 0xd6, 0xbe, 0xbe));

// 32 Bit registration

rf.register_Receiver(
  RFMOD_TRIBIT_INVERTED,  // mod
  19468,                  // initseq
  4968,                   // lo_prefix
  5512,                   // hi_prefix
  1018,                   // first_lo_ign
  506,                    // lo_short
  1018,                   // lo_long
  0,                      // hi_short (0 => take lo_short)
  0,                      // hi_long  (0 => take lo_long)
  968,                    // lo_last
  19468,                  // sep
  32                      // nb_bits
);

rf.register_callback(callback_head_up, 500,
                     new BitVector(32, 4, 0x78, 0x10, 0xd7, 0xbf));

rf.register_callback(callback_button_released, 500,
                    new BitVector(32, 4, 0x78, 0x10, 0x00, 0x68));                     

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
                                        
    
    Serial.print(F("Waiting for signal\n"));

    rf.activate_interrupts_handler();
}


void loop() {
    rf.do_events();
    pinMode(OUTPUT_PIN_1, OUTPUT);
    pinMode(OUTPUT_PIN_2, OUTPUT);
    if (ButtonPressed) {
        digitalWrite(OUTPUT_PIN_1, HIGH); // Replace OUTPUT_PIN with your actual pin
        digitalWrite(OUTPUT_PIN_2, LOW); // Replace OUTPUT_PIN with your actual pin
    } else {
        digitalWrite(OUTPUT_PIN_1, HIGH);
        digitalWrite(OUTPUT_PIN_2, HIGH); // Replace OUTPUT_PIN with your actual pin
    }
}

// vim: ts=4:sw=4:tw=80:et

