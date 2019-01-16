#include "main.h"

static int numColor = 0;

void EXTI0_IRQHandler(void) 
{
	if(EXTI_GetITStatus(EXTI_Line0)!= RESET)
        {
	 numColor++;
	 SwitchColor(abs(numColor)%6);
         EXTI_ClearITPendingBit(EXTI_Line0);
        }
}

void EXTI1_IRQHandler(void) 
{
	if(EXTI_GetITStatus(EXTI_Line1)!= RESET)
        {
	 numColor--;
	 SwitchColor(abs(numColor)%6);
         EXTI_ClearITPendingBit(EXTI_Line1);
        }
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



int main(void)
{

  /* Enable peripheral clock for LEDs and button port */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 , ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);


  /*Init buttons*/
  InitButtons();
  /*Init interrupt(out)*/
  InitEXTIButton(0);
  InitEXTIButton(1);

  /*Init interrupt (for EXTI)*/
  InitNVICButton(0);
  InitNVICButton(1);

   /*Init timer*/
  InitTIM1(20000, 168);
  InitPWMwithTIM1(15000);
  TIM_Cmd(TIM1, ENABLE);
  TIM_CtrlPWMOutputs(TIM1, ENABLE);


  /* Init LEDs */
  InitLedsAF();


  while (1)
  {
  }
}
