#define uchar unsigned char
#define uint unsigned int
#include <reg51.h>
#include <stdio.h>
#include <absacc.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define P128870 XBYTE[0x8000] //日历时钟12887 秒
#define P128871 XBYTE[0x8001] //日历时钟12887 秒报警
#define P128872 XBYTE[0x8002] //日历时钟12887 分
#define P128873 XBYTE[0x8003] //日历时钟12887 分报警
#define P128874 XBYTE[0x8004] //日历时钟12887 时
#define P128875 XBYTE[0x8005] //日历时钟12887 时报警
#define P128876 XBYTE[0x8006] //日历时钟12887 星期
#define P128877 XBYTE[0x8007] //日历时钟12887 日
#define P128878 XBYTE[0x8008] //日历时钟12887 月
#define P128879 XBYTE[0x8009] //日历时钟12887 年
#define P12887a XBYTE[0x800a] //日历时钟12887 寄存器A
#define P12887b XBYTE[0x800b] //日历时钟12887 寄存器B
#define P12887c XBYTE[0x800c] //日历时钟12887 寄存器C
#define P12887d XBYTE[0x800d] //日历时钟12887 寄存器D
#define OUT573 XBYTE[0xb000]  //扩展卡输出74HC573地址 0xb000
#define IN573 XBYTE[0xa000]   //扩展卡输入74HC573地址 0xa000
#define PA8255 XBYTE[0x9000]  //扩展卡8255的PA口
#define PB8255 XBYTE[0x9001]  //扩展卡8255的PB口
#define PC8255 XBYTE[0x9002]  //扩展卡8255的PC口
#define COM8255 XBYTE[0x9003] //扩展卡8255的控制口
#define ADC0809 XBYTE[0xc000] //AD采集口地址
#define DAC0808 XBYTE[0xd000] //DA输出口地址
//
sbit DAT=P1^0; //P1口显示74HC164的DAT
sbit CLK=P1^1; //P1口显示74HC164的CLK
sbit scl=P1^6; //24C02 scl
sbit sda=P1^7; //24C02 sda
sbit key1 = P1^5;
sbit key2 = P1^4;
sbit key3 = P1^3;
sbit key4 = P1^2;
sbit buzzer = P3^3;

uchar rcbuf[20];
uchar DispBuf[5]={ 0,0,0,0,0 };
uchar DispB[5]={ 0,0,0,0,0 };
uchar code DispCode[]={ 0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71,0x00,0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x20,0x6f,0x3f,0x3f,0x5e};
// 显示代码              0     1    2    3    4    5    6    7    8    9    A    b    c    d    E    F        '1'  '2'  '3'  '4'  '5'  '6'  '7'  '8'   g    o    o    d
uchar code DisNum[] = {0x67,0x67,0x38,0x40,0x5b,0x3f,0x06,0x7f,0x5b,0x4f,0x3f,0x66,0x3f,0x06,0x5b};  //Q+学号代码
uchar data buffer[10]; //用于缓存从24C02中读取数据
uchar rcbuf[20]; //串口数据缓
uint packetRecieved = 0;   //串口接收完数据标识
uchar KEY,js=0,t01,jj,t_js,ll,CPLTEMP,an0=0,abits=0,buf12887[8];key_value = 0;bell = 10;bell_stop = 5;bell_start =1;
bit bdata kk,jsbz=0,zs=0,stare=0,CPL=0;//stare=1闪烁
uint t0js=0,timer1;

void commsent(void);               //串行口发送子程序
uchar keys(void);                  //键盘扫描程序
void disp(void);                   //显示一组数据子程序
void delay(uint x);                //延时子程序
void sent(uchar x);                //显示一位数据子程序
void flash();                      //延时子程序
void x24c01_init();                //初始化24C02
void start();                      //启动24C02
void stop();                       //停止24C02
void writex(uchar j);              //24C02写一个字节子程序
uchar readx();                     //24C02读一个字节子程序
void clock();                      //24C02时钟
void x24c02_write(uchar address,uchar info); //向地址address写一个字节数据info
uchar x24c02_read(uchar address);  //从地址address读一个字节
void Delay300ms();		//@11.0592MHz
void dis_num();		//显示学号
void Delay100ms();
void Delay5us();
void key3_set();
void key2_set();
void toll();
void delay_ms(uint z);
void Packet(void);				   //处理串口数据包程序
void control(void);

