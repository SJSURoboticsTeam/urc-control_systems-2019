#ifndef SERVO_H
#define SERVO_H

class Servo
{
     private :
          uint32_t SERVO_MIN_PULSEWIDTH; //Minimum pulse width in microsecond
          uint32_t SERVO_MAX_PULSEWIDTH; //Maximum pulse width in microsecond
          uint32_t SERVO_MAX_DEGREE;     //Maximum angle in degree upto which servo can rotate
          uint32_t CURRENT_PULSEWIDTH;   //Minimum pulse width in microsecond
          int CURRENT_FREQUENCY;         //Servo frequency
          int SERVO_CHANNEL;             //Servo Timer channel
          int GPIO_PIN;                  //Servo Pin 
     public :
          /**
          * @Servo Object initialization.Store all the neccessary 
          *        value to configure a gpio into the current servo Object 
          *
          * @param const int gpio_pin (any valid gpio pin num)
          * @param const int frequency (frequency value in Hz)
          * @param const int channel (0-5)
          * @param uint32_t max_pulse (servo max pulse in ms)
          * @param uint32_t min_pulse (servo max min pulse in ms)
          * @param const uint32_t max_degree (servo max rotational degree)
          *
          * @return
          *     - None
          */
          Servo(const uint32_t gpio_pin, const uint32_t frequency, const uint32_t channel,
                const uint32_t max_pulse, const uint32_t min_pulse,const uint32_t max_degree);

          /**
          * @ Initialize MCPWM_GPIO and configure Servo_pwm default status.  
          *
          * @return
          *     - Error flag: 1==Ok ; -1 == MCPWM_GPIO Init Fail, -2 == Servo Config Fail      
          */
          int Servo_gpio_config();

          /**
          * @Configure Servo_pwm default status. 
          *
          * @param const int frequency (frequency value in Hz)
          * @param const int init_duty_cycle (set default duty cycle in percnetage)
          * @param const int channel_timer (0-2)
          * @param const int char pin_version ('A','B')
          *
          * @return
          *     - Error flag: 1==Ok ; -1 == Invalid pin version, -2 == Invalid channel timer   
          */
   	     int Servo_pwm_config(const int frequency, const int init_duty_cycle, const int channel_timer, const char pin_version);
          
          /**
          * @Move servo to desire angle . 
          *
          * @param uint32 set_angle (0-270)
          *
          * @return
          *     - Error flag: 1==Ok ; -1 == failed to write duty cycle;  
          */
          int Servo_write_angle(uint32_t set_angle);

          /**
          * @Convert angle to pulse_width . 
          *
          * @param uint32 degree_of_rotation (0-270)
          *
          * @return
          *     - pulsewidth in ms  
          */
          uint32_t Servo_per_degree_init(uint32_t degree_of_rotation);
};

#endif
