/// \file leds.h  
/// Library for working with colors on stm32.

#ifndef _LIBRARY_H_
#define _LIBRARY_H_

#include <stdint.h>
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_exti.h"
#include "stm32f4xx_syscfg.h"

void set_color(double r, double g, double b);
void switch_color(int num);
void turn_light_off();
void turn_light_on();
void start_TIM1();
void init_buttons_interractions(void);
void init_buttons(void);
void init_leds_AF_TIM1(void);
void set_brightness(double level);

#endif
