#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "beep.h"
#include "dac.h"
#include "pwm.h"
#include "timer.h"
   
void selfcheck(void);			//�Լ캯������

//ALIENTEK ̽����STM32F407������ ʵ��4
//����ͨ��ʵ�� -�⺯���汾
//����֧�֣�www.openedv.com
//�Ա����̣�http://eboard.taobao.com
//������������ӿƼ����޹�˾  
//���ߣ�����ԭ�� @ALIENTEK


/********************ע��********************

�ٶȣ�carSpeed=0~4000����0~3.3V
     ����ֵ��1700--6A4
	   ���ֵ��4000--FA0
	  
ת�٣�arr
      ��Сֵ��arr=4000--FA0  --250Hz
	  
temp160��bit0~4--ת��ֵ
         bit5  --ת�Ƿ���λ
		     bit6  --ת��λ
		     bit7  --ɲ��
		     bit8  --�Լ�
		     bit9  --����
		     bit10 --����
		     bit11 --��ʹ��
				 bit12~15--��
		 
temp161��bit0~11--�ٶ�ֵ
         bit12  --����
		     bit13  --ǰ��/����
		     bit14  --����
		     bit15  --ǰ���
		 
temp162��bit0~11--ת��ֵ
         bit12  --����
	    	 bit13  --����
	    	 bit14  --��ת
		     bit15  --��ת

********************************************/




/****************������ָ��******************

ǰ��ƣ�00 00 00 80 00 00				
�����ƣ�00 20 00 00 00 00
�����ƣ�00 10 00 00 00 10
����  ��00 00 00 40 00 00
ǰ��  ��00 00 00 10 00 00     ��ǰ����������1800--708  2000-7D0  �ٶ�ֵ
����  ��00 00 08 37 00 00		
ɲ��  ��00 00 00 00 80 00
��ת  ��00 40 00 00 00 00
��ת  ��00 80 00 00 00 00
��λ  ��00 00 00 00 40 00		������ת��λ
�Լ�  ��00 00 00 00 00 01   	
********************************************/





/********************************************

16����ת10���ƺ���

����ֵ��10������

********************************************/

int Hex_Dec(u16 hex)
{
	u8 i;
	u16 a=0;
	int sum=0;
	
	for(i=0;i<3;++i)
	{			
	    a=hex&0x000f;
	    hex=hex>>4;
		switch(a)
		{
			case 0xa:a=10;break;
			case 0xb:a=11;break;
			case 0xc:a=12;break;
      case 0xd:a=13;break;
      case 0xe:a=14;break;
      case 0xf:a=15;break;
		}
		if(i==0)
			sum=a*1;
		if(i==1)
			sum=sum+a*16;
		if(i==2)
			sum=sum+a*16*16;
	}
	return sum;
}

/****************�Լ캯��***********************/
void selfcheck(){		                                              			
			    printf("\r\n�Լ�����\r\n");  
				
				LED1=!LED1;
		        delay_ms(2000);
				printf("\r\nǰ���OK\r\n");
			    LED1=!LED1;
			    delay_ms(2000);
				
				BEEP=!BEEP;
			    delay_ms(500);
			    printf("\r\n����OK\r\n");
			     BEEP=!BEEP;
			     delay_ms(2000);
				
			    LED2=!LED2;
		        delay_ms(2000);
				printf("\r\n������OK\r\n");
			    LED2=!LED2;
			    delay_ms(2000); 
								
				DAC_SetChannel1Data(DAC_Align_12b_R, 2000);
				printf("\r\nǰ��OK\r\n");
				Output_Pulse(1800);
				printf("\r\n��תOK\r\n");
				delay_ms(8000); 

//				delay_ms(4000); 

				SHACHE=!SHACHE;
				DAC_SetChannel1Data(DAC_Align_12b_R, 0);
				printf("\r\nɲ��OK\r\n");
				delay_ms(500);
				SHACHE=!SHACHE;
				delay_ms(1000);		
				
			    LED3=!LED3;
		        delay_ms(4000);
				printf("\r\n������OK\r\n");
			    LED3=!LED3;
			    delay_ms(2000);
				
 			    DAOCHE=!DAOCHE;
			 	delay_ms(500);
			    DAC_SetChannel1Data(DAC_Align_12b_R, 2000);
				PA6=!PA6;
				Output_Pulse(18100);
				printf("\r\n����OK\r\n");
				delay_ms(8000); 
				
			
				printf("\r\n��תOK\r\n");
//				delay_ms(10000);
				
				SHACHE=!SHACHE;
				DAC_SetChannel1Data(DAC_Align_12b_R, 0);
			    delay_ms(500);
			    SHACHE=!SHACHE;
				delay_ms(500);
				DAOCHE=!DAOCHE;
				printf("\r\n�Լ����\r\n");			
}


