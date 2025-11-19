#ifdef __AVR_ATmega328P__
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#define F_CPU 16000000UL

#define NORMAL_MODE_VALUE(timer_bit, n_seconds, prescaler) ((int)(((1UL) << (timer_bit)) - ((n_seconds) * ((F_CPU) / (prescaler)))))
#define CTC_MODE_VALUE(n_seconds, prescaler) ((int)(((n_seconds) * ((F_CPU) / (prescaler))) - (1UL)))

/* 
https://github.com/arduino/ArduinoCore-avr/blob/87faf934a742fd6aa9fc269c99de5d529363f204/bootloaders/atmega/ATmegaBOOT_168.c#L375C1-L384C7

#elif defined(__AVR_ATmega168__) || defined(__AVR_ATmega328P__) || defined (__AVR_ATmega328__)

#ifdef DOUBLE_SPEED
	UCSR0A = (1<<U2X0); //!!!!!!! Double speed mode USART0
	UBRR0L = (uint8_t)(F_CPU/(BAUD_RATE*8L)-1);
	UBRR0H = (F_CPU/(BAUD_RATE*8L)-1) >> 8;
#else
	UBRR0L = (uint8_t)(F_CPU/(BAUD_RATE*16L)-1);
	UBRR0H = (F_CPU/(BAUD_RATE*16L)-1) >> 8;
#endif
*/

#define UBRR_VALUE_LOW_SPEED(UART_BAUDRATE) ((unsigned char)(((F_CPU)/((UART_BAUDRATE) * (16UL)))-((double)(1UL))))
#define UBRR_VALUE_DOUBLE_SPEED(UART_BAUDRATE) ((unsigned char)(((F_CPU)/((UART_BAUDRATE) * (8L)))-((double)(1UL))))

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
#include "gtest/gtest.h"
using ::testing::InitGoogleTest;

// Demonstrate some basic assertions.
TEST(MyTest, BasicAssertions) {
//   // Expect two strings not to be equal.
//   EXPECT_STRNE("hello", "world");
//   // Expect equality.
//   EXPECT_EQ(7 * 6, 42);
	EXPECT_EQ(UBRR_VALUE(9600), 103);
	EXPECT_EQ(UBRR_VALUE(4800), 207);
}


int main(int argc, char** argv) {
  	InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

#endif

