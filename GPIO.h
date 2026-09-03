#ifndef UNOR3_GPIO_H
#define UNOR3_GPIO_H

// Optional AVR headers
// for testing conflicts

// #include <avr/io.h>     


// Required if using GPIO.h alone
// and not using Arlow.h

#include "error.h"



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

// Analog pin mapping
// TODO: To be added with ADC

#define USER_LED        (D13)

#define INPUT           (0x00)
#define OUTPUT          (0x01)
#define INPUT_PULLUP    (0x10)

#define LOW             (0x00)
#define HIGH            (0x01)


GPIO_Type* getPort(uint8_t pin_code);

void enablePullup(bool enabled);
void setPullup(uint8_t pin_code, uint8_t state);

void initGPIO(uint8_t pin_code, uint8_t mode);
void validateGPIO(GPIO_Type* port, uint8_t pin);
bool validPort(GPIO_Type* port);
bool validPin(uint8_t pin);

void writePin(uint8_t pin_code, uint8_t pin_state);
void togglePin(uint8_t pin_code);
bool readPin(uint8_t pin_code);



/******************************************
 * Function definitions
 ******************************************/

GPIO_Type* getPort(uint8_t pin_code) {
    uint8_t port = pin_code & 0xF0;
    GPIO_Type* GPIO;

    switch(port) {
        case _PB:
            return GPIOB;
            break;
        case _PC:
            return GPIOC;
            break;
        case _PD:
            return GPIOD;
            break;
        default:
            errorSOS();
    }
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
        errorSOS();
    }
    
    writePin(pin_code, state);
}

void initGPIO(uint8_t pin_code, uint8_t mode) {
    uint8_t pin = pin_code & 0x0F;
    GPIO_Type* GPIO = getPort(pin_code);

    validateGPIO(GPIO, pin);

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
        errorSOS();
    }
}

void validateGPIO(GPIO_Type* port, uint8_t pin) {
    if (!validPort(port) || !validPin(pin)) {
        errorSOS();
    }
}

bool validPort(GPIO_Type* port) {
    return (port == GPIOB || port == GPIOC || port == GPIOD);
}

bool validPin(uint8_t pin) {
    return (pin <= MAX_PIN);
}

void writePin(uint8_t pin_code, uint8_t pin_state) {
    uint8_t pin = pin_code & 0x0F;
    GPIO_Type* GPIO = getPort(pin_code);

    validateGPIO(GPIO, pin);

    if (pin_state == HIGH) {
        GPIO->PORT |= (1u << pin);
    }

    else if (pin_state == LOW) {
        GPIO->PORT &= ~(1u << pin);
    }

    else {
        errorSOS();
    }
}

void togglePin(uint8_t pin_code) {
    uint8_t pin = pin_code & 0x0F;
    GPIO_Type* GPIO = getPort(pin_code);

    validateGPIO(GPIO, pin);

    GPIO->PIN |= (1u << pin);
}

bool readPin(uint8_t pin_code) {
    uint8_t pin = pin_code & 0x0F;
    GPIO_Type* GPIO = getPort(pin_code);

    validateGPIO(GPIO, pin);
    
    return GPIO->PIN & (1u << pin);
}



#endif
