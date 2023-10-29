/*
 * main.c
 *
 *  Created on: Oct 20, 2023
 *      Author: AHMED
 */

#include <stdbool.h>
#include <avr/delay.h>

#include "LIB/STD_TYPES.h"

#include "MCAL/DIO/DIO_Interface.h"
#include "MCAL/Port/Port_Interface.h"

#include "HAL/LCD2/LCD_Interface.h"

#define HAZL2OM 	{0b01110,0b01110,0b00100,0b11111,0b00100,0b01010,0b10010,0b10010}
#define PAD1		{0b11000,0b11000,0b11000,0b11000,0b11000,0b11000,0b11000,0b11000}
#define PAD2		{0b00011,0b00011,0b00011,0b00011,0b00011,0b00011,0b00011,0b00011}
#define BALL 		{0b00000,0b00000,0b01110,0b01110,0b01110,0b01110,0b00000,0b00000}


#define END 	(0x80 + 19)
#define START 	(0x80)


u8 Pad1x = 1, Pad2x= 1, Ballx = 1, Bally = (0x80 + 10);

void MoveBall()
{
	u8 PressedButton = 1;
	u8 ReadValue;
	DIO_GetPinValue(DIO_u8PORTD, DIO_u8PIN_5,&ReadValue);

	while(  0 == ReadValue )
	{
		DIO_GetPinValue(DIO_u8PORTD, DIO_u8PIN_5,&ReadValue);

		if( PressedButton == 1 )
		{
			Ballx++;
			PressedButton = 0;
			if (Ballx > 3)
			{
				Ballx=1;
			}
		}
	}

}
void MovePad()
{

	u8 PressedButton = 1;
	u8 ReadValue;
	DIO_GetPinValue(DIO_u8PORTD, DIO_u8PIN_7,&ReadValue);

	while(  0 == ReadValue )
	{
		DIO_GetPinValue(DIO_u8PORTD, DIO_u8PIN_7,&ReadValue);

		if( PressedButton == 1 )
		{
			Pad1x++;
			PressedButton = 0;
			if (Pad1x > 3)
			{
				Pad1x=1;
			}
		}
	}

	PressedButton = 1;

	DIO_GetPinValue(DIO_u8PORTD, DIO_u8PIN_6,&ReadValue);

	while(  0 == ReadValue )
	{
		DIO_GetPinValue(DIO_u8PORTD, DIO_u8PIN_6,&ReadValue);

		if( PressedButton == 1 )
		{
			Pad2x++;
			PressedButton = 0;
			if (Pad2x > 3)
			{
				Pad2x=1;
			}
		}
	}

}

void Winner()
{
	LCD_voidSendCommand(LCD_ClearDisplay);
	LCD_voidGoToPosition(1,5);
	LCD_voidWriteString((u8*)"Winner!!!");
	LCD_voidGoToPosition(2,5);

}


int main(void)
{
	PORT_voidInit();
	LCD_voidInit();

	//u8 Arr_Hazl2om[8] = HAZL2OM;
	u8 Arr_Pad1[8] = PAD1;
	u8 Arr_Pad2[8] = PAD2;
	u8 Arr_Ball[8] = BALL;
	u8 Player1[] = "Ahmed  ";
	u8 Player2[] = "Hazim  ";
	u8 MoveLeft = 1, Player1Score = 0, Player2Score = 0;
	//LCD_voidDisplaySpecialChar(Arr_Hazl2om,0,0,0);
	//	LCD_voidDisplaySpecialChar(Arr_Pad,1,0,0);
	//	LCD_voidDisplaySpecialChar(Arr_Ball,0,1,0);


	while(1)
	{
		LCD_voidSendCommand(LCD_ClearDisplay);
		LCD_voidGoToPosition(0,0);
		LCD_voidWriteString(Player1);
		LCD_voidWriteIntData(Player1Score);
		LCD_voidGoToPosition(0,10);
		LCD_voidWriteString(Player2);
		LCD_voidWriteIntData(Player2Score);
		LCD_voidDisplaySpecialChar(Arr_Pad1,0, Pad1x, START);
		LCD_voidDisplaySpecialChar(Arr_Pad2,2, Pad2x, END);
		LCD_voidDisplaySpecialChar(Arr_Ball,1, Ballx, Bally);

		if(MoveLeft)
		{
			Bally--;
			if(START == Bally)
			{
				if(Ballx == Pad1x)
				{
					Bally += 2;
					MoveLeft = 0;
				}
				else
				{
					Player2Score++;
					DIO_u8SetPinValue(DIO_u8PORTC, DIO_u8PIN_6, DIO_u8PIN_HIGH);// buzzer
					if(Player2Score == 3)
					{
						Winner();
						LCD_voidWriteString(Player2);
						_delay_ms(2000);
						Player2Score = 0;
						Player1Score = 0;
					}
					Ballx = 2;
					Bally = (0x80 + 10);
				}
			}
		}
		else
		{
			Bally++;
			if(END == Bally)
			{
				if(Ballx == Pad2x)
				{
					Bally -= 2;
					MoveLeft = 1;
				}
				else
				{
					Player1Score++;
					DIO_u8SetPinValue(DIO_u8PORTC, DIO_u8PIN_6, DIO_u8PIN_HIGH);
					if(Player1Score == 3)
					{
						Winner();
						LCD_voidWriteString(Player1);
						_delay_ms(2000);
						Player1Score = 0;
						Player2Score = 0;
					}
					Ballx = 2;
					Bally = (0x80 + 10);

				}
			}
		}

		MovePad();
		MoveBall();
		_delay_ms(350);
		DIO_u8SetPinValue(DIO_u8PORTC, DIO_u8PIN_6, DIO_u8PIN_LOW);
	}

	return 0;
}



