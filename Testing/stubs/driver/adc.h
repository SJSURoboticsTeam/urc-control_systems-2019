#ifndef ADC_H_
#define ADC_H_

typedef enum {
    ADC1_CHANNEL_0 = 0, /*!< ADC1 channel 0 is GPIO36 */
    ADC1_CHANNEL_1,     /*!< ADC1 channel 1 is GPIO37 */
    ADC1_CHANNEL_2,     /*!< ADC1 channel 2 is GPIO38 */
    ADC1_CHANNEL_3,     /*!< ADC1 channel 3 is GPIO39 */
    ADC1_CHANNEL_4,     /*!< ADC1 channel 4 is GPIO32 */
    ADC1_CHANNEL_5,     /*!< ADC1 channel 5 is GPIO33 */
    ADC1_CHANNEL_6,     /*!< ADC1 channel 6 is GPIO34 */
    ADC1_CHANNEL_7,     /*!< ADC1 channel 7 is GPIO35 */
    ADC1_CHANNEL_MAX,
} adc1_channel_t;

typedef enum {
    ADC_WIDTH_BIT_9  = 0, /*!< ADC capture width is 9Bit*/
    ADC_WIDTH_BIT_10 = 1, /*!< ADC capture width is 10Bit*/
    ADC_WIDTH_BIT_11 = 2, /*!< ADC capture width is 11Bit*/
    ADC_WIDTH_BIT_12 = 3, /*!< ADC capture width is 12Bit*/
    ADC_WIDTH_MAX,
} adc_bits_width_t;

typedef struct {
    uint32_t gain_m;       /**<Gradient of Gain Curve */
    uint32_t gain_c;       /**<Offset of Gain Curve */
    uint32_t offset_m;      /**<Gradient of Offset Curve */
    uint32_t offset_c;      /**<Offset of Offset Curve */
    uint32_t bit_shift;     /**<Bit shift used find corresponding LUT points
                                given an ADC reading*/
    uint32_t voltage[];     /**<Array of voltages in mV representing the
                                ADC-Voltage curve */
} esp_adc_cal_lookup_table_t;

typedef struct {
    uint32_t v_ref;         /**<Reference Voltage of current ESP32 Module in mV*/
    uint32_t gain;          /**<Scaling factor used to correct LUT voltages to
                                current v_ref. Bit shifted by << ADC_CAL_GAIN_SCALE
                                for uint32 arithmetic */
    uint32_t offset;        /**<Offset in mV used to correct LUT Voltages to current v_ref */
    uint32_t ideal_offset;  /**<Offset in mV at the ideal reference voltage */
    adc_bits_width_t bit_width;     /**<Bit width of ADC e.g. ADC_WIDTH_BIT_12 */
    const esp_adc_cal_lookup_table_t *table;    /**<Pointer to LUT */
} esp_adc_cal_characteristics_t;

#endif