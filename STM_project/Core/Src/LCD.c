/**
*	@author Dominik Luczak
*	@date 2015
*	@details LCD driver HD44780 in 4 bits mode. Before use initialize GPIO.
*/

#include "LCD.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdint.h>
#include <ctype.h>

/**
* Software busy delay
* @param[in] tick Number of ticks to wait
*/
static void software_delay(uint32_t tick)
{
	uint32_t delay;
	while(tick-->0)
	{
		for(delay=5; delay>0; delay--){
			asm("nop");
			asm("nop");
			asm("nop");
			asm("nop");
		}
	}
}

/**
* Initialization of LCD display in 4 bits mode
* @details Before use initialize GPIOs
*/
void LCD_init(void)
{
	software_delay(1000000);
	LCD_send_4bits(0x03,0,0);	software_delay(1000000);
	LCD_send_4bits(0x03,0,0);	software_delay(1000000);
	LCD_send_4bits(0x03,0,0);	software_delay(400000);
	//Set 4-bit
	LCD_send_4bits(0x02,0,0);	software_delay(400000);

	//Function SET
	LCD_write_command(LCD_FUNCTION_INSTRUCTION | LCD_FUNCTION_DL_4BIT | LCD_FUNCTION_LINE_NUMBER_2 | LCD_FUNCTION_FONT_5x8); software_delay(50000);
	//Display on
	LCD_write_command(LCD_DISPLAY_INSTRUCTION | LCD_DISPLAY_ON | LCD_DISPLAY_CURSOR_OFF | LCD_DISPLAY_BLINK_OFF);software_delay(100000);
	//Display clear
	LCD_write_command(LCD_CLEAR_INSTRUCTION);software_delay(100000);
	//Entry mode
	LCD_write_command(LCD_ENTRY_MODE_INSTRUCTION | LCD_ENTRY_MODE_INCREMENT | LCD_ENTRY_MODE_SHIFT_DISPLAY_OFF);software_delay(100000);
	//Init end

	//Return home
	LCD_write_command(LCD_HOME_INSTRUCTION);	software_delay(100000);
	
	LCD_write_text("SM - lab");
	LCD_goto_line(1);
	LCD_write_text("Projekt PID");

	uint8_t custom_char1[] = LCD_CUSTOM_CHAR_ARROW_UP_PATERN;
	LCD_create_custom_character(custom_char1, 0);
	uint8_t custom_char2[] = LCD_CUSTOM_CHAR_ARROW_DOWN_PATERN;
	LCD_create_custom_character(custom_char2, 1);
	uint8_t custom_char3[] = LCD_CUSTOM_CHAR_ARROW_OUT_PATERN;
	LCD_create_custom_character(custom_char3, 2);
	uint8_t custom_char4[] = LCD_CUSTOM_CHAR_ARROW_INTO_PATERN;
	LCD_create_custom_character(custom_char4, 3);
	uint8_t custom_char5[] = LCD_CUSTOM_CHAR_ARROW_ENTER_PATERN;
	LCD_create_custom_character(custom_char5, 4);
	uint8_t custom_char6[] = LCD_CUSTOM_CHAR_ARROW_PLUS_MINUS_PATERN;
	LCD_create_custom_character(custom_char6, 5);

	uint8_t custom_char_7[] = LCD_CUSTOM_CHAR_DEGREE_PATTERN;
	LCD_create_custom_character(custom_char_7, 6);


}


void LCD_send_4bits(uint8_t data_to_send, char RS, char RW)
{
	LCD_GPIO_SET_VALUE(LCD_GPIO_RS_Pin, RS, LCD_GPIO_RS_Port);
	//set RW to LOW (GND) by hardware

	if(data_to_send&(0x01<<0)){LCD_DATABIT_ON(4);}else{LCD_DATABIT_OFF(4);}
	if(data_to_send&(0x01<<1)){LCD_DATABIT_ON(5);}else{LCD_DATABIT_OFF(5);}
	if(data_to_send&(0x01<<2)){LCD_DATABIT_ON(6);}else{LCD_DATABIT_OFF(6);}
	if(data_to_send&(0x01<<3)){LCD_DATABIT_ON(7);}else{LCD_DATABIT_OFF(7);}
	software_delay(100);
	
	LCD_GPIO_ON(LCD_GPIO_E_Pin, LCD_GPIO_E_Port);	software_delay(100);
	LCD_GPIO_OFF(LCD_GPIO_E_Pin, LCD_GPIO_E_Port);	software_delay(100);
	LCD_GPIO_ON(LCD_GPIO_E_Pin, LCD_GPIO_E_Port);	software_delay(1000);
}

void LCD_send_8bits_twice_4bits(uint8_t data, char RS, char RW)
{
	LCD_send_4bits((data>>4), RS, RW);	//high part
	LCD_send_4bits(data, RS, RW);		//low part
}

void LCD_write_command(uint8_t command)
{
	LCD_send_8bits_twice_4bits(command, 0, 0);
	software_delay(10000);
}

void LCD_write_data(char byte_data)
{
	LCD_send_8bits_twice_4bits(byte_data, 1, 0);
}

void LCD_write_char(char character)
{
	if(isprint(character))	LCD_write_data(character);
}



void LCD_write_text(char* pText){
	while(*pText!='\0')
	{
		LCD_write_char(*pText);
		pText++;
	}
}

void LCD_goto_xy(uint8_t line, uint8_t y)
{
	switch(line){
		case 0: line=0x00; break;
		case 1: line=0x40; break;
		default: line=0;
	}
	LCD_write_command(LCD_DDRAM_ADDRESS | (line+y));
}

void LCD_goto_line(uint8_t line)
{
	LCD_goto_xy(line, 0);
}

void LCD_printf_line(const char * format, ... )
{
	#define LCD_BUFFER_SIZE (LCD_MAXIMUM_LINE_LENGTH+1)
	char text_buffer[LCD_BUFFER_SIZE];
	uint8_t length=0;
	va_list args;
	va_start (args, format);
	length=vsnprintf(text_buffer, LCD_BUFFER_SIZE, format, args);
	LCD_write_text(text_buffer);
	va_end (args);
	if(length>=1 && length<LCD_MAXIMUM_LINE_LENGTH){
		snprintf(text_buffer, LCD_BUFFER_SIZE, "%*c", LCD_MAXIMUM_LINE_LENGTH-length, ' ');
		LCD_write_text(text_buffer);
	}
}

uint8_t LCD_printf(const char * format, ... )
{
	#define LCD_BUFFER_SIZE (LCD_MAXIMUM_LINE_LENGTH+1)
	char text_buffer[LCD_BUFFER_SIZE];
	uint8_t length=0;
	va_list args;
	va_start (args, format);
	length=vsnprintf(text_buffer, LCD_BUFFER_SIZE, format, args);
	LCD_write_text(text_buffer);
	va_end (args);
	return length;
}


void LCD_create_custom_character(uint8_t* pPattern, uint8_t position)
{
	LCD_write_command(LCD_CGRAM_ADDRESS | (position*8));
	for (uint8_t i=0; i<8; i++)
		LCD_write_data(pPattern[i]);
}




