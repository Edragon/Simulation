#include<reg52.h>
sbit led_Red=P1^0;	     //红灯
sbit led_Green=P1^1;	 //绿点
sbit lock_jdq=P0^0;	     //开锁继电器
sbit K1_zhiwen=P2^1;	 //	指纹正确
sbit K2_zhiwen=P2^4;	 //	指纹错误
sbit K3_kaisuo=P2^5;	 //	开锁按键
sbit bell=P2^6;	         //	蜂鸣器
void Delay1ms(unsigned int count)
{
	unsigned int i,j;
	for(i=0;i<count;i++)
	for(j=0;j<120;j++);
}
void init()
{
	P1 = 0xFF;
	P0 = 0xFF;
	P2 = 0xFF;
	P3 = 0xFF;
	bell=0;
}
void keyscan()
{
	if(K1_zhiwen==0)
	{
	 	Delay1ms(10);
			if(K1_zhiwen==0)
			{
			 	led_Green=0;
				lock_jdq=0;
				bell=0;
				Delay1ms(500);
				led_Green=1;
				bell=1;
				Delay1ms(500);
				led_Green=0;
				bell=0;
				Delay1ms(500);
				led_Green=1;
				bell=1;
				Delay1ms(500);
				lock_jdq=1;
				while(!K1_zhiwen);
			}
	}
	if(K2_zhiwen==0)
	{
	 	Delay1ms(10);
			if(K2_zhiwen==0)
			{
			 	led_Red=0;
				Delay1ms(500);
				led_Red=1;
				Delay1ms(500);
				led_Red=0;
				Delay1ms(500);
				led_Red=1;
				Delay1ms(500);
				while(!K2_zhiwen);
			}
	}
	if(K3_kaisuo==0)
	{
	 	Delay1ms(10);
			if(K3_kaisuo==0)
			{
			 	led_Green=0;
				lock_jdq=0;
				Delay1ms(500);
				led_Green=1;
				Delay1ms(500);
				led_Green=0;
				Delay1ms(500);
				led_Green=1;
				Delay1ms(500);
				lock_jdq=1;
				while(!K3_kaisuo);
			}
	}

}
void main()
{  
	init();
 	while(1)
	{	
	keyscan();
	}
}