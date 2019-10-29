/*
 * LCD.h
 *
 *  Created on: Sep 30, 2019
 *      Author: Luke McDaniel
 */

#ifndef LCD_H_
#define LCD_H_

void nibble(uint8_t command);
void LCD_init(void);
void Write_char_LCD(char character);
void Clear_LCD(void);
void Home_LCD(void);
void Write_string_LCD(char *str);
void helloworld(void);

#endif /* LCD_H_ */
