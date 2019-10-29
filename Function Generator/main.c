#include "msp.h"
#include "Keypad.h"
#include "SPI.h"
#include "set_DCO.h"
#include "delay.h"

#define TRIWAVE 6
#define SQRWAVE 7
#define SAWWAVE 8
#define SINWAVE 9

typedef enum {Sqr, Sin, Saw, Tri} state_type;

/**
 * main.c
 */

void main(void)
{
	uint8_t key;
	uint8_t useful_key;

	int waveType = SQRWAVE;                         // can be square, sawtooth, or sine wave
	int dutyCycle = 5;                              // can be 0 - 9
	int freq = 1;                                   // can be 1 - 5

	CS->CTL1 |= CS_CTL1_SELS_3;                     // sets DCOCLK to source of SMCLK
	set_DCO(12);                                    // set DCOCLK frequency to 12 MHz
	Data_init();                                    // initialize data for waveforms
	passVars(waveType, dutyCycle, freq);
	keypad_init();                                  // initialize keypad
	SPI_init();            // start with 100 Hz square wave 50% duty cycle
	delay_us(1000);
	TimerA_init();
	uint8_t state = Sqr;
	while(1){                                       // keep all of main in one infinite loop
	    switch(state) {
	    case Sqr :
	        key = detectPress();                       // call first detect
	        while (key == 0xFF) {                       // while there is no key pressed keep waiting for one to be pressed
	            key = detectPress();
	        }
	        useful_key = key;                           // save key value after key is pressed
	        if (useful_key == 0x01) {
	            freq = 1;
	        }
	        else if (useful_key == 0x02) {
	            freq = 2;
	        }
	        else if (useful_key == 0x03) {
	            freq = 3;
            }
	        else if (useful_key == 0x04) {
                freq = 4;
	        }
	        else if (useful_key == 0x05) {
                freq = 5;
            }
	        else if (useful_key == SQRWAVE) {
                waveType = SQRWAVE;
                dutyCycle = 5;
                freq = 1;
                state = Sqr;
            }
	        else if (useful_key == SAWWAVE) {
	            waveType = SAWWAVE;
	            dutyCycle = 5;
	            freq = 1;
                state = Saw;
            }
            else if (useful_key == SINWAVE) {
                waveType = SINWAVE;
                dutyCycle = 5;
                freq = 1;
                state = Sin;
            }
            else if (useful_key == TRIWAVE) {
                waveType = TRIWAVE;
                dutyCycle = 5;
                freq = 1;
                state = Tri;
            }
	        else if (useful_key == 0x0A) {
	            if (dutyCycle > 1) {            // min duty cycle 10%
	                dutyCycle--;
	            }
            }
            else if (useful_key == 0x0B) {
                if (dutyCycle < 9) {            // max duty cycle 90%
                    dutyCycle++;
                }
            }
            else if (useful_key == 0x00) {
                dutyCycle = 5;
            }
	        passVars(waveType, dutyCycle, freq);
	        while (key == useful_key){                      // keep looking for new key
                key = detectPress();
            }
	        break;
	    case Saw :
	        key = detectPress();                       // call first detect
            while (key == 0xFF) {                       // while there is no key pressed keep waiting for one to be pressed
                key = detectPress();
            }
            useful_key = key;                           // save key value after key is pressed
	        if (useful_key == 0x01) {
                freq = 1;
            }
            else if (useful_key == 0x02) {
                freq = 2;
            }
            else if (useful_key == 0x03) {
                freq = 3;
            }
            else if (useful_key == 0x04) {
                freq = 4;
            }
            else if (useful_key == 0x05) {
                freq = 5;
            }
            else if (useful_key == SQRWAVE) {
                waveType = SQRWAVE;
                dutyCycle = 5;
                freq = 1;
                state = Sqr;
            }
            else if (useful_key == SAWWAVE) {
                waveType = SAWWAVE;
                dutyCycle = 5;
                freq = 1;
                state = Saw;
            }
            else if (useful_key == SINWAVE) {
                waveType = SINWAVE;
                dutyCycle = 5;
                freq = 1;
                state = Sin;
            }
            else if (useful_key == TRIWAVE) {
                waveType = TRIWAVE;
                dutyCycle = 5;
                freq = 1;
                state = Tri;
            }
	        passVars(waveType, dutyCycle, freq);
            break;
	    case Sin :
	        key = detectPress();                       // call first detect
            while (key == 0xFF) {                       // while there is no key pressed keep waiting for one to be pressed
                key = detectPress();
            }
            useful_key = key;                           // save key value after key is pressed
            if (useful_key == 0x01) {
                freq = 1;
            }
            else if (useful_key == 0x02) {
                freq = 2;
            }
            else if (useful_key == 0x03) {
                freq = 3;
            }
            else if (useful_key == 0x04) {
                freq = 4;
            }
            else if (useful_key == 0x05) {
                freq = 5;
            }
            else if (useful_key == SQRWAVE) {
                waveType = SQRWAVE;
                dutyCycle = 5;
                freq = 1;
                state = Sqr;
            }
            else if (useful_key == SAWWAVE) {
                waveType = SAWWAVE;
                dutyCycle = 5;
                freq = 1;
                state = Saw;
            }
            else if (useful_key == SINWAVE) {
                waveType = SINWAVE;
                dutyCycle = 5;
                freq = 1;
                state = Sin;
            }
            else if (useful_key == TRIWAVE) {
                waveType = TRIWAVE;
                dutyCycle = 5;
                freq = 1;
                state = Tri;
            }
            passVars(waveType, dutyCycle, freq);
            break;
	    case Tri :
            key = detectPress();                       // call first detect
            while (key == 0xFF) {                       // while there is no key pressed keep waiting for one to be pressed
                key = detectPress();
            }
            useful_key = key;                           // save key value after key is pressed
            if (useful_key == 0x01) {
                freq = 1;
            }
            else if (useful_key == 0x02) {
                freq = 2;
            }
            else if (useful_key == 0x03) {
                freq = 3;
            }
            else if (useful_key == 0x04) {
                freq = 4;
            }
            else if (useful_key == 0x05) {
                freq = 5;
            }
            else if (useful_key == SQRWAVE) {
                waveType = SQRWAVE;
                dutyCycle = 5;
                freq = 1;
                state = Sqr;
            }
            else if (useful_key == SAWWAVE) {
                waveType = SAWWAVE;
                dutyCycle = 5;
                freq = 1;
                state = Saw;
            }
            else if (useful_key == SINWAVE) {
                waveType = SINWAVE;
                dutyCycle = 5;
                freq = 1;
                state = Sin;
            }
            else if (useful_key == TRIWAVE) {
                waveType = TRIWAVE;
                dutyCycle = 5;
                freq = 1;
                state = Tri;
            }
            passVars(waveType, dutyCycle, freq);
            break;
	    }
    }
}
