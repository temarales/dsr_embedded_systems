#include "main.h"

static int lightNum = 0;
static int flag = 1;
static int direction = 1;

static void SwitchLight(int num)
{
   switch (num) {
   case 0:
       {
       GPIO_ResetBits(GPIOA, GPIO_Pin_8);
       break;
       }
   case 1:
        {
        GPIO_ResetBits(GPIOA, GPIO_Pin_9);
        break;
        }
   case 2:
        {
        GPIO_ResetBits(GPIOA, GPIO_Pin_10);
        break;
        }
    default:
	{
        break;
	}
        }
}


void TIM2_IRQHandler(void)
{
   if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
   {    if (flag == 1)
        {
            lightNum += direction;
            SwitchLight(abs(lightNum) % 3);
            flag = -flag;
        }
        else
   	{
	    GPIO_SetBits(GPIOA, GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10);
	    flag = -flag;
	}
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    }
}

void EXTI0_IRQHandler(void) 
{
	if(EXTI_GetITStatus(EXTI_Line0)!= RESET)
        {
         direction = 1;
         EXTI_ClearITPendingBit(EXTI_Line0);
        }
}

void EXTI1_IRQHandler(void) 
{
	if(EXTI_GetITStatus(EXTI_Line1)!= RESET)
        {
         direction = -1;
         EXTI_ClearITPendingBit(EXTI_Line1);
        }
}

static void InitLedsOnSmall(void )
{
  GPIO_InitTypeDef GPIO_InitStructure = {0};
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}

static void InitEXTIZeroB(void)
{
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE,EXTI_PinSource0);
  EXTI_InitTypeDef EXTI_InitStruct = {0};
  EXTI_InitStruct.EXTI_Line= EXTI_Line0;
  EXTI_InitStruct.EXTI_LineCmd= ENABLE;
  EXTI_InitStruct.EXTI_Mode= EXTI_Mode_Interrupt;
  EXTI_InitStruct.EXTI_Trigger= EXTI_Trigger_Rising;
  EXTI_Init(&EXTI_InitStruct);
}

static void InitEXTIFirstB(void)
{
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE,EXTI_PinSource1);
  EXTI_InitTypeDef EXTI_InitStruct = {0};
  EXTI_InitStruct.EXTI_Line= EXTI_Line1;
  EXTI_InitStruct.EXTI_LineCmd= ENABLE;
  EXTI_InitStruct.EXTI_Mode= EXTI_Mode_Interrupt;
  EXTI_InitStruct.EXTI_Trigger= EXTI_Trigger_Rising;
  EXTI_Init(&EXTI_InitStruct);
}

static void InitButtons(void )
{
  GPIO_InitTypeDef GPIO_InitStructureButton = {0};
  GPIO_InitStructureButton.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
  GPIO_InitStructureButton.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructureButton.GPIO_Mode  = GPIO_Mode_IN;
  GPIO_InitStructureButton.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_Init(GPIOE, &GPIO_InitStructureButton);
}

static void InitNVICFirstB(void)
{
   NVIC_InitTypeDef nvic_struct_for_EXTI = {0};
   nvic_struct_for_EXTI.NVIC_IRQChannel= EXTI1_IRQn;
  nvic_struct_for_EXTI.NVIC_IRQChannelPreemptionPriority= 0;
  nvic_struct_for_EXTI.NVIC_IRQChannelSubPriority= 0;
  nvic_struct_for_EXTI.NVIC_IRQChannelCmd= ENABLE;
  NVIC_Init(&nvic_struct_for_EXTI);
}

static void InitNVICZeroB(void)
{
  NVIC_InitTypeDef nvic_struct_for_EXTI = {0};
   nvic_struct_for_EXTI.NVIC_IRQChannel= EXTI0_IRQn;
  nvic_struct_for_EXTI.NVIC_IRQChannelPreemptionPriority= 0;
  nvic_struct_for_EXTI.NVIC_IRQChannelSubPriority= 0;
  nvic_struct_for_EXTI.NVIC_IRQChannelCmd= ENABLE;
  NVIC_Init(&nvic_struct_for_EXTI);
}

static void InitTIM2 (int period, int prescaler)
{
  TIM_TimeBaseInitTypeDef tim_struct = {0};
  tim_struct.TIM_Period = period - 1;
  tim_struct.TIM_Prescaler = prescaler - 1;
  tim_struct.TIM_ClockDivision = 0;
  tim_struct.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE); 
  TIM_TimeBaseInit(TIM2, &tim_struct);
  TIM_Cmd(TIM2, ENABLE);
}

static void AddInterruptTIM2(void)
{
  NVIC_InitTypeDef nvic_struct = {0};
  nvic_struct.NVIC_IRQChannel= TIM2_IRQn;
  nvic_struct.NVIC_IRQChannelPreemptionPriority= 0;
  nvic_struct.NVIC_IRQChannelSubPriority= 1;
  nvic_struct.NVIC_IRQChannelCmd= ENABLE;
  NVIC_Init(&nvic_struct);
}

int main(void)
{
  lightNum = 0;
  flag = 1;
  direction = 1;
  
  /* Enable peripheral clock for LEDs and button port */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);


  /* Init LEDs */
  InitLedsOnSmall();

  /*Init buttons and it's interrupts*/
  InitButtons();

  InitEXTIFirstB();
  InitEXTIZeroB();

  InitNVICFirstB();
  InitNVICZeroB();

  /*Init timer*/
  InitTIM2(2000, 42000);

  /*Init timer interrupt*/
  AddInterruptTIM2();

  while (1)
  {
  }
}
