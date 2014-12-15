/*
 * tabaco.c
 *
 * Created: 01/05/2013 11:49:34
 * Author: Cesar Sanz
 */ 


#include <avr/io.h>
#include <avr/interrupt.h>

#define DDR_SALIDA       DDRD
#define PUERTO_SALIDA    PORTD
#define PIN_SALIDA	     PIND0

int main(void)
{
	//Configuracion de puertos
	DDR_SALIDA|=(1<<PIN_SALIDA);
	
	//Configuracion del temporizador
	TCCR1B |= (1 << WGM12); // Configurar timer 1 para modo CTC
	TIMSK |= (1 << OCIE1A); // Activar interrupcion CTC
	OCR1A = 58594; // Cuando el timer llege a este valor, salta la interrupcion. Velocidad: 1MHz/1024
	TCCR1B |= ((1 << CS10) | (0 << CS11) | (1 << CS12)); // Preescalado Fcpu/1024
	
	//Inicializacion interrupciones externas
	GICR |= (1<<INT0) | (1<<INT1);  //Habilitamos las interrupciones en INT0 e INT1
	MCUCR |= (0<<ISC11) | (1<<ISC10) | (0<<ISC01) | (1<<ISC00); //La interrupcion se genera con cualquier flanco, ascendente o descendente
	sei(); //  Activar interrupciones globales
	
    while(1)
    {     	 
    }
}

ISR(TIMER1_COMPA_vect)
{
	//En esta interrupcion se entra cada 60 segundos
	PUERTO_SALIDA&=~(1<<PIN_SALIDA);
}

ISR(INT0_vect)
{
	//Boton abrir
	PUERTO_SALIDA|=(1<<PIN_SALIDA);
	TCNT1=0;  //Reiniciar el timer
}

ISR(INT1_vect)
{
	//Boton cerrar
	PUERTO_SALIDA&=~(1<<PIN_SALIDA);
}		