// Font size 6 is intended to display numbers and time
// This font only contains characters [space] 0 1 2 3 4 5 6 7 8 9 . : a p m
// The Pipe character | is a narrow space to aid formatting
// All other characters print as a space


#include "Font64.h"

#ifdef __AVR__
 #include <avr/io.h>
 #include <avr/pgmspace.h>
#elif defined(ESP8266)
 #include <pgmspace.h>
#else
 #define PROGMEM
#endif

PROGMEM const unsigned char widtbl_f64[96] =          // character width table
{
        15, 15, 15, 15, 15, 15, 15, 15,     // char 32 - 39
        15, 15, 15, 15, 15, 20, 18, 15,     // char 40 - 47
        30, 30, 30, 30, 30, 30, 30, 30,     // char 48 - 55
        30, 30, 18, 15, 15, 15, 15, 15,     // char 56 - 63
        15, 15, 15, 15, 15, 15, 15, 15,     // char 64 - 71
        15, 15, 15, 15, 15, 15, 15, 15,     // char 72 - 79
        15, 15, 15, 15, 15, 15, 15, 15,     // char 80 - 87
        15, 15, 15, 15, 15, 15, 15, 15,     // char 88 - 95
        15, 30, 15, 15, 15, 15, 15, 15,     // char 96 - 103
        15, 15, 15, 15, 15, 45, 15, 15,     // char 104 - 111
        32, 15, 15, 15, 15, 15, 15, 15,     // char 112 - 119
        15, 15, 15, 15, 10, 15, 15, 15      // char 120 - 127
};





// Row format, MSB left