void Delay5us()		//@11.0592MHz
{
	unsigned char i;
	i = 11;
	while (--i);
}

void Delay300ms()		//@11.0592MHz
{
	unsigned char i, j, k;
	i = 13;
	j = 156;
	k = 83;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}

void Delay100ms()		//@11.0592MHz
{
	unsigned char i, j, k;
	i = 2;
	j = 67;
	k = 183;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}
void flash()  //延时子程序
{
   uchar u;
   u=u;
}
void x24c02_init() //初始化24C02
{
   scl=1; flash(); sda=1; flash();
}
void start()  //启动24C02
{
   sda=1; flash(); scl=1; flash();
   sda=0; flash(); scl=0; flash();
}
void stop()  //停止24C02
{
   sda=0; flash(); scl=1; flash(); sda=1; flash();
}
void writex(uchar j)  //24C02写一个字节子程序
{
   uchar i,temp;
   temp=j;
   for (i=0;i<8;i++){
      temp=temp<<1; scl=0; flash();
      sda=CY; flash(); scl=1; flash();
   }
   scl=0; flash(); sda=1; flash();
}
uchar readx()  //24C02读一个字节子程序
{
   uchar i,j,k=0;
   scl=0; flash(); sda=1;
   for (i=0;i<8;i++){
      flash(); scl=1; flash();
      if (sda==1) j=1;
      else j=0;
      k=(k<<1)|j; scl=0;
   }
   flash(); return(k);
}
void clock() //24C02时钟
{
   uchar i=0;
   scl=1; flash();
   while ((sda==1)&&(i<255))i++;
   scl=0; flash();
}
uchar x24c02_read(uchar address)  //从地址address读一个字节
{
   uchar i;
   start(); writex(0xa0);
   clock(); writex(address);
   clock(); start();
   writex(0xa1); clock();
   i=readx(); stop();
   delay(10);
   return(i);
}
void x24c02_write(uchar address,uchar info) //向地址address写一个字节数据info
{
   EA=0;
   start(); writex(0xa0);
   clock(); writex(address);
   clock(); writex(info);
   clock(); stop();
   EA=1;
   delay(50);
}
void delay(uint x)  //延时子程序
{
   uint i;
   for (i=0;i<x;i++) ;
}
void setup12887(void)  //设置时钟12887日期和时间
{
    uchar i;
    i=P12887d;
    P12887a=0x70;    P12887b=0xa2;           P128870=buf12887[0];
    P128871=0xff;    P128872=buf12887[1];
    P128873=0xff;    P128874=buf12887[2];    P128875=0xff;
    P128876=buf12887[3];    P128877=buf12887[4];
    P128878=buf12887[5];    P128879=buf12887[6];
    P12887b=0x22;           P12887a=0x20;
    i=P12887c;
}
void read12887(void)  //读时钟12887日期和时间
{
    uchar a, tmp;
    tmp=P12887c; tmp=P12887c;
    t0js=0;
    do{
        a=P12887a;
      }while(((a&0x80)==0x80)&&(t0js<900));
    buf12887[0]=P128870;buf12887[1]=P128872;
    buf12887[2]=P128874; buf12887[3]=P128876;
    buf12887[4]=P128877; buf12887[5]=P128878;
    buf12887[6]=P128879;
}
void start12887(void)  //启动时钟12887
{
    uchar i;
    i=P12887d;
    P12887a=0x70;    P12887b=0xa2;    P128871=0xff;
    P128873=0xff;    P128875=0xff;
    P12887b=0x22;    P12887a=0x20;
    i=P12887c;
}
void time_0(void) interrupt 1 using 2  //定时器中断处理程序
{
    uchar i,t_js2 = 0;
		TR0=0;
    t0js++;   
		if(t0js>=1000)
		{
			t0js=0;
			buf12887[0]++;
			if (buf12887[0]>=60) 
				{
					buf12887[0]=0;   		
					buf12887[1]++;
					if (buf12887[1]>=99){buf12887[1]=0;}
				}		      
			if (zs==1)
			{   
				if (js>4) js=0;
				 else js++;
				 DispBuf[0]=js+17;
				 DispBuf[1]=buf12887[1]/10;
				 DispBuf[2]=buf12887[1]%10;
				 DispBuf[3]=buf12887[0]/10;
				 DispBuf[4]=buf12887[0]%10;
				 for (i=0; i<5; i++) DispB[i]=~DispCode[DispBuf[i]];
				 disp();
//				 x24c02_write(0xa0,buf12887[0])；
//				 x24c02_write(0xa1,buf12887[1])；
			}
		}
			if(t_js2 == 1)
			{				
				t_js2++;
			}
			else if(t_js2 ==2)
			{t_js2 = 0;buzzer =0;}
			
			if(t_js == 1)
			{	
				buzzer = 1;
				t_js2++;
			}
	
    timer1++; if (timer1==0) t01=1;
    TH0=-(922/256+1);        /*1ms 11.0592mhz*/
    TL0=-(922%256);
    TR0=1;
}

