#include "msp.h"
#include "Keypad.h"
#include "LCD.h"
#include "DigitalLock.h"
#include <stdlib.h>
#include <stdint.h>


/**
 * main.c
 */
void main(void)
{
    P1->SEL0 &= BIT0;       // P1.0 set as GPIO
    P1->SEL1 &= BIT0;
    P1->DIR |= BIT0;
    P1->OUT &= ~(BIT0);
	//  WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
    char *theCode;
    theCode = malloc(sizeof(int) * 4);
    int x = 0;
    int i = 0;
    int flag = 0;
    int correctness = 0;
    uint8_t unlock = 0x00;
    while(1) {
        P1->OUT |= BIT0;
        unlock = 0x00;
        flag = 0;
        correctness = 0;
        i = 0;
        x = 0;
        LCD_init();
        Lock_init();                                            // initialize lock
        while (correctness == 0) {
            theCode = EnterCode();
            flag = 0;
            for (x = 0; x < 4; x++) {
                if (theCode[x] != CODE[x]) {
                    flag = 1;
                }
            }
            if (flag == 1) {
                Clear_LCD();
                writeLocked();                                      // write "LOCKED" to screen
                for (i = 0; i < 34; i++) {                          // go to next line
                    Write_char_LCD(' ');
                }
                writeEnterKey();                                    // write "ENTER KEY" to screen
                correctness = 0;
            }
            else {
                correctness = 1;
            }
        }
        P1->OUT &= ~(BIT0);
        Clear_LCD();
        writeUnlocked();
        unlock = detectPress();
        while(unlock != 0x0B) {
            unlock = detectPress();
        }
    }
}
