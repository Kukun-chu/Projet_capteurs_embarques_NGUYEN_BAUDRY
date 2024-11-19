#include "mbed.h"

// LED
#ifdef LED1
DigitalOut led(LED1);
#else
DigitalOut led(PinName(LED1));
#endif


void ping() {
    for (int i = 0; i < 100; i++) {
        printf("Ping\n");
    }
}

void pong() {
    for (int i = 0; i < 100; i++) {
        printf("Pong\n");
    }
}

int main() {
    Thread thread_ping(osPriorityNormal, 1024);
    Thread thread_pong(osPriorityNormal, 1024);

    thread_ping.start(ping);
    thread_pong.start(pong);

    while (true) {
        led = !led; // cligno
        //printf("Alive!\n");
        ThisThread::sleep_for(500ms);
    }
}
