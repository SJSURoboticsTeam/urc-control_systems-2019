#ifndef LEDC_H_
#define LEDC_H_

typedef enum {
    LEDC_TIMER_0 = 0, /*!< LEDC timer 0 */
    LEDC_TIMER_1,     /*!< LEDC timer 1 */
    LEDC_TIMER_2,     /*!< LEDC timer 2 */
    LEDC_TIMER_3,     /*!< LEDC timer 3 */
} ledc_timer_t;

typedef enum {
    LEDC_CHANNEL_0 = 0, /*!< LEDC channel 0 */
    LEDC_CHANNEL_1,     /*!< LEDC channel 1 */
    LEDC_CHANNEL_2,     /*!< LEDC channel 2 */
    LEDC_CHANNEL_3,     /*!< LEDC channel 3 */
    LEDC_CHANNEL_4,     /*!< LEDC channel 4 */
    LEDC_CHANNEL_5,     /*!< LEDC channel 5 */
    LEDC_CHANNEL_6,     /*!< LEDC channel 6 */
    LEDC_CHANNEL_7,     /*!< LEDC channel 7 */
    LEDC_CHANNEL_MAX,
} ledc_channel_t;

typedef enum {
    LEDC_TIMER_10_BIT = 10, /*!< LEDC PWM duty resolution of 10 bits */
    LEDC_TIMER_11_BIT = 11, /*!< LEDC PWM duty resolution of 11 bits */
    LEDC_TIMER_12_BIT = 12, /*!< LEDC PWM duty resolution of 12 bits */
    LEDC_TIMER_13_BIT = 13, /*!< LEDC PWM duty resolution of 13 bits */
    LEDC_TIMER_14_BIT = 14, /*!< LEDC PWM duty resolution of 14 bits */
    LEDC_TIMER_15_BIT = 15, /*!< LEDC PWM duty resolution of 15 bits */
} ledc_timer_bit_t;

typedef enum {
    LEDC_HIGH_SPEED_MODE = 0, /*!< LEDC high speed speed_mode */
    LEDC_LOW_SPEED_MODE,      /*!< LEDC low speed speed_mode */
    LEDC_SPEED_MODE_MAX,      /*!< LEDC speed limit */
} ledc_mode_t;
void ledc_update_duty(ledc_mode_t speed_mode, ledc_channel_t channel);

void ledc_set_duty(ledc_mode_t speed_mode, ledc_channel_t channel, uint32_t duty);

#endif