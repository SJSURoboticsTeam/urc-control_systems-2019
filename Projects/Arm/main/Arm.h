#include <stdlib.h>
#include <stdio.h>

#ifndef PROTOPROJECT_H_
#define PROTOPROJECT_H_

#ifdef _cplusplus
extern "C" {
#endif

// Control
	
// 	Rotunda Servo:
// 	Shoulder DC motor:
// 	Elbow servo:
// 	Wrist pitch:
	
	enum MOTOR_TYPE{
		Shoulder_Motor  	= 0,
		Elbow_Servo 		= 1,
		Wrist_Roll_Motor1	= 2,
		Wrist_Roll_Motor2	= 3,
		Wrist_Pitch 		= 4,
		Claw_Servo 			= 5,
		Rotunda_Servo 		= 6,
	};

	typedef Stuct{
		GPIO_Pin,
		Orientation_limit_min,
		Orientation_limi_max,
		Current_Orientation
	} motor_info_struct;


	motor_info_struct myMotors{Rotunda_Servo+1};

		/*
		 * @Param: Orientation in degrees, 0-360
		 * @Param: The specific motor inteded to update
		 * @Return: If the servo is capable of adjusting to that position
		 */
		bool setMotorPosition(uint16_t orientation, MOTOR_TYPE motor_to_set);	

		/*
		 * @Return: The current orientation of the Rotunda servo, 0-360 degrees
		 */
		motor_info_struct.Current_Orientation getMotorPosition(MOTOR_TYPE motor_to_read);
		//whatever type this is ^

//	Wrist roll (2 DC Motors):
		/*
		 * @Param: The magnatude in which the Wrist should rotate
		 */
		void rotateWrist(uint16_t degrees);

	
//	Claw open/close + force control:
		/*
		 * Sets the claw's position to the maximum open position
		 */
		void openClaw(void);

		/*
		 * Sets the claw's position to the minimum closed position
		 */
		void closedClaw(void);

//	Laser on/off:
		
		/*
		 * Activates the laser
		 */
		void laserOn(void);
		
		/*
		 * Deactivates the laser
		 */
		void laserOff(void);

//	Preset positions:

		/*
		 * Sets the Arm to its original verticle position
		 */
		void returnToOrigin(void);
		

		/*
		 * This function place the arm in a position level to the ground with the claw open	
		 */
		bool grabPOD(void);
			
			/*
			* Grabs pod from rover and places it on the ground
			*
		 	* @Return: If there is a pod on board/if the deployment was successful.
		 	*/
		bool deployPod(void);

			/*
			 * Assuming claw is alread level with the pod,
			 * This will close the claw, which grabs the pod
			 * 		and manipulates the arm to move to the 
			 *		POD cubby and place it there
			*/
		bool storePOD(void)

// Feedback
// 	IMU x 4 (Shoulder, elbow, wrist pitch):
// 	Rotunda orientation (IMU or POT):

		/*
		 * @Param: IMU Enum indication which IMU to read
		 * @Return: Whether or not the ESP was able to read from the Encoder
		 */
		bool updateIMU(IMU imu_to_update);

//	Magnetic Encoder (Wrist rotation):

		/*
		 * @Return: Whether or not the ESP was able to read from the Encoder
		 */
		bool readMagneticEncoder();

//	Claw current/force:
		/*
		 * Measures the current supplied to the Claw's Motor/Servo
		 * Helps determine the opposed force the Claw recieves
		 *		to determine if it claped an object
		 */
		uint32_t getClawCurrent(void);



		//sends Current_Orientation of all motors, all IMU data, and Magnetic encoder data
	bool sendToMC(motor_info_struct*, IMU_Data*, Magenetic_Encoder*);

enum IMUs
{
	Shoulder,
	Elbow,
	//Wrist, magnetic encoder?
	//Rotunda, maybe uses the pot in the servo
};

typedef Struct
{
	int32_t AccelerometerX;
	int32_t AccelerometerY;
	int32_t AccelerometerZ;

	int32_t GyroX;
	int32_t GyroY;
	int32_t GyroZ;

	int32_t MagnometerX;
	int32_t MagnometerY
	int32_t MagnometerZ;
}IMU_Data;

typedef Struct 
{
	//???
}Magenetic_Encoder;

#ifdef _cplusplus
}
#endif

#endif

