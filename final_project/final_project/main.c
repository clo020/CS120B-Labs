/*
 * matrix_real.c
 *
 * Created: 3/11/2019 9:24:32 PM
 * Author : Elise Lin
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "io.h"
#include "io.c"
#include "timer.h"
#include "usart.h"

unsigned char player = 0x00;
unsigned char reset = 0x00;
unsigned char temp_re = 0x00;
unsigned char stime = 0;
unsigned char timer = 0;
unsigned char temp;
enum Dip_states {init, Wait_Init, t, start, preset, pplayer, stop} state;
	
void dip() {
	player = (~PINA) & 0x01;
	reset = (~PINA) & 0x02;
	temp_re = 2;
	char timeArr[4];
	temp =0;
	switch(state){
		
		case init:
			LCD_DisplayString(1, "   Let's Play  ");
			LCD_WriteData(0x01);
			LCD_Cursor(1);
			LCD_WriteData(0x00);
			if(reset||player) {
				state = Wait_Init;
			}
			else {
				state = init;
				}
				break;
		case Wait_Init:
			 if(!reset||!player) {
				 stime = 1;
				 state = t;
			 }
				 else {
				 state = Wait_Init;
			 }
			 break;
		case t:
			
			if(USART_HasReceived(0)) {
				stime = USART_Receive(0);
			}
				itoa(stime, timeArr, 10);
				LCD_DisplayString(1, "Score:");
				LCD_Cursor(7);
				LCD_WriteData(timeArr[0]);
				if (stime > 9) {
					LCD_WriteData(timeArr[1]);
				}
				if (stime > 99) {
					LCD_WriteData(timeArr[2]);
				}
				if (stime > 999) {
					LCD_WriteData(timeArr[3]);
				}
				if(USART_HasReceived(0)) {
					stime = USART_Receive(0);
				}
			
			state = start;
			break;
		case start:
			if(USART_HasReceived(0)) {
				stime = USART_Receive(0);
			}
				itoa(stime, timeArr, 10);
				LCD_DisplayString(1, "Score:");
				LCD_Cursor(7);
				LCD_WriteData(timeArr[0]);
				if (stime > 9) {
					LCD_WriteData(timeArr[1]);
				}
				if (stime > 99) {
					LCD_WriteData(timeArr[2]);
				}
				if (stime > 999) {
					LCD_WriteData(timeArr[3]);
				}
				if(USART_HasReceived(0)) {
					stime = USART_Receive(0);
				}
			
				if(reset){
					state = preset;
				}
				else if (player) {
					state = pplayer;
				}
				else {
					state = t;
				}
				break;
			
		case preset:
			if(USART_IsSendReady(0)) {
				USART_Send(temp_re, 0);
			}
		
			state = stop;
			break;
		case pplayer:
			if(USART_IsSendReady(0)) {
			USART_Send(player, 0);
			}
// 			if(USART_HasReceived(0)) {
// 				stime = USART_Receive(0);
// 			}
			state = stop;
			break;
		case stop:
			if(player||reset){
				state = stop;
			}
			else{
				state = t;
			}
			break;
		default:
			state = init;
			break;	
 	}
		
		
	
}


int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;	//BUTT
	DDRC = 0xFF; PORTC = 0x00;	//lcd
	DDRD = 0xFF; PORTD = 0x00;	//MATRIX
	
	TimerSet(35);
	TimerOn();
	
	LCD_init();

	initUSART(0);
	
	state = init;
	
	while (1)
	{
		dip();
		while(!TimerFlag);
		TimerFlag = 0;
	}
}

