/*
 * display.c
 *
 *  Created on: 4 Nov 2018
 *      Author: Yihang Guo
 */

#include "display.h"
#include "stm32l476g_discovery_glass_lcd.h"


void Button_init(){

		GPIO_InitTypeDef buttonConf;

		// configuration for button
		buttonConf.Pin = GPIO_PIN_0;
		buttonConf.Mode = GPIO_MODE_INPUT;
		buttonConf.Pull = GPIO_PULLDOWN;
		buttonConf.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		buttonConf.Alternate = 0;
		HAL_GPIO_Init(GPIOA, &buttonConf);

		buttonConf.Pin = GPIO_PIN_1;
		buttonConf.Mode = GPIO_MODE_INPUT;
		buttonConf.Pull = GPIO_PULLDOWN;
		buttonConf.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		buttonConf.Alternate = 0;
		HAL_GPIO_Init(GPIOA, &buttonConf);


		buttonConf.Pin = GPIO_PIN_2;
		buttonConf.Mode = GPIO_MODE_INPUT;
		buttonConf.Pull = GPIO_PULLDOWN;
		buttonConf.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		buttonConf.Alternate = 0;
		HAL_GPIO_Init(GPIOA, &buttonConf);

		buttonConf.Pin = GPIO_PIN_5;
		buttonConf.Mode = GPIO_MODE_INPUT;
		buttonConf.Pull = GPIO_PULLDOWN;
		buttonConf.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		buttonConf.Alternate = 0;
		HAL_GPIO_Init(GPIOA, &buttonConf);

		// Initial unit
		unit = 0;
}


// Simplified change unit
void changeUnit(){
	if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1)==GPIO_PIN_SET){
		unit = 1;
	}
	else if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5)==GPIO_PIN_SET){
		unit = 2;
	}
	else{
		unit = 0;
	}
}


// Display integer
void displayValue(int a){
	char str[5];
	uint8_t string[5];
	itoa(a,str,10);
	for (int i=0; i<=4; ++i){
		string[i] = (uint8_t)(str[i]);
	}
	BSP_LCD_GLASS_Clear();
	BSP_LCD_GLASS_DisplayString(string);
}

// Display integer
void displayUnit(int value, int unit){
	//int vkph = value*1.61;
	//int vmps = value*0.45;

	int vkph = value * 3.6;
	int vmph = value * 0.447;
	if ((unit != 0) || (unit != 1) || (unit != 2)){
		unit = 1; //show KPH if unit is invalid
	}
	char character[6] = "MPHKS/";
	uint8_t ucharacter[6];
	for (int i=0;i<=5;++i){
		ucharacter[i] = (uint8_t)(character[i]);
	}
	if (unit==2){
		displayValue(vmph);
		BSP_LCD_GLASS_DisplayChar(&ucharacter[0], POINT_OFF, DOUBLEPOINT_OFF, 3);
		BSP_LCD_GLASS_DisplayChar(&ucharacter[1], POINT_OFF, DOUBLEPOINT_OFF, 4);
		BSP_LCD_GLASS_DisplayChar(&ucharacter[2], POINT_OFF, DOUBLEPOINT_OFF, 5);
	}
	else if (unit==1){
		displayValue(vkph);
		BSP_LCD_GLASS_DisplayChar(&ucharacter[3], POINT_OFF, DOUBLEPOINT_OFF, 3);
		BSP_LCD_GLASS_DisplayChar(&ucharacter[1], POINT_OFF, DOUBLEPOINT_OFF, 4);
		BSP_LCD_GLASS_DisplayChar(&ucharacter[2], POINT_OFF, DOUBLEPOINT_OFF, 5);
	}
	else if (unit==0){
		displayValue(value);
		BSP_LCD_GLASS_DisplayChar(&ucharacter[0], POINT_OFF, DOUBLEPOINT_OFF, 3);
		BSP_LCD_GLASS_DisplayChar(&ucharacter[5], POINT_OFF, DOUBLEPOINT_OFF, 4);
		BSP_LCD_GLASS_DisplayChar(&ucharacter[4], POINT_OFF, DOUBLEPOINT_OFF, 5);
	}
}











