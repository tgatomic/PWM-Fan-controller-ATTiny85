/*
 * PWM_Code.c
 *
 * Created: 2017-02-21 10:06:14
 * Author : Atomic
 */ 

 #define F_CPU 8000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

volatile uint8_t PWM_val;
volatile uint8_t step;
volatile uint8_t button_pressed;

#define TRUE 1
#define FALSE 0

int main(void)
{
    /* Initiate the PWM timer */
    DDRB |= (1<<PB0) | (1<<PB4);;
    TCCR0A |= (1 << FOC0A) | (1 << WGM01) | (1 << WGM00);
    TCCR0B |= 1 << CS00;
    TCNT0 = 0;
    OCR0A = 0;

    /* Initiate the interrupt */
    MCUCR |= (1<<ISC01) | (1<<ISC00);     // Set the interrupt to rising edge

    GIMSK |= (1<<INT0); // Enable the interrupt
    sei();              // Enable global interrupt

    /* Set variables */
    button_pressed = FALSE;
    step = 1; // 1 so we end upp in case 1 first buttonpress

    /* Initiate fan */
    OCR0A = 100;
    _delay_ms(250);
    OCR0A = 40;

    while (1) 
    {
        if (button_pressed){

            switch (step)
            {
                case 0:
                    OCR0A = 40;
                    step++;
                    PORTB &= ~(1<<PB4);
                    break;
                case 1:
                    OCR0A = 100;
                    step++;
                    break;
                case 2:
                    OCR0A = 150;
                    step++;
                    break;
                case 3:
                    OCR0A = 200;
                    step++;
                    break;
                case 4:
                    OCR0A = 250;
                    step = 0;
                    PORTB |= (1<<PB4);
                    break;
            }
            _delay_ms(250);
            button_pressed = FALSE;
        }
    }
}

ISR(INT0_vect){
    if (button_pressed == FALSE)
    {
        button_pressed = TRUE;
    }
}

