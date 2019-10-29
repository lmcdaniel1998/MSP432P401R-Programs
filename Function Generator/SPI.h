/*
 * SPI.h
 *
 *  Created on: Oct 22, 2019
 *      Author: type
 */

#ifndef SPI_H_
#define SPI_H_

void SPI_init(void);
void SPI_out(void);
void passVars(int waveType, int dutyCycle, int freq);
void TimerA_init(void);
void Data_init(void);

#endif /* SPI_H_ */
