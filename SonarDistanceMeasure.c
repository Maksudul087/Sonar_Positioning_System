/*
 * SonarDistanceMeasure.c
 *
 * Created: 01/10/2018 1:22:23 AM
 *  Author: maksudul_mimon
 */ 



#undef F_CPU
#define F_CPU 1000000
#define D4 eS_PORTB4
#define D5 eS_PORTB5
#define D6 eS_PORTB6
#define D7 eS_PORTB7
#define RS eS_PORTB0
#define EN eS_PORTB1

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>
#include <stdlib.h>
#include "lcd.h" /* Include LCD header file */




static volatile int pulse0 = 0;
static volatile int i0 = 0;

static volatile int pulse1 = 0;
static volatile int i1 = 0;
int main(void)
{
	DDRA = 0xFF;
	DDRB = 0xFF;
	DDRD = 0b1110011;
	DDRC = 0b11111011;
	_delay_ms(50);
	
	GICR|=(1<<INT0);
	MCUCR|=(1<<ISC00);
	
	GICR|=(1<<INT1);
	MCUCR|=(1<<ISC10);
	
	TCCR1A = 0;
	
	int16_t COUNTA0 = 0;
	char SHOWA0 [16];
	
	int16_t COUNTA1 = 0;
	char SHOWA1 [16];
	
	
	
	Lcd4_Init();
	Lcd4_Clear();
	
	
	
	while(1)
	{
		sei();
		
		PORTD|=(1<<PIND0);
		_delay_us(15);
		PORTD &=~(1<<PIND0);
		
		PORTC|=(1<<PINC0);
		_delay_us(15);
		PORTC &=~(1<<PINC0);
		
		COUNTA0 = pulse0/58;
		
		COUNTA1 = pulse1/58;
		
		DDRA = 0x00;
		
		Lcd4_Set_Cursor(1,0);
		Lcd4_Write_String ("DISTANCE=");

		if(COUNTA0 > 5 && COUNTA0<=25){
			DDRA =0xFF;
			if(COUNTA0<=10 && COUNTA0>=5){
				PORTA = 0b11111001;
			}
			else if(COUNTA0<=15){
				PORTA = 0b11110101;
			}
			else if(COUNTA0<=20){
				PORTA = 0b11101101;
			}
			else if(COUNTA0<=25){
				PORTA = 0b11011101;
			}
			else{
				PORTA = 0b11111100;
			}
			
			dtostrf(COUNTA0, 2, 2, SHOWA0);
			strcat(SHOWA0,"cm ");
			Lcd4_Set_Cursor(1,10);
			Lcd4_Write_String(SHOWA0);
			Lcd4_Set_Cursor(2,6);
			Lcd4_Write_String ("sonar 0!");
			
		}
		else if(COUNTA1 > 5 && COUNTA1<=25){
				DDRA = 0xFF;
			if(COUNTA1<=10 && COUNTA0>=5){
				PORTA = 0b11111010;
			}
			else if(COUNTA1<=15){
				PORTA = 0b11110110;
			}
			else if(COUNTA1<=20){
				PORTA = 0b11101110;
			}
			else if(COUNTA1<=25){
				PORTA = 0b11011110;
			}
			else{
				PORTA = 0b11111100;
			}
			
		dtostrf(COUNTA1, 2, 2, SHOWA1);
		strcat(SHOWA1,"cm ");
		Lcd4_Set_Cursor(1,10);
		Lcd4_Write_String(SHOWA1);
		Lcd4_Set_Cursor(2,6);
		Lcd4_Write_String ("sonar 1!");
		}
		
		
		cli();
	}
}
ISR(INT0_vect)
{
	if (i0==1)
	{
		TCCR1B=0;
		pulse0=TCNT1;
		TCNT1=0;
		i0=0;
	}
	if (i0==0)
	{
		TCCR1B|=(1<<CS10);
		i0=1;
	}
}

ISR(INT1_vect)
{
	if (i1==1)
	{
		TCCR1B=0;
		pulse1=TCNT1;
		TCNT1=0;
		i1=0;
	}
	if (i1==0)
	{
		TCCR1B|=(1<<CS10);
		i1=1;
	}
}
