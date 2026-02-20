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

#define head_Up_PIN 4
#define head_Dn_PIN 5
#define foot_Up_PIN 6
#define foot_Dn_PIN 7 

bool ButtonPressed = false;
int  But_ID = 0; // Variable to store the button ID

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
    But_ID = 1; // Set the button ID to 1 for Head Up
    ButtonPressed = true;
}
void callback_button_released(const BitVector *recorded) {
    Serial.print(F("Button released\n"));
    But_ID = 0; // Reset the button ID to 0 when released
    ButtonPressed = false;
}


// Callback functions for the other buttons

void callback_head_down(const BitVector *recorded) {
    Serial.print(F("Head Down pressed\n")); 
    But_ID = 2; // Set the button ID to 2 for Head Down
    ButtonPressed = true;
}

void callback_foot_up(const BitVector *recorded) {
    Serial.print(F("Foot Up pressed\n"));
    But_ID = 3; // Set the button ID to 3 for Foot Up
    ButtonPressed = true;
}

void callback_foot_down(const BitVector *recorded) {
    Serial.print(F("Foot Down pressed\n"));
    But_ID = 4; // Set the button ID to 4 for Foot Down
    ButtonPressed = true;
    }

void callback_flat(const BitVector *recorded) {
    Serial.print(F("Flat pressed\n"));
    But_ID = 5; // Set the button ID to 5 for Flat
    ButtonPressed = true;
}

void callback_G(const BitVector *recorded) {
    Serial.print(F("G pressed\n"));
    But_ID = 6; // Set the button ID to 6 for G
    ButtonPressed = true;
}

void callback_lift_1(const BitVector *recorded) {
    Serial.print(F("Lift 1 pressed\n"));
    But_ID = 7; // Set the button ID to 7 for Lift 1
    ButtonPressed = true;
}

void callback_lift_2(const BitVector *recorded) {
    Serial.print(F("Lift 2 pressed\n"));
    But_ID = 8; // Set the button ID to 8 for Lift 2
    ButtonPressed = true;
}

void callback_anti_snore(const BitVector *recorded) {
    Serial.print(F("Anti Snore pressed\n"));
    But_ID = 9; // Set the button ID to 9 for Anti Snore
    ButtonPressed = true;
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
   
    pinMode(head_Up_PIN, OUTPUT);
    pinMode(head_Dn_PIN, OUTPUT);
    pinMode(foot_Up_PIN, OUTPUT);
    pinMode(foot_Dn_PIN, OUTPUT);

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
    
    if (ButtonPressed) {
        Serial.print(F("Button ID: "));
        Serial.println(But_ID);

        // Perform actions based on the button ID
        switch (But_ID) {
            case 1: // Head Up
                digitalWrite(head_Up_PIN, LOW);
                digitalWrite(head_Dn_PIN, HIGH);
                break;
            case 2: // Head Down
                digitalWrite(head_Dn_PIN, LOW);
                digitalWrite(head_Up_PIN, HIGH);
                break;
            case 3: // Foot Up
                digitalWrite(foot_Up_PIN, LOW);
                digitalWrite(foot_Dn_PIN, HIGH);
                break;
            case 4: // Foot Down
                digitalWrite(foot_Dn_PIN, LOW);
                digitalWrite(foot_Up_PIN, HIGH);
                break;
            case 5: // Flat
                // Add your action for Flat here
                break;
            case 6: // G
                // Add your action for G here
                break;
            case 7: // Lift 1
                // Add your action for Lift 1 here
                break;
            case 8: // Lift 2
                // Add your action for Lift 2 here
                break;
            case 9: // Anti Snore
                // Add your action for Anti Snore here
                break;
            default:
                Serial.println(F("Unknown Button ID"));
        }
    }
    else {
        // If no button is pressed, ensure all outputs are LOW
        digitalWrite(head_Up_PIN, HIGH);
        digitalWrite(head_Dn_PIN, HIGH);
        digitalWrite(foot_Up_PIN, HIGH); 
        digitalWrite(foot_Dn_PIN, HIGH); 
    }
}

// vim: ts=4:sw=4:tw=80:et
