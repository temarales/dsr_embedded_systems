#ifndef _LIBRARY_H_
#define _LIBRARY_H_

#include <stdint.h>
#include "stm32f4xx.h"

void SetColor(double r, double g, double b);
void SwitchColor(int num);
void InitTIM1(int period, int prescaler);
void InitPWMwithTIM1(int pulse);
void InitLedsAF(void);

#endif