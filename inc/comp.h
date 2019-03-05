/*
 * comp.h
 *
 *  Created on: 20 Nov 2018
 *      Author: Yihang Guo
 */


#include "stm32l4xx.h"
#include "stdio.h"
#include "stdlib.h"
#include "display.h"
#include "uart.h"



COMP_HandleTypeDef hcomp1;
TIM_HandleTypeDef Timer3Handle;

/***
uint16_t comp_counter = 0;
uint16_t comp_frequency = 0;
float comp_velocity = 0;
uint16_t timewindowHZ = 10; //10hz
***/


void COMP1_Init();
void COMP_GPIO_Init();
void timer3Config();
void COMP_IRQHandler();
void TIM3_IRQHandler();
void Frequecy_Cal();




