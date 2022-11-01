#include "oled_driver.h"
#include "main.h"
#include "spi.h"



#define OLED_RESET_LOW()					HAL_GPIO_WritePin(SPI_RES_PORT,SPI_RES_PIN, GPIO_PIN_RESET)	//低电平复位
#define OLED_RESET_HIGH()					HAL_GPIO_WritePin(SPI_RES_PORT,SPI_RES_PIN, GPIO_PIN_SET)
	 
#define OLED_CMD_MODE()						HAL_GPIO_WritePin(SPI_DC_PORT,SPI_DC_PIN,GPIO_PIN_RESET) 		//命令模式
#define OLED_DATA_MODE()					HAL_GPIO_WritePin(SPI_DC_PORT,SPI_DC_PIN,GPIO_PIN_SET)		//数据模式

#define OLED_CS_HIGH()   					HAL_GPIO_WritePin(SPI_CS_PORT,SPI_CS_PIN,GPIO_PIN_SET)
#define OLED_CS_LOW()  		  			HAL_GPIO_WritePin(SPI_CS_PORT,SPI_CS_PIN,GPIO_PIN_RESET)
	


void WriteCmd(unsigned char cmd)
{	
	OLED_CMD_MODE();
	OLED_CS_LOW();
	
	HAL_SPI_Transmit_DMA(&hspi1,&cmd,1);
	
	OLED_CS_HIGH();
	OLED_DATA_MODE();
}

void WriteDat(unsigned char dat)
{
	OLED_DATA_MODE();
	OLED_CS_LOW();
	HAL_SPI_Transmit_DMA(&hspi1,&dat,1);
	OLED_CS_HIGH();
	OLED_DATA_MODE();
}

void OLED_FILL(unsigned char BMP[])
{
	uint8_t i,j;
	unsigned char *p;
	p=BMP;

  for(i=0;i<8;i++)
	{
		WriteCmd(0xb0+i);		//page0-page1
		WriteCmd(0x00);		//low column start address
		WriteCmd(0x10);	

		for(j=0;j<128;j++)
		{
			WriteDat(*p++);
		}
	}
}


void OLED_Init(void)
{
	OLED_RESET_LOW();
	HAL_Delay(200);
	OLED_RESET_HIGH(); 
	
	
	WriteCmd(0xAE); //display off
	WriteCmd(0x20);	//Set Memory Addressing Mode	
	WriteCmd(0x10);	//00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
	WriteCmd(0xb0);	//Set Page Start Address for Page Addressing Mode,0-7
	WriteCmd(0xc8);	//Set COM Output Scan Direction
	WriteCmd(0x00); //---set low column address
	WriteCmd(0x10); //---set high column address
	WriteCmd(0x40); //--set start line address
	WriteCmd(0x81); //--set contrast control register
	WriteCmd(0xff); //亮度调节 0x00~0xff
	WriteCmd(0xa1); //--set segment re-map 0 to 127
	WriteCmd(0xa6); //--set normal display
	WriteCmd(0xa8); //--set multiplex ratio(1 to 64)
	WriteCmd(0x3F); //
	WriteCmd(0xa4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
	WriteCmd(0xd3); //-set display offset
	WriteCmd(0x00); //-not offset
	WriteCmd(0xd5); //--set display clock divide ratio/oscillator frequency
	WriteCmd(0xf0); //--set divide ratio
	WriteCmd(0xd9); //--set pre-charge period
	WriteCmd(0x22); //
	WriteCmd(0xda); //--set com pins hardware configuration
	WriteCmd(0x12);
	WriteCmd(0xdb); //--set vcomh
	WriteCmd(0x20); //0x20,0.77xVcc
	WriteCmd(0x8d); //--set DC-DC enable
	WriteCmd(0x14); //
	WriteCmd(0xaf); //--turn on oled panel
	OLED_CLS();
}


void OLED_CLS(void)//清屏
{
	unsigned char m,n;
	for(m=0;m<8;m++)
	{
		WriteCmd(0xb0+m);		//page0-page1
		WriteCmd(0x00);		//low column start address
		WriteCmd(0x10);		//high column start address
		for(n=0;n<128;n++)
		{
				WriteDat(0x00);
		}
	}
}

void OLED_ON(void)
{
	WriteCmd(0X8D);  //设置电荷泵
	WriteCmd(0X14);  //开启电荷泵
	WriteCmd(0XAF);  //OLED唤醒
}



void OLED_OFF(void)
{
	WriteCmd(0X8D);  //设置电荷泵
	WriteCmd(0X10);  //关闭电荷泵
	WriteCmd(0XAE);  //OLED休眠
}