void ex1int(void) interrupt 2 using 1  //日历时钟DS12887中断程序
{
   uchar i,j,ch;
   if (js>4) js=0;
   else js++;
   i=P12887c; i=P12887c;
   buf12887[0]=P128870;
   buf12887[1]=P128872;
   DispBuf[0]=js+17;
   DispBuf[1]=buf12887[1]/16;
   DispBuf[2]=buf12887[1]&0x0f;
   DispBuf[3]=buf12887[0]/16;
   DispBuf[4]=buf12887[0]&0x0f;
   for (i=0; i<5; i++) DispB[i]=~DispCode[DispBuf[i]];
   for (i=0; i<5; i++) {
      ch=DispB[4-i];
      for (j=0; j<8; j++) {
         ch=ch>>1;  DAT=CY;  CLK=0; CLK=1;
      }
   }
}

void disp(void)      //显示一组数据子程序
{
   uchar i;
   for (i=0; i<=4; i++) sent(DispB[4-i]);
}
void sent(uchar x)  //显示一位数据子程序
{
   uchar i,ch;
   ch=x;
   for (i=0; i<8; i++) {
      ch=ch>>1;  DAT=CY;  CLK=0; CLK=1;
   }
}
uchar keys(void) //键盘扫描程序
{
   uchar j, K;
   K=P1&0x3c;    //取P1口
   if (K==0x3c) return 0;   //是干扰返回0
   delay(400);
   if (K==0x3c) return 0;   //是干扰返回0
   j=K;
   while ((P1&0xf0)!=0xf0); //等待抬手
   return j;                //返回键值
}

//串行通信首码为 0xeb,0xed 长度 rcbuf[0] 尾码 0xec
void commsent(void)  //串行口发送处理子程序
{
   uchar i;
   jsbz=0;
   SBUF=0xeb; while (TI==0); TI=0;
   delay(30);
   SBUF=0xed; while (TI==0); TI=0;
   delay(30);
   for (i=0; i<rcbuf[0]; i++) {
      SBUF=rcbuf[i]; while (TI==0); TI=0;
      delay(30);
   }
   SBUF=0xec; while (TI==0); TI=0;
}

