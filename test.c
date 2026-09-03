#include <string.h>
#include "Arlow.h"


int main(void) {
    char* prompt = "Press 1 to toggle LED";
    char input = 0;
    
    initUSART(8, 'n', 1, ASYNC, 9600, true, true);
    initGPIO(USER_LED, OUTPUT);

    for (size_t i = 0; i < strlen(prompt); i++) {
        sendByte(prompt[i]);
    }

    sendByte('\r');
    sendByte('\n');

    while(1) {
        input = receiveByte();

        if (input == '1') {
            togglePin(USER_LED);
        }
    }
}

