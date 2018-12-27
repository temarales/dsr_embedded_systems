#include "main.h"

#define SWITCH_DELAY 500000

static int lightNum = 0;

void SwitchLight(int num)
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
        break;
        }
  
}


/*void TIM2_IRQHandler(void)
{
   static int flag = 1;
   if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
   {    if (flag == 1)
        {
            GPIO_ResetBits(GPIOA, GPIO_Pin_8);
	    GPIO_SetBits(GPIOA, GPIO_Pin_10);
            flag = -flag;
        }
        else
   	{
	    GPIO_ResetBits(GPIOA, GPIO_Pin_10);
            GPIO_SetBits(GPIOA, GPIO_Pin_8);
	    flag = -flag;
	}
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    }
}*/
static int flag = 1;
void TIM2_IRQHandler(void)
{
   if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
   {    if (flag == 1)
        {
            SwitchLight(lightNum);
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

void InitLedsOnSmall(void )
{
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}

int main(void)
{
  lightNum = 0;
  flag = 0;
  TIM_TimeBaseInitTypeDef tim_struct = {0};
  NVIC_InitTypeDef nvic_struct = {0};
  EXTI_InitTypeDef EXTI_InitStruct;
  NVIC_InitTypeDef nvic_struct_for_EXTI;

  /* Enable peripheral clock for LEDs and button port */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE,EXTI_PinSource0);


  /*Init timer*/
  tim_struct.TIM_Period = 2000 - 1;
  tim_struct.TIM_Prescaler = 42000 - 1;
  tim_struct.TIM_ClockDivision = 0;
  tim_struct.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE); //?
  TIM_TimeBaseInit(TIM2, &tim_struct);
  TIM_Cmd(TIM2, ENABLE);

  /*Init interrupt (in programm)*/
  nvic_struct.NVIC_IRQChannel= TIM2_IRQn;
  nvic_struct.NVIC_IRQChannelPreemptionPriority= 0;
  nvic_struct.NVIC_IRQChannelSubPriority= 1;
  nvic_struct.NVIC_IRQChannelCmd= ENABLE;
  NVIC_Init(&nvic_struct);

  /*Init interrupt (for EXTI)*/
  nvic_struct_for_EXTI.NVIC_IRQChannel= TIM2_IRQn;
  nvic_struct_for_EXTI.NVIC_IRQChannelPreemptionPriority= 0;
  nvic_struct_for_EXTI.NVIC_IRQChannelSubPriority= 0;
  nvic_struct_for_EXTI.NVIC_IRQChannelCmd= ENABLE;
  NVIC_Init(&nvic_struct_for_EXTI);


  /* Init LEDs */
  InitLedsOnSmall();

  /* Turn all the leds off */
  //GPIO_ResetBits(GPIOA, GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10);
  //int fflag = 1, i, isOn = 0 ;
  //int a, b; 

  while (1)
  {
     //int timer = TIM_GetCounter(TIM2);
    
    	/*a = GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_0); //right button
    	b = GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_1);
    
       if (a==0)
    {
        a = 1;
        flag = 1;
     }
     else if (b == 0)
     {
        b = 1;
        flag = -1;
     }*/

      /*if(timer == 0 && isOn == 0)
	{ 
           lightNum+=flag;
            if (lightNum < 0)
               lightNum = 2;
            else if (lightNum > 2)
                lightNum = 0;
	   SwitchLight(lightNum);
          isOn =1;
	}
    if (timer == 1000)
        {
	   GPIO_SetBits(GPIOA, GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10); 
            isOn=0;
        }*/
     
  }
}
