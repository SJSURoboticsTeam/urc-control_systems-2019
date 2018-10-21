#ifndef ARM_DEFINE_H_
#define ARM_DEFINE_H_
#define FORWARD 0x1
#define REVERSE 0x0
#define FREQUENCY 9600
#define MOTOR_TIMER_RESOLUTION 10
#define MOTOR_FREQ 25000
#define SERVO_LOW_SPEED_FREQ 400
#define ARM_TIMER_RESOLUTION 10
#define MAX 1023
#define MIN 0
#define HIGH_SPEED_MODE LEDC_HIGH_SPEED_MODE
#define HIGH_SPEED_TIMER LEDC_TIMER_0
#define LOW_SPEED_MODE LEDC_LOW_SPEED_MODE
#define LOW_SPEED_TIMER LEDC_TIMER_1

#define PWM_MAX 1023
#define PWM_MIN 0

//JOINTS

#define ROTUNDA 		 	GPIO_NUM_16 // servo
#define SHOULDER 		 	GPIO_NUM_23  // motor
#define SHOULDER_DIR_PIN 	GPIO_NUM_22 
#define ELBOW 			 	GPIO_NUM_4 // servo

//WRIST PITCH

#define GEARBOX_1_IN_1   	GPIO_NUM_33
#define GEARBOX_1_IN_2   	GPIO_NUM_32

//WRIST ROTATION

#define GEARBOX_2_IN_1   	GPIO_NUM_26
#define GEARBOX_2_IN_2   	GPIO_NUM_25

//CLAW

#define LEADSCREW_IN_1   	GPIO_NUM_18
#define LEADSCREW_IN_2   	GPIO_NUM_5

#define CAM_SERVO 		 	GPIO_NUM_0
#define CAM_MUX_SEL 	 	GPIO_NUM_2

//SENSORS
#define SHOULDER_MAG 	 	GPIO_NUM_35
#define WRIST_ENCODER_A  	GPIO_NUM_27
#define WRIST_ENCODER_B  	GPIO_NUM_17
// #define WRIST_MAG 		 	GPIO_NUM_21

//SENSORS PROGRAMMING
#define SHOUDLER_MAG_PRGM 	GPIO_NUM_18
#define WRIST_MAG_PRGM 		GPIO_NUM_23

//OTHER
#define I2C3_CLK GPIO_NUM_19
#define I2C3_DATA GPIO_NUM_21
#define TMS GPIO_NUM_14
#define TDI GPIO_NUM_12
#define TCK GPIO_NUM_13
#define TDO GPIO_NUM_15

#define MANUAL 0
#define AUTOMATIC 1
  
#define DIRECT 0
#define PID_REVERSE 1
  
#define P_ON_M 0
#define P_ON_E 1

#define LEFT 1
#define RIGHT 2

#define IDLE 0
#define OPEN 2
#define CLOSE 1

#define SHOULDER_PWM_MAX 400
#define SHOULDER_PWM_MIN -300

#define PITCH_PWM_MAX 400
#define PITCH_PWM_MIN -400

#define ROTATION_PWM_MAX 1023
#define ROTATION_PWM_MIN 0

#define ROTUNDA_PWM_MAX 1023
#define ROTUNDA_PWM_MIN 0

#define ROTUNDA_PWM_12_BIT_MAX 4095
#define ROTUNDA_PWM_12_BIT_MIN 819

#define CAM_PWM_MAX 1023
#define CAM_PWM_MIN 0

#define SERVO_PWM_MAX 1015
#define SERVO_PWM_MIN 205

#define SERVO_PWM_12_BIT_MAX 4095
#define SERVO_PWM_12_BIT_MIN 819

#define ROTUNDA_MAX 150
#define ROTUNDA_MIN -150
#define SHOULDER_MAX 180
#define SHOULDER_MIN 0
#define ELBOW_MAX 150
#define ELBOW_MIN -150
#define PITCH_MAX 130
#define PITCH_MIN -130
#define ROTATION_MAX 150
#define ROTATION_MIN -150
#define CAM_MAX 90
#define CAM_MIN 0