void dis_num()		//显示学号
{
		unsigned char i;
		for (i=0; i<5; i++) DispB[i]=~DisNum[i];
		disp();  //显示 q q l - 2 
		Delay100ms();		//@11.0592MHz
		Delay100ms();		//@11.0592MHz
		Delay100ms();		//@11.0592MHz
		for (i=0; i<5; i++) DispB[i]=~DisNum[5 + i];
		disp();  //显示 0 1 8 2 3 
		Delay100ms();		//@11.0592MHz
		Delay100ms();		//@11.0592MHz
		Delay100ms();		//@11.0592MHz
		for (i=0; i<5; i++) DispB[i]=~DisNum[10 + i];
		disp();  //显示 0 4 0 1 2
		Delay100ms();		//@11.0592MHz
		Delay100ms();		//@11.0592MHz
		Delay100ms();		//@11.0592MHz
}

void key2_set()		//按键2数据设置
{
	uchar i;
	if(key_value == 2)	//设置秒
	{
		 DispBuf[1]=buf12887[1]/10;
		 DispBuf[2]=buf12887[1]%10;
		 DispBuf[3]=buf12887[0]/10;
		 DispBuf[4]=buf12887[0]%10;
		 for (i=0; i<5; i++) DispB[i]=~DispCode[DispBuf[i]];
		 disp();
		 Delay100ms();		//@11.0592MHz
			
		 buf12887[0]++;

		 if(buf12887[0] >= 60)
		 {
			buf12887[1]++; 
			buf12887[0] = 0;
			if(buf12887[1] > 99)
			{
				buf12887[1]=0;
			}
		 }
		 DispBuf[1]=buf12887[1]/10;
		 DispBuf[2]=buf12887[1]%10;
		 DispBuf[3]=buf12887[0]/10;
		 DispBuf[4]=buf12887[0]%10;
		 for (i=0; i<5; i++) DispB[i]=~DispCode[DispBuf[i]];
		 disp();
		}
	
	else if(key_value == 3)
	{
		 DispBuf[1]=buf12887[1]/10;
		 DispBuf[2]=buf12887[1]%10;
		 DispBuf[3]=buf12887[0]/10;
		 DispBuf[4]=buf12887[0]%10;
		 for (i=0; i<5; i++) DispB[i]=~DispCode[DispBuf[i]];
		 disp();
		 //Delay100ms();		//@11.0592MHz
		 buf12887[1]++;
		
			if(buf12887[1] > 99)
			{
				buf12887[1]=0;
			}
		 DispBuf[1]=buf12887[1]/10;
		 DispBuf[2]=buf12887[1]%10;
		 DispBuf[3]=buf12887[0]/10;
		 DispBuf[4]=buf12887[0]%10;
		 for (i=0; i<5; i++) DispB[i]=~DispCode[DispBuf[i]];
		 disp();
	}
	else if(key_value == 4)
	{
		 DispBuf[1]=bell_stop%10;
		 DispBuf[2]=0;
		 DispBuf[3]=0;
		 DispBuf[4]=0;
		 for (i=0; i<5; i++) DispB[i]=~DispCode[DispBuf[i]];
		 bell_stop++;
		 if(!(0 <= bell_stop <=9 )){bell_stop = 0;}
		 DispBuf[1]=bell_stop%10;
		 DispBuf[2]=0;
		 DispBuf[3]=0;
		 DispBuf[4]=0;
		 for (i=0; i<5; i++) DispB[i]=~DispCode[DispBuf[i]];
		 disp();
	}
		else if(key_value == 5)
	{
		 DispBuf[1]=bell_start%10;
		 DispBuf[2]=0;
		 DispBuf[3]=0;
		 DispBuf[4]=0;
		 for (i=0; i<5; i++) DispB[i]=~DispCode[DispBuf[i]];
	   disp();
		 bell_start++;
		 if(!(0 <= bell_start <= 3)){bell_start = 0;}
		 DispBuf[1]=bell_start%10;
		 DispBuf[2]=0;
		 DispBuf[3]=0;
		 DispBuf[4]=0;
		 for (i=0; i<5; i++) DispB[i]=~DispCode[DispBuf[i]];
		 disp();
	}
}
void key3_set()		//按键3数据设置
{			
	uchar i;
	if(key_value == 2)	//设置秒
	{
		 DispBuf[1]=buf12887[1]/10;
		 DispBuf[2]=buf12887[1]%10;
		 DispBuf[3]=buf12887[0]/10;
		 DispBuf[4]=buf12887[0]%10;
		 for (i=0; i<5; i++) DispB[i]=~DispCode[DispBuf[i]];
		 disp();
		 Delay100ms();		//@11.0592MHz
		 if(buf12887[0] == 0)
		 {
			if(buf12887[1] == 0)
			{
				buf12887[0] = 0;
				buf12887[1] = 0;
			}
			else{buf12887[1]--;buf12887[0]=59;}
		 }
		 else{buf12887[0]--;}
		 DispBuf[1]=buf12887[1]/10;
		 DispBuf[2]=buf12887[1]%10;
		 DispBuf[3]=buf12887[0]/10;
		 DispBuf[4]=buf12887[0]%10;
		 for (i=0; i<5; i++) DispB[i]=~DispCode[DispBuf[i]];
		 disp();
	}

	else if(key_value == 3)		
	{				
		 DispBuf[1]=buf12887[1]/10;
		 DispBuf[2]=buf12887[1]%10;
		 DispBuf[3]=buf12887[0]/10;
		 DispBuf[4]=buf12887[0]%10;
		 for (i=0; i<5; i++) DispB[i]=~DispCode[DispBuf[i]];
		 disp();
		 Delay100ms();		//@11.0592MHz
		 if(buf12887[1] > 0)
		 {
			buf12887[1]--;
		 }
		 else{buf12887[1]=0;}
		 DispBuf[1]=buf12887[1]/10;
		 DispBuf[2]=buf12887[1]%10;
		 DispBuf[3]=buf12887[0]/10;
		 DispBuf[4]=buf12887[0]%10;
		 for (i=0; i<5; i++) DispB[i]=~DispCode[DispBuf[i]];
		 disp();
	}
	else if(key_value == 4)
	{
		 DispBuf[1]=bell_stop%10;
		 DispBuf[2]=0;
		 DispBuf[3]=0;
		 DispBuf[4]=0;
		 for (i=0; i<5; i++) DispB[i]=~DispCode[DispBuf[i]];
		 disp();
		 bell_stop--;
		 if(!(0 <= bell_stop <=9 )){bell_stop = 0;}
		 DispBuf[1]=bell_stop%10;
		 DispBuf[2]=0;
		 DispBuf[3]=0;
		 DispBuf[4]=0;
		 for (i=0; i<5; i++) DispB[i]=~DispCode[DispBuf[i]];
		 disp();
	}
		else if(key_value == 5)
	{
		 DispBuf[1]=bell_start%10;
		 DispBuf[2]=0;
		 DispBuf[3]=0;
		 DispBuf[4]=0;
		 for (i=0; i<5; i++) DispB[i]=~DispCode[DispBuf[i]];
	   disp();
		 bell_start--;
		 if(!(0 <= bell_start <= 3)){bell_start = 0;}
		 DispBuf[1]=bell_start%10;
		 DispBuf[2]=0;
		 DispBuf[3]=0;
		 DispBuf[4]=0;
		 for (i=0; i<5; i++) DispB[i]=~DispCode[DispBuf[i]];
		 disp();
	}
}
void toll()
{
	if(((buf12887[1] - bell) == 0) && (buf12887[0] ==0 )) {t_js=1;}
	else if((buf12887[1] - bell)%(bell_stop + bell_start) == 0){t_js=1;}
	else if((buf12887[1] - bell + bell_start )%(bell_stop + bell_start) == 0){t_js=1;}
	else {t_js = 0;}
}
void delay_ms(uint z)
{
	unsigned int i,j;
	for(j = z;j > 0; j--)
		for(i = 112;i > 0;i--);
}

