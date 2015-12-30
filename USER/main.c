#include "stm32f10x.h"
#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "timer.h"
	 	 
#include "lcd.h"  
#include "malloc.h"
#include "sdio_sdcard.h"  
#include "w25qxx.h"    
#include "ff.h"  
#include "exfuns.h"   
#include "text.h"

/************************************************
  基于ALIENTEK 战舰STM32F103开发板
	*串口通信发送端-对串口信号进行归零处理之后发送*
 日期：2015年12月29日
 作者：wmb007@126.com
 PWM Wave占空比可以固定在50%，但是在输出时嚎梢则经过一个单稳态触发器来固定正脉宽的宽度以满足实验的要求。
************************************************/
 
// int main(void)
// {	
//	 u16 times=1;
//	 u8 key_value = 0;
//	 u8 mode = 0;
//	 u8 Baud_index=0;
//	 delay_init();	
//	 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
//	 uart_init(100*Baud_Rate[0]);
// 	 LED_Init();	
//	 KEY_Init(); 
//	 TIM3_PWM_Init(749,0); //不分频，PWM Frequency = 72000000/750 Hz = 96000Hz
//	 TIM_SetCompare2(TIM3,375);  //GPIOB.5 GPIOB.8 TIM_SetCompare2(TIM3,7); TIM_SetCompare2(TIM3,742);

// }
int main(void)
{
	 u16 times=1;
	 u8 key_value = 0;
	 u8 mode = 0;
	 u8 Baud_index=0;
	u8 key;   	    
//	u16 len;	
	u8  *testString_0 = "Key Technology Research and Application of Visible Light Communication By wmb007";
	u8  *testString_1 = "可见光通信系统的关键技术研究及应用; By 厦门大学 M.B. Wang";
	u8  *testString_2 = "Key Technology Research and Application of Visible Light Communication By M.B. Wang";
	
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(9600);	 //串口初始化为9600
 	LED_Init();			     //LED端口初始化
	KEY_Init();          //初始化与按键连接的硬件接口
	TIM3_PWM_Init(749,0); //不分频，PWM Frequency = 72000000/750 Hz = 96000Hz
	TIM_SetCompare2(TIM3,375);  //GPIOB.5 GPIOB.8 TIM_SetCompare2(TIM3,7); TIM_SetCompare2(TIM3,742);
		
		LCD_Init();			   		//初始化LCD   
		W25QXX_Init();				//初始化W25Q128
		my_mem_init(SRAMIN);		//初始化内部内存池
		exfuns_init();				//为fatfs相关变量申请内存  
		f_mount(fs[0],"0:",1); 		//挂载SD卡 
		f_mount(fs[1],"1:",1); 		//挂载FLASH.
		while(font_init()) 			//检查字库
		{
	 
			LCD_Clear(WHITE);		   	//清屏
			POINT_COLOR=RED;			//设置字体为红色	   	   	  
			LCD_ShowString(30,50,200,16,16,"WarShip STM32");
			while(SD_Init())			//检测SD卡
			{
				LCD_ShowString(30,70,200,16,16,"SD Card Failed!");
				delay_ms(200);
				LCD_Fill(30,70,200+30,70+16,WHITE);
				delay_ms(200);		    
			}								 						    
			LCD_ShowString(30,70,200,16,16,"SD Card OK");
			LCD_ShowString(30,90,200,16,16,"Font Updating...");
			key=update_font(20,110,16,"0:");//更新字库
			while(key)//更新失败		
			{			 		  
				LCD_ShowString(30,110,200,16,16,"Font Update Failed!");
				delay_ms(200);
				LCD_Fill(20,110,200+20,110+16,WHITE);
				delay_ms(200);		       
			} 		  
			LCD_ShowString(30,110,200,16,16,"Font Update Success!   ");
			delay_ms(1500);	
			LCD_Clear(WHITE);//清屏	       
		}  
		POINT_COLOR=RED;       
		Show_Str(20,5,200,24,"发送数据(Send Data):",16,0);
		Show_Str(20,90,200,24,"波特率(BaudRate):",16,0);		
		POINT_COLOR=BLUE;  
		LCD_ShowNum(30,120,100*Baud_Rate[0],6,24);			
		Show_Str(30,30,240,48,testString_0,16,0);	
   while(1)
	 {	
		 key_value = KEY_Scan(1);
		if(key_value == KEY2_PRES){ //KEY2_PRES means GPIOE.2 = '0'; 
			mode++;
			if(mode > 2) mode = 0;	

			if(mode == 0){
				LCD_Fill(29,29,270,80,WHITE);
				Show_Str(30,30,240,48,testString_0,16,0);									
			}else if(mode == 1){
				LCD_Fill(29,29,270,80,WHITE);
				Show_Str(30,30,240,48,testString_1,16,0);								
			}else{
				LCD_Fill(29,29,270,80,WHITE);
				Show_Str(30,30,240,48,testString_2,16,0);								
			}
		}
		if(key_value == WKUP_PRES){ //WKUP_PRES means GPIOA.0 = '1';
			Baud_index++;
			if(Baud_index > 15){
				Baud_index = 1;
			}
			uart_init(100*Baud_Rate[Baud_index]);	
			LCD_ShowNum(30,120,100*Baud_Rate[Baud_index],6,24);			
		}
		if(key_value == KEY1_PRES){ //KEY2_PRES means GPIOE.3 = '0'; 
			Baud_index--;
			if(Baud_index < 1) { 
				Baud_index = 15;
			}
			uart_init(100*Baud_Rate[Baud_index]);
			LCD_ShowNum(30,120,100*Baud_Rate[Baud_index],6,24);			
		}
		
		if(mode == 0){
			printf("\r\nTest Informations %d: <mode %d> %s\r\n",times,mode,testString_0);
		}else if(mode == 1){
			printf("\r\n测试信息 %d: <mode %d> %s\r\n",times,mode,testString_1);
		}else{
			printf("\r\nTest Informations %d: <mode %d> %s\r\n",times,mode,testString_2);
		}
			
		if(++times > 20){
			times=1;
			LED = ~LED;
//			LED0 = ~LED0;//GPIOB.5
			LED1 = ~LED1;//GPIOE.5
		}
		
			delay_ms(50);		
	 }
	

// 	while(1)
//	{
//		
//		if(USART_RX_STA&0x8000)
//		{					   
//			len=USART_RX_STA&0x3fff;//得到此次接收到的数据长度，超过限制的长度会出bug 300	
//			USART_RX_BUF[len] = '<';
//			USART_RX_BUF[len+1] = '<';
//			USART_RX_BUF[len+2] = ' ';
//			USART_RX_BUF[len+3] =  0;
//				
//			LCD_ShowNum(220,90,len,3,24);
//			LCD_Fill(29,119,240,330,WHITE);
//			Show_Str(30,120,200,200,USART_RX_BUF,16,0);						
//			USART_RX_STA=0;
//		}

//		if(times%30==0)LED1=!LED1;//闪烁LED,提示系统正在运行.
//			delay_ms(10);   
//	}	 
}