PROGMEM const unsigned char chr_f64_20[96] =          // 2 bytes per row
{
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,    // row 1 - 6
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,    // row 7 - 12
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,    // row 13 - 18
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,    // row 19 - 24
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,    // row 25 - 30
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,    // row 31 - 36
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,    // row 37 - 42
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00     // row 43 - 48
};
PROGMEM const unsigned char chr_f64_2D[144] =         // 3 bytes per row
{
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,    // row 1 - 4
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,    // row 5 - 8
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,    // row 9 - 12
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,    // row 13 - 16
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0xFC, 0x00,    // row 17 - 20
        0x1F, 0xFC, 0x00, 0x1F, 0xFC, 0x00, 0x1F, 0xFC, 0x00, 0x00, 0x00, 0x00,    // row 21 - 24
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,    // row 25 - 28
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,    // row 29 - 32
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,    // row 33 - 36
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,    // row 37 - 40
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,    // row 41 - 44
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00     // row 45 - 48
};
PROGMEM const unsigned char chr_f64_2E[144] =         // 3 bytes per row
{
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,    // row 1 - 4
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,    // row 5 - 8
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,    // row 9 - 12
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,    // row 13 - 16
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,    // row 17 - 20
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,    // row 21 - 24
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,    // row 25 - 28
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xC0, 0x00,    // row 29 - 32
        0x07, 0xC0, 0x00, 0x07, 0xC0, 0x00, 0x07, 0xC0, 0x00, 0x07, 0xC0, 0x00,    // row 33 - 36
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,    // row 37 - 40
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,    // row 41 - 44
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00     // row 45 - 48
};
PROGMEM const unsigned char chr_f64_30[192] =         // 4 bytes per row
{
        0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x80, 0x00, 0x03, 0xFF, 0xF0, 0x00,    // row 1 - 3
        0x07, 0xFF, 0xF8, 0x00, 0x0F, 0xFF, 0xFC, 0x00, 0x1F, 0xC0, 0xFE, 0x00,    // row 4 - 6
        0x1F, 0x00, 0x3E, 0x00, 0x1E, 0x00, 0x1E, 0x00, 0x3E, 0x00, 0x1F, 0x00,    // row 7 - 9
        0x3C, 0x00, 0x0F, 0x00, 0x3C, 0x00, 0x0F, 0x00, 0x3C, 0x00, 0x0F, 0x00,    // row 10 - 12
        0x78, 0x00, 0x07, 0x80, 0x78, 0x00, 0x07, 0x80, 0x78, 0x00, 0x07, 0x80,    // row 13 - 15
        0x78, 0x00, 0x07, 0x80, 0x78, 0x00, 0x07, 0x80, 0x78, 0x00, 0x07, 0x80,    // row 16 - 18
        0x78, 0x00, 0x07, 0x80, 0x78, 0x00, 0x07, 0x80, 0x78, 0x00, 0x07, 0x80,    // row 19 - 21
        0x78, 0x00, 0x07, 0x80, 0x78, 0x00, 0x07, 0x80, 0x78, 0x00, 0x07, 0x80,    // row 22 - 24
        0x78, 0x00, 0x07, 0x80, 0x78, 0x00, 0x07, 0x80, 0x3C, 0x00, 0x0F, 0x00,    // row 25 - 27
        0x3C, 0x00, 0x0F, 0x00, 0x3C, 0x00, 0x0F, 0x00, 0x3E, 0x00, 0x1F, 0x00,    // row 28 - 30
        0x1E, 0x00, 0x1E, 0x00, 0x1F, 0x00, 0x3E, 0x00, 0x1F, 0xC0, 0xFE, 0x00,    // row 31 - 33
        0x0F, 0xFF, 0xFC, 0x00, 0x07, 0xFF, 0xF8, 0x00, 0x03, 0xFF, 0xF0, 0x00,    // row 34 - 36
        0x00, 0x7F, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,    // row 37 - 39
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,    // row 40 - 42
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,    // row 43 - 45
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00     // row 46 - 48
};
PROGMEM const unsigned char chr_f64_31[192] =         // 4 bytes per row
{
        0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xC0, 0x00, 0x00, 0x01, 0xC0, 0x00,    // row 1 - 3
        0x00, 0x03, 0xC0, 0x00, 0x00, 0x03, 0xC0, 0x00, 0x00, 0x07, 0xC0, 0x00,    // row 4 - 6
        0x00, 0x0F, 0xC0, 0x00, 0x00, 0x3F, 0xC0, 0x00, 0x07, 0xFF, 0xC0, 0x00,    // row 7 - 9
        0x07, 0xFF, 0xC0, 0x00, 0x07, 0xFB, 0xC0, 0x00, 0x07, 0xC3, 0xC0, 0x00,    // row 10 - 12
        0x00, 0x03, 0xC0, 0x00, 0x00, 0x03, 0xC0, 0x00, 0x00, 0x03, 0xC0, 0x00,    // row 13 - 15
        0x00, 0x03, 0xC0, 0x00, 0x00, 0x03, 0xC0, 0x00, 0x00, 0x03, 0xC0, 0x00,    // row 16 - 18
        0x00, 0x03, 0xC0, 0x00, 0x00, 0x03, 0xC0, 0x00, 0x00, 0x03, 0xC0, 0x00,    // row 19 - 21
        0x00, 0x03, 0xC0, 0x00, 0x00, 0x03, 0xC0, 0x00, 0x00, 0x03, 0xC0, 0x00,    // row 22 - 24
        0x00, 0x03, 0xC0, 0x00, 0x00, 0x03, 0xC0, 0x00, 0x00, 0x03, 0xC0, 0x00,    // row 25 - 27
        0x00, 0x03, 0xC0, 0x00, 0x00, 0x03, 0xC0, 0x00, 0x00, 0x03, 0xC0, 0x00,    // row 28 - 30
        0x00, 0x03, 0xC0, 0x00, 0x00, 0x03, 0xC0, 0x00, 0x00, 0x03, 0xC0, 0x00,    // row 31 - 33
        0x00, 0x03, 0xC0, 0x00, 0x00, 0x03, 0xC0, 0x00, 0x00, 0x03, 0xC0, 0x00,    // row 34 - 36
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,    // row 37 - 39
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,    // row 40 - 42
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,    // row 43 - 45
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00     // row 46 - 48
};
PROGMEM const unsigned char chr_f64_32[192] =         // 4 bytes per row
{
        0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0xC0, 0x00, 0x00, 0xFF, 0xF8, 0x00,    // row 1 - 3
        0x03, 0xFF, 0xFC, 0x00, 0x07, 0xFF, 0xFE, 0x00, 0x07, 0xE0, 0x7F, 0x00,    // row 4 - 6
        0x0F, 0x80, 0x1F, 0x00, 0x0F, 0x80, 0x0F, 0x00, 0x0F, 0x00, 0x0F, 0x80,    // row 7 - 9
        0x1F, 0x00, 0x07, 0x80, 0x1E, 0x00, 0x07, 0x80, 0x1E, 0x00, 0x07, 0x80,    // row 10 - 12
        0x1E, 0x00, 0x07, 0x80, 0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 0x0F, 0x80,    // row 13 - 15
        0x00, 0x00, 0x0F, 0x80, 0x00, 0x00, 0x1F, 0x00, 0x00, 0x00, 0x3F, 0x00,    // row 16 - 18
        0x00, 0x00, 0x7E, 0x00, 0x00, 0x01, 0xFC, 0x00, 0x00, 0x07, 0xF8, 0x00,    // row 19 - 21
        0x00, 0x1F, 0xF0, 0x00, 0x00, 0x3F, 0xE0, 0x00, 0x00, 0xFF, 0x80, 0x00,    // row 22 - 24
        0x01, 0xFE, 0x00, 0x00, 0x03, 0xF8, 0x00, 0x00, 0x07, 0xE0, 0x00, 0x00,    // row 25 - 27
        0x0F, 0xC0, 0x00, 0x00, 0x0F, 0x80, 0x00, 0x00, 0x1F, 0x00, 0x00, 0x00,    // row 28 - 30
        0x1E, 0x00, 0x00, 0x00, 0x1E, 0x00, 0x00, 0x00, 0x3F, 0xFF, 0xFF, 0x80,    // row 31 - 33
        0x3F, 0xFF, 0xFF, 0x80, 0x3F, 0xFF, 0xFF, 0x80, 0x3F, 0xFF, 0xFF, 0x80,    // row 34 - 36
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,    // row 37 - 39
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,    // row 40 - 42
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,    // row 43 - 45
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00     // row 46 - 48
};
PROGMEM const unsigned char chr_f64_33[192] =         // 4 bytes per row
{
        0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x80, 0x00, 0x03, 0xFF, 0xF0, 0x00,    // row 1 - 3
        0x07, 0xFF, 0xFC, 0x00, 0x0F, 0xFF, 0xFC, 0x00, 0x1F, 0xC0, 0xFE, 0x00,    // row 4 - 6
        0x1F, 0x00, 0x3E, 0x00, 0x3E, 0x00, 0x1F, 0x00, 0x3E, 0x00, 0x1F, 0x00,    // row 7 - 9
        0x3C, 0x00, 0x0F, 0x00, 0x3C, 0x00, 0x0F, 0x00, 0x3C, 0x00, 0x0F, 0x00,    // row 10 - 12
        0x00, 0x00, 0x1F, 0x00, 0x00, 0x00, 0x1F, 0x00, 0x00, 0x00, 0x3E, 0x00,    // row 13 - 15
        0x00, 0x00, 0x7E, 0x00, 0x00, 0x3F, 0xFC, 0x00, 0x00, 0x3F, 0xF0, 0x00,    // row 16 - 18
        0x00, 0x3F, 0xFC, 0x00, 0x00, 0x3F, 0xFE, 0x00, 0x00, 0x00, 0x7F, 0x00,    // row 19 - 21
        0x00, 0x00, 0x1F, 0x00, 0x00, 0x00, 0x0F, 0x80, 0x00, 0x00, 0x0F, 0x80,    // row 22 - 24
        0x78, 0x00, 0x07, 0x80, 0x78, 0x00, 0x07, 0x80, 0x78, 0x00, 0x07, 0x80,    // row 25 - 27
        0x7C, 0x00, 0x0F, 0x80, 0x7C, 0x00, 0x1F, 0x80, 0x3E, 0x00, 0x1F, 0x00,    // row 28 - 30
        0x3F, 0x00, 0x3F, 0x00, 0x1F, 0xC0, 0xFE, 0x00, 0x0F, 0xFF, 0xFC, 0x00,    // row 31 - 33
        0x07, 0xFF, 0xF8, 0x00, 0x03, 0xFF, 0xF0, 0x00, 0x00, 0x7F, 0x80, 0x00,    // row 34 - 36
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,    // row 37 - 39
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,    // row 40 - 42
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,    // row 43 - 45
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00     // row 46 - 48
};
PROGMEM const unsigned char chr_f64_34[192] =         // 4 bytes per row
{
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x00,    // row 1 - 3
        0x00, 0x01, 0xF0, 0x00, 0x00, 0x03, 0xF0, 0x00, 0x00, 0x07, 0xF0, 0x00,    // row 4 - 6
        0x00, 0x07, 0xF0, 0x00, 0x00, 0x0F, 0xF0, 0x00, 0x00, 0x1E, 0xF0, 0x00,    // row 7 - 9
        0x00, 0x1E, 0xF0, 0x00, 0x00, 0x3C, 0xF0, 0x00, 0x00, 0x78, 0xF0, 0x00,    // row 10 - 12
        0x00, 0xF8, 0xF0, 0x00, 0x00, 0xF0, 0xF0, 0x00, 0x01, 0xE0, 0xF0, 0x00,    // row 13 - 15
        0x03, 0xC0, 0xF0, 0x00, 0x07, 0xC0, 0xF0, 0x00, 0x07, 0x80, 0xF0, 0x00,    // row 16 - 18
        0x0F, 0x00, 0xF0, 0x00, 0x1F, 0x00, 0xF0, 0x00, 0x1E, 0x00, 0xF0, 0x00,    // row 19 - 21
        0x3C, 0x00, 0xF0, 0x00, 0x78, 0x00, 0xF0, 0x00, 0x7F, 0xFF, 0xFF, 0x80,    // row 22 - 24
        0x7F, 0xFF, 0xFF, 0x80, 0x7F, 0xFF, 0xFF, 0x80, 0x7F, 0xFF, 0xFF, 0x80,    // row 25 - 27
        0x00, 0x00, 0xF0, 0x00, 0x00, 0x00, 0xF0, 0x00, 0x00, 0x00, 0xF0, 0x00,    // row 28 - 30
        0x00, 0x00, 0xF0, 0x00, 0x00, 0x00, 0xF0, 0x00, 0x00, 0x00, 0xF0, 0x00,    // row 31 - 33
        0x00, 0x00, 0xF0, 0x00, 0x00, 0x00, 0xF0, 0x00, 0x00, 0x00, 0xF0, 0x00,    // row 34 - 36
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,    // row 37 - 39
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,    // row 40 - 42
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,    // row 43 - 45
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00     // row 46 - 48
};
PROGMEM const unsigned char chr_f64_35[192] =         // 4 bytes per row
{
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xFF, 0xFE, 0x00,    // row 1 - 3
        0x07, 0xFF, 0xFE, 0x00, 0x07, 0xFF, 0xFE, 0x00, 0x07, 0xFF, 0xFE, 0x00,    // row 4 - 6
        0x07, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x00,    // row 7 - 9
        0x0F, 0x00, 0x00, 0x00, 0x0E, 0x00, 0x00, 0x00, 0x0E, 0x00, 0x00, 0x00,    // row 10 - 12
        0x0E, 0x00, 0x00, 0x00, 0x1E, 0x3F, 0xC0, 0x00, 0x1E, 0xFF, 0xF0, 0x00,    // row 13 - 15
        0x1F, 0xFF, 0xF8, 0x00, 0x1F, 0xFF, 0xFC, 0x00, 0x1F, 0xC0, 0xFE, 0x00,    // row 16 - 18
        0x1F, 0x00, 0x3F, 0x00, 0x1E, 0x00, 0x1F, 0x00, 0x00, 0x00, 0x0F, 0x00,    // row 19 - 21
        0x00, 0x00, 0x0F, 0x80, 0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 0x07, 0x80,    // row 22 - 24
        0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 0x07, 0x80,    // row 25 - 27
        0x3C, 0x00, 0x07, 0x80, 0x3C, 0x00, 0x0F, 0x80, 0x3C, 0x00, 0x0F, 0x00,    // row 28 - 30
        0x3E, 0x00, 0x1F, 0x00, 0x1F, 0x00, 0x3F, 0x00, 0x1F, 0xC0, 0xFE, 0x00,    // row 31 - 33
        0x0F, 0xFF, 0xFC, 0x00, 0x07, 0xFF, 0xF8, 0x00, 0x03, 0xFF, 0xF0, 0x00,    // row 34 - 36
        0x00, 0x7F, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,    // row 37 - 39
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,    // row 40 - 42
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,    // row 43 - 45
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00     // row 46 - 48
};
PROGMEM const unsigned char chr_f64_36[192] =         // 4 bytes per row
{
        0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0xFF, 0xF0, 0x00,    // row 1 - 3
        0x03, 0xFF, 0xF8, 0x00, 0x07, 0xFF, 0xFC, 0x00, 0x0F, 0xE0, 0x7E, 0x00,    // row 4 - 6
        0x1F, 0x80, 0x1F, 0x00, 0x1F, 0x00, 0x0F, 0x00, 0x1E, 0x00, 0x0F, 0x80,    // row 7 - 9
        0x3E, 0x00, 0x07, 0x80, 0x3C, 0x00, 0x07, 0x80, 0x3C, 0x00, 0x00, 0x00,    // row 10 - 12
        0x3C, 0x00, 0x00, 0x00, 0x78, 0x00, 0x00, 0x00, 0x78, 0x3F, 0x80, 0x00,    // row 13 - 15
        0x78, 0xFF, 0xF0, 0x00, 0x7B, 0xFF, 0xF8, 0x00, 0x7F, 0xFF, 0xFC, 0x00,    // row 16 - 18
        0x7F, 0xC0, 0xFE, 0x00, 0x7F, 0x00, 0x3E, 0x00, 0x7E, 0x00, 0x1F, 0x00,    // row 19 - 21
        0x7C, 0x00, 0x0F, 0x00, 0x7C, 0x00, 0x0F, 0x80, 0x78, 0x00, 0x07, 0x80,    // row 22 - 24
        0x78, 0x00, 0x07, 0x80, 0x78, 0x00, 0x07, 0x80, 0x78, 0x00, 0x07, 0x80,    // row 25 - 27
        0x78, 0x00, 0x07, 0x80, 0x3C, 0x00, 0x0F, 0x80, 0x3C, 0x00, 0x0F, 0x00,    // row 28 - 30
        0x3E, 0x00, 0x1F, 0x00, 0x1F, 0x00, 0x3F, 0x00, 0x1F, 0xC0, 0xFE, 0x00,    // row 31 - 33
        0x0F, 0xFF, 0xFC, 0x00, 0x07, 0xFF, 0xF8, 0x00, 0x01, 0xFF, 0xF0, 0x00,    // row 34 - 36
        0x00, 0x7F, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,    // row 37 - 39
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,    // row 40 - 42
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,    // row 43 - 45
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00     // row 46 - 48
};
PROGMEM const unsigned char chr_f64_37[192] =         // 4 bytes per row
{
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0xFF, 0xFF, 0x80,    // row 1 - 3
        0x3F, 0xFF, 0xFF, 0x80, 0x3F, 0xFF, 0xFF, 0x80, 0x3F, 0xFF, 0xFF, 0x80,    // row 4 - 6
        0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x00, 0x1E, 0x00,    // row 7 - 9
        0x00, 0x00, 0x3C, 0x00, 0x00, 0x00, 0x7C, 0x00, 0x00, 0x00, 0xF8, 0x00,    // row 10 - 12
        0x00, 0x01, 0xF0, 0x00, 0x00, 0x01, 0xE0, 0x00, 0x00, 0x03, 0xE0, 0x00,    // row 13 - 15
        0x00, 0x07, 0xC0, 0x00, 0x00, 0x07, 0xC0, 0x00, 0x00, 0x0F, 0x80, 0x00,    // row 16 - 18
        0x00, 0x0F, 0x00, 0x00, 0x00, 0x1F, 0x00, 0x00, 0x00, 0x1E, 0x00, 0x00,    // row 19 - 21
        0x00, 0x3E, 0x00, 0x00, 0x00, 0x3E, 0x00, 0x00, 0x00, 0x3C, 0x00, 0x00,    // row 22 - 24
        0x00, 0x7C, 0x00, 0x00, 0x00, 0x78, 0x00, 0x00, 0x00, 0x78, 0x00, 0x00,    // row 25 - 27
        0x00, 0xF8, 0x00, 0x00, 0x00, 0xF0, 0x00, 0x00, 0x00, 0xF0, 0x00, 0x00,    // row 28 - 30
        0x00, 0xF0, 0x00, 0x00, 0x00, 0xF0, 0x00, 0x00, 0x01, 0xF0, 0x00, 0x00,    // row 31 - 33
        0x01, 0xE0, 0x00, 0x00, 0x01, 0xE0, 0x00, 0x00, 0x01, 0xE0, 0x00, 0x00,    // row 34 - 36
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,    // row 37 - 39
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,    // row 40 - 42
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,    // row 43 - 45
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00     // row 46 - 48
};
PROGMEM const unsigned char chr_f64_38[192] =         // 4 bytes per row
{
        0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0x00, 0x00, 0x01, 0xFF, 0xE0, 0x00,    // row 1 - 3
        0x07, 0xFF, 0xF8, 0x00, 0x0F, 0xFF, 0xFC, 0x00, 0x1F, 0xC0, 0xFE, 0x00,    // row 4 - 6
        0x1F, 0x00, 0x3E, 0x00, 0x3E, 0x00, 0x1F, 0x00, 0x3E, 0x00, 0x1F, 0x00,    // row 7 - 9
        0x3C, 0x00, 0x0F, 0x00, 0x3C, 0x00, 0x0F, 0x00, 0x3C, 0x00, 0x0F, 0x00,    // row 10 - 12
        0x3E, 0x00, 0x1F, 0x00, 0x1E, 0x00, 0x1E, 0x00, 0x1F, 0x00, 0x3E, 0x00,    // row 13 - 15
        0x0F, 0xC0, 0xFC, 0x00, 0x07, 0xFF, 0xF8, 0x00, 0x03, 0xFF, 0xF0, 0x00,    // row 16 - 18
        0x07, 0xFF, 0xF8, 0x00, 0x0F, 0xFF, 0xFC, 0x00, 0x1F, 0x80, 0x7E, 0x00,    // row 19 - 21
        0x3E, 0x00, 0x1F, 0x00, 0x3C, 0x00, 0x0F, 0x00, 0x7C, 0x00, 0x0F, 0x80,    // row 22 - 24
        0x78, 0x00, 0x07, 0x80, 0x78, 0x00, 0x07, 0x80, 0x78, 0x00, 0x07, 0x80,    // row 25 - 27
        0x78, 0x00, 0x07, 0x80, 0x78, 0x00, 0x07, 0x80, 0x7C, 0x00, 0x0F, 0x80,    // row 28 - 30
        0x7C, 0x00, 0x0F, 0x80, 0x3E, 0x00, 0x1F, 0x00, 0x3F, 0x80, 0x7F, 0x00,    // row 31 - 33
        0x1F, 0xFF, 0xFE, 0x00, 0x0F, 0xFF, 0xFC, 0x00, 0x07, 0xFF, 0xF8, 0x00,    // row 34 - 36
        0x00, 0xFF, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,    // row 37 - 39
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,    // row 40 - 42
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,    // row 43 - 45
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00     // row 46 - 48
};
PROGMEM const unsigned char chr_f64_39[192] =         // 4 bytes per row
{
        0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x80, 0x00, 0x03, 0xFF, 0xE0, 0x00,    // row 1 - 3
        0x07, 0xFF, 0xF8, 0x00, 0x0F, 0xFF, 0xFC, 0x00, 0x1F, 0xC0, 0xFE, 0x00,    // row 4 - 6
        0x3F, 0x00, 0x3E, 0x00, 0x3E, 0x00, 0x1F, 0x00, 0x3C, 0x00, 0x0F, 0x00,    // row 7 - 9
        0x7C, 0x00, 0x0F, 0x00, 0x78, 0x00, 0x07, 0x80, 0x78, 0x00, 0x07, 0x80,    // row 10 - 12
        0x78, 0x00, 0x07, 0x80, 0x78, 0x00, 0x07, 0x80, 0x78, 0x00, 0x07, 0x80,    // row 13 - 15
        0x7C, 0x00, 0x0F, 0x80, 0x3C, 0x00, 0x0F, 0x80, 0x3E, 0x00, 0x1F, 0x80,    // row 16 - 18
        0x1F, 0x00, 0x3F, 0x80, 0x1F, 0xC0, 0xFF, 0x80, 0x0F, 0xFF, 0xFF, 0x80,    // row 19 - 21
        0x07, 0xFF, 0xF7, 0x80, 0x03, 0xFF, 0xC7, 0x80, 0x00, 0x7F, 0x07, 0x80,    // row 22 - 24
        0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x00, 0x0F, 0x00,    // row 25 - 27
        0x78, 0x00, 0x0F, 0x00, 0x78, 0x00, 0x1F, 0x00, 0x7C, 0x00, 0x1E, 0x00,    // row 28 - 30
        0x3C, 0x00, 0x3E, 0x00, 0x3E, 0x00, 0x7E, 0x00, 0x1F, 0x81, 0xFC, 0x00,    // row 31 - 33
        0x0F, 0xFF, 0xF8, 0x00, 0x07, 0xFF, 0xF0, 0x00, 0x03, 0xFF, 0xC0, 0x00,    // row 34 - 36
        0x00, 0x7F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,    // row 37 - 39
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,    // row 40 - 42
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,    // row 43 - 45
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00     // row 46 - 48
};
PROGMEM const unsigned char chr_f64_3A[144] =         // 3 bytes per row
{
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,    // row 1 - 4
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,    // row 5 - 8
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xC0, 0x00, 0x07, 0xC0, 0x00,    // row 9 - 12
        0x07, 0xC0, 0x00, 0x07, 0xC0, 0x00, 0x07, 0xC0, 0x00, 0x00, 0x00, 0x00,    // row 13 - 16
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,    // row 17 - 20
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,    // row 21 - 24
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xC0, 0x00,    // row 25 - 28
        0x07, 0xC0, 0x00, 0x07, 0xC0, 0x00, 0x07, 0xC0, 0x00, 0x07, 0xC0, 0x00,    // row 29 - 32
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,    // row 33 - 36
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,    // row 37 - 40
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,    // row 41 - 44
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00     // row 45 - 48
};
PROGMEM const unsigned char chr_f64_61[192] =         // 4 bytes per row
{
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,    // row 1 - 3
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,    // row 4 - 6
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,    // row 7 - 9
        0x00, 0x7F, 0xC0, 0x00, 0x01, 0xFF, 0xF8, 0x00, 0x07, 0xFF, 0xFC, 0x00,    // row 10 - 12
        0x07, 0xFF, 0xFE, 0x00, 0x0F, 0xC0, 0x7E, 0x00, 0x1F, 0x00, 0x1F, 0x00,    // row 13 - 15
        0x1E, 0x00, 0x0F, 0x00, 0x1E, 0x00, 0x0F, 0x00, 0x1E, 0x00, 0x0F, 0x00,    // row 16 - 18
        0x00, 0x00, 0x0F, 0x00, 0x00, 0x00, 0x1F, 0x00, 0x00, 0x0F, 0xFF, 0x00,    // row 19 - 21
        0x01, 0xFF, 0xFF, 0x00, 0x07, 0xFF, 0xFF, 0x00, 0x0F, 0xFF, 0xCF, 0x00,    // row 22 - 24
        0x1F, 0xF0, 0x0F, 0x00, 0x1F, 0x00, 0x0F, 0x00, 0x3E, 0x00, 0x0F, 0x00,    // row 25 - 27
        0x3C, 0x00, 0x0F, 0x00, 0x3C, 0x00, 0x0F, 0x00, 0x3C, 0x00, 0x1F, 0x00,    // row 28 - 30
        0x3C, 0x00, 0x3F, 0x00, 0x3E, 0x00, 0x7F, 0x00, 0x1F, 0x01, 0xFF, 0xC0,    // row 31 - 33
        0x1F, 0xFF, 0xE7, 0xC0, 0x0F, 0xFF, 0xC7, 0xC0, 0x07, 0xFF, 0x03, 0xC0,    // row 34 - 36
        0x01, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,    // row 37 - 39
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,    // row 40 - 42
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,    // row 43 - 45
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00     // row 46 - 48
};
PROGMEM const unsigned char chr_f64_6D[288] =         // 6 bytes per row
{
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,    // row 1 - 2
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,    // row 3 - 4
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,    // row 5 - 6
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,    // row 7 - 8
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xE0, 0x1F, 0xC0, 0x00,    // row 9 - 10
        0x1E, 0x3F, 0xF0, 0x7F, 0xF0, 0x00, 0x1E, 0xFF, 0xF8, 0xFF, 0xF8, 0x00,    // row 11 - 12
        0x1E, 0xFF, 0xFD, 0xFF, 0xFC, 0x00, 0x1F, 0xE0, 0x7F, 0xE0, 0x7C, 0x00,    // row 13 - 14
        0x1F, 0x80, 0x3F, 0x80, 0x3E, 0x00, 0x1F, 0x00, 0x1F, 0x00, 0x1E, 0x00,    // row 15 - 16
        0x1F, 0x00, 0x1F, 0x00, 0x1E, 0x00, 0x1E, 0x00, 0x1E, 0x00, 0x1E, 0x00,    // row 17 - 18
        0x1E, 0x00, 0x1E, 0x00, 0x1E, 0x00, 0x1E, 0x00, 0x1E, 0x00, 0x1E, 0x00,    // row 19 - 20
        0x1E, 0x00, 0x1E, 0x00, 0x1E, 0x00, 0x1E, 0x00, 0x1E, 0x00, 0x1E, 0x00,    // row 21 - 22
        0x1E, 0x00, 0x1E, 0x00, 0x1E, 0x00, 0x1E, 0x00, 0x1E, 0x00, 0x1E, 0x00,    // row 23 - 24
        0x1E, 0x00, 0x1E, 0x00, 0x1E, 0x00, 0x1E, 0x00, 0x1E, 0x00, 0x1E, 0x00,    // row 25 - 26
        0x1E, 0x00, 0x1E, 0x00, 0x1E, 0x00, 0x1E, 0x00, 0x1E, 0x00, 0x1E, 0x00,    // row 27 - 28
        0x1E, 0x00, 0x1E, 0x00, 0x1E, 0x00, 0x1E, 0x00, 0x1E, 0x00, 0x1E, 0x00,    // row 29 - 30
        0x1E, 0x00, 0x1E, 0x00, 0x1E, 0x00, 0x1E, 0x00, 0x1E, 0x00, 0x1E, 0x00,    // row 31 - 32
        0x1E, 0x00, 0x1E, 0x00, 0x1E, 0x00, 0x1E, 0x00, 0x1E, 0x00, 0x1E, 0x00,    // row 33 - 34
        0x1E, 0x00, 0x1E, 0x00, 0x1E, 0x00, 0x1E, 0x00, 0x1E, 0x00, 0x1E, 0x00,    // row 35 - 36
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,    // row 37 - 38
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,    // row 39 - 40
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,    // row 41 - 42
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,    // row 43 - 44
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,    // row 45 - 46
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00     // row 47 - 48
};
PROGMEM const unsigned char chr_f64_70[192] =         // 4 bytes per row
{
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,    // row 1 - 3
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,    // row 4 - 6
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,    // row 7 - 9
        0x00, 0x0F, 0xE0, 0x00, 0x1E, 0x3F, 0xFC, 0x00, 0x1E, 0x7F, 0xFE, 0x00,    // row 10 - 12
        0x1E, 0xFF, 0xFF, 0x00, 0x1F, 0xF0, 0x3F, 0x80, 0x1F, 0xC0, 0x0F, 0x80,    // row 13 - 15
        0x1F, 0x80, 0x07, 0xC0, 0x1F, 0x00, 0x03, 0xC0, 0x1F, 0x00, 0x03, 0xC0,    // row 16 - 18
        0x1F, 0x00, 0x03, 0xE0, 0x1E, 0x00, 0x01, 0xE0, 0x1E, 0x00, 0x01, 0xE0,    // row 19 - 21
        0x1E, 0x00, 0x01, 0xE0, 0x1E, 0x00, 0x01, 0xE0, 0x1E, 0x00, 0x01, 0xE0,    // row 22 - 24
        0x1E, 0x00, 0x01, 0xE0, 0x1E, 0x00, 0x01, 0xE0, 0x1E, 0x00, 0x01, 0xE0,    // row 25 - 27
        0x1E, 0x00, 0x03, 0xE0, 0x1F, 0x00, 0x03, 0xC0, 0x1F, 0x00, 0x07, 0xC0,    // row 28 - 30
        0x1F, 0x80, 0x07, 0xC0, 0x1F, 0xC0, 0x0F, 0x80, 0x1F, 0xF0, 0x3F, 0x80,    // row 31 - 33
        0x1E, 0xFF, 0xFF, 0x00, 0x1E, 0x7F, 0xFE, 0x00, 0x1E, 0x3F, 0xFC, 0x00,    // row 34 - 36
        0x1E, 0x0F, 0xE0, 0x00, 0x1E, 0x00, 0x00, 0x00, 0x1E, 0x00, 0x00, 0x00,    // row 37 - 39
        0x1E, 0x00, 0x00, 0x00, 0x1E, 0x00, 0x00, 0x00, 0x1E, 0x00, 0x00, 0x00,    // row 40 - 42
        0x1E, 0x00, 0x00, 0x00, 0x1E, 0x00, 0x00, 0x00, 0x1E, 0x00, 0x00, 0x00,    // row 43 - 45
        0x1E, 0x00, 0x00, 0x00, 0x1E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00     // row 46 - 48
};

