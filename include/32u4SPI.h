#ifndef _32u4SPI_h
#define _32u4SPI_h

typedef struct {
    volatile uint8_t *_port;
    uint8_t _pin;
    uint8_t _dataDir; // 0 is input
} _portPin;

typedef struct {
    _portPin MOSI_PIN;
    _portPin MISO_PIN;
    _portPin SCK_PIN;
    _portPin DC_PIN;
    _portPin CS_PIN;
    _portPin RST_PIN;
} _spiPinCfg;

typedef struct {
    bool SPIE_;
    bool SPE_;
    bool DORD_;
    bool MSTR_;
    bool CPOL_;
    bool CPHA_;
    bool SPR1_;
    bool SPR0_;
} _spiSPCRCfg;

void _32u4SPI_Init(const _spiSPCRCfg spiSPCRCfg, const _spiPinCfg spiPinCfg); // _CPHA: Clock Phase, _CPOL Clock Polarity. See SPI section in the 32u4 datasheet

uint8_t _32u4SPI_TRX(const uint8_t data, const uint8_t DC); // data: 1 byte of data, DC: 1 data, 0 cmd

void _32u4SPI_Reset(const uint8_t port, const uint8_t pin, const uint8_t delayUs); // port: port of pin (e.g. PORTF), pin: pin number (e.g. PF5 = 5)

#endif // #define _32u4SPI_h