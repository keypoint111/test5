/*
 * aht20.h
 *
 *  Created on: Apr 25, 2024
 *      Author: lenovo
 */

#ifndef INC_AHT20_H_
#define INC_AHT20_H_

#include "i2c.h"
 void AHT20_Init (void);

 void AHT20_Read(float *Temperature , float *Humidity);
#endif /* INC_AHT20_H_ */

