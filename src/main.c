#include "include.h"

#define FCPU 16000000

int main(void) { 
    _spiPinCfg spiPinCfg= {
        .MOSI_PIN = {._dataDir = 1, ._port = &PORTB, ._pin = PB2},
        .MISO_PIN = {._dataDir = 0, ._port = &PORTB, ._pin = PB3},
        .SCK_PIN = {._dataDir = 1, ._port = &PORTB, ._pin = PB1},
        .DC_PIN = {._dataDir = 1, ._port = &PORTF, ._pin = PF7},
        .CS_PIN = {._dataDir = 1, ._port = &PORTF, ._pin = PF6},
        .RST_PIN = {._dataDir = 1, ._port = &PORTF, ._pin = PF5}
    };
    
    _spiSPCRCfg spiSPRCCfg = {
        .SPIE_ = 0,
        .SPE_ = 1,
        .DORD_ = 1, // lsb first
        .MSTR_ = 1,
        .CPOL_ = 1,
        .CPHA_ = 1,
        .SPR1_ = 1,
        .SPR0_ = 1
    };

    _32u4SPI_Init(spiSPRCCfg, spiPinCfg);
    _32u4SPI_TRX(0x01, 0);
}
