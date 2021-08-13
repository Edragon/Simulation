#include<reg51.h>
#define uchar  unsigned char
#define uint   unsigned int

sbit  key  = P3^4;//时钟/秒表
sbit  key1 = P3^7;//秒表暂停/清零
sbit  K1   = P1^0;//设置时间
sbit  K2   = P1^1;//定时
sbit  K3   = P1^2;//选择时分秒
sbit  K4   = P1^3;//加时间
sbit  K5   = P1^4;//减时间
 uchar code DSY_CODE[] = {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90}; //数码管段码
 uchar code wei[]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
 uchar hour,minute,second ,select = 0,uhour=24 ,umin=60 , usec=60 ,flag0,keynum0;
 uchar k=1,s,m,h,scount,ssec=0,smin=0,count=0;

 void delayMS(uint X)
 {
    uint i;
	while(X--)  
	for(i = 0;i< 120;i++) ;
 }

  void display(uchar ws,uchar wm, uchar wh)	//数码管显示函数
  { 

	
	P2=wei[0];					//位选0x01 0000 0001
  	P0=DSY_CODE[wh/10];
  	delayMS(3);
	P2=0x00;					//消影
   
	P2=wei[1];
	P0=DSY_CODE[wh%10];           
  	delayMS(3);
	P2=0x00;

	P2=wei[2];
	P0=0xbf;           
  	delayMS(3);
	P2=0x00;
	   
	P2=wei[3];
  	P0=DSY_CODE[wm/10];       
  	delayMS(3);
	P2=0x00;
  
	P2=wei[4];
  	P0=DSY_CODE[wm%10];            
  	delayMS(3);
	P2=0x00;

	P2=wei[5];
	P0=0xbf; 
  	delayMS(3);
	P2=0x00;
   
	P2=wei[6];
  	P0=DSY_CODE[ws/10];
  	delayMS(3);
	P2=0x00;
   
	P2=wei[7];       
  	P0=DSY_CODE[ws%10];
  	delayMS(3);
	P2=0x00;
}

 

