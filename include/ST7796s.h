/*
The ST7796s expects data to be ordered from MSB to LSB, D7 to D0.
CS low to select
CPOL = 0, CPHA = 0
*/

#ifndef _ST7796s_h
#define _ST7796s_h

#include <stdio.h>

typedef struct {
    uint8_t BSTON;
    uint8_t MY;
    uint8_t MX;
    uint8_t MV;
    uint8_t ML;
    uint8_t RGB;
    uint8_t ST245;
    uint8_t ST24;
    uint8_t ST23;
    uint8_t IFPF2;
    uint8_t IFPF1;
    uint8_t IFPF0;
    uint8_t IDMON;
    uint8_t PTLON;
    uint8_t SLPOUT;
    uint8_t NORON;
    uint8_t ST15;
    uint8_t ST14; // NOT USED (0)
    uint8_t INVON;
    uint8_t ST12; // NOT USED (0)
    uint8_t ST11; // NOT USED (0)
    uint8_t DISON;
    uint8_t TEON;
    uint8_t GCSEL2;
    uint8_t GCSEL1;
    uint8_t GCSEL0;
    uint8_t TEM;
    uint8_t ST4; // FOR FUTURE USE
    uint8_t ST3; // FOR FUTURE USE
    uint8_t ST2; // FOR FUTURE USE
    uint8_t ST1; // FOR FUTURE USE
    uint8_t ST0; // FOR FUTURE USE
} _DST; 

_DST _ST7796sRDDST(void);

void ST7796sInit(void);

#endif // #ifndef _ST7796s_h