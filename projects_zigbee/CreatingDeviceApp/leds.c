#include "leds.h"


void init_TIM1(int period, int prescaler);
void init_PWM_with_TIM1(int pulse);
void init_NVIC_button(int button);
void init_EXTI_button(int button);

void switch_color(int num)
{
  switch(num)
  {
     case(0):
       {
	  set_color(255, 255, 255);
	  break;
       }

     case(1):
       {
	  set_color(255, 20, 147); //pink
	  break;
       }

     case(2):
       {
	  set_color(255, 255, 0); //yellow
	  break;
       }

     case(3):
       {
	  set_color(30, 144, 255); //kind of blue
	  break;
       }

     case(4):
       {
	  set_color(127, 255, 0); //kind of green 
	  break;
       }

     case(5):
       {
	  set_color(0, 0, 255); //orange 
	  break;
       }

     default:
       {
	  set_color(0, 0, 0);
	  break;
       }
  }
}

void init_EXTI_button(int button)
{
  EXTI_InitTypeDef EXTI_InitStruct = {0};
  if (button == 0)
   {
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE,EXTI_PinSource0);
  	EXTI_InitStruct.EXTI_Line= EXTI_Line0;
   }
   else if (button == 1)
   {
	 SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE,EXTI_PinSource1);
  	EXTI_InitStruct.EXTI_Line= EXTI_Line1;
   }
  EXTI_InitStruct.EXTI_LineCmd= ENABLE;
  EXTI_InitStruct.EXTI_Mode= EXTI_Mode_Interrupt;
  EXTI_InitStruct.EXTI_Trigger= EXTI_Trigger_Falling;
  EXTI_Init(&EXTI_InitStruct);
}

void init_NVIC_button(int button)
{
  NVIC_InitTypeDef nvic_struct_for_EXTI = {0};
  if (button == 0)
     { 
  	nvic_struct_for_EXTI.NVIC_IRQChannel= EXTI0_IRQn;
     }
  else if (button == 1)
     {
	nvic_struct_for_EXTI.NVIC_IRQChannel= EXTI1_IRQn;
     }
  nvic_struct_for_EXTI.NVIC_IRQChannelPreemptionPriority = 0;
  nvic_struct_for_EXTI.NVIC_IRQChannelSubPriority = 0;
  nvic_struct_for_EXTI.NVIC_IRQChannelCmd= ENABLE;
  NVIC_Init(&nvic_struct_for_EXTI);
}

void set_brightness(double level)
{
  TIM_SetCompare1(TIM1, (int)(TIM_GetCapture1(TIM1) + 20000 * (1 - level)));
  TIM_SetCompare2(TIM1, (int)(TIM_GetCapture2(TIM1) + 20000 * (1 - level)));
  TIM_SetCompare3(TIM1, (int)(TIM_GetCapture3(TIM1) + 20000 * (1 - level)));
}

void set_color(double r, double g, double b)
{
  TIM_SetCompare1(TIM1, (int)((20000 - 1)* (1 - r/255)));
  TIM_SetCompare2(TIM1, (int)((20000 - 1) * (1 - g/255)));
  TIM_SetCompare3(TIM1, (int)((20000 - 1) * (1 - b/255)) );
}

void turn_light_off()
{
  set_color(255, 255, 255);
}

void turn_light_on()
{
  set_color(0, 0, 0);
}


void init_TIM1(int period, int prescaler)
{
  TIM_TimeBaseInitTypeDef tim_struct = {0};
  tim_struct.TIM_Period = period;
  tim_struct.TIM_Prescaler = prescaler;
  tim_struct.TIM_ClockDivision = 0;
  tim_struct.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM1, &tim_struct);
}

void init_PWM_with_TIM1(int pulse)
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

void init_leds_AF_TIM1(void)
{
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
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

void init_buttons(void)
{
  GPIO_InitTypeDef GPIO_InitStructureBN;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
  GPIO_InitStructureBN.GPIO_Pin   = GPIO_Pin_0 | GPIO_Pin_1;
  GPIO_InitStructureBN.GPIO_Mode  = GPIO_Mode_IN;
  GPIO_InitStructureBN.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructureBN.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructureBN.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_Init(GPIOE, &GPIO_InitStructureBN);
}

void init_buttons_interractions(void)
{
  /*RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
  init_EXTI_button(0);
  init_EXTI_button(1);

  init_NVIC_button(0);
  init_NVIC_button(1);*/

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE,EXTI_PinSource0);
  EXTI_InitTypeDef EXTI_InitStruct;
  EXTI_InitStruct.EXTI_Line= EXTI_Line0;
  EXTI_InitStruct.EXTI_LineCmd= ENABLE;
  EXTI_InitStruct.EXTI_Mode= EXTI_Mode_Interrupt;
  EXTI_InitStruct.EXTI_Trigger= EXTI_Trigger_Rising;
  EXTI_Init(&EXTI_InitStruct);
  NVIC_InitTypeDef nvic_struct_for_EXTI;
  nvic_struct_for_EXTI.NVIC_IRQChannel= EXTI0_IRQn;
  nvic_struct_for_EXTI.NVIC_IRQChannelPreemptionPriority= 0;
  nvic_struct_for_EXTI.NVIC_IRQChannelSubPriority= 0;
  nvic_struct_for_EXTI.NVIC_IRQChannelCmd= ENABLE;
  NVIC_Init(&nvic_struct_for_EXTI);

  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE,EXTI_PinSource1);
  EXTI_InitTypeDef EXTI_InitStruct1;
  EXTI_InitStruct1.EXTI_Line= EXTI_Line1;
  EXTI_InitStruct1.EXTI_LineCmd= ENABLE;
  EXTI_InitStruct1.EXTI_Mode= EXTI_Mode_Interrupt;
  EXTI_InitStruct1.EXTI_Trigger= EXTI_Trigger_Rising;
  EXTI_Init(&EXTI_InitStruct1);
  NVIC_InitTypeDef nvic_struct_for_EXTI1;
  nvic_struct_for_EXTI1.NVIC_IRQChannel= EXTI1_IRQn;
  nvic_struct_for_EXTI1.NVIC_IRQChannelPreemptionPriority= 0;
  nvic_struct_for_EXTI1.NVIC_IRQChannelSubPriority= 0;
  nvic_struct_for_EXTI1.NVIC_IRQChannelCmd= ENABLE;
  NVIC_Init(&nvic_struct_for_EXTI1);
}

void start_TIM1(void)
{
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 , ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
  init_TIM1(20000, 168);
  init_PWM_with_TIM1(15000);
  TIM_Cmd(TIM1, ENABLE);
  TIM_CtrlPWMOutputs(TIM1, ENABLE);
}



/*void init_all()
{
  //RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  //RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
  //RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 , ENABLE);
  //RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

  init_buttons();
  init_buttons_interractions();
  init_leds_AF_TIM1();
}*/
