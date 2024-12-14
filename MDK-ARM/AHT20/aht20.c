#include "aht20.h"

#define AHT20_ADDRESS 0x70 // �ӻ���ַ
//AHT20 ����������
 void AHT20_Init () //AHT20��ʼ������  ��סҪ��"aht20.h"������
{
  uint8_t readBuffer;//���ڽ���״̬��Ϣ
  HAL_Delay(40);
  HAL_I2C_Master_Receive(&hi2c1, AHT20_ADDRESS, &readBuffer, 1, HAL_MAX_DELAY);//I2C��ȡ�����������ݺ��� readBuffer��ʱ�����һ���ֽڵ�״̬�֡�
  if((readBuffer & 0x08) == 0x00) //�жϵ���λ�Ƿ�Ϊ0 ����0xBE�����ʼ��
  {
	  uint8_t sendBuffer [3] = {0xBE , 0x08 , 0x00};
	  HAL_I2C_Master_Transmit(&hi2c1, AHT20_ADDRESS, sendBuffer, 3, HAL_MAX_DELAY);//I2C���ͺ���
  }

}

 void AHT20_Read(float *Temperature , float *Humidity) //AHT20��ȡ�¶�ʪ�Ⱥ���  ��סҪ��"aht20.h"������
{
	uint8_t sendBuffer [3] = {0xAC , 0x33 , 0x00};
	uint8_t readBuffer [6];
	HAL_I2C_Master_Transmit(&hi2c1, AHT20_ADDRESS, sendBuffer, 3, HAL_MAX_DELAY);
	HAL_Delay(75);
	HAL_I2C_Master_Receive(&hi2c1, AHT20_ADDRESS, readBuffer, 6, HAL_MAX_DELAY);
	if((readBuffer[0] & 0x80) == 0x00)
	{
		uint32_t date = 0;//������ʪ����Ҫ2�����ֽ� ����Ҫ32
		date = ((uint32_t )readBuffer[3] >> 4) + ((uint32_t )readBuffer[2] << 4) + ((uint32_t )readBuffer[1] << 12);//�����ݽ�����λƴ��.
		*Humidity = date * 100.0f / (1 << 20);//(1 << 20) ��Ϊ2��20�η�. ��100.0���Ա�ʾΪ�ٷ���

		date = (((uint32_t )readBuffer[3] & 0x0F)<< 16) + ((uint32_t )readBuffer[4] << 8) + (uint32_t )readBuffer[5];//& 0x0F: ������޷���������ʮ��������0x0F���а�λ�������0x0F�Ķ����Ʊ�ʾΪ00001111����������ᱣ��readBuffer[3]�ĵ���λ����������λ���㡣
		*Temperature = date * 200.0f / (1 << 20) - 50;
	}
}

