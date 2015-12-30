#include "stm32f10x.h"
#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "timer.h"

/************************************************
  基于ALIENTEK 战舰STM32F103开发板
	*串口通信发送端-对串口信号进行归零处理之后发送*
 日期：2015年12月29日
 作者：wmb007@126.com
 PWM Wave占空比可以固定在50%，但是在输出时嚎梢则经过一个单稳态触发器来固定正脉宽的宽度以满足实验的要求。
************************************************/
 
 int main(void)
 {	
	 u16 times=1;
	 u8 key_value = 0;
	 u8 mode = 0;
	 u8 Baud_index=0;
	 delay_init();	
	 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	 uart_init(100*Baud_Rate[0]);
 	 LED_Init();	
	 KEY_Init(); 
	 TIM3_PWM_Init(749,0); //不分频，PWM Frequency = 72000000/750 Hz = 96000Hz
	 TIM_SetCompare2(TIM3,375);  //GPIOB.5 GPIOB.8 TIM_SetCompare2(TIM3,7); TIM_SetCompare2(TIM3,742);
   while(1)
	 {	
		 key_value = KEY_Scan(1);
		if(key_value == KEY2_PRES){ //KEY2_PRES means GPIOE.2 = '0'; 
			mode++;
			if(mode > 2) mode = 0;		
		}
		if(key_value == WKUP_PRES){ //WKUP_PRES means GPIOA.0 = '1';
			Baud_index++;
			if(Baud_index > 15){
				Baud_index = 1;
			}
			uart_init(100*Baud_Rate[Baud_index]);		
		}
		if(key_value == KEY1_PRES){ //KEY2_PRES means GPIOE.3 = '0'; 
			Baud_index++;
			if(Baud_index < 1) { 
				Baud_index = 15;
			}
			uart_init(100*Baud_Rate[Baud_index]);			
		}
		
		if(mode == 0){
			printf("\r\nTest Informations %d: <mode %d>\r\n",times,mode);
			printf("\r\n Key Technology Research and Application of Visible Light Communication 2015/12/30; By XMU EED M.B. Wang\r\n");
		
		}else if(mode == 1){
			printf("\r\n测试信息 %d: <mode %d>\r\n",times,mode);
			printf("\r\n 可见光通信系统的关键技术研究及应用 2015/12/30; By 厦门大学 M.B. Wang\r\n");
		}else{
			printf("\r\nTest Informations %d: <mode %d> Key Technology Research and Application of Visible Light Communication. By XMU EED M.B. Wang\r\n",times,mode);
		}
			
		if(++times > 20){
			times=1;
			LED = ~LED;
//			LED0 = ~LED0;//GPIOB.5
			LED1 = ~LED1;//GPIOE.5
		}
		
			delay_ms(50);		
	 }
 }
