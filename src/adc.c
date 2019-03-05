/*
 * adc.c
 *
 *  Created on: 4 Nov 2018
 *      Author: Yihang GUO
 *
 */

//ADC handle
#include "adc.h"

void ADC_init(){

	adc1h.Instance = ADC1;
	adc1h.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV1;//
	adc1h.Init.Resolution = ADC_RESOLUTION_12B;//highest resolution
	adc1h.Init.NbrOfConversion = 1; //specify the number of ranks, only 1 channel was chose
	adc1h.Init.ScanConvMode = ENABLE;
	adc1h.Init.EOCSelection = ADC_EOC_SINGLE_CONV; //single trigger
	adc1h.Init.ContinuousConvMode = DISABLE;
	adc1h.Init.DiscontinuousConvMode = DISABLE;
	adc1h.Init.ExternalTrigConv =  ADC_EXTERNALTRIG_T4_TRGO; //Timer 4
	adc1h.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING; //rising triger
	adc1h.Init.DataAlign = ADC_DATAALIGN_RIGHT; //right align
	adc1h.Init.DMAContinuousRequests = ENABLE; //continuous mode
	adc1h.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN; //overwrite the original data
	//over-sampling disable
	adc1h_channel.Channel = ADC_CHANNEL_8; //channel 8 PA 3
	//adc1h_channel.Channel = ADC_CHANNEL_7;//PA2
	adc1h_channel.Rank = ADC_REGULAR_RANK_1;
	adc1h_channel.SamplingTime = ADC_SAMPLETIME_2CYCLES_5;//ADC_SAMPLETIME_2CYCLES_5
	adc1h_channel.SingleDiff = ADC_SINGLE_ENDED; //differ ADC will measure the voltage diff
	adc1h_channel.OffsetNumber = ADC_OFFSET_NONE;
	adc1h_channel.Offset = 0x000;

	//enable the ADC synchronous clock
	__HAL_RCC_ADC_CLK_ENABLE();

	HAL_ADC_Init(&adc1h);

	HAL_ADC_ConfigChannel(&adc1h, &adc1h_channel);

	HAL_NVIC_SetPriority(ADC1_IRQn, 7, 0);
	HAL_NVIC_EnableIRQ(ADC1_IRQn);

	ADCcounter = 0;

}

// Every time ADC1 have interrupt call this function
void ADC1_IRQHandler(){

	ADCcounter ++;

	ADCvalue[ADCcounter] = HAL_ADC_GetValue(&adc1h);
	//ADCvalue[ADCcounter] = 4096 * sin(ADCcounter*1600*2*3.14/40000);// test signal
	//ADCvalue[ADCcounter] = 10;

	if(ADCcounter == 1024){
		for (count = 1; count <= 1024; count ++ ){
			ADCvalue_float[count] = (float) ADCvalue[count];

			ADCdata = ADCvalue_float[count];
			uint8_t str[20] ={0};
			uint8_t nCh = '\n';
			sprintf(str, "%f", ADCdata);

			HAL_UART_Transmit(&huart2, str, 10, 100); //float: 10 integer: 4
			HAL_UART_Transmit(&huart2, &nCh, 1, 100);

		}

		HAL_ADC_Stop_IT(&adc1h);

		ADCspeed = ADC_speedCalculation(ADCvalue_float);

		changeUnit();
		displayUnit(ADCspeed, unit);



		HAL_ADC_Start_IT(&adc1h);
		ADCcounter = 0;
	}

	HAL_ADC_IRQHandler(&adc1h); //clear flag

}


void ADC_GPIO_Init()
{
	GPIO_InitTypeDef Pinconf;

	//Pinconf.Pin = GPIO_PIN_2;// PA 2
	Pinconf.Pin = GPIO_PIN_3; // PA 3
	Pinconf.Mode = GPIO_MODE_ANALOG_ADC_CONTROL; //ADC conversion
	Pinconf.Pull = GPIO_NOPULL;
	Pinconf.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	Pinconf.Alternate = 0;
	HAL_GPIO_Init(GPIOA, &Pinconf);

	GPIO_InitTypeDef LED2conf;

	LED2conf.Pin = GPIO_PIN_2;
	LED2conf.Mode = GPIO_MODE_OUTPUT_PP;
	LED2conf.Pull = GPIO_NOPULL;
	LED2conf.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	LED2conf.Alternate = 0;
	HAL_GPIO_Init(GPIOB, &LED2conf);


}


void TIM4_Init(){
	//Timer4Handle.Instance = TIM4;
	//Timer4Handle.Init.CounterMode = TIM_COUNTERMODE_UP;
	//Timer4Handle.Init.ClockDivision = 0;
	//sampling frequency is 3200 > 2*1600, 4000000/3200= 1250
	//Timer4Handle.Init.Period = 50;
	//Timer4Handle.Init.Prescaler = 25;
	Timer4Handle.Instance = TIM4;
	Timer4Handle.Init.CounterMode = TIM_COUNTERMODE_UP;
	Timer4Handle.Init.ClockDivision = 0;
	Timer4Handle.Init.Period = 200; //200
	Timer4Handle.Init.Prescaler = 10;	//10

	TIM_MasterConfigTypeDef TIM_TRGO_Config;
	TIM_TRGO_Config.MasterOutputTrigger = TIM_TRGO_UPDATE;
	TIM_TRGO_Config.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;

	__HAL_RCC_TIM4_CLK_ENABLE();

	HAL_TIM_Base_Init(&Timer4Handle);
	HAL_TIMEx_MasterConfigSynchronization(&Timer4Handle, &TIM_TRGO_Config);

	HAL_TIM_Base_Start(&Timer4Handle);

}



/****
//read a sample from the ADC
uint16_t sampleInput(void){ //32
	uint16_t value = 0; //32
	uint8_t buffer[]="      ";
	value = (uint16_t) HAL_ADC_GetValue(&adc1h);
	sprintf(buffer,"%6u",value);
	BSP_LCD_GLASS_DisplayString(buffer);
	return value;
}
****/








