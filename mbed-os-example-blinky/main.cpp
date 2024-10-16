/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"
#include "PinNames.h"
#include "gpio_irq_api.h"

// Blinking rate in milliseconds
#define BLINKING_RATE     500ms

void blinking_test();
void button_test();

int main()
{
    //blinking_test();
    button_test();
    return 0;
}

void blinking_test() {
    // Initialise the digital pin LED1 as an output
    #ifdef LED1
        DigitalOut led(LED1);
    #else
        bool led;
    #endif

    while (true) {
        led = 1;          // set LED1 pin to high
        printf("myled = %d \n\r", (uint8_t)led);
        ThisThread::sleep_for(500ms);

        led.write(0);     // set LED1 pin to low
        printf("led = %d \n\r", led.read());
        ThisThread::sleep_for(500ms);
        
        //led = !led;
        //ThisThread::sleep_for(BLINKING_RATE);
    }
}

void button_test() {
    // Initialise the digital pin LED1 as an output
    #ifdef LED1
        DigitalOut led(LED1);
    #else
        bool led;
    #endif

    // Initialise the digital pin BUTTON1
    #ifdef BUTTON1
        DigitalIn button(BUTTON1);
    #else
        bool led;
    #endif

    while (true) {
        if (button.read() == 1) {
            led = 1;          // set LED1 pin to high
        }
        else{
            led.write(0);     // set LED1 pin to low
        }
        printf("button_state = %d \n\r", button.read());
    }
}