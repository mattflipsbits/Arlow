#include "Arlow.h"

#define DELAY       (50000)
#define TEST_PIN    (0x1A)

int main(void) {
    initGPIO(TEST_PIN, OUTPUT);

    while(1) {
        togglePin(TEST_PIN);

        for (volatile uint32_t i = 0; i < DELAY; i++)
            ;
    }
}
