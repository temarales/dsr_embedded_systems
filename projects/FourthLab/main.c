#include "main.h"

static int pulse = 15000;

void EXTI0_IRQHandler(void) 
{
	if(EXTI_GetITStatus(EXTI_Line0)!= RESET)
        {
         if (pulse + 1000 <= 20000)
         {
             pulse += 1000;
             TIM_SetCompare1(TIM1, pulse);
         }
         EXTI_ClearITPendingBit(EXTI_Line0);
        }
}

void EXTI1_IRQHandler(void) 
{
	if(EXTI_GetITStatus(EXTI_Line1)!= RESET)
        {
         if (pulse - 1000 >= 0)  
         {
	   pulse -= 1000;
           TIM_SetCompare1(TIM1, pulse);
         }
         EXTI_ClearITPendingBit(EXTI_Line1);
        }
}
static void InitTIM1(int period, int prescaler)
{
  TIM_TimeBaseInitTypeDef tim_struct = {0};
  tim_struct.TIM_Period = period;
  tim_struct.TIM_Prescaler = prescaler;
  tim_struct.TIM_ClockDivision = 0;
  tim_struct.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM1, &tim_struct);
}
static void InitButtons(void )
{
  GPIO_InitTypeDef GPIO_InitStructureButton = {0};
  GPIO_InitStructureButton.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
  GPIO_InitStructureButton.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructureButton.GPIO_Mode  = GPIO_Mode_IN;
  GPIO_InitStructureButton.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_Init(GPIOE, &GPIO_InitStructureButton);
}

static void InitEXTIButton(int button)
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

static void InitNVICButton(int button)
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
  nvic_struct_for_EXTI.NVIC_IRQChannelPreemptionPriority= 0;
  nvic_struct_for_EXTI.NVIC_IRQChannelSubPriority= 0;
  nvic_struct_for_EXTI.NVIC_IRQChannelCmd= ENABLE;
  NVIC_Init(&nvic_struct_for_EXTI);
}

static void InitPWMwithTIM1(int pulse)
{
  TIM_OCInitTypeDef timerPWM = {0};
  timerPWM.TIM_OCMode = TIM_OCMode_PWM1;
  timerPWM.TIM_OutputState = TIM_OutputState_Enable;
  timerPWM.TIM_Pulse = pulse;
  timerPWM.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OC1Init(TIM1, &timerPWM);

  TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
}

static void InitLedAF()
{
  GPIO_PinAFConfig(GPIOA, GPIO_Pin_8, GPIO_AF_TIM1);

  GPIO_InitTypeDef GPIO_InitStructure = {0};
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_TIM1);
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}

int main(void)
{
  /* Enable peripheral clock for LEDs and button port */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 , ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);


    /*Configure GPIO Pin to its alternative function*/
  InitLedAF();

  /*Init button*/
  InitButtons();

  /*Init interrupt (for EXTI)*/

  InitEXTIButton(0);
  InitEXTIButton(1);

  InitNVICButton(0);
  InitNVICButton(1);

   /*Init timer for AF*/
  InitTIM1(20000, 168);

    /*Timer for PWM*/
  InitPWMwithTIM1(15000);
    
  TIM_Cmd(TIM1, ENABLE);
  TIM_CtrlPWMOutputs(TIM1, ENABLE);


  


  while (1)
  {
  }
}
