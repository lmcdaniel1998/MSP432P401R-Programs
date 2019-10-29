/*
 * SPI.c
 *
 *  Created on: Oct 22, 2019
 *      Author: Luke McDaniel
 */

#include "msp.h"
#include <math.h>

#define TRIWAVE 6
#define SQRWAVE 7
#define SAWWAVE 8
#define SINWAVE 9
#define PI 3.14159265

#define GAIN BIT5
#define SHWON BIT4
#define sqThreeVolt 3750
#define swThreeVolt 3475
#define threeVolt 1732
#define zeroVolt 0

#define SQR_lOOHZ 12080
#define SAW_lOOHZ 1030
#define SIN_lOOHZ 1030
#define TRI_lOOHZ 1030

int gWaveType;
int gDutyCycle;
int gFreq;
int gFreqTune;
int gFreqB = 0;

int squareArray[2];
int sawArray[116];
int sinArray[115];
int triArray[116];

uint16_t outVolt;
int out;

void SPI_init(void) {
    // SPI stuff
    P1-> SEL0 |= (BIT5 | BIT6);
    P1-> SEL1 &= ~(BIT5 | BIT6);
    P1-> DIR |= (BIT5 | BIT6);
    P1-> OUT |= (BIT5 | BIT6);
    //__enable_irq();                                     // enable interrupts
    EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_SWRST;             // Put eUSCI state machine in reset
    //                keep in reset,        manager mode,       synchronous mode,    set clock polarity,  specify msb in first
    EUSCI_B0->CTLW0 = EUSCI_B_CTLW0_SWRST | EUSCI_B_CTLW0_MST | EUSCI_B_CTLW0_SYNC | EUSCI_B_CTLW0_CKPL | EUSCI_B_CTLW0_MSB;
    EUSCI_B0->CTLW0 |= EUSCI_A_CTLW0_UCSSEL_2;          // use SMCLK for SPI
    EUSCI_B0->CTLW0 &= ~EUSCI_B_CTLW0_SWRST;            // Initialize USCI state machine
    //NVIC->ISER[0] = 1 << (EUSCIB0_IRQn & 31);           // set interrupt register for EUSCIB0
    //while(1) {
        //EUSCI_B0->IFG |= EUSCI_B_IFG_TXIFG;             // Clear TXIFG flag
        //EUSCI_B0->IE |= EUSCI_B_IE_TXIE;                // Enable TX interrupt
    //}
}

void passVars(int waveType, int dutyCycle, int freq) {
    gWaveType = waveType;
    gDutyCycle = dutyCycle;
    gFreq = freq;
}

void TimerA_init(void) {
    TIMER_A0 -> CCR[0]= 0;                              // timer will automatically trigger
    TIMER_A0 -> CCTL[0]= TIMER_A_CCTLN_CCIE;            // capture compare mode
    //              select SMCLK,           use continuous mode
    TIMER_A0-> CTL= TIMER_A_CTL_TASSEL_2 | TIMER_A_CTL_MC_2;
    NVIC-> ISER[0] = 1 << (TA0_0_IRQn & 0x1F);          // set interrupt register for Timer_A0
    __enable_irq();
}

void Data_init(void) {
    int i;
    double sinVal;
    int sawVal = 0;
    for (i = 0; i < 115; i++) {                          // populate sin array with values
        sinVal = (threeVolt * sin(((2 * PI) / 116) * i) + 1732);
        sinArray[i] = (int)sinVal;
    }

    for (i = 0; i < 58; i++) {                          // populate tri array with values
        triArray[i] = sawVal;
        sawVal += 30;
    }
    for (i = 58; i < 116; i++) {
        triArray[i] = sawVal;
        sawVal -= 30;
    }

    sawVal = 0;
    for (i = 0; i < 116; i++) {                          // populate saw array with values
            sawArray[i] = sawVal;
            sawVal += 30;
        }
    sawArray[115] = swThreeVolt;

    squareArray[0] = zeroVolt;                          // populate square array with values
    squareArray[1] = sqThreeVolt;
}

void SPI_out(void) {
    uint8_t loByte;
    uint8_t hiByte;
    loByte = outVolt & 0xFF;                            // create low byte
    hiByte = (outVolt >> 8) & 0x0F;                     // shift outVolt over to create high byte
    hiByte |= (GAIN | SHWON);                           // set gain to 1 and don't shut down
    // use port 3.0 as CS for DAC
    P3->DIR |= BIT0;
    P3->OUT &= ~BIT0;                                   // turn CS off
    EUSCI_B0->TXBUF = hiByte;                           // send high byte
    while (!(EUSCI_B0->IFG & EUSCI_B_IFG_TXIFG));       // wait for high byte to be transmitted
    EUSCI_B0->TXBUF = loByte;                           // send low byte
    while (!(EUSCI_B0->IFG & EUSCI_B_IFG_RXIFG));       // wait for low byte to be transmitted
    P3->OUT |= BIT0;                                    // turn on CS to signal full 12 bits have been transmitted
}

void TA0_0_IRQHandler(void) {
    static int out = 0;
    static int frequency;
    TIMER_A0->CCTL[0] &= ~(TIMER_A_CCTLN_CCIFG);        // clear timer flag

    if (gWaveType == SQRWAVE) {
        frequency = SQR_lOOHZ / gFreq;
        if (out < gDutyCycle) {                         // square wave high
            outVolt = squareArray[1];
            SPI_out();
            out++;
        }
        else if(out < 9) {                              // square wave low
            outVolt = squareArray[0];
            SPI_out();
            out++;
        }
        else {
            out = 0;
        }
        TIMER_A0 -> CCR[0]+= frequency;
    }

    if (gWaveType == SAWWAVE) {
        gFreqB = gFreq;
        frequency = SAW_lOOHZ;
        if (out < 116) {
            outVolt = sawArray[out];                     // out saw wave
            SPI_out();
            out += gFreq;
        }
        else {
            out = 0;
        }
        TIMER_A0 -> CCR[0]+= frequency;
    }
    if (gWaveType == SINWAVE) {
        frequency = SIN_lOOHZ;
        if (out < 115) {
            outVolt = sinArray[out];                    // out sin wave
            SPI_out();
            out += gFreq;
        }
        else {
            out = 0;
        }
        TIMER_A0 -> CCR[0]+= frequency;
    }
    if (gWaveType == TRIWAVE) {
        frequency = TRI_lOOHZ;
        if (out < 115) {
            outVolt = triArray[out];                    // out tri wave
            SPI_out();
            out += gFreq;
        }
        else {
            out = 0;
        }
        TIMER_A0 -> CCR[0]+= frequency;
    }
}
