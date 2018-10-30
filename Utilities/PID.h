#ifndef PID_h
#define PID_h
#define LIBRARY_VERSION	1.2.1

class PID
{


  public:

  //Constants used in some of the functions below
  #define AUTOMATIC	1
  #define MANUAL	0
  #define DIRECT  0
  #define REVERSE  1
  #define P_ON_M 0
  #define P_ON_E 1

  //commonly used functions **************************************************************************
    
    // * Constructor. Links the PID to the Input, Output, and 
	//   Setpoint. Initial tuning parameters are also set here.
    //   (overload for specifying proportional mode)
    PID(double* Input, double* Output, double* Setpoint,
        double Kp, double Ki, double Kd, int POn, int ControllerDirection);

    // * Constructor. Links the PID to the Input, Output, and 
    //   Setpoint. Initial tuning parameters are also set here
    PID::PID(double* Input, double* Output, double* Setpoint,
        double Kp, double Ki, double Kd, int ControllerDirection)     
	
    // * Sets PID to either Manual (0) or Auto (non-0)
    void SetMode(int Mode);               

    // * Performs the PID calculation. It should be
    //   called every time loop() cycles. ON/OFF and
    //   calculation frequency can be set using SetMode
    //   SetSampleTime respectively
    bool Compute();                       

    // * Clamps the output to a specific range. 0-255 by default, but
	//   it's likely the user will want to change this depending on
	//   the application
    void SetOutputLimits(double Min, double Max);
	

  //available but not commonly used functions ********************************************************
    

    // * While most users will set the tunings once in the 
    //   constructor, this function gives the user the option
    //   of changing tunings during runtime for Adaptive control
    void SetTunings(double, double, double);

    // * Overload for specifying proportional mode
    void SetTunings(double, double, double, int);         	  

	// * Sets the Direction, or "Action" of the controller.
	//   DIRECT means the output will increase when error is positive. 
	//   REVERSE means the opposite.  
	//   It's very unlikely that this will be needed once it is set in the constructor.
	void SetControllerDirection(int Direction);	  

    // * Sets the frequency, in milliseconds, with which 
    //   the PID calculation is performed.  default is 100
    void SetSampleTime(int);
										  
										  
										  
  //Display functions ****************************************************************

	// * These functions query the pid for interal values.
	//   They were created mainly for the pid front-end,
	//   where it's important to know what is actually 
	//   inside the PID.
	double GetKp();						  
	double GetKi();						  
	double GetKd();						  
	int GetMode();						  
	int GetDirection();					  

  private:
	void Initialize();
    
	double kp;                  // * (P)roportional Tuning Constant
    double ki;                  // * (I)ntegral Tuning Constant
    double kd;                  // * (D)erivative Tuning Constant

	int controllerDirection;
	int pOn;

    // * Pointers to the Input, Output, and Setpoint variables
    //   This creates a hard link between the variables and the 
    //   PID, freeing the user from having to constantly tell us
    //   what these values are.  with pointers we'll just know.
    double *myInput;           
    double *myOutput;
    double *mySetpoint;
			  
	unsigned long lastTime;
	double outputSum, lastInput;

	unsigned long SampleTime;
	double outMin, outMax;
	bool inAuto;
	

	// * Bool to add Proportional component to Error vs. Measurement
	//	 If true proportional is an Error component
	//	 If false proportional is a Measurement component
	bool pOnE;
};
#endif