void comm(void) interrupt 4 using 3  //串行口中断程序
{
   uchar i;
   if (RI==1) {
      ES=0; jsbz=0;
      while (RI==0); RI=0;
      if (SBUF!=0xeb) goto EXT;  //首码
      while (RI==0); RI=0;
      if (SBUF!=0xed) goto EXT;  //首码
      while (RI==0); RI=0;
      rcbuf[0]=SBUF;             //长度
      if (rcbuf[0]>30) goto EXT;
      for (i=1; i<rcbuf[0]; i++) 
	  {
         while (RI==0); RI=0;
         rcbuf[i]=SBUF;          //数据
      }
      while (RI==0); RI=0;
      if (SBUF==0xec)    //尾码
   	  {
				jsbz=1; 
	      packetRecieved = 1;//包接收完毕
	  }
   }
EXT:
   ES=1;
}
void Packet(void)
{
	uchar i;
	TR0 = 0;
	if (rcbuf[0]==3)
		{	
			buf12887[0]=rcbuf[1];
			buf12887[1]=rcbuf[2];
			DispBuf[1]=buf12887[1]/10;
			DispBuf[2]=buf12887[1]%10;
			DispBuf[3]=buf12887[0]/10;
			DispBuf[4]=buf12887[0]%10;
			for (i=0; i<5; i++) DispB[i]=~DispCode[DispBuf[i]];
			disp();
		}
	packetRecieved = 0;
	TR0 = 1; 
}


