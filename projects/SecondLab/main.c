#include "main.h"

#define SWITCH_DELAY 500000

int pulse = 15000;

static void InitButtons(void )
{
  GPIO_InitTypeDef GPIO_InitStructureButton = {0};
  GPIO_InitStructureButton.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
  GPIO_InitStructureButton.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructureButton.GPIO_Mode  = GPIO_Mode_IN;
  GPIO_InitStructureButton.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_Init(GPIOE, &GPIO_InitStructureButton);
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

static void InitTIM2(int period, int prescaler)
{
  TIM_TimeBaseInitTypeDef tim_struct = {0};
  tim_struct.TIM_Period = period - 1;
  tim_struct.TIM_Prescaler = prescaler - 1;
  tim_struct.TIM_ClockDivision = 0;
  tim_struct.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM2, &tim_struct);
}

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
        break;
        }
}

int main(void)
{

  /* Enable peripheral clock for LEDs and button port */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);


  /*Init timer*/
  InitTIM2(2000, 42000);
  TIM_Cmd(TIM2, ENABLE);

  /* Init LEDs */
  InitLedsOnSmall();

  /*Init button*/
  InitButtons();


  /* Turn all the leds off */
  GPIO_ResetBits(GPIOA, GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10);


  int flag = 1, lightNum = 0, isOn = 0 ;
  int a, b; 

  while (1)
  {
     int timer = TIM_GetCounter(TIM2);
    
    	a = GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_0); //right button
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
     }

      if(timer == 0 && isOn == 0)
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
        }
     
  }
}






