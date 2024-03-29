#define F_CPU 16000000UL
#define __UBRR_VALUE(USART_BAUDRATE) ((int)  ((((F_CPU) / ((double)(16UL))) + ((double)(USART_BAUDRATE) / ((double)(2UL)))) / (double)(USART_BAUDRATE)-((double)(1UL))))

/*
inline __attribute__((always_inline)) int calculate_UBRR() {
	return 123;
}
*/
/*
UBRRL = (uint8_t)( (F_CPU + BAUD_RATE * 4L) / (BAUD_RATE * 8L) - 1 );

  UBRRH = (((F_CPU/BAUD_RATE)/16)-1)>>8; 	// set baud rate
  UBRRL = (((F_CPU/BAUD_RATE)/16)-1);

https://github.com/search?q=repo%3Aarduino%2FArduinoCore-avr+UBRRL&type=code
*/


// _Static_assert does not work with in
#define UBRR_VALUE(USART_BAUDRATE) ({  \
	_Static_assert(USART_BAUDRATE <= 115200, "Cannot larger than 115200"); \
	__UBRR_VALUE(USART_BAUDRATE); \
	\
})

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

