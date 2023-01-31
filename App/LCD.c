#include "LCD.h"
#include "LCDfont.h"  
#include "stdlib.h"

uint8_t LCD_GRAM[128][8];   // ����LCD��ʾ����

/**
	* @brief	������ʾ��LCD
	* @param	None
	* @return	None
	*/
void LCD_Refresh_Gram(void)
{
    uint8_t i,n;
    for(i = 0; i < 8; i ++ )
    {
    LCD_WR_Byte (0xb0+i,LCD_CMD);    //����ҳ��ַ��0~7��
		LCD_WR_Byte (0x00,LCD_CMD);      //������ʾλ�á��е͵�ַ
		LCD_WR_Byte (0x10,LCD_CMD);      //������ʾλ�á��иߵ�ַ  
		for(n=0;n<128;n++)
        {
            LCD_WR_Byte(LCD_GRAM[n][i],LCD_DATA);
        }
    }
}

/**
	* @brief	��ST7567д��һ���ֽ�
	* @param	dat:Ҫд�������
	* @param	cmd:����/�����־;	0-����;	1-����
	* @return	None
	*/
void LCD_WR_Byte(uint8_t dat,uint8_t cmd)
{	
	uint8_t i;			  
	if(cmd)
	{
		LCD_DC_Set();
	}
	else 
	{
		LCD_DC_Clr();	
	}        
	LCD_CS_Clr();
	for(i=0;i<8;i++)
	{			  
		LCD_SCLK_Clr();
		if(dat&0x80)
		{
			LCD_SDIN_Set();
		}
		else 
		{
			LCD_SDIN_Clr();
		}
		LCD_SCLK_Set();
		dat<<=1;   
	}				 		  
	LCD_CS_Set();
	LCD_DC_Set();   	  
} 

/**
	* @brief	������ʾ����
	* @param	None
	* @return	None
	*/
void LCD_Display_On(void)
{
	LCD_WR_Byte(0XAF,LCD_CMD);  //DISPLAY ON
}

/**
	* @brief	�ر���ʾ����
	* @param	None
	* @return	None
	*/
void LCD_Display_Off(void)
{
	LCD_WR_Byte(0XAE,LCD_CMD);  //DISPLAY OFF
}		   		

/**
	* @brief	��������
	* @param	None
	* @return	None
	*/
void LCD_Clear(void)  
{  
	uint8_t i,n;  
	for(i=0;i<8;i++)for(n=0;n<128;n++)LCD_GRAM[n][i]=0X00;  
	LCD_Refresh_Gram();//������ʾ
}

/**
	* @brief	���㺯��
	* @param	X�������꣨0~127��
	* @param	Y�������꣨0~63��
    * @param  T��1-��䣻0-���
	* @return	
	*/
void LCD_DrawPoint(uint8_t x,uint8_t y,uint8_t t)
{
	uint8_t pos,bx,temp=0;
	if(x>127||y>63)return;//������Χ��.
	pos=7-y/8;
	bx=y%8;
	temp=1<<(7-bx);
	if(t)LCD_GRAM[x][pos]|=temp;
	else LCD_GRAM[x][pos]&=~temp;	    
}

/**
	* @brief	���ָ������
	* @param	X1/X2��X1 < X2��0 <= X1/2 <= 127
	* @param	Y1/Y2��Y1 < Y2��0 <= Y1/2 <= 63
    * @param  dot��0-��գ�1-���
	* @return	None
	*/
void LCD_Fill(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t dot)  
{  
	uint8_t x,y;  
	for(x=x1;x<=x2;x++)
	{
		for(y=y1;y<=y2;y++)LCD_DrawPoint(x,y,dot);
	}													    
	LCD_Refresh_Gram();//������ʾ
} 

/**
	* @brief	��ָ��λ����ʾһ���ַ������������ַ�
	* @param	X��0~127
	* @param	Y��0~63
	* @param	chr��Ҫ��ʾ���ַ�
	* @param	size��ѡ������-16/121
	* @param	mode��0-������ʾ��1-������ʾ
	* @return	None
	*/
void LCD_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t size,uint8_t mode)
{      			    
	uint8_t temp,t,t1;
	uint8_t y0=y;
	chr=chr-' ';//�õ�ƫ�ƺ��ֵ				   
    for(t=0;t<size;t++)
    {   
		if(size==12)temp=oled_asc2_1206[chr][t];  //����1206����
		else temp=oled_asc2_1608[chr][t];		 //����1608���� 	                          
        for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)LCD_DrawPoint(x,y,mode);
			else LCD_DrawPoint(x,y,!mode);
			temp<<=1;
			y++;
			if((y-y0)==size)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
	}
	LCD_Refresh_Gram();		
}

uint32_t LCD_pow(uint8_t m,uint8_t n)
{
  uint32_t result=1;	 
	while(n--)result*=m;    
	return result;
}				  

/**
	* @brief	��ʾ2������
	* @param	X�����X����
	* @param	Y�����Y����
	* @param	num����ֵ��0~4294967295��
	* @param	len�����ֵ�λ��
	* @param	size�������С-16/12
	* @return	None
	*/