/********************������********************/

int main(void)
{ 
	/********��������********/
	
	u8 t=0;
	u8 len=0;	
	u32 i=0;
	u16 res=0;
  u16 temp=0;
	u16 temp160=0;
	u16 temp161=0;
	u16 temp162=0;
	u16 carSpeed=0;
	u16 angel_hex=0;
	u16 rot_speed_hext=0;
	int angel=0;
	int Rangel=0;
	int Langel=0;
	int angel_re=0;
	int rot_speed=0;
	uint8_t statue1=0;
	uint8_t statue2=0;
	uint8_t statue3=0;
	uint8_t statue5=0;
	uint8_t statue6=0;
	uint8_t statue7=0;
	uint8_t statue8=0;
	uint8_t statue9=0;
/*********************��ʼ��***********************/
	
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);   //����ϵͳ�ж����ȼ�����2
	delay_init(168);		                          //��ʱ��ʼ�� 
	uart_init(115200);	                              //���ڳ�ʼ��������Ϊ115200
	
	BEEP_Init();
 	QianDaDeng_Init();                                //��ʼ��ǰ��� �������������ȣ���
	ZuoShanDeng_Init();                               //��ʼ��������
	YouShanDeng_Init();                               //��ʼ��������
 	ShaChe_Init();                                    //��ʼ��ɲ��
	DaoChe_Init();                                    //��ʼ������
	GaoSu_Init();                                     //��ʼ������
	DiSu_Init();                                      //��ʼ������
	HongWai_Init();
	Dac1_Init();                                      //��ʼ��DAC
	NVIC_Config();  					
	GPIO_Config();						//�ж�����Ƕ�׿��������������������ն˺��¼��ġ��������жϵ�ʹ�ܺͳ��ܣ��жϵ����ȼ����
	GPIO_EN_DIR_Init();				//��GPIOģʽ�£�ÿ��IO��������Ϊ��������
	TIM2_Master__TIM3_Slave_Configuration(2250);	// ����TIM2���������Ϊ10Hz  ����������TIM2Ϊ����ʱ����TIM3Ϊ�Ӷ�ʱ��
	DAC_SetChannel1Data(DAC_Align_12b_R,carSpeed);       //��ʼֵΪ0
	
