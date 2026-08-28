/*
 *  Blink
 *  
 *  A basic Blink example using Arlow.
 *  Assumes directory structure of the repo is intact.
 *
 *  Can be built from this directory with 'make'
 */

#include "../Arlow.h"

#define DELAY   (250000)

int main(void) {
    initGPIO(D13, OUTPUT);

    while (1) {
        togglePin(D13);

        for (volatile uint32_t i = 0; i < DELAY; i++)
            ;   // soft timing loop - intentionally empty
    }
}
