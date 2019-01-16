#include "leds.h"

void SetColor(double r, double g, double b)
{
  TIM_SetCompare1(TIM1, (int)((20000 - 1)* (r/255)));
  TIM_SetCompare2(TIM1, (int)((20000 - 1) * (g/255)));
  TIM_SetCompare3(TIM1, (int)((20000 - 1) * (b/255)) );
}

void SwitchColor(int num)
{
  switch(num)
  {
     case(0):
       {
	  SetColor(255, 255, 255);
	  break;
       }

     case(1):
       {
	  SetColor(255, 20, 147); //pink
	  break;
       }

     case(2):
       {
	  SetColor(255, 255, 0); //yellow
	  break;
       }

     case(3):
       {
	  SetColor(30, 144, 255); //kind of blue
	  break;
       }

     case(4):
       {
	  SetColor(127, 255, 0); //kind of green 
	  break;
       }

     case(5):
       {
	  SetColor(0, 0, 255); //orange 
	  break;
       }

     default:
       {
	  SetColor(0, 0, 0);
	  break;
       }
  }
}

void InitTIM1(int period, int prescaler)
{
  TIM_TimeBaseInitTypeDef tim_struct = {0};
  tim_struct.TIM_Period = period;
  tim_struct.TIM_Prescaler = prescaler;
  tim_struct.TIM_ClockDivision = 0;
  tim_struct.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM1, &tim_struct);
}

void InitPWMwithTIM1(int pulse)
{
  TIM_OCInitTypeDef timerPWM = {0};
  timerPWM.TIM_OCMode = TIM_OCMode_PWM1;
  timerPWM.TIM_OutputState = TIM_OutputState_Enable;
  timerPWM.TIM_Pulse = pulse;
  timerPWM.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OC1Init(TIM1, &timerPWM);
  TIM_OC2Init(TIM1, &timerPWM);
  TIM_OC3Init(TIM1, &timerPWM);

  TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
  TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);
  TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);

}

void InitLedsAF(void)
{
  GPIO_InitTypeDef GPIO_InitStructure = {0};
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;

 /*Configure GPIO Pins to its alternative function*/
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_TIM1);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_TIM1);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_TIM1);
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}
