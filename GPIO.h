#ifndef UNOR3_GPIO_H
#define UNOR3_GPIO_H

// Optional AVR headers
// for testing conflicts

// #include <avr/io.h>     



// GENERAL NOTE
// Leading underscores are used to avoid conflict
// with the existing definitions in AVR headers



/******************************************
 * Defines and function prototypes
 ******************************************/

// PIN: Writing 1 toggles bit. Independent of DDR
// DDR: Direction register. 0-input, 1-output
// PORT: Output: 0-low, 1-high
//       Input: 0-deactivates pullup resistor, 1-activates pullup resistor

typedef struct {
    volatile uint8_t PIN;
    volatile uint8_t DDR;
    volatile uint8_t PORT;
} GPIO_Type;

#define INVALID_PORT    ((GPIO_Type*)(NULL))
#define GPIOB           ((GPIO_Type*)(0x23))
#define GPIOC           ((GPIO_Type*)(0x26))
#define GPIOD           ((GPIO_Type*)(0x29))

#define _MCUCR           (*(volatile uint8_t*)(0x55))
#define _PUD             (4)

// Encoding
// Internal port encoding
#define _PB             (0x10)
#define _PC             (0x20)
#define _PD             (0x30)

#define _PIN0           (0x00)
#define _PIN1           (0x01)
#define _PIN2           (0x02)
#define _PIN3           (0x03)
#define _PIN4           (0x04)
#define _PIN5           (0x05)
#define _PIN6           (0x06)
#define _PIN7           (0x07)
#define MAX_PIN         (0x07)

#define _PB0            (_PB + _PIN0)
#define _PB1            (_PB + _PIN1)
#define _PB2            (_PB + _PIN2)
#define _PB3            (_PB + _PIN3)
#define _PB4            (_PB + _PIN4)
#define _PB5            (_PB + _PIN5)
#define _PB6            (_PB + _PIN6)
#define _PB7            (_PB + _PIN7)

#define _PC0            (_PC + _PIN0)
#define _PC1            (_PC + _PIN1)
#define _PC2            (_PC + _PIN2)
#define _PC3            (_PC + _PIN3)
#define _PC4            (_PC + _PIN4)
#define _PC5            (_PC + _PIN5)
#define _PC6            (_PC + _PIN6)
#define _PC7            (_PC + _PIN7)

#define _PD0            (_PD + _PIN0)
#define _PD1            (_PD + _PIN1)
#define _PD2            (_PD + _PIN2)
#define _PD3            (_PD + _PIN3)
#define _PD4            (_PD + _PIN4)
#define _PD5            (_PD + _PIN5)
#define _PD6            (_PD + _PIN6)
#define _PD7            (_PD + _PIN7)

// Digital pin mapping
#define D0              (_PD0)
#define D1              (_PD1)
#define D2              (_PD2)
#define D3              (_PD3)
#define D4              (_PD4)
#define D5              (_PD5)
#define D6              (_PD6)
#define D7              (_PD7)
#define D8              (_PB0)
#define D9              (_PB1)
#define D10             (_PB2)
#define D11             (_PB3)
#define D12             (_PB4)
#define D13             (_PB5)

#define USER_LED        (D13)

#define INPUT           (0x00)
#define OUTPUT          (0x01)
#define INPUT_PULLUP    (0x10)

#define LOW             (0x00)
#define HIGH            (0x01)

GPIO_Type* getPort(uint8_t pin_code);

void enablePullup(bool enabled);
void setPullup(uint8_t pin_code, uint8_t state);

bool validPort(GPIO_Type* port);
bool validPin(uint8_t pin);

void initGPIO(uint8_t pin_code, uint8_t mode);
void writePin(uint8_t pin_code, uint8_t pin_state);
void togglePin(uint8_t pin_code);
bool readPin(uint8_t pin_code);

void errorGPIO();



/***********************************************
 * Function definitions
 ***********************************************/

