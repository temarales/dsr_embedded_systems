/// \file leds.h  
/// Library for working with colors on stm32.

#ifndef _LIBRARY_H_
#define _LIBRARY_H_

#include <stdint.h>
#include "stm32f4xx.h"

/// \brief Color setting(RGB).
/// \param[in] r Brightness of red: 0..255
/// \param[in] g Brightness of green: 0..255
/// \param[in] b Brightness of blue: 0..255
void SetColor(double r, double g, double b);

/// \brief Switching defined colors.
/// Switch color by number
/// \param[in] num Color number in defined colors
void SwitchColor(int num);

/// \brief Initializing TIM1.
/// Initializing TIM1
/// \param[in] period Timer's period
/// \param[in] prescaler Timer's prescaler
void InitTIM1(int period, int prescaler);

/// \brief Initializing TIM1 for PWM.
/// \param[in] pulse Pulse of PWM
void InitPWMwithTIM1(int pulse);

/// \brief Initializing leds 8-10.
/// Initializing leds 8-10, mode - alternative function
void InitLedsAF(void);

#endif