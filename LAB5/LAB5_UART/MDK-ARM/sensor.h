#ifndef SENSOR_H
#define SENSOR_H
#include "main.h"
/* Khoi tao ADC: cho phep nhanh tuong thich chi khi mo phong Proteus. */
HAL_StatusTypeDef Sensor_Init(uint8_t allow_proteus_compat);
extern volatile uint8_t sensor_calibration_compat_used;

/* Tra HAL_OK khi *value nhan duoc mau ADC moi. */
HAL_StatusTypeDef Sensor_ReadOnce(uint32_t *value);
#endif