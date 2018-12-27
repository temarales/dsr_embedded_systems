#include "main.h"

#define SWITCH_DELAY 500000

int pulse = 15000;

void SetColor(double r, double g, double b)
{
  TIM_SetCompare1(TIM1, (int)((20000 - 1)* (r/255)));
  TIM_SetCompare2(TIM1, (int)((20000 - 1) * (g/255)));
  TIM_SetCompare3(TIM1, (int)((20000 - 1) * (b/255)) );
}

void EXTI0_IRQHandler(void) 
{
	if(EXTI_GetITStatus(EXTI_Line0)!= RESET)
        {
         SetColor(255, 255, 0);
         EXTI_ClearITPendingBit(EXTI_Line0);
        }
}

void EXTI1_IRQHandler(void) 
{
	if(EXTI_GetITStatus(EXTI_Line1)!= RESET)
        {
         SetColor(0, 0, 0);
         EXTI_ClearITPendingBit(EXTI_Line1);
        }
}

void InitButtons(void )
{
  GPIO_InitTypeDef GPIO_InitStructureButton;
  GPIO_InitStructureButton.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
  GPIO_InitStructureButton.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructureButton.GPIO_Mode  = GPIO_Mode_IN;
  GPIO_InitStructureButton.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_Init(GPIOE, &GPIO_InitStructureButton);
}


int main(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructureButton;
  TIM_TimeBaseInitTypeDef tim_struct;
  NVIC_InitTypeDef nvic_struct;
  EXTI_InitTypeDef EXTI_InitStruct;
  NVIC_InitTypeDef nvic_struct_for_EXTI;

  /* Enable peripheral clock for LEDs and button port */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 , ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE,EXTI_PinSource0);
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE,EXTI_PinSource1);

  /*Init interrupt(out)*/
  EXTI_InitStruct.EXTI_Line= EXTI_Line0;
  EXTI_InitStruct.EXTI_LineCmd= ENABLE;
  EXTI_InitStruct.EXTI_Mode= EXTI_Mode_Interrupt;
  EXTI_InitStruct.EXTI_Trigger= EXTI_Trigger_Falling;
  EXTI_Init(&EXTI_InitStruct);

  EXTI_InitStruct.EXTI_Line= EXTI_Line1;
  EXTI_InitStruct.EXTI_LineCmd= ENABLE;
  EXTI_InitStruct.EXTI_Mode= EXTI_Mode_Interrupt;
  EXTI_InitStruct.EXTI_Trigger= EXTI_Trigger_Falling;
  EXTI_Init(&EXTI_InitStruct);

  /*Init interrupt (for EXTI)*/
  nvic_struct_for_EXTI.NVIC_IRQChannel= EXTI0_IRQn;
  nvic_struct_for_EXTI.NVIC_IRQChannelPreemptionPriority= 0;
  nvic_struct_for_EXTI.NVIC_IRQChannelSubPriority= 0;
  nvic_struct_for_EXTI.NVIC_IRQChannelCmd= ENABLE;
  NVIC_Init(&nvic_struct_for_EXTI);

  nvic_struct_for_EXTI.NVIC_IRQChannel= EXTI1_IRQn;
  nvic_struct_for_EXTI.NVIC_IRQChannelPreemptionPriority= 0;
  nvic_struct_for_EXTI.NVIC_IRQChannelSubPriority= 0;
  nvic_struct_for_EXTI.NVIC_IRQChannelCmd= ENABLE;
  NVIC_Init(&nvic_struct_for_EXTI);

   /*Init timer for AF*/
  tim_struct.TIM_Period = 20000;
  tim_struct.TIM_Prescaler = 168;
  tim_struct.TIM_ClockDivision = 0;
  tim_struct.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM1, &tim_struct);

/*Timer for PWM*/
    TIM_OCInitTypeDef timerPWM;
    timerPWM.TIM_OCMode = TIM_OCMode_PWM1;
    timerPWM.TIM_OutputState = TIM_OutputState_Enable;
    timerPWM.TIM_Pulse = pulse;//15000;
    timerPWM.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC1Init(TIM1, &timerPWM);
    TIM_OC2Init(TIM1, &timerPWM);
    TIM_OC3Init(TIM1, &timerPWM);

   TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
   TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);
   TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);
    
    TIM_Cmd(TIM1, ENABLE);
    TIM_CtrlPWMOutputs(TIM1, ENABLE);

    
 

  /* Init LEDs */
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;

 /*Configure GPIO Pin to its alternative function*/
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_TIM1);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_TIM1);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_TIM1);
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /*Init button*/
  InitButtons();


  while (1)
  {
  }
}