//通过count获得hour，min，sec变量值 
void timecontrol()
{       
	second++;
	if(second==60)
	{
		second=0;
		minute++;
		if(minute==60)
		{
			minute=0;
			hour++;
			if(hour==24)
			hour=0;
		}
	}
}
 void keyscan_settime() //时间设置模块
 {
   uchar set_hour,set_min,set_sec;
   if(K1 == 0)
   {
	 delayMS(10);
	 if(K1 == 0)
	 {
      set_sec = second,set_min = minute, set_hour = hour;
	  TR0 = 0;          //关闭定时器
	  while(K1 == 0)	//等待用户按键
	  {
      display(set_sec,set_min,set_hour);

   if(K3 == 0)
	{
	  delayMS(10);
	  if(K3 == 0)
	  {
		while(!K3);
		
		 select++;	      //选择时分秒
		 if(select == 3)
		 select=0;
		 }
	}

	if(K4 == 0)
		{
		 delayMS(10);
		 if(K4 == 0)
		 {
		  while(!K4);
		   switch(select)
		   {
			case 0:if(set_sec<59) set_sec++;
				   else	 set_sec = 0;break;

			case 1:if(set_min<59)	 set_min++;
				   else set_min = 0;break;				   
				   
			case 2:if(set_hour<23) set_hour++;
				  else set_hour = 0;  break;
  
  			default:;
  
        }			  

	}
 }

 if(K5 == 0)
 {
  	 delayMS(10);
	 if(K5 == 0)
	 {
	  while(!K5);
	   switch(select)
		   {
			case 0:if(set_sec > 0) set_sec--;
				   else	 set_sec = 59;break;

			case 1:if(set_min > 0 )	 set_min--;
				   else set_min = 59;break;				   
				   
			case 2:if(set_hour > 0) set_hour--;
				  else set_hour = 23;  break;
  
  			default:;
			}
		}
	}
  
  }
  TR0 = 1;
  hour = set_hour,  minute = set_min , second = set_sec;
  
  }
 }
  display(second,minute,hour);

 }
 void keyscan_usertime()	  //用户定时模块
 {                		   
   if(K2 == 0)  
    {
	 delayMS(10);
	  if(K2 == 0)
	  { 
		usec = second,umin = minute,uhour = hour;   //开始定时		                 
		  while(K2==0)
		  {
		 display(usec,umin,uhour ); 
		 if(K3 == 0)
		 {
		  delayMS(10);		  
		  if(K3 == 0)
		  {	
		    while(!K3);			 
			select++;
			if( select==3)
			   select = 0;
			  }

			}


		   if(K4 == 0)	//加时
		   {
		    delayMS(10);
			 if(K4 == 0)
			 {
			  while(!K4);
			  switch(select)
			   {
			case 0:if(usec<59) usec++;
				   else	 usec = 0;break;

			case 1:if(umin<59)	 umin++;
				   else umin = 0;break;				   
				   
			case 2:if(uhour<23) uhour++;
				  else uhour = 0;  break;
  
  			default:;
  
        }			  
	  }
	}

 if(K5 ==0)
  {
	 delayMS(10);
     if(K5 == 0)
  {
   while(!K5);
	 switch(select)
		   {
			case 0:if(usec > 0) usec--;
				   else	 usec = 59;break;

			case 1:if(umin > 0 )umin--;
				   else umin = 59;break;				   
				   
			case 2:if(uhour > 0) uhour--;
				  else uhour = 23;  break;
  
  			default:;
		     	}
		    }

       	}
	  }
  }

  }
 }
 
 
   void set_miaobiao() //秒表
   {
    
	if(scount==100){  
		scount=0;
		  ssec++;
		if(ssec==60){
			smin++;
			ssec=0;
			if(smin==60){
				smin=0;
			}
		}
	}
   
 }
	void miaobiao_display() { 								
	if(flag0==0){
		keynum0=(keynum0+1)%3;
		switch(keynum0){
	   		case 1:EA=1;ET0=1;TR0=1;break;//秒表开始
			case 2:EA=0;ET0=0;TR0=0;break;//秒表暂停
			case 0:scount=0;ssec=0;smin=0;break; //秒表清零
   		}   
	} 
}
 
 //T0 中断控制器时钟运行
   void T0_INT() interrupt  1
 
 {

	 TH0 =(65536-10000)/256;              	
     TL0 =(65536-10000)%256;	
	 scount++;										//秒表
	 count++;										//时钟
     set_miaobiao();		
    if(count == 100)
	{                 			//判定1s时间是否已到
		  count = 0;
	   timecontrol();

	}
}

void key_cl()
 {
 switch(k)
	{							//秒表 时钟选择
	
	   case 1:				       //时钟		 6
	   	EA=1;                   //总中断允许
		ET0=1;                 	 //T0中断允许
		TR0=1;
		                        //开定时器0            						
		while(key)
		{		
		 keyscan_settime();
		 keyscan_usertime();
	
		 } 
		break;
		case 2:	 					//秒表
		keynum0=0;
		flag0=1;		   									           						
		while(key){
			if(flag0!=key1){
				delayMS(5); 
				flag0=key1;
				miaobiao_display();
				
	 		}  	
			display(scount,ssec,smin);
		} 
		break;   
		default:;
	} 	
 }



 void main()
 {
   	 TMOD = 0x01;//定时器0工作在方式1
	 TH0=(65536-10000)/256;              	
     TL0=(65536-10000)%256;	
	 while(1)
	 {
		if(key==0)
		{
			delayMS(20);
	        if(key==0)
			{		   
			  k++;
			  if(k >= 3){k=1;}
			  while(!key);				
     		}
	    }
		key_cl();
	  }
 }
	 	 







     	
 
 





					 
