/*
 * Keypad.c
 *
 *  Created on: Oct 4, 2019
 *      Author: Luke McDaniel
 */

#include "msp.h"
#include <stdint.h>
#include "delay.h"

#define COL1 BIT0
#define COL2 BIT1
#define COL3 BIT2
#define ROW1 BIT3
#define ROW2 BIT2
#define ROW3 BIT1
#define ROW4 BIT0

void keypad_init(void) {
    // (4.3 - 4.0) : rows      (5.0 - 5.2) : columns
    P5->DIR |= (COL1 | COL2 | COL3);                    // set P5 as output
    P5->OUT &= ~(COL1 | COL2 | COL3);                   // set as 0
    P4->DIR &= ~(ROW1 | ROW2 | ROW3 | ROW4);            // set P4 as input
    P4->REN |= (ROW1 | ROW2 | ROW3 | ROW4);             // enable resistors
    P4->OUT &= ~(ROW1 | ROW2 | ROW3 | ROW4);            // specify as pull-down resistors
}

uint8_t detectPress(void) {
    int col;
    uint8_t rows;
    for (col = 0; col < 3; col++) {                     // cycle through columns
        P5->OUT &= ~(COL1 | COL2 | COL3);               // reset all ports
        P5->OUT |= (COL1 << col);                       // shift 1 bit over
        __delay_cycles(80);                             // let outputs settle
        rows = P4->IN & (ROW1 | ROW2 | ROW3 | ROW4);    // read input from rows
        if (rows != 0) {
            break;                                      // break out of loop if button press is detected
        }
    }
    switch(col) {
    case 3 :                                            // no button press detected
        return 0xFF;                                    // return all 1's
    case 2 :                                            // third column is high
        switch(rows) {
        case ROW1 :
            return 0x03;
        case ROW2 :
            return 0x06;
        case ROW3 :
            return 0x09;
        case ROW4 :
            return 0x0B;
        }
    case 1 :                                            // second column is high
        switch(rows) {
        case ROW1 :
            return 0x02;
        case ROW2 :
            return 0x05;
        case ROW3 :
            return 0x08;
        case ROW4 :
            return 0x00;
        }
    case 0 :                                            // first column is high
        switch(rows) {
        case ROW1 :
            return 0x01;
        case ROW2 :
            return 0x04;
        case ROW3 :
            return 0x07;
        case ROW4 :
            return 0x0A;
        }
    }
}
