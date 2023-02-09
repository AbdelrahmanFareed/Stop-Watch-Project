#include<avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h>

unsigned char sec = 0 ;
unsigned char min = 0 ;
unsigned char hou= 0 ;
unsigned char tick = 0 ;

void INT0_Init(void)
{
	GICR |= (1<<INT0);
	MCUCR |=(1<<ISC01);
	DDRD &= ~(1<<PD2);
	PORTD |=(1<<PD2);
}
void INT1_Init(void)
{
	GICR |= (1<<INT1);
	MCUCR =(1<<ISC11) |(1<<ISC10);
	DDRD &= ~(1<<PD3);

}
void INT2_Init(void)
{
	GICR |= (1<<INT2);
	MCUCSR &= ~(1<<ISC01);
	DDRB &= ~(1<<PB2);
	PORTB |=(1<<PB2);
}
void Timer1_CTC_Init(unsigned short num_of_ticks )
{
	TCNT1 = 0 ; 		   					     /* Timer Initial Value */
	OCR1A = num_of_ticks ;						 /* Compare Value */
	TCCR1A = (1<<FOC1A) | (1<<FOC1B); 			 /* Non PWM , No Need For OC1A , OC1B */
	TIMSK |= (1<<OCIE1A) ;       				 /* output compare interrupt enable for channel A of Timer One */
	TCCR1B = (1<<CS10)|(1<<CS12)|(1<<WGM12) ;    /* Prescaler 1024 , ctc mode */
}
ISR(TIMER1_COMPA_vect)
{
	tick=1;
}
ISR(INT0_vect)
{
	sec=0;
	min=0;
	hou=0;
}
ISR(INT1_vect)
{
	/*stop the clock*/
	TCCR1B &= ~(1<<0) ;
	TCCR1B &= ~(1<<1) ;
	TCCR1B &= ~(1<<2) ;
}
ISR(INT2_vect)
{
	/*start the clock*/
	TCCR1B = (1<<CS10)|(1<<CS12)|(1<<WGM12) ;

}
int main(void)
{
	SREG |=(1<<7);
	DDRA |= 0x7F;
	PORTA |= 0x7F;
	DDRC |=0x0F;
	PORTC &= 0xF0 ;
	INT0_Init();
	INT1_Init();
	INT2_Init();
	Timer1_CTC_Init(976);
	while(1)
	{
		/* display seconds */
		PORTA  |= (1<<5) ;
		PORTC = (PORTC & 0xF0) | (sec % 10) ;
		_delay_ms(5);
		PORTA  |= (1<<4) ;
		PORTC = (PORTC & 0xF0) | (sec / 10) ;
		_delay_ms(5) ;
		/* display minutes */
		PORTA  |= (1<<3) ;
		PORTC = (PORTC & 0xF0) | (min % 10) ;
		_delay_ms(5);
		PORTA  |= (1<<2) ;
		PORTC = (PORTC & 0xF0) | (min / 10) ;
		_delay_ms(5) ;
		/* display hours */
		PORTA  |= (1<<1) ;
		PORTC = (PORTC & 0xF0) | (hou % 10) ;
		_delay_ms(5);
		PORTA  |= (1<<0) ;
		PORTC = (PORTC & 0xF0) | (hou / 10) ;
		_delay_ms(5) ;

		if( tick==1)
		{
			sec++ ;
			if(sec == 60 )
			{
				sec = 0 ;
				min++ ;
			}
			if(min == 60 )
			{
				min = 0 ;
				hou++ ;
			}
			if(hou == 24)
			{
				hou = 0 ;
			}
			tick=0;


		}
	}
}