/********��ѭ��********/

	while(1)
	{

		if(Rangel>Langel)
		    angel_re=Rangel-Langel+5000;
		else 
			angel_re=Langel-Rangel+5000;
		
		if(USART_RX_STA&0x8000)               //�������   
		{	
            temp160=0;
	        temp161=0;
	        temp162=0;			
		  	len=USART_RX_STA&0x3fff;          //�õ��˴ν��յ������ݳ���
			printf("\r\n�����͵���ϢΪ:\r\n");        

	      /********���ݴ���********/	
			
			for(t=0;t<len;t++)
			{
				res=USART_RX_BUF[t];          //�򴮿�1�������ݣ�������1���ܴӴ������ַ���������
                if(t==0)
				{
					temp160=res;		         
				}
                else if(t==1) 
				{
					temp=res<<8;
					temp160=temp160|temp;     //byte1��2				
				}
				else if(t==2)
				{
					temp161=res;
				}
				else if(t==3)
				{
					temp=res<<8;
					temp161=temp161|temp;     //byte3��4
				}
				else if(t==4)
				{
                    temp162=res;
				}
                else if(t==5)
				{
					temp=res<<8; 
					temp162=temp162|temp;     //byte5��6
				}					
				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);  //�ȴ����ͽ���			
			}
			
	
			/********ָ���ж�********/	
			
			
			if((temp162&0x0040)==0x0040)                        //ת��λ
			 {
					   printf("ת��λ");
					   if((temp162&0x0020)==0x0020)                      //�Ƕȷ���λ
				   { 
					   printf("ת�Ƿ���λ");
					   if(PA5==1)           //������ʹ�ܣ�����λ ��0ʹ��
						{
							GPIO_ResetBits(GPIOA,GPIO_Pin_5);
							PA6=!PA6;
							Output_Pulse(angel_re);
						}else 
						{
							PA6=!PA6;
							Output_Pulse(angel_re);
						}
				   }
						 else
				   {
							if(PA5==1)
							{
								PA6=!PA6;
								GPIO_ResetBits(GPIOA,GPIO_Pin_5);
								Output_Pulse(angel_re);
							}
							else 
							{
								PA6=!PA6;
								Output_Pulse(angel_re);	
							}
				   }
				  for(i=0;i<20000;++i)
				   {
						 printf("\r\nδ��λ\r\n");
						  if(PE3==0)   //Ӧ����ת��λ������
						{
							TIM_Cmd(TIM2, DISABLE);
							TIM_Cmd(TIM3, DISABLE);
							angel_re=0;
									printf("\r\n�Ѹ�λ\r\n");
									break;
						}
				   
				  }
			}
			 
		    if((temp162&0x0080)==0x0080)	                    //ɲ��
			 {
					 
				 statue1 = GPIO_ReadOutputDataBit(GPIOF,  GPIO_Pin_5);
				 if(statue1!=1){					  		
					   GPIO_WriteBit( GPIOF,GPIO_Pin_5,  Bit_SET);  //�����ǰ����ɲ�� ����ɲ��λ ��1 ��ɲ��
					   statue1=1;
				}    
					  printf("\r\nɲ��\r\n");
				 statue1 = GPIO_ReadOutputDataBit(GPIOF,  GPIO_Pin_5);
					  printf("ɲ���ĵ�ǰ״̬��%d",statue1); 
			 }
			 else {	
                     statue1 = GPIO_ReadOutputDataBit(GPIOF,  GPIO_Pin_5);				 //��ɲ��
			  		 if(statue1==1) { 
					 GPIO_WriteBit( GPIOF,GPIO_Pin_5,  Bit_RESET);  //�����ǰ��ɲ������ɲ��λ��0             
				 }
			 }
            if((temp162&0x0100)==0x0100)
				{ 			                                    //�Լ�
				 selfcheck(); 
				}
			if((temp162&0x0200)==0x0200)                       //����        ����������
			 {
					DISU=!DISU;
				
				 printf("���ٵĵ�ǰ״̬��%d",statue2);
					printf("\r\n���ٵ�\r\n"); 
				
			 }	
			if((temp162&0x0400)==0x0400)                       //����        ��������������
			 {
					GAOSU=!GAOSU;
				 
				 
					printf("\r\n���ٵ�\r\n");
			 }   			
			if((temp162&0x0800)==0x0800)                       //��ʹ��       ������������������
			 {
				    statue2 = GPIO_ReadOutputDataBit(GPIOA,  GPIO_Pin_5);
				    printf(" ʹ�ܵĵ�ǰ״̬��%d",statue2); 
					GPIO_SetBits(GPIOA,GPIO_Pin_5);		       //��λ1
					printf("\r\nʹ���Ѷ�\r\n");
				     printf(" ʹ�ܵĵ�ǰ״̬��%d",statue2); 
			 }            				             
            if((temp161&0x2000)==0x2000)                        //ǰ��/����
			 {	
				 statue5 = GPIO_ReadOutputDataBit(GPIOF,  GPIO_Pin_7);
				 printf("�����ĵ�ǰ״̬��%d",statue5);
				 
						DAOCHE=!DAOCHE;		
					
					delay_ms(500);
					printf("\r\n����\r\n");				
			 }	
			if((temp161&0x1000)==0x1000)	                    //�����������ٶ�ֵ
			 {
				    printf("\r\n����\r\n");
				    carSpeed=Hex_Dec(temp161&0x0fff);                 
			        DAC_SetChannel1Data(DAC_Align_12b_R, carSpeed);		//�����ٶ�ֵ
		            printf("\r\n�ٶ�ֵΪ��%d\r\n",carSpeed);				
			 }                  					
			if((temp161&0x4000)==0x4000)	                    //����
			 {
				    
				 statue6 = GPIO_ReadOutputDataBit(GPIOF,  GPIO_Pin_4);
				 printf("���ѵĵ�ǰ״̬��%d",statue6);
				 if(statue6==0)    {}     //�����ǰ�����ѣ��򲻲���
					 else {
						BEEP=!BEEP;		//�����ǰ�������ѣ�������λȡ��
					 }
                    printf("\r\n����\r\n");				
			 }
			if((temp161&0x8000)==0x8000)                        //ǰ���
			 {
				 
				 statue7 = GPIO_ReadOutputDataBit(GPIOF,  GPIO_Pin_1);
				
						 LED1=!LED1;		
				    printf("\r\nǰ���\r\n");				
			 }								
    	    if((temp160&0x1000)==0x1000)	                    //����
			 {
		            LED2=!LED2; 
				 statue8 = GPIO_ReadOutputDataBit(GPIOF,  GPIO_Pin_2);
				 printf("�����ĵ�ǰ״̬��%d",statue8);
				 
								
		
			        printf("\r\n������\r\n");				
			 }
			if((temp160&0x2000)==0x2000)	                    //����
			{
		            LED3=!LED3;
				statue9 = GPIO_ReadOutputDataBit(GPIOF,  GPIO_Pin_3);
				 printf("�����ĵ�ǰ״̬statue��%d",statue9);
				  if(statue9==0)    {}     //�����ǰ��ǰ��������򲻲���
					 else if(statue8==1){
						 LED2=!LED2;		//�����ǰǰ��Ʋ�������ǰ���λȡ��
					 }
			        printf("\r\n������\r\n");				
			}			
            if((temp160&0x4000)==0x4000)	                    //��ת
			{
                    printf("\r\n��ת\r\n");
					GPIO_ResetBits(GPIOA,GPIO_Pin_6);		
				if(PE3==0)
					Rangel=0;					
				if(carSpeed>=2000)			
				{
					DAC_SetChannel1Data(DAC_Align_12b_R, 1800);
					printf("\r\n��ǰ�ٶ�ֵ��%d\r\n",1800);
				}								    
                else
                    DAC_SetChannel1Data(DAC_Align_12b_R, 1600);							
				
				angel_hex=temp160&0x001f;                       //�Ƕ�
				angel=Hex_Dec(angel_hex);
				printf("\r\nת��Ƕ�ֵ��%d��\r\n",angel);
				
				rot_speed_hext=temp162&0x0fff;	            		//ת��		
				rot_speed=Hex_Dec(rot_speed_hext);              	
				Frequence_Setting(rot_speed);		
                rot_speed=(1e6)/rot_speed;				
				printf("\r\nƵ�ʣ�%dHz\r\n",rot_speed);
							
				angel=angel/0.0225;                            //������
				Output_Pulse(angel);  
				Rangel=angel+Rangel;
			}
            if((temp160&0x8000)==0x8000)	                    //��ת
			{

                   printf("\r\n��ת\r\n");	
				   GPIO_SetBits(GPIOA,GPIO_Pin_6);	
				if(PE3==0)
					Langel=0;					
				if(carSpeed>=2000)			
				{
					DAC_SetChannel1Data(DAC_Align_12b_R, 1800);
					printf("\r\n��ǰ�ٶ�ֵ��%d\r\n",1800);
				}								    
				else
				    DAC_SetChannel1Data(DAC_Align_12b_R, 1600);	
						
						angel_hex=temp160&0x001f;                       //�Ƕ�
						angel=Hex_Dec(angel_hex);
						printf("\r\nת��Ƕ�ֵ��%d��\r\n",angel);
						
						rot_speed_hext=temp162&0x0fff;	            		//ת��		
						rot_speed=Hex_Dec(rot_speed_hext);              	
						Frequence_Setting(rot_speed);		
				        rot_speed=(1e6)/rot_speed;				
						printf("\r\nƵ�ʣ�%dHz\r\n",rot_speed);
						angel=angel/0.0225;                            //������

						Output_Pulse(angel);
						Langel=angel+Langel;
			}			
					printf("\r\n\r\n");//���뻻��
					USART_RX_STA=0;
	    }    //�������� if��������
	}     // while()����
}     //main��������

