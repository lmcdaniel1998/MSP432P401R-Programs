/*
 * DigitalLock.c
 *
 *  Created on: Oct 7, 2019
 *      Author: Luke McDaniel
 */

#include "msp.h"
#include "Keypad.h"
#include "LCD.h"
#include <stdlib.h>
#include <stdint.h>

void writeLocked(void) {
    Write_char_LCD('L');
    Write_char_LCD('O');
    Write_char_LCD('C');
    Write_char_LCD('K');
    Write_char_LCD('E');
    Write_char_LCD('D');
}

void writeEnterKey(void) {
    Write_char_LCD('E');
    Write_char_LCD('N');
    Write_char_LCD('T');
    Write_char_LCD('E');
    Write_char_LCD('R');
    Write_char_LCD(' ');
    Write_char_LCD('K');
    Write_char_LCD('E');
    Write_char_LCD('Y');
}

void writeUnlocked(void) {
    Write_char_LCD('U');
    Write_char_LCD('N');
    Write_char_LCD('L');
    Write_char_LCD('O');
    Write_char_LCD('C');
    Write_char_LCD('K');
    Write_char_LCD('E');
    Write_char_LCD('D');
}

void Lock_init(void) {
    int i = 0;
    keypad_init();                                      // initialize keypad
    LCD_init();                                         // initialize LCD
    Clear_LCD();

    writeLocked();                                      // write "LOCKED" to screen
    for (i = 0; i < 34; i++) {                          // go to next line
       Write_char_LCD(' ');
    }
    writeEnterKey();                                    // write "ENTER KEY" to screen
}

void LCD_first(int count) {
    int i = 0;
    if (count == 1) {
        Clear_LCD();
        writeLocked();                                  // write "LOCKED" to screen
        for (i = 0; i < 34; i++) {                      // go to next line
            Write_char_LCD(' ');
        }
    }
}

char* EnterCode(void) {
    char *enteredCode;                           // char array to store entered code
    enteredCode = malloc(sizeof(int) * 4);
    int codeCount = 0;                                  // count how many characters are entered
    int i = 0;
    uint8_t key;
    uint8_t useful_key;
    while(codeCount < 4) {                              // get key presses until 4 digits are entered
        //while(1){                                       // let user enter a 4 digit code
            key = detectPress();                        // call first detect
            while (key == 0xFF) {                       // while there is no key pressed keep waiting for one to be pressed
                key = detectPress();
            }
            useful_key = key;                           // save key value after key is pressed
            // LCD decoder
            switch(useful_key) {
            case 0x00 :
                enteredCode[codeCount] = '0';
                codeCount++;
                LCD_first(codeCount);
                break;
            case 0x01 :
                enteredCode[codeCount] = '1';
                codeCount++;
                LCD_first(codeCount);
                break;
            case 0x02 :
                enteredCode[codeCount] = '2';
                codeCount++;
                LCD_first(codeCount);
                break;
            case 0x03 :
                enteredCode[codeCount] = '3';
                codeCount++;
                LCD_first(codeCount);
                break;
            case 0x04 :
                enteredCode[codeCount] = '4';
                codeCount++;
                LCD_first(codeCount);
                break;
            case 0x05 :
                enteredCode[codeCount] = '5';
                codeCount++;
                LCD_first(codeCount);
                break;
            case 0x06 :
                enteredCode[codeCount] = '6';
                codeCount++;
                LCD_first(codeCount);
                break;
            case 0x07 :
                enteredCode[codeCount] = '7';
                codeCount++;
                LCD_first(codeCount);
                break;
            case 0x08 :
                enteredCode[codeCount] = '8';
                codeCount++;
                LCD_first(codeCount);
                break;
            case 0x09 :
                enteredCode[codeCount] = '9';
                codeCount++;
                LCD_first(codeCount);
                break;
            case 0x0A :
                // Write_char_LCD('*');                             // * is pressed
                Clear_LCD();                                        // clear LCD
                writeLocked();                                      // write "LOCKED" to screen
                for (i = 0; i < 34; i++) {                          // go to next line
                    Write_char_LCD(' ');
                }
                writeEnterKey();                                    // write "ENTER KEY" to screen
                for(i = 0; i < 4; i++) {                            // clear code string
                    enteredCode[i] = ' ';
                }
                codeCount = 0;
                break;
            case 0x0B :
                // Write_char_LCD('#');                             // skip this key
                Clear_LCD();                                        // clear LCD
                writeLocked();                                      // write "LOCKED" to screen
                for (i = 0; i < 34; i++) {                          // go to next line
                    Write_char_LCD(' ');
                }
                writeEnterKey();                                    // write "ENTER KEY" to screen
                for(i = 0; i < 4; i++) {                            // clear code string
                    enteredCode[i] = ' ';
                }
                codeCount = 0;
                break;
            }
            while (key == useful_key){                              // keep looking for new key
                key = detectPress();
            }
            if (useful_key != 0x0A && useful_key != 0x0B) {
                Write_char_LCD(enteredCode[(codeCount - 1)]);
            }
        //}
    }
    return enteredCode;
}
