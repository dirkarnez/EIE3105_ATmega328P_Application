#define F_CPU 16000000UL
#define UBRR_VALUE(USART_BAUDRATE) ((int)(((F_CPU / ((double)(USART_BAUDRATE) * (double)(16UL))) - (double)(1UL))))

#ifdef __AVR_ATmega328P__
#include <avr/io.h>

void Timer_1_Delay();		// Prototype for Delay Function 

int main(void)
{
    DDRB = 1<<0;			// PB0 as Output
	PORTB = 1<<0;			// Initial LOW
    while (1)				// INF Loop
    {
		//Timer_1_Delay();	// Call 1 s Delay
		// PORTB ^= (1<<0);	// Toggle
		PORTB = 0xFF;
    }
}

void Timer_1_Delay()
{
// Timer 1 16 bit timer with prescaler = 1024
// 1 * 16M / 1024 = 15625
	OCR1AH = 0x3D;		// CTC OCR
	OCR1AL = 0x08;		// 15625 - 1 = 0x3D08
	TCCR1A = 0x00;		// CTC Mode
	TCCR1B = 0x0D;		// CTC Mode, Prescaler = 1024
	while ((TIFR1 &(1<<OCF1A))==0);	// Continue Until Overflow
	TCCR1B = 0x00;		// Stop Timer 1
	TIFR1 = (1<<OCF1A);	// Reset OCR1A
}

#else
#include <stdio.h>
#include <gtest/gtest.h>

int main () {
	EXPECT_EQ(UBRR_VALUE(9600), 103);
	EXPECT_EQ(UBRR_VALUE(4800), 207);
}
#endif
