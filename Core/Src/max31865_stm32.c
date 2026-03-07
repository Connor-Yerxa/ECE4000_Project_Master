#include "max31865_stm32.h"
#include <math.h>

// IEC 751 / alpha=0.00385 coefficients for CallendarVan Dusen
#define RTD_A  (3.90830e-3f)
#define RTD_B  (-5.77500e-7f)

float RTD_NOMINAL_OHMS = 1000.0;
float RREF_OHMS = 4000.0;

// ----------------- helpers -----------------
static inline void cs_low(MAX31865_HandleTypeDef *dev) {
    HAL_GPIO_WritePin(dev->cs_port, dev->cs_pin, GPIO_PIN_RESET);
}

static inline void cs_high(MAX31865_HandleTypeDef *dev) {
    HAL_GPIO_WritePin(dev->cs_port, dev->cs_pin, GPIO_PIN_SET);
}

HAL_StatusTypeDef MAX31865_Write8(MAX31865_HandleTypeDef *dev, uint8_t reg, uint8_t value) {
    uint8_t tx[2];
    tx[0] = (uint8_t)(reg | 0x80u); // MSB=1 => write
    tx[1] = value;

    cs_low(dev);
    HAL_Delay(10);
    HAL_StatusTypeDef st = HAL_SPI_Transmit(dev->hspi, tx, 2, HAL_MAX_DELAY);
    cs_high(dev);
    return st;
}

HAL_StatusTypeDef MAX31865_ReadN(MAX31865_HandleTypeDef *dev, uint8_t reg, uint8_t *buf, uint16_t len) {
    uint8_t addr = (uint8_t)(reg & 0x7Fu); // MSB=0 => read

    cs_low(dev);
    HAL_StatusTypeDef st = HAL_SPI_Transmit(dev->hspi, &addr, 1, HAL_MAX_DELAY);
    if (st == HAL_OK) {
        st = HAL_SPI_Receive(dev->hspi, buf, len, HAL_MAX_DELAY);
    }
    cs_high(dev);
    return st;
}

static HAL_StatusTypeDef read8(MAX31865_HandleTypeDef *dev, uint8_t reg, uint8_t *v) {
    return MAX31865_ReadN(dev, reg, v, 1);
}

// ----------------- high-level -----------------
HAL_StatusTypeDef MAX31865_ClearFault(MAX31865_HandleTypeDef *dev) {
    uint8_t cfg = 0;
    HAL_StatusTypeDef st = read8(dev, MAX31865_REG_CONFIG, &cfg);
    if (st != HAL_OK) return st;

    // Per datasheet: write 1 to Fault Status Clear (D1)
    cfg |= MAX31865_CFG_FAULTSTAT_CLR;
    st = MAX31865_Write8(dev, MAX31865_REG_CONFIG, cfg);
    return st;
}

HAL_StatusTypeDef MAX31865_ReadFault(MAX31865_HandleTypeDef *dev, uint8_t *fault_status) {
    return read8(dev, MAX31865_REG_FAULT_STATUS, fault_status);
}

HAL_StatusTypeDef MAX31865_Init(MAX31865_HandleTypeDef *dev,
                                SPI_HandleTypeDef *hspi,
                                GPIO_TypeDef *cs_port, uint16_t cs_pin,
                                max31865_wires_t wires,
                                bool filter_50hz) {
    if (!dev || !hspi || !cs_port) return HAL_ERROR;

    dev->hspi          = hspi;
    dev->cs_port       = cs_port;
    dev->cs_pin        = cs_pin;
    dev->wires         = wires;
    dev->filter_50hz   = filter_50hz;

    // Conservative defaults
    dev->bias_settle_ms = 10;
    dev->conv_wait_ms   = filter_50hz ? 65 : 55; // 62.5ms (50Hz) or 52ms (60Hz), plus margin

    // Ensure CS idles high
    cs_high(dev);
    HAL_Delay(5);

    // Build configuration: normally-off, bias off, 1-shot only, set wiring + filter
    uint8_t cfg = 0;
    if (wires == MAX31865_WIRES_3) cfg |= MAX31865_CFG_3WIRE;
    if (filter_50hz)               cfg |= MAX31865_CFG_FILT_50HZ;
//    cfg |= MAX31865_CFG_BIAS;
    // (bias off, auto off)

    HAL_StatusTypeDef st = MAX31865_Write8(dev, MAX31865_REG_CONFIG, cfg);
    if (st != HAL_OK) return st;

    // Set wide thresholds (no false trips) and clear any latched faults
    // Threshold registers are 16-bit; easiest is two writes each
    st = MAX31865_Write8(dev, MAX31865_REG_LFT_MSB, 0x00);
    if (st != HAL_OK) return st;
    st = MAX31865_Write8(dev, MAX31865_REG_LFT_LSB, 0x00);
    if (st != HAL_OK) return st;
    st = MAX31865_Write8(dev, MAX31865_REG_HFT_MSB, 0xFF);
    if (st != HAL_OK) return st;
    st = MAX31865_Write8(dev, MAX31865_REG_HFT_LSB, 0xFF);
    if (st != HAL_OK) return st;

    st = MAX31865_ClearFault(dev);
    return st;
}

