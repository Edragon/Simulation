#include<reg51.h>
#define uchar  unsigned char
#define uint   unsigned int

sbit  key  = P3^4;//ʱ��/���
sbit  key1 = P3^7;//�����ͣ/����
sbit  K1   = P1^0;//����ʱ��
sbit  K2   = P1^1;//��ʱ
sbit  K3   = P1^2;//ѡ��ʱ����
sbit  K4   = P1^3;//��ʱ��
sbit  K5   = P1^4;//��ʱ��
 uchar code DSY_CODE[] = {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90}; //����ܶ���
 uchar code wei[]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
 uchar hour,minute,second ,select = 0,uhour=24 ,umin=60 , usec=60 ,flag0,keynum0;
 uchar k=1,s,m,h,scount,ssec=0,smin=0,count=0;

 void delayMS(uint X)
 {
    uint i;
	while(X--)  
	for(i = 0;i< 120;i++) ;
 }

  void display(uchar ws,uchar wm, uchar wh)	//�������ʾ����
  { 

	
	P2=wei[0];					//λѡ0x01 0000 0001
  	P0=DSY_CODE[wh/10];
  	delayMS(3);
	P2=0x00;					//��Ӱ
   
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

 

//ͨ��count���hour��min��sec����ֵ 
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
 void keyscan_settime() //ʱ������ģ��
 {
   uchar set_hour,set_min,set_sec;
   if(K1 == 0)
   {
	 delayMS(10);
	 if(K1 == 0)
	 {
      set_sec = second,set_min = minute, set_hour = hour;
	  TR0 = 0;          //�رն�ʱ��
	  while(K1 == 0)	//�ȴ��û�����
	  {
      display(set_sec,set_min,set_hour);

   if(K3 == 0)
	{
	  delayMS(10);
	  if(K3 == 0)
	  {
		while(!K3);
		
		 select++;	      //ѡ��ʱ����
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
 void keyscan_usertime()	  //�û���ʱģ��
 {                		   
   if(K2 == 0)  
    {
	 delayMS(10);
	  if(K2 == 0)
	  { 
		usec = second,umin = minute,uhour = hour;   //��ʼ��ʱ		                 
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


		   if(K4 == 0)	//��ʱ
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
 
 
   void set_miaobiao() //���
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
	   		case 1:EA=1;ET0=1;TR0=1;break;//���ʼ
			case 2:EA=0;ET0=0;TR0=0;break;//�����ͣ
			case 0:scount=0;ssec=0;smin=0;break; //�������
   		}   
	} 
}
 
 //T0 �жϿ�����ʱ������
   void T0_INT() interrupt  1
 
 {

	 TH0 =(65536-10000)/256;              	
     TL0 =(65536-10000)%256;	
	 scount++;										//���
	 count++;										//ʱ��
     set_miaobiao();		
    if(count == 100)
	{                 			//�ж�1sʱ���Ƿ��ѵ�
		  count = 0;
	   timecontrol();

	}
}

void key_cl()
 {
 switch(k)
	{							//��� ʱ��ѡ��
	
	   case 1:				       //ʱ��		 6
	   	EA=1;                   //���ж�����
		ET0=1;                 	 //T0�ж�����
		TR0=1;
		                        //����ʱ��0            						
		while(key)
		{		
		 keyscan_settime();
		 keyscan_usertime();
	
		 } 
		break;
		case 2:	 					//���
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
   	 TMOD = 0x01;//��ʱ��0�����ڷ�ʽ1
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
	 	 







     	
 
 





					 
