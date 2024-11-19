/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"
#include "PinNames.h"
#include "gpio_irq_api.h"

// Adresse I2C du capteur de température sur la carte 6TRON (à vérifier dans la documentation de la carte)
#define TEMP_SENSOR_ADDR (0x48 << 1) // mbed utilise des adresses 8 bits
#define HUM_SENSOR_ADDR (0x40 <<1) // mbed utilise des adresses 8 bits

#define TEMP_REGISTER 0x00            // Registre de température (à confirmer dans la documentation du capteur)
#define TEMP_REGISTER 0xE5            // Registre de l'humidite


// Blinking rate in milliseconds
#define BLINKING_RATE 500ms

void blinking_test();
void button_test();
void button_irq();
void flip_irq();
void timer();
void start_counting();
void end_counting();
void ticker_func();
void freq();
void increment_freq_rate();
void temperature_sensor_handler();
void humidity_sensor_handler();
void scanning_device();

// Initialisation de l'I2C et de la sortie série
I2C i2c(P1_I2C_SDA, P1_I2C_SCL);


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

// Init Ticker
Ticker ticker;


int main()
{
    //blinking_test();
    //button_test();
    //button_irq();
    //timer();
    //ticker_func();
    //freq();
    //scanning_device();

    while (1) {
        temperature_sensor_handler();
        humidity_sensor_handler();
        ThisThread::sleep_for(1000ms); // Attendre une seconde avant la prochaine lecture
    }
}



void scanning_device() {
    printf("Scanning I2C devices...\n");
    for (int addr = 0; addr < 128; addr++) {
        if (i2c.write(addr << 1, NULL, 0) == 0) {
            printf("Found device at address 0x%02X\n", addr);
        }
    }
    printf("Scan complete.\n");
}


// Fonction pour lire et afficher l'humidite
void humidity_sensor_handler() {
    //char cmd[1];
    char read_hum_data[2] =  {0};
    char reg_adress[1] = {0x00};
    reg_adress[0] = TEMP_REGISTER;

    //i2c.write(HUM_SENSOR_ADDR, &reg_adress, 1);
    if (i2c.write(HUM_SENSOR_ADDR, reg_adress, 1) != 0) {
        printf("Erreur lors de l'envoi de la commande au capteur.\n");
        //return;
    }

    // Ajouter un délai pour permettre la conversion (selon la datasheet)
    ThisThread::sleep_for(10ms);

    // Lire les données de température (2 octets)
    // i2c.read(HUM_SENSOR_ADDR, read_data, 2);
    if (i2c.read(HUM_SENSOR_ADDR, read_hum_data, 2) != 0) {
        printf("Erreur lors de la lecture de l'humidite.\n");
        //return;
    }

    int hum_raw = (read_hum_data[0] << 8) | read_hum_data[1];

    // Suppression des bits de statut (si applicable)
    hum_raw &= 0x3FFF;


    // Simuler deux décimales en multipliant par 100 et en affichant sous forme d'entier
    //int humidity = hum_raw * 100 / 128; // Multiplier par 100 pour simuler 2 décimales

    // Conversion en pourcentage avec 2 décimales (sans float)
    int humidity = hum_raw * 10000 / 16383;
    
    // Affichage de la température sur le terminal avec printf
    printf("Humidite : %d.%02d \n", humidity / 100, humidity % 100);
}


// Fonction pour lire et afficher la température
void temperature_sensor_handler() {
    //char cmd[1];
    char read_data[2] =  {0};
    char reg_adress[1] = {0x00};
    
    //cmd[0] = TEMP_REGISTER; // Envoie la commande pour lire le registre de température

    //i2c.write(TEMP_SENSOR_ADDR, &reg_adress, 1);
    if (i2c.write(TEMP_SENSOR_ADDR, reg_adress, 1) != 0) {
        printf("Erreur lors de l'envoi de la commande au capteur.\n");
        //return;
    }

    // Lire les données de température (2 octets)
    // i2c.read(TEMP_SENSOR_ADDR, read_data, 2);
    if (i2c.read(TEMP_SENSOR_ADDR, read_data, 2) != 0) {
        printf("Erreur lors de la lecture de la temperature.\n");
        //return;
    }

    int temp_raw = (read_data[0] << 8) | read_data[1];

    // Simuler deux décimales en multipliant par 100 et en affichant sous forme d'entier
    int temperature = temp_raw * 100 / 128; // Multiplier par 100 pour simuler 2 décimales

    // Affichage de la température sur le terminal avec printf
    printf("Temperature : %d.%02d deg Celsius\n", temperature / 100, temperature % 100);
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
}

void end_counting() {
    t.stop();
    led = !led;
    handlerTimer = 1;
}

void ticker_func() {
    ticker.attach(&flip_irq, 1s);
}

void freq() {
    // Initialise the digital pin BUTTON1
    #ifdef BUTTON1
        InterruptIn button(BUTTON1);
    #else
        bool button;
    #endif

    std::chrono::milliseconds freq_rate = 100ms;
    ticker.attach(&flip_irq, freq_rate);
    button.fall(&increment_freq_rate);  // attach the address of the flip function to the rising edge
    while(1) {           // wait around, interrupts will interrupt this!
        ThisThread::sleep_for(250ms);
        if(handlerTimer == 1) {
            freq_rate *= 10;
            handlerTimer = 0;
            ticker.attach(&flip_irq, freq_rate);
        }
    }
}

void increment_freq_rate() {
    handlerTimer = 1;
}