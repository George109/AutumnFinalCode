/*
 * adc.h
 *
 *  Created on: 4 Nov 2018
 *      Author: Yihang Guo
 */

#ifndef ADC_H_
#define ADC_H_
#define SIZE 1024

#include "stm32l4xx.h"
#include "stdio.h"
#include "stdlib.h"
#include "display.h"
#include "uart.h"
#include "math.h"

uint32_t ADCvalue[SIZE];
//float ADCvalue[SIZE];
float ADCvalue_float[SIZE];


ADC_HandleTypeDef adc1h;
ADC_ChannelConfTypeDef adc1h_channel;
TIM_HandleTypeDef Timer4Handle;

int ADCcounter;
int ADCspeed;
float ADCdata;
int count;


void ADC_init();
void ADC_GPIO_Init();
void ADC1_IRQHandler();
void TIM4_Init();




#endif /* ADC_H_ */
