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
  ����ALIENTEK ս��STM32F103������
	*����ͨ�ŷ��Ͷ�-�Դ����źŽ��й��㴦��֮����*
 ���ڣ�2015��12��29��
 ���ߣ�wmb007@126.com
 PWM Waveռ�ձȿ��Թ̶���50%�����������ʱ�����򾭹�һ������̬���������̶�������Ŀ��������ʵ���Ҫ��
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
//	 TIM3_PWM_Init(749,0); //����Ƶ��PWM Frequency = 72000000/750 Hz = 96000Hz
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
	u8  *testString_1 = "�ɼ���ͨ��ϵͳ�Ĺؼ������о���Ӧ��; By ���Ŵ�ѧ M.B. Wang";
	u8  *testString_2 = "Key Technology Research and Application of Visible Light Communication By M.B. Wang";
	
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(9600);	 //���ڳ�ʼ��Ϊ9600
 	LED_Init();			     //LED�˿ڳ�ʼ��
	KEY_Init();          //��ʼ���밴�����ӵ�Ӳ���ӿ�
	TIM3_PWM_Init(749,0); //����Ƶ��PWM Frequency = 72000000/750 Hz = 96000Hz
	TIM_SetCompare2(TIM3,375);  //GPIOB.5 GPIOB.8 TIM_SetCompare2(TIM3,7); TIM_SetCompare2(TIM3,742);
		
		LCD_Init();			   		//��ʼ��LCD   
		W25QXX_Init();				//��ʼ��W25Q128
		my_mem_init(SRAMIN);		//��ʼ���ڲ��ڴ��
		exfuns_init();				//Ϊfatfs��ر��������ڴ�  
		f_mount(fs[0],"0:",1); 		//����SD�� 
		f_mount(fs[1],"1:",1); 		//����FLASH.
		while(font_init()) 			//����ֿ�
		{
	 
			LCD_Clear(WHITE);		   	//����
			POINT_COLOR=RED;			//��������Ϊ��ɫ	   	   	  
			LCD_ShowString(30,50,200,16,16,"WarShip STM32");
			while(SD_Init())			//���SD��
			{
				LCD_ShowString(30,70,200,16,16,"SD Card Failed!");
				delay_ms(200);
				LCD_Fill(30,70,200+30,70+16,WHITE);
				delay_ms(200);		    
			}								 						    
			LCD_ShowString(30,70,200,16,16,"SD Card OK");
			LCD_ShowString(30,90,200,16,16,"Font Updating...");
			key=update_font(20,110,16,"0:");//�����ֿ�
			while(key)//����ʧ��		
			{			 		  
				LCD_ShowString(30,110,200,16,16,"Font Update Failed!");
				delay_ms(200);
				LCD_Fill(20,110,200+20,110+16,WHITE);
				delay_ms(200);		       
			} 		  
			LCD_ShowString(30,110,200,16,16,"Font Update Success!   ");
			delay_ms(1500);	
			LCD_Clear(WHITE);//����	       
		}  
		POINT_COLOR=RED;       
		Show_Str(20,5,200,24,"��������(Send Data):",16,0);
		Show_Str(20,90,200,24,"������(BaudRate):",16,0);		
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
			printf("\r\n������Ϣ %d: <mode %d> %s\r\n",times,mode,testString_1);
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
//			len=USART_RX_STA&0x3fff;//�õ��˴ν��յ������ݳ��ȣ��������Ƶĳ��Ȼ��bug 300	
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

//		if(times%30==0)LED1=!LED1;//��˸LED,��ʾϵͳ��������.
//			delay_ms(10);   
//	}	 
}
