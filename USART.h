#ifndef UNOR3_USART_H
#define UNOR3_USART_H

#include <ctype.h>      // for tolower()
#include "error.h"

// TODO:    implement SPI mode when
//          covering the SPI drivers


/******************************************
 * Defines and function prototypes
 ******************************************/

typedef struct {
    volatile uint8_t UCSRA;
    volatile uint8_t UCSRB;
    volatile uint8_t UCSRC;
    const uint8_t RESERVED;
    volatile uint8_t UBRRL;
    volatile uint8_t UBRRH;
    volatile uint8_t UDR;
}USART_Type;

#define USART   ((USART_Type*)(0xC0))

// Configuration encoding

#define STOP_1          (0)
#define STOP_2          (1)
#define PARITY_NONE     (0b00)
#define PARITY_EVEN     (0b10)
#define PARITY_ODD      (0b11)
#define SIZE_5          (0b000)
#define SIZE_6          (0b001)
#define SIZE_7          (0b010)
#define SIZE_8          (0b011)
#define SIZE_9          (0b111)

#define ASYNC           (0b00)
#define SYNC            (0b01)
#define MASTER_SPI      (0b11)

// UCSRA bits
#define _MPCM           (0)
#define _U2X            (1)
#define _UPE            (2)
#define _DOR            (3)
#define _FE             (4)
#define _UDRE           (5)
#define _TXC            (6)
#define _RXC            (7)

//UCSRB bits
#define _TXB8           (0)
#define _RXB8           (1)
#define _UCSZH          (2)
#define _TXEN           (3)
#define _RXEN           (4)
#define _UDRIE          (5)
#define _TXCIE          (6)
#define _RXCIE          (7)

//UCSRC bits
#define _UCPOL          (0)
#define _UCSZL          (1)     // 2-bit setting
#define _USBS           (3)
#define _UPM            (4)     // 2-bit setting
#define _UMSEL          (6)     // 2-bit setting

void initUSART(uint8_t size, char parity, uint8_t stop_bits,
               uint8_t mode, uint32_t baud, bool tx_enable, bool rx_enable);

void setFrameSize(uint8_t size);
void setParity(uint8_t parity);
void setStopBits(uint8_t stop_bits);
void setSyncMode(uint8_t mode);
void setBaudRate(uint32_t baud);

void enableTX(bool enabled);
void enableRX(bool enabled);

void sendByte(uint8_t byte);

/******************************************
 * Function definitions
 ******************************************/

void initUSART(uint8_t size, char parity, uint8_t stop_bits, 
               uint8_t mode, uint32_t baud, bool tx_enable, bool rx_enable) {
    setFrameSize(size);
    setParity(parity);
    setStopBits(stop_bits);
    setSyncMode(mode);
    setBaudRate(baud);

    enableTX(tx_enable);
    enableRX(rx_enable);
}

void setFrameSize(uint8_t size) {

    switch (size) {
        case 5:
            size = SIZE_5;
            break;
        case 6:
            size = SIZE_6;
            break;
        case 7:
            size = SIZE_7;
            break;
        case 8:
            size = SIZE_8;
            break;
        case 9:
            size = SIZE_9;
            break;
        default:
            errorSOS(UNKNOWN_ERR);
    }

    // clear the size bits first
    USART->UCSRC &= ~(0b11 << _UCSZL);
    USART->UCSRB &= ~(1u << _UCSZH);

    // the lower two size bits need to be shifted
    USART->UCSRC |= ((size & 0b11) << _UCSZL);

    // the upper size bit is already in the right place
    USART->UCSRB |= (size & (1u << _UCSZH));
}

void setParity(uint8_t parity) {
    switch (tolower(parity)) {
        case 'n':
            parity = PARITY_NONE;
            break;
        case 'e':
            parity = PARITY_EVEN;
            break;
        case 'o':
            parity = PARITY_ODD;
            break;
        default:
            errorSOS(UNKNOWN_ERR);
    }

    USART->UCSRC &= ~(0b11 << _UPM);

    USART->UCSRC |= ((parity & 0b11) << _UPM); 
}

void setStopBits(uint8_t stop_bits) {
    switch (stop_bits) {
        case 1:
            USART->UCSRC &= ~(1u << _USBS);
            break;
        case 2:
            USART->UCSRC |= (1u << _USBS);
            break;
        default:
            errorSOS(UNKNOWN_ERR);
    }
}

void setSyncMode(uint8_t mode) {
    switch (mode) {
        case ASYNC:
            USART->UCSRA |= (1u << _U2X);   // double-speed in ASYNC mode
            break;
        case SYNC:
            USART->UCSRA &= ~(1u << _U2X);  // regular speed in SYNC mode
            break;
        case MASTER_SPI:
            // TODO?: Any other config for SPI mode?
            break;
        default:
            errorSOS(UNKNOWN_ERR);
    }

    USART->UCSRC &= ~(0b11 << _UMSEL);

    USART->UCSRC |= ((mode & 0b11) << _UMSEL);
}

void setBaudRate(uint32_t baud) {
    // ASYNC mode - double speed
    if (USART->UCSRA & (1u << _U2X)) {
        switch (baud) {
            case 2400:
                baud = 832;
                break;
            case 4800:
                baud = 416;
                break;
            case 9600:
                baud = 207;
                break;
            case 14400:
                baud = 138;
                break;
            case 19200:
                baud = 103;
                break;
            case 28800:
                baud = 68;
                break;
            case 38402:
                baud = 51;
                break;
            case 57600:
                baud = 32;
                break;
            case 76800:
                baud = 25;
                break;
            case 115200:
                baud = 16;
                break;
            case 230400:
                baud = 8;
                break;
            case 250000:
                baud = 7;
                break;
            case 500000:
                baud = 3;
                break;
            case 1000000:
                baud = 1;
                break;
            default:
                errorSOS(UNKNOWN_ERR);
        }
    }

    else {
        switch (baud) {
            case 2400:
                baud = 416;
                break;
            case 4800:
                baud = 207;
                break;
            case 9600:
                baud = 103;
                break;
            case 14400:
                baud = 68;
                break;
            case 19200:
                baud = 51;
                break;
            case 28800:
                baud = 34;
                break;
            case 38400:
                baud = 25;
                break;
            case 57600:
                baud = 16;
                break;
            case 76800:
                baud = 12;
                break;
            case 115200:
                baud = 8;
                break;
            case 230400:
                baud = 3;
                break;
            case 250000:
                baud = 3;
                break;
            case 500000:
                baud = 1;
                break;
            case 1000000:
                baud = 0;
                break;
            default:
                errorSOS(UNKNOWN_ERR);
        }
    }
       
    USART->UBRRH = (baud >> 8) & 0x0f;
    USART->UBRRL = baud & 0xff;
}

void enableTX(bool enabled) {
    if (enabled) {
        USART->UCSRB |= (1u << _TXEN);
    }

    else {
        USART->UCSRB &= ~(1u << _TXEN);
    }
}

void enableRX(bool enabled) {
    if (enabled) {
        USART->UCSRB |= (1u << _RXEN);
    }

    else {
        USART->UCSRB &= ~(1u << _RXEN);
    }
}

bool readyToTX() {
    return (USART->UCSRA & (1u << _UDRE));
}

bool readyToRX() {
    return (USART->UCSRA & (1u << _RXC));
}

void sendByte(uint8_t byte) {
    while (!readyToTX())
        ;   // intentionally empty

    USART->UDR = byte;
}

uint8_t receiveByte() {
    return (readyToRX() ? USART->UDR : 0);
}

#endif
