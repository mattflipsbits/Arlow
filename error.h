#ifndef UNOR3_ERROR_H
#define UNOR3_ERROR_H


/******************************************
 * Defines and function prototypes
 ******************************************/
// GPIO registers needed for user LED control
#define _PINB   (*(volatile uint8_t*)(0x23))
#define _DDRB   (*(volatile uint8_t*)(0x24))
#define _PORTB  (*(volatile uint8_t*)(0x25)) 

// GPIO Error Codes


// USART Registers
#define _UCSRA  (*(volatile uint8_t*)(0xC0))
#define _UCSRB  (*(volatile uint8_t*)(0xC1))
#define _UCSRC  (*(volatile uint8_t*)(0xC2))
#define _UBRRL  (*(volatile uint8_t*)(0xC4))

// USART Error Codes


// SOS mode uses its own discrete functions for
// initializing the necessary GPIO and USART since:
// 1) it doesn't need the generality and
// 2) it avoids potential circular dependency
//    errors with the regular peripheral headers.
//    Trust me, I tried it, and there were lots
//    of pesky implicit declaration warnings unless
//    you got real specific with the order of
//    #includes and function prototypes.

void initSOS();
void toggleUserLED();
void errorSOS();




/******************************************
 * Function definitions
 ******************************************/

void initSOS() {
    // Just need a quick and dirty output init
    // of the user LED (at PB5) and set it to low
    _DDRB |= (1u << 5);
    _PORTB &= ~(1u << 5);

    // Initialize USART to 8N1, 9600 baud
    // 8
    _UCSRC |= (0b11 << 1);
    _UCSRB &= ~(1u << 2);

    // N
    _UCSRC &= ~(0b11 << 4);

    // 1
    _UCSRC &= ~(1u << 3);

    // 9600 baud, async
    _UCSRA |= (1u << 1);   // ASYNC mode, double-speed
    int baud = 207;
    _UBRRL = baud & 0xff;

    // enable transmission
    _UCSRB |= (1u << 3);
}

void toggleUserLED() {
    _PINB |= (1u << 5);
}

// Loops SOS on the user LED using soft delays
// TODO: Communicate error via serial port to host computer
void errorSOS() {
    const uint32_t DOT = 50000;
    const uint32_t DASH = 3 * DOT;
    const uint32_t SPACE = 4 * DOT;         
 
    // Spaces are 7 dots in normal Morse, but we only
    // need to add 4 since 3 dots of space are included
    // in the previous letter   

    initSOS();

    // Note: volatile required in the empty timing loops
    //       otherwise they get optimized out

    while (1) {
        // S
        for (int i = 0; i < 6; i++) {
            toggleUserLED();

            for (volatile uint32_t j = 0; j < DOT; j++)
                ;   // intentionally empty
        }

        // O
        for (int i = 0; i < 6; i++) {
            toggleUserLED();

            if (i % 2 == 0) {
                for (volatile uint32_t j = 0; j < DASH; j++)
                    ;   // intentionally empty;
            }   

            else {
                for (volatile uint32_t j = 0; j < DOT; j++)
                    ;   // intentionally empty
            }
        }
        

        // S
        for (int i = 0; i < 6; i++) {
            toggleUserLED();

            for (volatile uint32_t j = 0; j < DOT; j++)
                ;   // intentionally empty
        }

        // space
        for (volatile uint32_t j = 0; j < SPACE; j++)
            ;   // intentionally empty
    }
}



#endif
