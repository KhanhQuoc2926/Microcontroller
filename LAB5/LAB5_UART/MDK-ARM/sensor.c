#include "sensor.h"
extern ADC_HandleTypeDef hadc1;

/* Co chan doan: 1 neu da can nhanh tuong thich model, khong phai co bao loi. */
volatile uint8_t sensor_calibration_compat_used;

HAL_StatusTypeDef Sensor_Init(uint8_t allow_proteus_compat)
{
    sensor_calibration_compat_used = 0;
    HAL_StatusTypeDef status = HAL_ADCEx_Calibration_Start(&hadc1);
    if (status == HAL_OK) return HAL_OK;

    /* Da xac nhan tren model: RSTCAL khong tu xoa, nhung CAL van hoan tat.
     * Chi ap dung khi nguoi dung chon che do Proteus va dung loi RSTCAL.
     * Board that: allow_proteus_compat = 0, tra loi HAL de xu ly dung loi that. */
    if (!allow_proteus_compat || !(hadc1.Instance->CR2 & ADC_CR2_RSTCAL)) {
        return status;
    }
    sensor_calibration_compat_used = 1;
    CLEAR_BIT(hadc1.Instance->CR2, ADC_CR2_RSTCAL);
    SET_BIT(hadc1.Instance->CR2, ADC_CR2_CAL);

    /* Van phai cho CAL tu xoa; tuyet doi khong tu xoa CAL de gia thanh cong. */
    uint32_t start = HAL_GetTick();
    while (hadc1.Instance->CR2 & ADC_CR2_CAL) {
        if ((uint32_t)(HAL_GetTick() - start) >= 20u) {
            return HAL_TIMEOUT;
        }
    }
    /* Phuc hoi state HAL sau khi model da ket thuc CAL thanh cong. */
    ADC_STATE_CLR_SET(hadc1.State,
                     HAL_ADC_STATE_BUSY_INTERNAL | HAL_ADC_STATE_ERROR_INTERNAL,
                     HAL_ADC_STATE_READY);
    return HAL_OK;
}

/* value la dia chi bien nhan ket qua. Chi ghi ket qua neu do thanh cong. */
HAL_StatusTypeDef Sensor_ReadOnce(uint32_t *value){
	/* Bat dau mot phep do PA0 / ADC1_IN0 o che do software start. */
	HAL_StatusTypeDef status = HAL_ADC_Start(&hadc1);
	if(status != HAL_OK) return status;
	
	status = HAL_ADC_PollForConversion(&hadc1, 10);			// Cho ket qua toi da 10 ms, khong phai chu ky lay mau.
	if(status == HAL_OK){
		
		*value = HAL_ADC_GetValue(&hadc1); // ADC 12 bit: gia tri tu 0 den 4095.
	}
	/* Dung ADC ca khi poll loi; ben goi kiem tra status de biet ket qua. */
	(void)HAL_ADC_Stop(&hadc1);
	return status;
}