void main(void)
{
   uchar i,m=0;
   TMOD=0x21; PCON=0x00; SCON=0x50; TH1=0xfd; TL1=0xfd;
   TR1=1;IE=0; TR0=1; PS=1; IT0=1; jj=0,ll=0,t_js=0;
   delay(1000); t01=0; CLK=1; jsbz=0;
   EA=1; EX0=0; EX1=0;  ET0=1; ES=1;
	 buzzer = 0;
	 x24c02_init();
	 dis_num();//显示我的姓名拼音-学号
	 while(m != 2)
	 {
		buffer[m] = x24c02_read(m);
		m++;
		buf12887[0] = buffer[0];
		buf12887[1] = buffer[1];
	 }
   while(1) 
	{		
		  if(packetRecieved)//处理串口数据包
		  {
				 Packet();
		  }
			
		  if((10 <= buf12887[1]) && (buf12887[1] <= 60))
		  {toll();}
			else{buzzer =0;}
      KEY=keys();  //读取键盘值
      switch (KEY) {
       case 0x1c:
          //  第一个键处理程序
          //EX1=0;
					zs=0;
					TR0 = 0;
//          rcbuf[0]=20; for (i=1; i<20; i++) rcbuf[i]=i;
//          commsent(); //发送20个字节数据
//					delay_ms(1);
					x24c02_write(0x00,buf12887[0]);
					delay_ms(1);
					x24c02_write(0x01,buf12887[1]);
			    for (i=25; i<29; i++) DispB[i]=~DispCode[i];
          disp();
					delay_ms(50);
				  DispBuf[1]=buf12887[1]/10;
					DispBuf[2]=buf12887[1]%10;
					DispBuf[3]=buf12887[0]/10;
					DispBuf[4]=buf12887[0]%10;
					for (i=0; i<5; i++) DispB[i]=~DispCode[DispBuf[i]];
					disp();
					TR0 = 1;
			 		while(!key1);
          break;
			 case 0x2c:
          //  第二个键处理程序	减时
					key2_set();
					while(!key2);
          break;
       case 0x34:
          //  第三个键处理程序  加时
					key3_set();
			 		while(!key3);
          break;
       case 0x38:
          //  第四个键处理程序
          //EX1=1;
			 
					key_value++;
			 if(key_value >= 6){key_value = 0;}
					while(!key4);
          break;
       default: break;
       }
				if(key_value == 1)
					{
						zs = 1;
						TR0 = 1;
					}
				else
				{
					TR0 = 0;
				} 
  }
}






