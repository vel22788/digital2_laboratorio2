/*
 * main.c
 *
 * Created: 7/31/2024 5:53:37 PM
 *  Author: Luis Angel Velasquez
 */ 

#define F_CPU 16000000
#include <avr/io.h>
#include <xc.h>
#include <util/delay.h>
#include <stdio.h>
#include "BITS_8_LCD/BITS_8_LCD.h"
#include "UART/UART.h"
#include "ADC/ADC.h"

int main(void) {
	// Configuración de ADC
	DDRC &= ~((1 << DDC1) | (1 << DDC0));
	PORTC |= (1 << PORTC1) | (1 << PORTC0);

	// Iniciar LCD, Adc y UART
	init_ADC(0, 0, 128);
	initUART(9600, 0, 0, 0, 0, 1, 8, 103);
	init_8BITS_LCD();

	char buffer[16];
	int adc_value1, adc_value2;
	int voltage1, voltage2;
	int counter = 0;
	
	while (1) {
		adc_value1 = readADC(0);  // Canal 0
		adc_value2 = readADC(1);  // Canal 1
		
		voltage1 = adc_value1 * (5.0 / 255.0) * 100;
		voltage2 = adc_value2 * (5.0 / 255.0) * 100;
		
		LCD_Set_Cursor(1, 1);
		LCD_TEXT("S1");
		LCD_Set_Cursor(1, 2);
		snprintf(buffer, sizeof(buffer), " %d.%.02d ", voltage1 / 100, voltage1 % 100);
		LCD_TEXT(buffer);
		writeUART_Text("V1");
		writeUART_Text(buffer);

		// Mostrar voltaje2 en el LCD
		LCD_Set_Cursor(7, 1);
		LCD_TEXT("S2");
		LCD_Set_Cursor(7, 2);
		snprintf(buffer, sizeof(buffer), " %d.%.02d ", voltage2 / 100, voltage2 % 100);
		LCD_TEXT(buffer);
		writeUART_Text("V2");
		writeUART_Text(buffer);

		// Mostrar contador en el LCD
		LCD_Set_Cursor(13, 1);
		LCD_TEXT("S3");
		LCD_Set_Cursor(13, 2);
		snprintf(buffer, sizeof(buffer), " %d", counter);
		LCD_TEXT(buffer);

		// Leer desde la UART si hay datos disponibles
		if (UCSR0A & (1 << RXC0)) {
			char received_char = receivedChar();

			// Actualizar contador basado en el carácter recibido
			if (received_char == '+') {
				counter++;
				} else if (received_char == '-') {
				counter--;
			}
		}

		_delay_ms(500);  // Actualizar cada 500 ms
	}
}