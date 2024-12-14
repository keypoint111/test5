#include "aht20.h"

#define AHT20_ADDRESS 0x70 // 从机地址
//AHT20 的驱动程序
 void AHT20_Init () //AHT20初始化函数  记住要在"aht20.h"中声明
{
  uint8_t readBuffer;//用于接收状态信息
  HAL_Delay(40);
  HAL_I2C_Master_Receive(&hi2c1, AHT20_ADDRESS, &readBuffer, 1, HAL_MAX_DELAY);//I2C读取函数，读数据函数 readBuffer此时获得了一个字节的状态字。
  if((readBuffer & 0x08) == 0x00) //判断第三位是否为0 发送0xBE命令初始化
  {
	  uint8_t sendBuffer [3] = {0xBE , 0x08 , 0x00};
	  HAL_I2C_Master_Transmit(&hi2c1, AHT20_ADDRESS, sendBuffer, 3, HAL_MAX_DELAY);//I2C发送函数
  }

}

 void AHT20_Read(float *Temperature , float *Humidity) //AHT20读取温度湿度函数  记住要在"aht20.h"中声明
{
	uint8_t sendBuffer [3] = {0xAC , 0x33 , 0x00};
	uint8_t readBuffer [6];
	HAL_I2C_Master_Transmit(&hi2c1, AHT20_ADDRESS, sendBuffer, 3, HAL_MAX_DELAY);
	HAL_Delay(75);
	HAL_I2C_Master_Receive(&hi2c1, AHT20_ADDRESS, readBuffer, 6, HAL_MAX_DELAY);
	if((readBuffer[0] & 0x80) == 0x00)
	{
		uint32_t date = 0;//接收温湿度需要2个半字节 所以要32
		date = ((uint32_t )readBuffer[3] >> 4) + ((uint32_t )readBuffer[2] << 4) + ((uint32_t )readBuffer[1] << 12);//对数据进行移位拼接.
		*Humidity = date * 100.0f / (1 << 20);//(1 << 20) 意为2的20次方. 乘100.0可以表示为百分数

		date = (((uint32_t )readBuffer[3] & 0x0F)<< 16) + ((uint32_t )readBuffer[4] << 8) + (uint32_t )readBuffer[5];//& 0x0F: 将这个无符号整数与十六进制数0x0F进行按位与操作。0x0F的二进制表示为00001111，这个操作会保留readBuffer[3]的低四位，即将高四位清零。
		*Temperature = date * 200.0f / (1 << 20) - 50;
	}
}

