#ifndef MOTOR_CONTROL_REV1_HPP_
#define MOTOR_CONTROL_REV1_HPP_

#include <stdlib.h>
#include "driver/ledc.h"

class Motor
{
    private:
    ledc_channel_t pwm_s_channel;
    ledc_channel_t pwm_b_channel;
    ledc_timer_t pwm_timer;
    uint32_t signal_pin;
    uint32_t brake_pin;
    uint64_t direction_pin;
    uint32_t pwm_frequency;
    uint32_t signal_duty_cycle;
    uint32_t brake_duty_cycle;
    uint32_t max_duty = ((1 << LEDC_TIMER_10_BIT) - 1);
    uint32_t top_duty;
    uint32_t base_duty;
    uint32_t range;	
    ledc_timer_config_t motor_timer;
    ledc_channel_config_t motor;
    ledc_channel_config_t brake;
    gpio_config_t direction;

	public:
		Motor()
		{
			// does nothing
		}

		/**
		 * Parameters:
		 *     signal    - Indicates the pin to be used as a pwm signal.
		 *     brake     - Indicates tje pin to be used as the brake signal.
		 *     direction - Indicates the pin to be used as the direction signal.
		 *     s_channel - Value 0 - 7, must be unique for each motor.
		 *     b_channel - Value 0 - 7, must be same for all motors.
		 *     timer     - Value 0 - 3, determines which set frequency the motor 
		 *                 will use.
		 *     frequency - The specified frequency of the RC circuit in Hz as 
		 *				   specified by the EE assigned to the subsystem.
		 *     min		 - The minimum PWM duty cycle used for control.
		 *	   max       - The maximum PWM duty cycle used for control.
		 */
		void InitMotor(uint32_t pin_signal, uint32_t pin_brake, 
				       uint32_t pin_direction, uint32_t s_channel,
				       uint32_t b_channel, uint32_t timer, uint32_t frequency,
				       float min, float max);
		
		/**
         * This function sets the frequency of the PWM in hertz by updating the 
         * timer that has been assigned to the signal. There are only four 
         * timers available, so setting the frequency here will update the 
         * frequency for all objects assigned to the same timer.
         */
		void SetFrequency(uint32_t frequency);

		/**
         * This function calculates and updates the speed in the current 
         * direction the motor is rotating, based upon the input percentage.
         */
		void SetSpeed(double percentage);

		/**
         * This function calculates and updates the speed at the specified 
         * direction for the motor to rotate, based upon the input percentage.
         */
		void SetSpeedAndDirection(double percentage, bool direction);

		/**
		 * This function sets the speed for the motors to spend by directly 
		 * adjusting the duty cycle to the specified value.
		 */
		void SetSpeedDuty(uint32_t duty);

		/**
		 * This function changes the signal on the direction to pin to change
		 * the direction the wheels spin.
		 */
		void SetDirection(bool dir);

		/**
		 * This function applies the brakes at the specified percentage.
		 */
		void Brake(double percentage);

	/**
	 * Deconstructor
	 */
	~Motor()
	{
		// Does Nothing
	}
};

#endif
