/**
  ******************************************************************************
  * @file    main.c
  * @author  Yihang Guo
  * @version V1.0
  * @date    Nov 04 2018
  * @brief   Task (Fall Semester)
  ******************************************************************************
*/


#include "stm32l4xx.h"
#include "stm32l476g_discovery.h"
#include "stm32l476g_discovery_glass_lcd.h"
#include "clock.h"
#include "display.h"
#include "adc.h"
#include "processing.h"
#include "uart.h"
#include "comp.h"

/*** NEW DESIGN DISPLAY ***/

int display(){

	int value = 100;

	__HAL_RCC_GPIOA_CLK_ENABLE();

	HAL_Init();

	Button_init();

	BSP_LCD_GLASS_Init();

	BSP_LCD_GLASS_Clear();


	while(1){
		changeUnit();
		HAL_Delay(100);
		displayUnit(value, 3);
	}
}



/***** ADC main *******/

int ADC(){

	SystemClock_Config(); //80 MHz Clock signal

	HAL_Init();

	BSP_LCD_GLASS_Init();
	Button_init();
	//LCD_Init(); //YL's initial

	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOE_CLK_ENABLE();

	MX_USART2_UART_Init();
	HAL_UART_MspInit(&huart2);
	ADC_GPIO_Init();
	ADC_init();
	TIM4_Init();

	BSP_LED_Init(LED_GREEN);

	HAL_ADC_Start_IT(&adc1h);

	while(1){

		if( HAL_ADC_PollForConversion(&adc1h, 2000) == HAL_TIMEOUT )
		{
			BSP_LED_On(LED_GREEN);
			HAL_Delay(1000);
			BSP_LED_Off(LED_GREEN);
		}

	}

}

/*******
	//uint32_t adcvalue;
	//BSP_LCD_GLASS_Clear();
	//HAL_ADC_PollForConversion(&adc1h, 100);
	//ADC_NVIC_Config();
	//ADC1_IRQHandler(); // This function can not be call in main, it will run automatically
	//adcvalue = HAL_ADC_GetValue(&adc1h);
	//sprintf(t,"%d/n", adcvalue);
	//HAL_UART_Transmit(&huart2,t,10,2);
	float floatValue = (float)(adcvalue)/1241.2;
	uint8_t str[20] ={0};
	gcvt(floatValue,3,str);
	//itoa(adcvalue,str,10);
	BSP_LCD_GLASS_Clear();
	BSP_LCD_GLASS_DisplayString(str);
	//HAL_Delay(200);
	*****/

//Task 5: Display main
/***
int displayOld(){

	BSP_JOY_Init(JOY_MODE_GPIO);
	int unit = 0;
	//int value = 20;
	//int value = 223;
	int value = 100;
	HAL_Init();
	BSP_LCD_GLASS_Init();
	BSP_LCD_GLASS_Clear();
	//displayValue();
	while(1){
	unit = changeUnit();
	displayUnit(value, unit);
	}
}
**/

//signal processing

int processingMain(){

	HAL_Init();
	//__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOE_CLK_ENABLE();
	BSP_LED_Init(LED_GREEN);
	BSP_LCD_GLASS_Init();
	BSP_LCD_GLASS_Clear();
	float32_t input[1024];
	float32_t output[2048];
	float32_t realOutput[1024];
	float32_t maxvalue;
	uint32_t maxindex;
	float32_t frequency;
	float32_t speed_ms;
	float32_t speed_mph;
	int speed;
	int i = 0;
	int frq = 1000;
	arm_rfft_fast_instance_f32 data;
	data.pTwiddleRFFT = twiddleCoef_rfft_1024;
	arm_rfft_fast_init_f32(&data,1024);
	//fft_generate_test_data(input, 1024, 978);
	fft_generate_test_data(input, 1024, frq);
	arm_rfft_fast_f32(&data, input, output, 0);
	arm_cmplx_mag_f32(output, realOutput, 1024);
	//special code
	for (i = 0; i < 2; i++ ){
		realOutput[i] = 0;
	}
	realOutput[512] = 0;
	arm_max_f32(realOutput, 1024, &maxvalue, &maxindex);
	frequency = (float32_t)maxindex * 4000 / 1024;
	//frequency = (float32_t)maxindex * 2000 / 1024;
	speed_ms = frequency * 300000000 / (2 * 10587000000);
	//speed = (int) speed_ms;
	speed_mph = speed_ms / 0.45;
	speed = (int) speed_mph;
	while(1){
	//unit = changeUnit();
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_SET);
	displayUnit(speed, unit);
	}
}

//comparator

int main(){

	SystemClock_Config();
	HAL_Init();
	BSP_LCD_GLASS_Init();
	COMP1_Init();
	COMP_GPIO_Init();
	timer3Config();
	HAL_COMP_Start(&hcomp1);

	//Uart init

	MX_USART2_UART_Init();
	HAL_UART_MspInit(&huart2);

	processingMain();
	//ADC();
	//display();
	//displayOld();
	while(1){
	}
}
