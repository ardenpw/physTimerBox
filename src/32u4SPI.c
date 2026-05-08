#include "include.h"

void dwait(void) { while(!(SPSR & (1<<SPIF))); }

void _32u4SPI_Init(SPICRCFG cfg) {
    PRR0 &= ~(1 << PRSPI);

    DDRB &= ~(1 << MISO_PIN);
    DDRB |= (1 << MOSI_PIN) | (1 << SCK_PIN);
    DDRF |= (1 << DC_PIN) | (1 << CS_PIN) | (1 << RST_PIN) | (1 << LED_PIN);

    SPCR = (1 << SPE) | (1 << DORD) | (1 << MSTR) | (1 << SPR1) | (1 << SPR0); //cfg
    SPCR &= ~(1 << CPOL);
    SPCR &= ~(1 << CPHA);

    /*
    uint8_t cfgVals[] = {cfg._SPIE, cfg._SPE, cfg._DORD, cfg._MSTR, cfg._CPOL, cfg._CPHA, cfg._SPR1, cfg._SPR0};
    volatile uint8_t cfgVal = 0;
    
    for (uint8_t i = 8; i > 0; i--) {
        if (cfgVals[i]) { cfgVal |= (1 << i); }
        else { cfgVal &= ~(1 << i); }
    }
    
    SPCR = cfgVal;
    */

    //idle highs
    PORTF |= (1 << RST_PIN); // pull high rst
    PORTF |= (1 << CS_PIN); //pull chip select high
    PORTF &= ~(1 << DC_PIN); //start in command mode
}

uint8_t _32u4SPI_TRX(const uint8_t data, const uint8_t DCP) { // 1 = data, 0 = cmd
    if (DCP) { PORTF |= (1 << DC_PIN); }
    else { PORTF &= ~(1 << DC_PIN); }
    PORTF &= ~(1 << CS_PIN);
    SPDR = data;
    dwait();
    PORTF |= (1 << CS_PIN);
    return SPDR;
}

void _32u4SPI_Reset(const uint8_t ms) {
    PORTF |= (1 << RST_PIN);
    //_delay_us(us);
    _delay_ms(ms);
    PORTF &= ~(1 << RST_PIN);
    _delay_ms(ms);
    //_delay_us(us);
    PORTF |= (1 << RST_PIN);
}

void _32u4SPI_TRX_MultiByte(const uint8_t *cmdData, const uint8_t *DCP, const uint8_t numCmd, uint8_t *rxBuf, const uint8_t numRXBytes) {
    PORTF &= ~(1 << CS_PIN);
    
    for (uint8_t i = 0; i < numCmd; i++) {
        _32u4SPI_TRX(cmdData[i], DCP[i]);
    }
    
    for (uint8_t i = 0; i < numRXBytes; i++) {
        rxBuf[i] = _32u4SPI_TRX(0x00, 1);
    }

    PORTF |= (1 << CS_PIN);
}
