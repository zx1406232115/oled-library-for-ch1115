#include "OLED_Driver.h"

//#include <intrins.h>
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Pin Definition
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
extern I2C_HandleTypeDef hi2c2;
//#define RES3	P3_0				// Reset
#define SCL3H	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,GPIO_PIN_SET)
#define SCL3L	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,GPIO_PIN_RESET)	
#define SDA3H	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_11,GPIO_PIN_SET)
#define SDA3L	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_11,GPIO_PIN_RESET)
#define RESH	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8,GPIO_PIN_SET)
#define RESL	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8,GPIO_PIN_RESET)					
uint8_t OLED_DisplayBuf[64/8][128];
bool OLED_ColorMode = true;
void 	delay_ms(Uint t)
{
	Uchar i,j;
	while(--t)
	{
	i=50;
	j=100;

		while(--j)
			while(--i);
	}
}

//****************************************************************************
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Read/Write Sequence
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void I2C_3(Uchar mcmd)
{
Uchar length = 8;			// Send Command

	while(length--)
	{
		if(mcmd & 0x80)
		{
			SDA3H;
		}
		else
		{
			SDA3L;
		}
		SCL3H;
		SCL3L;
		mcmd = mcmd << 1;
	}
}

void I2C_Ack3(void)
{
	SDA3H;
	SCL3H;
	SCL3L;
}

void I2C_Start3(void)
{
	SDA3L;
	SCL3H;
	SCL3L;
	I2C_3(0x78);
	I2C_Ack3();
}

void I2C_Stop3(void)
{
	SCL3H;
	SDA3L;
	SDA3H;
}

void Write_Command3(Uchar Data)
{
	I2C_Start3();
	I2C_3(0x00);
	I2C_Ack3();
	I2C_3(Data);
	I2C_Ack3();
	I2C_Stop3();
}
//硬件i2c
void Write_Command2(unsigned char Data){
	unsigned char pdata[2]={0x00,Data};
	HAL_I2C_Master_Transmit(&hi2c2, 0x78, pdata, 2, HAL_MAX_DELAY);
	// unsigned char pdata=0x00;
	// HAL_I2C_Master_Transmit(&hi2c2, 0x78, &pdata, 2, HAL_MAX_DELAY);
	// HAL_I2C_Master_Transmit(&hi2c2, 0x78, &Data, 2, HAL_MAX_DELAY);
}

void Write_Data2(unsigned char Data){
	unsigned char pdata[2]={0x40,Data};
	HAL_I2C_Master_Transmit(&hi2c2, 0x78, pdata, 2, HAL_MAX_DELAY);
	// unsigned char pdata=0x40;
	// HAL_I2C_Master_Transmit(&hi2c2, 0x78, &pdata, 2, HAL_MAX_DELAY);
	// HAL_I2C_Master_Transmit(&hi2c2, 0x78, &Data, 2, HAL_MAX_DELAY);
}
void Write_Data(Uchar Data)
{
	I2C_Start3();
	I2C_3(0x40);
	I2C_Ack3();
	I2C_3(Data);
	I2C_Ack3();
	I2C_Stop3();
}
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