HAL_StatusTypeDef MAX31865_ReadRTDRaw(MAX31865_HandleTypeDef *dev, uint16_t *raw15) {
    if (!dev || !raw15) return HAL_ERROR;

    HAL_StatusTypeDef st;
    uint8_t cfg;

    // Clear old faults
    st = MAX31865_ClearFault(dev);
    if (st != HAL_OK) return st;

    // Enable bias
    st = read8(dev, MAX31865_REG_CONFIG, &cfg);
    if (st != HAL_OK) return st;
    cfg |= MAX31865_CFG_BIAS;
    cfg &= ~MAX31865_CFG_MODEAUTO; // ensure not in auto
    st = MAX31865_Write8(dev, MAX31865_REG_CONFIG, cfg);
    if (st != HAL_OK) return st;

    // Let input network settle (datasheet references RC settling after bias enable)
    HAL_Delay(dev->bias_settle_ms);

    // Start 1-shot conversion (self-clearing). Conversion begins after CS goes high.
    cfg |= MAX31865_CFG_1SHOT;
    st = MAX31865_Write8(dev, MAX31865_REG_CONFIG, cfg);
    if (st != HAL_OK) return st;

    HAL_Delay(dev->conv_wait_ms);

    // Read RTD MSB/LSB (16-bit)
    uint8_t buf[2] = {0};
    st = MAX31865_ReadN(dev, MAX31865_REG_RTD_MSB, buf, 2);
    if (st != HAL_OK) return st;

    uint16_t rtd16 = ((uint16_t)buf[0] << 8) | buf[1];

    // Disable bias to reduce self-heating
    st = read8(dev, MAX31865_REG_CONFIG, &cfg);
    if (st == HAL_OK) {
        cfg &= ~MAX31865_CFG_BIAS;
        (void)MAX31865_Write8(dev, MAX31865_REG_CONFIG, cfg);
    }

    // Bit0 is fault; RTD value is upper 15 bits
    *raw15 = (uint16_t)(rtd16 >> 1);
    return HAL_OK;
}

float MAX31865_RTD_Resistance_Ohms(uint16_t raw15, float rref_ohms) {
    // raw15 is a 15-bit ratio (0..32767) of (Rrtd/Rref) * 2^15
    // Rrtd = (raw15 / 2^15) * Rref
    return ((float)raw15 / 32768.0f) * rref_ohms;
}

float MAX31865_Temperature_C(uint16_t raw15, float rtd_nominal_ohms, float rref_ohms) {
    // Convert to RTD resistance
    float Rt = MAX31865_RTD_Resistance_Ohms(raw15, rref_ohms);

    // Solve Callendar–Van Dusen for T >= 0 using quadratic form:
    // Rt = R0(1 + A*T + B*T^2)  =>  B*T^2 + A*T + (1 - Rt/R0) = 0
    // Take the positive root.
    float Z1 = -RTD_A;
    float Z2 = RTD_A * RTD_A - (4.0f * RTD_B);
    float Z3 = (4.0f * RTD_B) / rtd_nominal_ohms;
    float Z4 = 2.0f * RTD_B;

    float temp = Z2 + (Z3 * Rt);
    temp = (sqrtf(temp) + Z1) / Z4;

    if (temp >= 0.0f) return temp;

    // For negative temps, use a polynomial approximation (standard approach)
    // Normalize to PT100-equivalent curve:
    float ratio = Rt / rtd_nominal_ohms; // relative to R0
    float r = ratio * 100.0f;

    float rpoly = r;
    temp = -242.02f;
    temp += 2.2228f * rpoly;
    rpoly *= r; temp += 2.5859e-3f  * rpoly;
    rpoly *= r; temp -= 4.8260e-6f  * rpoly;
    rpoly *= r; temp -= 2.8183e-8f  * rpoly;
    rpoly *= r; temp += 1.5243e-10f * rpoly;

    return temp;
}