#define MAG_ENC_MAX 3650
#define MAG_ENC_MIN 2050
#define IMU_X_MAX 180
#define IMU_X_MIN -180
#define IMU_Y_MAX 90
#define IMU_Y_MIN -90
#define IMU_Z_MAX 180
#define IMU_Z_MIN -180

#define MAG_ENC_OFFSET 0

#define ROTUNDA_ALPHA 5
#define SHOULDER_ALPHA 0.015
#define ELBOW_ALPHA 10
#define CAM_ALPHA 10
#define PITCH_ALPHA 0.01
#define ROTATION_ALPHA 10
#define CLAW_ALPHA 10

#define I2C_EXAMPLE_SLAVE_NUM I2C_NUM_1    /*I2C port number for slave dev */
#define I2C_EXAMPLE_SLAVE_TX_BUF_LEN (2*DATA_LENGTH) 
#define I2C_EXAMPLE_SLAVE_RX_BUF_LEN (2*DATA_LENGTH) 

#define I2C_EXAMPLE_MASTER_SCL_IO GPIO_NUM_19    /* gpio number for I2C master clock */
#define I2C_EXAMPLE_MASTER_SDA_IO GPIO_NUM_21    /*!< gpio number for I2C master data  */
#define I2C_EXAMPLE_MASTER_TX_BUF_DISABLE 0   /*!< I2C master do not need buffer */
#define I2C_EXAMPLE_MASTER_RX_BUF_DISABLE 0   /*!< I2C master do not need buffer */
#define I2C_EXAMPLE_MASTER_FREQ_HZ 100000     /*!< I2C master clock frequency */

#define LIS3DH_SENSOR_ADDR 	0x30    /*!< slave address for LIS3DH sensor */
#define LIS3DH_SA0_0_READ 	0x31
#define LIS3DH_SA0_0_WRITE 	0x30
#define LIS3DH_SA0_1_READ 	0x33
#define LIS3DH_SA0_1_WRITE 	0x32

#define AS5600_SENSOR_ADDR 	0x36    /*!< slave address for AS5600 sensor */
#define AS5600_SLAVE_WRITE 	0x6C
#define AS5600_SLAVE_READ 	0x6D
#define AS5600_ZPOS_HIGH 	0x01
#define AS5600_ZPOS_LOW		0x02
#define AS5600_CONF_HIGH 	0x07
#define AS5600_CONF_LOW		0x08
#define AS5600_STATUS		0x0B
#define AS5600_RAW_HIGH     0x0C
#define AS5600_RAW_LOW		0x0D
#define AS5600_ANGLE_HIGH	0x0E
#define AS5600_ANGLE_LOW    0x0F
#define AS5600_BURN 		0xFF

