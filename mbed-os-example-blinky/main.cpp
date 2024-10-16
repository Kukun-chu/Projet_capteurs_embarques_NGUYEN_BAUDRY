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
void button_irq();
void flip_irq();
void timer();
void start_counting();
void end_counting();

// Initialise the digital pin LED1 as an output
#ifdef LED1
    DigitalOut led(LED1);
#else
    bool led;
#endif

// Init Timer
Timer t;

// Handler timer
int handlerTimer = 0;

int main()
{
    //blinking_test();
    //button_test();
    //button_irq();
    timer();
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
        bool button;
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

void button_irq() {

    // Initialise the digital pin BUTTON1
    #ifdef BUTTON1
        InterruptIn button(BUTTON1);
    #else
        bool button;
    #endif

    button.rise(&flip_irq);  // attach the address of the flip function to the rising edge
    button.fall(&flip_irq);  // attach the address of the flip function to the rising edge
    while(1) {           // wait around, interrupts will interrupt this!
        ThisThread::sleep_for(250ms);
    }
}

void flip_irq() {
    led = !led;
}

void timer() {
    // Initialise the digital pin BUTTON1
    #ifdef BUTTON1
        InterruptIn button(BUTTON1);
    #else
        bool button;
    #endif

    button.rise(&start_counting);  // attach the address of the flip function to the rising edge
    button.fall(&end_counting);  // attach the address of the flip function to the rising edge
    while(1) {           // wait around, interrupts will interrupt this!
        ThisThread::sleep_for(250ms);
        if(handlerTimer == 1) {
            printf("End time: %llu milliseconds\n", std::chrono::duration_cast<std::chrono::milliseconds>(t.elapsed_time()).count());
            handlerTimer = 0;
        }
    }
}

void start_counting() {
    t.start();
    led = !led;
    //printf("Start time: %f seconds\n", t.read());
}

void end_counting() {
    t.stop();
    led = !led;
    handlerTimer = 1;
    //printf("End time: %f seconds\n", t.read());

}