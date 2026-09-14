/*
 *  Outputs "Hello, world!" to the serial terminal
 *  while slowly blinking the user LED.
 *
 *  Terminal config: 8N1, async, 9600 baud
 */

#include "../Arlow.h"

#define DELAY   (1000000)

void sendString(char* msg);

int main(void) {
    initGPIO(USER_LED, OUTPUT);
    initUSART(8, 'n', 1, ASYNC, 9600, true, false);

    char* msg = "Hello, world!";

    while (1) {
        sendString(msg);

        togglePin(USER_LED);

        for (volatile uint32_t i = 0; i < DELAY; i++)
            ; // soft delay, intentionally empty
    }
}

void sendString(char* msg) {
    for (int i = 0; i < strlen(msg); i++) {
        sendByte(msg[i]);
    }

    sendByte('\r');
    sendByte('\n');
}
