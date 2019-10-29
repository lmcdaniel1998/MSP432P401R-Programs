/*
 * LCD.c
 *
 *  Created on: Sep 30, 2019
 *      Author: Luke McDaniel
 */

#include "msp.h"
#include "set_DCO.h"
#include "delay.h"
#include <stdint.h>

#define EN BIT0
#define RW BIT5
#define RS BIT7

void nibble(uint8_t command) {
    P2->OUT &= 0x0F;
    P2->OUT |= command;
    P3->OUT |= EN;
    delay_us(0);
    P3->OUT &= ~EN;
    delay_us(37);
}

void LCD_init(void) {

        uint8_t command;

        set_DCO(24);                            // set clock frequency to 24 MHz
        delay_us(40000);                        // wait for 40 ms after power is applied

        // setup GPIO
        P2->DIR = 0xFF;
        P3->DIR |= (EN | RS | RW);
        P3->OUT &= ~(EN | RS | RW);

        command = 0x30;                         // first function set (one part)
        nibble(command);

        command = 0x28;                         // second function set (two parts)
        nibble(command);
        command = command << 4;
        nibble(command);

        command = 0x28;                         // third function set (two parts)
        nibble(command);
        command = command << 4;
        nibble(command);

        command = 0x0F;                         // Display ON
        nibble(command);
        command = command << 4;
        nibble(command);

        command = 0x01;                         // Display Clear
        nibble(command);
        command = command << 4;
        nibble(command);
        delay_us(1550);

        command = 0x06;                         // Entry Mode Set
        nibble(command);
        command = command << 4;
        nibble(command);
}

void Write_char_LCD(char character) {
    uint8_t command;
    command = (int)character;
    P3->OUT |= RS;
    nibble(command);
    command = command << 4;
    nibble(command);
}

void Clear_LCD(void) {
    // Display Clear
    uint8_t command;
    P3->OUT &= ~RS;
    command = 0x01;
    nibble(command);
    command = command << 4;
    nibble(command);
    delay_us(1560);
}

void Write_string_LCD(char *str) {
    int i = 0;
    while (str[i] != '\0') {
        Write_char_LCD(str[i]);
        i++;
    }
}

void Home_LCD(void) {
    // Display Clear
     uint8_t command;
     P3->OUT &= ~RS;
     command = 0x02;
     nibble(command);
     command = command << 4;
     nibble(command);
     delay_us(1560);
}

void helloworld(void) {
    int i;
    Write_char_LCD('H');
    Write_char_LCD('E');
    Write_char_LCD('L');
    Write_char_LCD('L');
    Write_char_LCD('O');
    Write_char_LCD(' ');
    Write_char_LCD('W');
    Write_char_LCD('O');
    Write_char_LCD('R');
    Write_char_LCD('L');
    Write_char_LCD('D');
    for(i = 0; i < 29; i++)
    {
        Write_char_LCD(' ');
    }
    Write_char_LCD('A');
    Write_char_LCD('S');
    Write_char_LCD('S');
    Write_char_LCD('I');
    Write_char_LCD('G');
    Write_char_LCD('N');
    Write_char_LCD('M');
    Write_char_LCD('E');
    Write_char_LCD('N');
    Write_char_LCD('T');
    Write_char_LCD(' ');
    Write_char_LCD('3');
}
