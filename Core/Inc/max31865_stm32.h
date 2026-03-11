#ifndef MAX31865_STM32_H
#define MAX31865_STM32_H

#include "stm32f1xx_hal.h"
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// ---------- MAX31865 registers ----------
#define MAX31865_REG_CONFIG          0x00
#define MAX31865_REG_RTD_MSB         0x01  // 0x01 + 0x02 = 16-bit RTD data (LSB bit0 = fault)
#define MAX31865_REG_RTD_LSB         0x02
#define MAX31865_REG_HFT_MSB         0x03
#define MAX31865_REG_HFT_LSB         0x04
#define MAX31865_REG_LFT_MSB         0x05
#define MAX31865_REG_LFT_LSB         0x06
#define MAX31865_REG_FAULT_STATUS    0x07

// ---------- CONFIG bits ----------
#define MAX31865_CFG_BIAS            0x80
#define MAX31865_CFG_MODEAUTO        0x40
#define MAX31865_CFG_1SHOT           0x20
#define MAX31865_CFG_3WIRE           0x10
#define MAX31865_CFG_FAULTSTAT_CLR   0x02
#define MAX31865_CFG_FILT_50HZ       0x01
#define MAX31865_CFG_FILT_60HZ       0x00

// ---------- Fault Status bits ----------
#define MAX31865_FAULT_HIGHTHRESH    0x80
#define MAX31865_FAULT_LOWTHRESH     0x40
#define MAX31865_FAULT_REFINLOW      0x20
#define MAX31865_FAULT_REFINHIGH     0x10
#define MAX31865_FAULT_RTDINLOW      0x08
#define MAX31865_FAULT_OVUV          0x04

extern float RTD_NOMINAL_OHMS;
extern float RREF_OHMS;

typedef enum {
    MAX31865_WIRES_2 = 2,
    MAX31865_WIRES_3 = 3,
    MAX31865_WIRES_4 = 4
} max31865_wires_t;

typedef struct {
    SPI_HandleTypeDef *hspi;
    GPIO_TypeDef      *cs_port;
    uint16_t           cs_pin;

    max31865_wires_t   wires;
    bool               filter_50hz;

    uint16_t           bias_settle_ms;   // delay after enabling bias
    uint16_t           conv_wait_ms;     // delay for 1-shot conversion
} MAX31865_HandleTypeDef;

// ---- High-level API ----
HAL_StatusTypeDef MAX31865_Init(MAX31865_HandleTypeDef *dev,
                                SPI_HandleTypeDef *hspi,
                                GPIO_TypeDef *cs_port, uint16_t cs_pin,
                                max31865_wires_t wires,
                                bool filter_50hz);

HAL_StatusTypeDef MAX31865_ClearFault(MAX31865_HandleTypeDef *dev);
HAL_StatusTypeDef MAX31865_ReadFault(MAX31865_HandleTypeDef *dev, uint8_t *fault_status);

HAL_StatusTypeDef MAX31865_ReadRTDRaw(MAX31865_HandleTypeDef *dev, uint16_t *raw15);
float MAX31865_RTD_Resistance_Ohms(uint16_t raw15, float rref_ohms);

// Temperature conversion (Callendar–Van Dusen approach, similar to Adafruit)
float MAX31865_Temperature_C(uint16_t raw15, float rtd_nominal_ohms, float rref_ohms);

// ---- Low-level register access ----
HAL_StatusTypeDef MAX31865_Write8(MAX31865_HandleTypeDef *dev, uint8_t reg, uint8_t value);
HAL_StatusTypeDef MAX31865_ReadN(MAX31865_HandleTypeDef *dev, uint8_t reg, uint8_t *buf, uint16_t len);

#ifdef __cplusplus
}
#endif

#endif // MAX31865_STM32_H
