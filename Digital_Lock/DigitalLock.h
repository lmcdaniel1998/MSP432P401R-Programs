/*
 * DigitalLock.h
 *
 *  Created on: Oct 7, 2019
 *      Author: Luke McDaniel
 */

#ifndef DIGITALLOCK_H_
#define DIGITALLOCK_H_

extern char CODE[4] = "3691";

void writeLocked(void);
void writeEnterKey(void);
void writeUnlocked(void);
void Lock_init(void);
void LCD_first(int count);
char* EnterCode(void);

#endif /* DIGITALLOCK_H_ */
