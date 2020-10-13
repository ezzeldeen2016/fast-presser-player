/*
 * RTOStest.c
 *
 * Created: 24/08/2020 10:55:36 ص
 *  Author: omar.M.ezzeldeen
 */ 


#include <avr/io.h>
#define F_CPU 8000000UL
#include "GPIO.h"
#include "LCD.h"
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"
#include "semphr.h"
void lcd(void);
void Button_1(void);
void Button_2(void);
TaskHandle_t b1;
TaskHandle_t b2;
TaskHandle_t screen;
SemaphoreHandle_t s=NULL;
//______________queue_______________
QueueHandle_t button_1_q;
QueueHandle_t button_2_q;

//___________________________main_______________________
int main(void)
{
	bin_direction(B, 0, output);
	bin_direction(B, 7, output);
	bin_direction(D, 0, input);
	bin_direction(D, 1, input);
	bin_write(B, 0, 0);
	bin_write(B, 7, 0);
	lcd_init();	
	xTaskCreate(lcd,"lcd",85,NULL,1,&screen);
	xTaskCreate(Button_1,"button1",85,NULL,2,&b1);
	xTaskCreate(Button_2,"button2",85,NULL,2,&b2);
	vTaskStartScheduler();
	
	while(1)
	{
		
		/*lcd_print("i'm task one");
		lcd_command(0xC0);
		lcd_print("i'm task two");
		_delay_ms(100);
		lcd_command(0x01);
		lcd_command(0x06);
		lcd_command(0x18);
		lcd_command(0x80);*/
	}
		
		
}	

void lcd(void)
{
	char r=0,d=0;
	char x1=0,x2=0;
	char w=0;
	char c=0;
		while(1)
		{
			xQueueReceive(button_1_q,(void*)&x1,( TickType_t ) 0);
			xQueueReceive(button_2_q,(void*)&x2,( TickType_t ) 0);
			if(x1==10 || x2==10)
			{
				vTaskSuspend(b1);
				vTaskSuspend(b2);
				if(x1==10)
				{
					w=1;
				}
				if(x2==10)
				{
					w=2;
				}
				x1=0;
				x2=0;
			}
			if(w==1)
			{
				lcd_command(0x01);
				lcd_command(0x18);
				lcd_command(0x80);
				lcd_print("congratulations");
				lcd_command(0xc0);
				lcd_print("player_1");
				vTaskDelay(200);
				lcd_command(0x01);
				vTaskDelay(200);
				c++;
				if(c==2)
				{
					w=0;
					c=0;
					vTaskResume( b1);
					vTaskResume( b2);
				}
				
			}
			else if(w==2)
			{
				lcd_command(0x01);
				lcd_command(0x18);
				lcd_command(0x80);
				lcd_print("congratulations");
				lcd_command(0xc0);
				lcd_print("player_2");
				vTaskDelay(200);
				lcd_command(0x01);
				vTaskDelay(200);
				c++;
				if(c==2)
				{
					w=0;
					c=0;
					vTaskResume( b1);
					vTaskResume( b2);
				}
			}
			else
			{
			r=(x1%10)+48;
			d=(x1/10)+48;
			lcd_command(0x80);
			lcd_print("player_1:");
			lcd_data(d);
			lcd_data(r);
			r=(x2%10)+48;
			d=(x2/10)+48;
			lcd_command(0xc0);
			lcd_print("player_2:");
			lcd_data(d);
			lcd_data(r);
			vTaskDelay(1);
			}
		}
}

void Button_1(void)
{
	char x=0;
	char n=0;
	button_1_q=xQueueCreate(10,sizeof(char));
	while(1)
	{
		if(bin_read(D , 0)==1)
		{
			n=1;
		}
		if((bin_read(D , 0)==0) && (n==1))
		{
			x++;
			xQueueSend(button_1_q,(void*)&x,( TickType_t ) 0);
			n=0;
			if(x==10)
			{
				x=0;
			}
		}
		vTaskDelay(1);	
	}
}

void Button_2(void)
{
	char x=0;
	char n=0;
	button_2_q=xQueueCreate(10,sizeof(char));
	while(1)
	{
		if(bin_read(D , 1)==1)
		{
			n=1;
		}
		if((bin_read(D , 1)==0) && (n==1))
		{
			x++;
			xQueueSend(button_2_q,(void*)&x,( TickType_t ) 0);
			n=0;
			if(x==10)
			{
				x=0;
			}
		}
		vTaskDelay(1);
	}
}