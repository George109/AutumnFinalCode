#ifndef DISPLAY_H_
#define DISPLAY_H_

#include "stm32l4xx.h"
#include "stm32l476g_discovery.h"
#include "stm32l476g_discovery_glass_lcd.h"

int unit;

void Button_init();
void changeUnit();
void displayValue(int a);
void displayUnit(int value, int unit);



#endif /* DISPLAY_H_ */
