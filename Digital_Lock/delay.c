/*
 * delay.c
 *
 *  Created on: Sep 27, 2019
 *      Author: Luke McDaniel
 */

#include "msp.h"
#include <stdint.h>

#define MHZ15 0x00
#define MHZ3 0x10000
#define MHZ6 0x20000
#define MHZ12 0x30000
#define MHZ24 0x40000
#define MHZ48 0x50000
#define FOR0 0x70000


int delay_us(int microsec)
{
    int bit_freq = CS->CTL0;
    int freq = 0;
    int i = 0;
    int cycles;

    // 1.5 MHz
    if ((bit_freq & FOR0) == MHZ15) {
        freq = 15;
        cycles = (((microsec / freq)*3000)/1380) - 18;
    }
    // 3 MHz
    else if ((bit_freq & FOR0) == MHZ3) {
        freq = 30;
        cycles = (((microsec*3000 / freq)/1000));
    }
    // 6 MHz
    else if ((bit_freq & FOR0) == MHZ6) {
        freq = 60;
        cycles = (((microsec*32000 / freq)/1000));
    }
    // 12 MHz
    else if ((bit_freq & FOR0) == MHZ12) {
        freq = 120;
        cycles = (((microsec*105000 / freq)/1000));
    }
    // 24 MHz
    else if ((bit_freq & FOR0) == MHZ24) {
        freq = 240;
        cycles = (((microsec*5300 / freq)/10));
    }
    // 48 MHz
    else if ((bit_freq & FOR0) == MHZ48) {
        freq = 480;
        cycles = (((microsec*19000 / freq)/10));
    }
    else {
        // error return -1
        return -1;
    }



    for (i = 0; i < cycles; i++);

    return 0;
}
