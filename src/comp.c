/*
 * comp.c
 *
 *  Created on: 20 Nov 2018
 *      Author: Yihang Guo
 */
#include "comp.h"
//PB2

//COMP_HandleTypeDef hcomp1;
//TIM_HandleTypeDef Timer3Handle;

uint16_t comp_counter = 0;
uint16_t comp_frequency = 0;
float comp_velocity = 0;
uint16_t timewindowHZ = 1; //1 Hz
int comp_velocity_int = 0;


void COMP1_Init(void)
{

  //HAL_COMP_MspInit();
  hcomp1.Instance = COMP1;
  hcomp1.State = HAL_COMP_STATE_READY;
  hcomp1.Init.InvertingInput = COMP_INPUT_MINUS_VREFINT; // Comparator inverting input connected to VrefInt
  hcomp1.Init.NonInvertingInput = COMP_INPUT_PLUS_IO2; // non-inverting input connected to PB2 for COMP1;
  hcomp1.Init.OutputPol = COMP_OUTPUTPOL_NONINVERTED;
  hcomp1.Init.Hysteresis = COMP_HYSTERESIS_NONE;
  hcomp1.Init.BlankingSrce = COMP_BLANKINGSRC_NONE; // Comparator output without blanking
  hcomp1.Init.Mode = COMP_POWERMODE_HIGHSPEED; // propagation delay and power consumption (refer to the data sheet), set the power and speed
  hcomp1.Init.WindowMode = COMP_WINDOWMODE_DISABLE; // Comparators instances pair COMP1 and COMP2 are independent
  hcomp1.Init.TriggerMode = COMP_TRIGGERMODE_IT_RISING; // Set the comparator output triggering External Interrupt Line, Comparator output triggering External Interrupt Line event with interruption, on rising edge

  __HAL_RCC_SYSCFG_CLK_ENABLE();

  HAL_COMP_Init(&hcomp1);


  //configure interrupt
  HAL_NVIC_SetPriority(COMP_IRQn, 7, 0);
  HAL_NVIC_EnableIRQ(COMP_IRQn);
}

void COMP_GPIO_Init(){

	GPIO_InitTypeDef PinconfComp;

	PinconfComp.Pin = GPIO_PIN_2;
	PinconfComp.Mode = GPIO_MODE_ANALOG;
	PinconfComp.Pull = GPIO_NOPULL;
	PinconfComp.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	PinconfComp.Alternate = 0;
	HAL_GPIO_Init(GPIOB, &PinconfComp);

	/****
	GPIO_InitTypeDef LED2conf;

	LED2conf.Pin = GPIO_PIN_2;
	LED2conf.Mode = GPIO_MODE_OUTPUT_PP;
	LED2conf.Pull = GPIO_NOPULL;
	LED2conf.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	LED2conf.Alternate = 0;
	HAL_GPIO_Init(GPIOB, &LED2conf);
	****/

}



void timer3Config(void)
{
	Timer3Handle.Instance = TIM3;
	Timer3Handle.Init.Period = 40000;
	Timer3Handle.Init.Prescaler = 2000;
	Timer3Handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	Timer3Handle.Init.CounterMode = TIM_COUNTERMODE_UP;

	__HAL_RCC_TIM3_CLK_ENABLE();

	HAL_TIM_Base_Init(&Timer3Handle);

    HAL_TIM_Base_Start_IT(&Timer3Handle); //timer will time after this command

	HAL_NVIC_SetPriority(TIM3_IRQn, 8, 0);
	HAL_NVIC_EnableIRQ (TIM3_IRQn);
}


//Comparator i
void COMP_IRQHandler(){

	//static int counter = 0;
	comp_counter ++;
	HAL_COMP_IRQHandler(&hcomp1); //Clear the flag
}



//Timer interrupt
void TIM3_IRQHandler(){

	//HAL_COMP_Stop(&hcomp1);
	Frequecy_Cal();

	// Comparator Restart
	//HAL_COMP_Start_IT(&hcomp1);

	comp_counter = 0;

	comp_frequency = 0;

	//HAL_TIM_Base_Start_IT(&Timer3Handle);

	HAL_TIM_IRQHandler(&Timer3Handle); //Clear the flag
}



void Frequecy_Cal(){

	comp_frequency = comp_counter * timewindowHZ;

	comp_velocity = (float) comp_frequency * 0.0283366 / 2;

	comp_velocity_int = (int) comp_velocity;

	changeUnit();

	displayUnit(comp_velocity_int, 5); //unit = 5 FIXME

	uint8_t str[20] ={0};
	uint8_t nCh = '\n';

	//sprintf(str, "%f", comp_velocity);
	itoa(comp_frequency,str,10);
	HAL_UART_Transmit(&huart2, str, 4, 100); // 4 bytes
	HAL_UART_Transmit(&huart2, &nCh, 1, 100);

	//HAL_UART_Transmit(&huart2, str, 10, 100); //float: 10 integer: 4
	//HAL_UART_Transmit(&huart2, &nCh, 1, 100);

/****
	uint8_t str[20] ={0};
	uint8_t nCh = '\n';
	itoa(comp_frequency,str,10);

	HAL_UART_Transmit(&huart2, str, 4, 100); // 4 bytes
	HAL_UART_Transmit(&huart2, &nCh, 1, 100);
****/

}


