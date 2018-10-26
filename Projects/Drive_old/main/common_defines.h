#ifndef COMMON_DEFINES_H_
#define COMMON_DEFINES_H_

#define HIGH_SPEED_MODE LEDC_HIGH_SPEED_MODE
#define HIGH_SPEED_TIMER LEDC_TIMER_0
#define LOW_SPEED_MODE LEDC_LOW_SPEED_MODE
#define LOW_SPEED_TIMER LEDC_TIMER_1

#define ADC1_PIN_36 ADC1_CHANNEL_0
#define ADC1_PIN_37 ADC1_CHANNEL_1
#define ADC1_PIN_38 ADC1_CHANNEL_2
#define ADC1_PIN_39 ADC1_CHANNEL_3
#define ADC1_PIN_32 ADC1_CHANNEL_4
#define ADC1_PIN_33 ADC1_CHANNEL_5
#define ADC1_PIN_34 ADC1_CHANNEL_6
#define ADC1_PIN_35 ADC1_CHANNEL_7

#define ADC2_PIN_4  ADC2_CHANNEL_0
#define ADC2_PIN_0  ADC2_CHANNEL_1
#define ADC2_PIN_2  ADC2_CHANNEL_2
#define ADC2_PIN_15 ADC2_CHANNEL_3
#define ADC2_PIN_13 ADC2_CHANNEL_4
#define ADC2_PIN_12 ADC2_CHANNEL_5
#define ADC2_PIN_14 ADC2_CHANNEL_6
#define ADC2_PIN_27 ADC2_CHANNEL_7
#define ADC2_PIN_25 ADC2_CHANNEL_8
#define ADC2_PIN_26 ADC2_CHANNEL_9

#define I2C_EXAMPLE_MASTER_NUM I2C_NUM_0   /*!< I2C port number for master dev */
#define I2C_EXAMPLE_MASTER_FREQ_HZ 100000     /*!< I2C master clock frequency */
#define I2C_EXAMPLE_MASTER_TX_BUF_DISABLE 0   /*!< I2C master do not need buffer */
#define I2C_EXAMPLE_MASTER_RX_BUF_DISABLE 0   /*!< I2C master do not need buffer */
#define WRITE_BIT I2C_MASTER_WRITE /*!< I2C master write */
#define READ_BIT  I2C_MASTER_READ  /*!< I2C master read */
#define ACK_EN 	 0x1     /*!< I2C master will check ack from slave*/
#define ACK_DIS  0x0     /*!< I2C master will not check ack from slave */
#define ACK_VAL  I2C_MASTER_ACK         /*!< I2C ack value */
#define NACK_VAL I2C_MASTER_NACK         /*!< I2C nack value */
#define LAST_NACK I2C_MASTER_LAST_NACK
#define ACK_CHECK_EN                       0x1              /*!< I2C master will check ack from slave*/
#define ACK_CHECK_DIS                      0x0              /*!< I2C master will not check ack from slave */


#define ADC_V_REF 3300 							//in mV

#endif 