void Column_set(unsigned char column)
{
Write_Command2(0x10|(column>>4));
Write_Command2(0x00|(column&0x0f));	 
}
void Page_set(unsigned char page)
{
Write_Command2(0xb0+page);
}
//***********************************************************************
//黑屏
void clr_display3(void)
{
	Uchar i,j;
	for(j=0;j<6;j++)
	{
		Page_set(j);
        Column_set(40);
 		for(i=0;i<88;i++)		  				//显示单色数据到LCD
		{
			Write_Data2(0x00);
		}
	}
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//白屏
void ff_display3(void)
{
	Uchar i,j;
	for(j=0;j<6;j++)
	{
		Page_set(j);
        Column_set(40);
 		for(i=0;i<88;i++)		  				//显示单色数据到LCD
		{
			Write_Data2(0xFF);
		}
	}
}
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

void disA5(void)
{
	Uchar i,j;
	for(j=0;j<6;j++)
	{
		Page_set(j);
		Column_set(40);

 		for(i=0;i<44;i++)
		{
			Write_Data2(0xAA);
			Write_Data2(0x55);
		}
	}
}
/**
  * 函    数：OLED写数据
  * 参    数：Data 要写入数据的起始地址
  * 参    数：Count 要写入数据的数量
  * 返 回 值：无
  */
  void OLED_WriteData(uint8_t *Data, uint8_t Count)
  {
	  uint8_t i;
	  unsigned char pdata[129]={0};
	  pdata[0]=0x40;
	  /*循环Count次，进行连续的数据写入*/
	  for (i = 0; i < Count; i ++)
	  {
		if(OLED_ColorMode){
			pdata[i+1]=Data[i];
		}else{
			pdata[i+1]=~Data[i];
		}
		
	  }
	  HAL_I2C_Master_Transmit(&hi2c2, 0x78, pdata, Count+1, HAL_MAX_DELAY);
  }
  
/**
  * 函    数：OLED设置显示光标位置
  * 参    数：Page 指定光标所在的页，范围：0~7
  * 参    数：X 指定光标所在的X轴坐标，范围：0~127
  * 返 回 值：无
  * 说    明：OLED默认的Y轴，只能8个Bit为一组写入，即1页等于8个Y轴坐标
  */
  void OLED_SetCursor(uint8_t Page, uint8_t X)
  {
	  /*如果使用此程序驱动1.3寸的OLED显示屏，则需要解除此注释*/
	  /*因为1.3寸的OLED驱动芯片（SH1106）有132列*/
	  /*屏幕的起始列接在了第2列，而不是第0列*/
	  /*所以需要将X加2，才能正常显示*/
  //	X += 2;
	  
	  /*通过指令设置页地址和列地址*/
	  Page_set(Page);
	  Column_set(X);
  }
void OLED_Update(void)
{
	uint8_t j;
	/*遍历每一页*/
	for (j = 0; j < 8; j ++)
	{
		/*设置光标位置为每一页的第一列*/
		OLED_SetCursor(j, 0);
		/*连续写入128个数据，将显存数组的数据写入到OLED硬件*/
		OLED_WriteData(OLED_DisplayBuf[j], 128);
	}
	
	
}
void OLED_Init(void)
{
	
	/*写入一系列的命令，对OLED进行初始化配置*/
	Write_Command2(0xAE); /*display off*/
	Write_Command2(0xC8); /*Com scan direction*/
	Write_Command2(0xA1); /*set segment remap*/

	Write_Command2(0x00); /*set lower column address*/
	Write_Command2(0x10); /*set higher column address*/
	Write_Command2(0x40); /*set display start line*/
	Write_Command2(0xb0); /*set page address*/
	Write_Command2(0x81); /*contract control*/
	Write_Command2(0x80); /*128*/
	Write_Command2(0x82); /*IREF 内置*/
	Write_Command2(0x00); /* */
	Write_Command2(0x23);
	Write_Command2(0x01); /* */

	Write_Command2(0xA2);
	Write_Command2(0xA4);
	Write_Command2(0xA6); /*normal / reverse*/
	Write_Command2(0xA8); /*multiplex ratio*/
	Write_Command2(0x2F); /*duty*/

	Write_Command2(0xD3); /*set display offset*/
	Write_Command2(0x08);
	Write_Command2(0xD5); /*set osc division*/
	Write_Command2(0x50);
	Write_Command2(0xD9); /*set pre-charge period*/
	Write_Command2(0x22);
	Write_Command2(0xdb); /*set vcomh*/
	Write_Command2(0x35);
	Write_Command2(0xAd); /*set charge pump enable*/
	Write_Command2(0x8B);
	Write_Command2(0x33); /*9v*/
	Write_Command2(0xAF); /*display ON*/
	
	OLED_Update();				//更新显示，清屏，防止初始化后未显示内容时花屏
}
void OLED_SetColorMode(bool colormode){
	OLED_ColorMode = colormode;
}
/**
  * 函    数：将OLED显存数组部分更新到OLED屏幕
  * 参    数：X 指定区域左上角的横坐标，范围：0~OLED_WIDTH-1
  * 参    数：Y 指定区域左上角的纵坐标，范围：0~OLED_HEIGHT-1
  * 参    数：Width 指定区域的宽度，范围：0~OLED_WIDTH
  * 参    数：Height 指定区域的高度，范围：0~OLED_HEIGHT
  * 返 回 值：无
  * 说    明：此函数会至少更新参数指定的区域
  *           如果更新区域Y轴只包含部分页，则同一页的剩余部分会跟随一起更新
  * 说    明：所有的显示函数，都只是对OLED显存数组进行读写
  *           随后调用OLED_Update函数或OLED_UpdateArea函数
  *           才会将显存数组的数据发送到OLED硬件，进行显示
  *           故调用显示函数后，要想真正地呈现在屏幕上，还需调用更新函数
  */
  void OLED_UpdateArea(uint8_t X, uint8_t Y, uint8_t Width, uint8_t Height)
  {
	  uint8_t j;
	  
	  /*参数检查，保证指定区域不会超出屏幕范围*/
	  if (X > 128-1) {return;}
	  if (Y > 64-1) {return;}
	  if (X + Width > 128) {Width = 128 - X;}
	  if (Y + Height > 64) {Height = 64 - Y;}
	  
	  /*遍历指定区域涉及的相关页*/
	  /*(Y + Height - 1) / 8 + 1的目的是(Y + Height) / 8并向上取整*/
	  for (j = Y / 8; j < (Y + Height - 1) / 8 + 1; j ++)
	  {
		  /*设置光标位置为相关页的指定列*/
		  OLED_SetCursor(j, X);
		  /*连续写入Width个数据，将显存数组的数据写入到OLED硬件*/
		  OLED_WriteData(&OLED_DisplayBuf[j][X], Width);
	  }
	  
  }
  void OLED_Brightness(int16_t Brightness){
	if(Brightness>255){
		Brightness=255;
	}
	if(Brightness<0){
		Brightness=0;
	}
	Write_Command2(0x81);
	Write_Command2(Brightness);
}
