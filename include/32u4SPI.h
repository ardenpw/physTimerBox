#ifndef _32u4SPI_h
#define _32u4SPI_h

#include <stdint.h>

typedef struct {
    uint8_t _SPIE;
    uint8_t _SPE;
    uint8_t _DORD;
    uint8_t _MSTR;
    uint8_t _CPOL;
    uint8_t _CPHA;
    uint8_t _SPR1;
    uint8_t _SPR0;
} SPICRCFG;

void dwait(void);

void _32u4SPI_Init(SPICRCFG cfg);

uint8_t _32u4SPI_TRX(const uint8_t data, const uint8_t dataDir);

void _32u4SPI_Reset(const uint8_t ms);

void _32u4SPI_TRX_MultiByte(const uint8_t *cmdData, const uint8_t *DCP, const uint8_t numCmd, uint8_t *rxBuf, const uint8_t numRXBytes);

#endif // #define _32u4SPI_h