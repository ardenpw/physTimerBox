#include "include.h"

uint8_t tx(uint8_t data) {
    PORTF &= ~(1 << CS_PIN); // pull cs low to begin transfer
    SPDR = data;
    while (!(SPSR & (1 << SPIF))) {;}
    PORTF |= (1 << CS_PIN);
    PORTB ^= (1 << PB0);
    return SPDR;
}

int main(void) { 
    DDRB |= (1 << PB0);
    DDRB &= ~(1 << PB0);

    // pin config
    DDRF |= (1 << CS_PIN | 1 << RST_PIN | 1 << LED_PIN);
    DDRB |= (1 << DC_PIN | 1 << MOSI_PIN | 1 << SCK_PIN);
    DDRB &= ~(1 << MISO_PIN);
    PORTF |= (1 << CS_PIN | 1 << RST_PIN);
    PORTF &= ~(1 << LED_PIN);
    PORTB &= ~(1 << DC_PIN); // 1: data, 0: cmd

    // peripheral conifg
    PRR0 &= ~(1 << PRSPI); // disable power limit
    SPCR |= (1 << SPE | 1 << MSTR);
    SPCR &= ~(1 << DORD | 1 << CPOL | 1 << CPHA); // 0, 0 (cpol, cpha)

    // SPI clock config
    SPCR &= ~(1 << SPR1 | 1 << SPR0);
    SPSR &= ~(1 << SPI2X);

    // Screen stuff ---
    PORTF &= ~(1 << RST_PIN);
    _delay_ms(120); // according to the datasheet
    PORTF |= (1 << RST_PIN);

    PORTF &= ~(1 << CS_PIN);
    PORTB &= ~(1 << DC_PIN);

    tx(0x01); //SWRESET
    _delay_ms(5); // according to the datasheet
    tx(0x11); //SLPOUT
    _delay_ms(5); //according to the datasheet
    tx(0x38); //IDMOFF
    tx(0x13); //NORON

    tx(0x3A); //COLMOD
    PORTB |= (1 << DC_PIN);
    tx(0x55); //payload 0b01010101
    PORTB &= ~(1 << DC_PIN);

    tx(0x36); //MADCTL
    PORTB |= (1 << DC_PIN);
    tx(0b10); //payload 0
    PORTB &= ~(1 << DC_PIN);

    tx(0x29); //DISPON
    _delay_ms(250);
    PORTF |= (1 << CS_PIN);
    
    PORTF |= (1 << LED_PIN);

    // --------
    PORTF &= ~(1 << CS_PIN);
    PORTB &= ~(1 << DC_PIN);

    tx(0x2A);
    PORTB |= (1 << DC_PIN);
    tx(0x00);
    tx(0x00);
    tx(0xff);
    tx(0xff);
    PORTB &= ~(1 << DC_PIN);
 
    tx(0x2A);
    PORTB |= (1 << DC_PIN);
    tx(0x00);
    tx(0x00);
    tx(0xff);
    tx(0xff);
    PORTB &= ~(1 << DC_PIN);
    
    /*
    Assuming MV, MX, MY are all 0, the X (column) RAM pixel counter 
    increments by one for every two bytes of RGB565 received, from 
    XS to XE, then increments the Y (row) by one, then repeats this 
    from YS to YE
    */

    tx(0x2C); //RAMWR

    // RGB direction appears to be reversed
    // maybe we can also increase the spi clock speed as
    // its really slow rn
    uint16_t pixelData = 0; //RGB565 000

    PORTB |= (1 << DC_PIN);
    for (uint32_t i = 0; i < 153600; i++) {
        tx(pixelData >> 8);
        tx(pixelData);
    }

    PORTB &= ~(1 << DC_PIN);
    tx(0x00);
    
    
}