PROGMEM const unsigned char * const chrtbl_f64[96] =        // character pointer table
{
        chr_f64_20, chr_f64_20, chr_f64_20, chr_f64_20, chr_f64_20, chr_f64_20, chr_f64_20, chr_f64_20, 
        chr_f64_20, chr_f64_20, chr_f64_20, chr_f64_20, chr_f64_20, chr_f64_2D, chr_f64_2E, chr_f64_20, 
        chr_f64_30, chr_f64_31, chr_f64_32, chr_f64_33, chr_f64_34, chr_f64_35, chr_f64_36, chr_f64_37, 
        chr_f64_38, chr_f64_39, chr_f64_3A, chr_f64_20, chr_f64_20, chr_f64_20, chr_f64_20, chr_f64_20,
        chr_f64_20, chr_f64_20, chr_f64_20, chr_f64_20, chr_f64_20, chr_f64_20, chr_f64_20, chr_f64_20,
        chr_f64_20, chr_f64_20, chr_f64_20, chr_f64_20, chr_f64_20, chr_f64_20, chr_f64_20, chr_f64_20,
        chr_f64_20, chr_f64_20, chr_f64_20, chr_f64_20, chr_f64_20, chr_f64_20, chr_f64_20, chr_f64_20,
        chr_f64_20, chr_f64_20, chr_f64_20, chr_f64_20, chr_f64_20, chr_f64_20, chr_f64_20, chr_f64_20,
        chr_f64_20, chr_f64_61, chr_f64_20, chr_f64_20, chr_f64_20, chr_f64_20, chr_f64_20, chr_f64_20, 
        chr_f64_20, chr_f64_20, chr_f64_20, chr_f64_20, chr_f64_20, chr_f64_6D, chr_f64_20, chr_f64_20, 
        chr_f64_70, chr_f64_20, chr_f64_20, chr_f64_20, chr_f64_20, chr_f64_20, chr_f64_20, chr_f64_20,
        chr_f64_20, chr_f64_20, chr_f64_20, chr_f64_20, chr_f64_20, chr_f64_20, chr_f64_20, chr_f64_20
};