GPIO_Type* getPort(uint8_t pin_code) {
    uint8_t port = pin_code & 0xF0;
    GPIO_Type* GPIO;

    switch(port) {
        case _PB:
            GPIO = GPIOB;
            break;
        case _PC:
            GPIO = GPIOC;
            break;
        case _PD:
            GPIO = GPIOD;
            break;
        default:
            GPIO = INVALID_PORT;
    }

    // Note: may compact to just return statements in cases
    return GPIO;
}

void enablePullup(bool enabled) {
    if (enabled) {
        _MCUCR &= ~(1u << _PUD);
    }

    else {
        _MCUCR |= (1u << _PUD);
    }
}

void setPullup(uint8_t pin_code, uint8_t state) {
    if (_MCUCR & (1u << _PUD)) {
        errorGPIO();
    }
    
    writePin(pin_code, state);
}

bool validPort(GPIO_Type* port) {
    return (port == GPIOB || port == GPIOC || port == GPIOD);
}

bool validPin(uint8_t pin) {
    return (pin <= MAX_PIN);
}

void initGPIO(uint8_t pin_code, uint8_t mode) {
    uint8_t pin = pin_code & 0x0F;
    GPIO_Type* GPIO = getPort(pin_code);

    if (!validPort(GPIO) || !validPin(pin)) {
        errorGPIO();
    }

    if (mode == INPUT || mode == INPUT_PULLUP) {
        GPIO->DDR &= ~(1u << pin);

        if (mode == INPUT_PULLUP) {
            _MCUCR &= ~(1u << _PUD);

            GPIO->PORT |= (1u << pin);
        }
    }

    else if (mode == OUTPUT) {
        GPIO->DDR |= (1u << pin);
    }

    else {
        errorGPIO();
    }
}

void writePin(uint8_t pin_code, uint8_t pin_state) {
    uint8_t pin = pin_code & 0x0F;
    GPIO_Type* GPIO = getPort(pin_code);

    if (!validPort(GPIO) || !validPin(pin)) {
        errorGPIO();
    }

    if (pin_state == HIGH) {
        GPIO->PORT |= (1u << pin);
    }

    else if (pin_state == LOW) {
        GPIO->PORT &= ~(1u << pin);
    }

    else {
        errorGPIO();
    }
}

void togglePin(uint8_t pin_code) {
    uint8_t pin = pin_code & 0x0F;
    GPIO_Type* GPIO = getPort(pin_code);

    if (!validPort(GPIO) || !validPin(pin)) {
       errorGPIO();
    }

    GPIO->PIN |= (1u << pin);
}

bool readPin(uint8_t pin_code) {
    uint8_t pin = pin_code & 0x0F;
    GPIO_Type* GPIO = getPort(pin_code);

    if (!validPort(GPIO) || !validPin(pin)) {
        errorGPIO();
    }
    
    return GPIO->PIN & (1u << pin);
}

// Loops SOS on the user LED using soft delays
// TODO: Communicate error via serial port to host computer
void errorGPIO() {
    const uint32_t DOT = 50000;
    const uint32_t DASH = 3 * DOT;
    const uint32_t SPACE = 4 * DOT;         
 
    // Spaces are 7 dots in normal Morse, but we only
    // need to add 4 since 3 dots of space are included
    // in the previous letter   

    initGPIO(USER_LED, OUTPUT);
    writePin(USER_LED, LOW);

    // Note: volatile required in the empty timing loops
    //       otherwise they get optimized out

    while (1) {
        // S
        for (int i = 0; i < 6; i++) {
            togglePin(USER_LED);

            for (volatile uint32_t j = 0; j < DOT; j++)
                ;   // intentionally empty
        }

        // O
        for (int i = 0; i < 6; i++) {
            togglePin(USER_LED);

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
            togglePin(USER_LED);

            for (volatile uint32_t j = 0; j < DOT; j++)
                ;   // intentionally empty
        }

        // space
        for (volatile uint32_t j = 0; j < SPACE; j++)
            ;   // intentionally empty
    }
}


#endif
