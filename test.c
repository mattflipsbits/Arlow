#include "Arlow.h"


int main(void) {
    initGPIO(USER_LED, OUTPUT);
    initGPIO(0x18, OUTPUT);


    while(1) {
        togglePin(USER_LED);

        for (volatile uint32_t i; i < 1000000; i++)
            ;
    }
}