void LCD_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size)
{         	
	uint8_t t,temp;
	uint8_t enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/LCD_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				LCD_ShowChar(x+(size/2)*t,y,' ',size,1);
				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,1); 
	}
} 

/**
	* @brief	��ʾ�ַ�����16����
	* @param	X�����X����
	* @param	Y�����Y����
	* @param	*p���ַ���
	* @return	
	*/
void LCD_ShowString(uint8_t x,uint8_t y,const uint8_t *p)
{
	#define MAX_CHAR_POSX 122
	#define MAX_CHAR_POSY 58          
	while(*p!='\0')
	{       
		if(x>MAX_CHAR_POSX){x=0;y+=16;}
		if(y>MAX_CHAR_POSY){y=x=0;LCD_Clear();}
		LCD_ShowChar(x,y,*p,16,1);	 
		x+=8;
		p++;
	}  
}	   

/**
	* @brief	��������д�����ʼλ��
	* @param	X�����X����
	* @param	Y�����Y����
	* @return	None
	*/
void LCD_Set_Pos(unsigned char x, unsigned char y) 
{ 
    y = 7 - y;
	LCD_WR_Byte(0xb0+y,LCD_CMD);
	LCD_WR_Byte(((x&0xf0)>>4)|0x10,LCD_CMD);
	LCD_WR_Byte((x&0x0f)|0x01,LCD_CMD); 
} 

/**
	* @brief	��ʾ����
	* @param	X�����X����
	* @param	Y�����Y����
	* @param	no��Ҫ��ʾ�ĺ��ֵı��
	* @return	None
	*/
void LCD_ShowCHinese(uint8_t x,uint8_t y,uint8_t no)
{      			    
	uint8_t t;
	LCD_Set_Pos(x,y);	
  for(t=0;t<16;t++)
	{
    LCD_WR_Byte(Hzk[no][t],LCD_DATA);
  }	
	LCD_Set_Pos(x,y+1);
  for(t=0;t<16;t++)
	{	
    LCD_WR_Byte(Hzk[no][t+16],LCD_DATA);
  }
}

/**
	* @brief	��ʾBMPͼƬ128X64
	* @param	X0�����X����
	* @param	Y0�����Y����
	* @param	X1���յ�X����
	* @param	Y1���յ�Y����
	* @param	BMP[]��ͼƬ���
	* @return	None
	*/
void LCD_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[])
{ 	
	unsigned int j=0;
	unsigned char x,y;
	if(y1%8==0) y=y1/8;      
	else y=y1/8+1;
	for(y=y0;y<y1;y++)
	{
		LCD_Set_Pos(x0,y);
		for(x=x0;x<x1;x++)
		{      
			LCD_WR_Byte(BMP[j++],LCD_DATA);	    	
		}
	}
}

/**
	* @brief	LCDҺ����ʾ����ʼ������
	* @param	None
	* @return	None
	*/
void LCD_Init(void)
{ 	
 	GPIO_InitTypeDef  GPIO_InitStructure;
 	
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_5;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		      // �������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;        // �ٶ�10MHz
 	GPIO_Init(GPIOB, &GPIO_InitStructure);	                 // ��ʼ��GPIOA
  
 	GPIO_SetBits(GPIOB,GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_10 |GPIO_Pin_5|GPIO_Pin_11);	             // PA1,PA2 �����
 
  LCD_RST_Clr();
	delay_ms(100);
	LCD_RST_Set(); 

  delay_ms(5);    
	LCD_WR_Byte(0xe2,LCD_CMD);	 /*��λ*/
  delay_ms(5); 
	LCD_WR_Byte(0x2c,LCD_CMD);	 /*��λ*/
  delay_ms(5); 
	LCD_WR_Byte(0x2e,LCD_CMD);	 /*��λ*/
  delay_ms(5); 
	LCD_WR_Byte(0xa0,LCD_CMD);  /*��ɨ��˳��a0������*/
//	LCD_WR_Byte(0xa1,LCD_CMD);  /*��ɨ��˳��a0������*/
//	LCD_WR_Byte(0xc8,LCD_CMD);  /*��ɨ��˳��c8���µ��ϣ�c0���ϵ���*/
	LCD_WR_Byte(0xc0,LCD_CMD);  /*��ɨ��˳��c8���µ��ϣ�c0���ϵ���*/
	
	LCD_WR_Byte(0xa2,LCD_CMD);	 /*����ƫѹ��1/9*/
	LCD_WR_Byte(0x2f,LCD_CMD);	/*���Ƶ�Դ*/
	LCD_WR_Byte(0x25,LCD_CMD);	/*�ֵ��Աȶ�*/
	LCD_WR_Byte(0x81,LCD_CMD);  /*΢���Աȶ�,����΢���Աȶ�����*/
	LCD_WR_Byte(0x10,LCD_CMD);  /*���õ�ѹ�Ĳ���RRֵ*/
	LCD_WR_Byte(0x40,LCD_CMD);  /*��ʼ�У���һ�п�ʼ*/
//	LCD_WR_Byte(0x7F,LCD_CMD);  /*��ʼ�У���8�п�ʼ*/
	LCD_WR_Byte(0xaf,LCD_CMD);  /*����ʾ*/
	
	LCD_Fill(0,0,127,63,0);
	LCD_Clear();
}  
