#include "include.h"

static _portPin pins[6];

static void dWait(void) {
    while(!(SPSR & (1<<SPIF)));
}

static void _pinCfg(volatile uint8_t *_port, uint8_t _pin, const uint8_t _dataDir) {
    // decrement _port by 1 to get the DDR of that port
    if (_dataDir) { *(_port - 1) |= (1 << _pin); } 
    else { *(_port - 1) &= ~(1 << _pin); }
}

void _32u4SPI_Init(const _spiSPCRCfg spiSPCRCfg, const _spiPinCfg spiPinCfg) { // _CPHA: Clock Phase, _CPOL Clock Polarity (See SPI section in the 32u4 datasheet), _MSTR: (1 = device acts as master, 2 = device acts as slave)   
    uint8_t _spcr = 0;

    PRR0 &= ~(1 << PRSPI); //dissable power limit

    //configure each of our SPI pins
    pins[0] = spiPinCfg.MOSI_PIN;
    pins[1] = spiPinCfg.MISO_PIN;
    pins[2] = spiPinCfg.SCK_PIN;
    pins[3] = spiPinCfg.DC_PIN;
    pins[4] = spiPinCfg.CS_PIN;
    pins[5] = spiPinCfg.RST_PIN;

    // for each pin configure as input or output
    for (uint8_t i = 0; i < 6; i++) {
        _pinCfg(pins[i]._port, pins[i]._pin, pins[i]._dataDir);
    }

    //idle highs
    *pins[5]._port |= (1 << pins[5]._pin); // pull high rst
    *pins[4]._port |= (1 << pins[4]._pin); //pull chip select high
    *pins[3]._port &= ~(1 << pins[3]._pin); //start in command mode

    // configure SPCR register
    bool *cfgArr = (bool*)&spiSPCRCfg; // convert to bool arr
    for (uint8_t i = 0; i < 8; i++) {
        if (cfgArr[i]) { _spcr |= (1 << i); }
        else { _spcr &= ~(1 << i); }
    }
    SPCR = _spcr;
}

uint8_t _32u4SPI_TRX(const uint8_t data, const uint8_t DC) { // data: 1 byte of data, DC: (1 = data, 0 = cmd)
    if (DC) { *pins[3]._port |= (1 << pins[3]._pin); } // set DC if DC
    else { *pins[3]._port &= ~(1 << pins[3]._pin); }
    *pins[4]._port &= ~(1 << pins[4]._pin); // CS_PIN to low
    SPDR = data;
    dWait();
    uint8_t dataReturn = SPDR; // return our data
    *pins[4]._port |= (1 << pins[4]._pin); // CS_PIN back to high
    return dataReturn;
}

void _32u4SPI_Reset(const uint8_t port, const uint8_t pin, const uint8_t delayUs) { // port: port of pin (e.g. PORTF), pin: pin number (e.g. PF5 = 5)
    *pins[5]._port |= (1 << pins[5]._pin);
    _delay_us((double)delayUs);
    *pins[5]._port &= ~(1 << pins[5]._pin);
    _delay_us((double)delayUs);
    *pins[5]._port |= (1 << pins[5]._pin);
}
