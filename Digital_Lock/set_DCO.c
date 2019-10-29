/*
 * set_DCO.c
 *
 *  Created on: Sep 27, 2019
 *      Author: Luke McDaniel
 */

#include "msp.h"
#include <stdint.h>

void set_DCO(int freq)
{
    // unlocks board for edit
    CS->KEY = CS_KEY_VAL;
    // clear CTL0 register
    CS->CTL0 = 0;
    // enable use of DCO
    CS->CTL0 = CS_CTL0_DCOEN;

    // nominal frequency of 1.5 MHz selected
    if (freq == 1) {
        CS->CTL0 |= CS_CTL0_DCORSEL_0;
    }
    // nominal frequency of 3 MHz selected
    else if (freq == 3) {
        CS->CTL0 |= CS_CTL0_DCORSEL_1;
    }
    // nominal frequency of 6 MHz selected
    else if (freq == 6) {
        CS->CTL0 |= CS_CTL0_DCORSEL_2;
    }
    // nominal frequency of 12 MHz selected
    else if (freq == 12) {
        CS->CTL0 |= CS_CTL0_DCORSEL_3;
    }
    // nominal frequency of 24 MHz selected
    else if (freq == 24) {
        CS->CTL0 |= CS_CTL0_DCORSEL_4;
    }
    // nominal frequency of 48 MHz selected
    else if (freq == 48) {
        // Transition to VCORE Level 1: AM0_LDO --> AM1_LDO
        while ((PCM->CTL1 & PCM_CTL1_PMR_BUSY));
            PCM->CTL0 = PCM_CTL0_KEY_VAL | PCM_CTL0_AMR_1;
        while ((PCM->CTL1 & PCM_CTL1_PMR_BUSY));

        /* Configure Flash wait-state to 1 for both banks 0 & 1 */
        FLCTL->BANK0_RDCTL = (FLCTL->BANK0_RDCTL & ~(FLCTL_BANK0_RDCTL_WAIT_MASK)) | FLCTL_BANK0_RDCTL_WAIT_1;
        FLCTL->BANK1_RDCTL  = (FLCTL->BANK0_RDCTL & ~(FLCTL_BANK1_RDCTL_WAIT_MASK)) | FLCTL_BANK1_RDCTL_WAIT_1;

        CS->CTL0 |= CS_CTL0_DCORSEL_5;
    }
    // default to 1.5 MHz if a wrong value is input
    else {
        // error use nominal DCO frequency
        CS->CTL0 |= CS_CTL0_DCORSEL_0;
    }

    // sets DCOCLK to source of MCLK
    CS->CTL1 |= CS_CTL1_SELM__DCOCLK;
}