#define BNO055_ADDR_COM3_HIGH 	0x29
#define BNO055_ADDR_COM3_LOW  	0x28
#define BNO055_WRITE_LOW_ADDR 	0x50
#define BNO055_READ_LOW_ADDR  	0x51
#define BNO055_WRITE_HIGH_ADDR 	0x52
#define BNO055_READ_HIGH_ADDR  	0x53
#define BNO055_OPR_MODE_ADDR	0x3D
#define BNO055_NDOF_MODE		0x0C
#define BNO055_ACC_OFFSET_X_LSB            0x55
#define BNO055_ACC_OFFSET_X_MSB            0x56
#define BNO055_ACC_OFFSET_Y_LSB            0x57
#define BNO055_ACC_OFFSET_Y_MSB            0x58
#define BNO055_ACC_OFFSET_Z_LSB            0x59
#define BNO055_ACC_OFFSET_Z_MSB            0x5A
#define BNO055_MAG_OFFSET_X_LSB            0x5B
#define BNO055_MAG_OFFSET_X_MSB            0x5C
#define BNO055_MAG_OFFSET_Y_LSB            0x5D
#define BNO055_MAG_OFFSET_Y_MSB            0x5E
#define BNO055_MAG_OFFSET_Z_LSB            0x5F
#define BNO055_MAG_OFFSET_Z_MSB            0x60
#define BNO055_GYR_OFFSET_X_LSB            0x61
#define BNO055_GYR_OFFSET_X_MSB            0x62
#define BNO055_GYR_OFFSET_Y_LSB            0x63
#define BNO055_GYR_OFFSET_Y_MSB            0x64
#define BNO055_GYR_OFFSET_Z_LSB            0x65
#define BNO055_GYR_OFFSET_Z_MSB            0x66
#define BNO055_ACC_RADIUS_LSB              0x67
#define BNO055_ACC_RADIUS_MSB              0x68
#define BNO055_MAG_RADIUS_LSB              0x69
#define BNO055_MAG_RADIUS_MSB              0x6A
#define BNO055_X_LOW_ADDR 	 	0x08
#define BNO055_X_HIGH_ADDR 	 	0x09
#define BNO055_Y_LOW_ADDR 	 	0x0A
#define BNO055_Y_HIGH_ADDR 	 	0x0B
#define BNO055_Z_LOW_ADDR 	 	0x0C
#define BNO055_Z_HIGH_ADDR 	 	0x0D

#define OUT_X 0x28    /* Address of x register */
#define OUT_Y 0x2A    /* Address of y register */
#define OUT_Z 0x2C    /* Address of z register */
#define CTRL_REG1 0x20
#define CTRL_REG3 0x22
#define CTRL_REG4 0x23
#define CTRL_REG5 0x24
#define WRITE_BIT I2C_MASTER_WRITE /*!< I2C master write */
#define READ_BIT  I2C_MASTER_READ  /*!< I2C master read */
#define ACK_EN 	 0x1     /*!< I2C master will check ack from slave*/
#define ACK_DIS  0x0     /*!< I2C master will not check ack from slave */

#define ACK_VAL  0x0         /*!< I2C ack value */
#define NACK_VAL 0x1         /*!< I2C nack value */
#define LAST_NACK I2C_MASTER_LAST_NACK

// #define ACK_VAL  I2C_MASTER_ACK         /*!< I2C ack value */
// #define NACK_VAL I2C_MASTER_NACK         /*!< I2C nack value */
// #define LAST_NACK I2C_MASTER_LAST_NACK

#define BNO055_SENSOR_ADDR  0x29    /*!< slave address for IMU */
#define OPR_MODE 0x3D         // address for operation mode register
#define UNIT_SEL 0x3B         // address for unit select register
#define BNO055_ADDR                        0x29
#define BNO055_OPR_MODE                    0x3D
#define BNO055_UNIT_SEL 				   0x3B
#define BNO055_HEADING_LSB                 0x1A
#define BNO055_HEADING_MSB                 0x1B
#define BNO055_ROLL_LSB                    0x1C
#define BNO055_ROLL_MSB                    0x1D
#define BNO055_PITCH_LSB                   0x1E
#define BNO055_PITCH_MSB                   0x1F

#define MPU_9250_ADDR 	0x69
#define MPU_9250_X_HIGH	0x3B
#define MPU_9250_X_LOW	0x3C
#define MPU_9250_Y_HIGH	0x3D
#define MPU_9250_Y_LOW	0x3E
#define MPU_9250_Z_HIGH	0x3F
#define MPU_9250_Z_LOW	0x40

#define BNO055_CALIB_STAT                  0x35
#define ACK_CHECK_EN                       0x1              /*!< I2C master will check ack from slave*/
#define ACK_CHECK_DIS                      0x0              /*!< I2C master will not check ack from slave */
#endif