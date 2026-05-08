/*
The ST7796s transmits and receives data ordered from MSB to LSB, D7 to D0.
CS low to select
CPOL = 0, CPHA = 0 (commonly used by ST77XX variants & visible in datasheet)
*/

#include "include.h"

_DST _ST7796sRDDST(void) {
    _DST status = {0};

    PORTF &= ~(1 << CS_PIN); 

    _32u4SPI_TRX(0x09, 0);
    _32u4SPI_TRX(0x00, 1);

    uint8_t bytes[4];
    for (uint8_t i = 0; i < 4; i++) {
        bytes[i] = _32u4SPI_TRX(0x00, 1);
    }

    status.MY = (bytes[1] >> 1) & 1;

    if (status.SLPOUT) {
        DDRB |= (1 << PB0);
        while (1) {
            PORTB |= (1 << PB0); // pull LED high
            _delay_ms(100);
            PORTB &= ~(1 << PB0); // pull LED low
            _delay_ms(100);
        }
    }

    return status;
}

void ST7796sInit(void) {
    SPICRCFG cfg = {0, 1, 1, 1, 0, 0, 1, 1};
    _32u4SPI_Init(cfg);

    _32u4SPI_Reset(120);

    //begin initial configuration
    _32u4SPI_TRX(0x01, 0);
    _delay_ms(5);
    _32u4SPI_TRX(0x11, 0);
    _32u4SPI_TRX(0x38, 0);
    _32u4SPI_TRX(0x13, 0);
    _32u4SPI_TRX(0x20, 0);
    _32u4SPI_TRX(0x29, 0);
    _32u4SPI_TRX(0x51, 0);
    _32u4SPI_TRX(0x80, 1);

    /*
    uint8_t rxBuf[5] = {0};
    _32u4SPI_TRX_MultiByte((uint8_t){0x09}, (uint8_t){0}, 1, rxBuf, 5);
    
    for (uint8_t i = 1; i < 5; i++) {
        
    }
    */
    

    _ST7796sRDDST();


}
