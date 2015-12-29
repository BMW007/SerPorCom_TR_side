
*串口通信发送端-对串口信号进行归零处理之后发送*

 CORE 用来存放核心文件和启动文件， 
 
 OBJ 是用来存放编译过程文件以及 hex 文件，
 
 STM32F10x_FWLib 文件夹顾名思义用来存放 ST 官方提供的库函数源码文件。
 
 USER 目录除了用来放工程文件外，还用来存放主函数文件 main.c,以及其他包括
 stm32f10x.h， system_stm32f10x.c， system_stm32f10x.h 等等。

 SYSTEM 文件夹，下面有 3 个子目录分别为 sys,usart,delay，
 存放的是我们每个实验都要使用到的共用代码，该代码是由 ALIENTEK 编写