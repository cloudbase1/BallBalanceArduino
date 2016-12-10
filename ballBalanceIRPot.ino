#include<Servo.h>
#include<PID_v1.h>


#define SERVO_PIN  9                                  //Servo Pin
#define MAX_DISTANCE 80// Max distance in cm
#define MIN_DISTANCE 10// Min distance in CM
// Setup some parameters
float Kp = 1.5;                                            //Initial Proportional Gain
float Ki = 0.5;                                           //Initial Integral Gain
float Kd = 1.0;                                          //Intitial Derivative Gain
double myDirection = 1;
double myLevel = 125;
double minOutput = -15;
double maxOutput = 15;
double ScaleFactor = 1;
double offset = 150.0;// IR zero position output
// Declare PID signal variables 
double Setpoint, Input, Output, ServoOutput;
double  avgDistance, sumOfRaw,  rawDistance, error;
double numOfSamples = 1.0;

PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);           //Initialize PID object, which is in the class PID.                                                                                                            
Servo myServo;                                                       //Initialize Servo.

// Initial setup of Arduino
// If level is 100 degrees and max tilt for move left is 130 then move left is center +  error.
// If level is 100 and max tilt right is 40 then then move right if center - error.
// The PID should give to correct sign. if not just switch the myDirection modifier.
void setup() {
  Serial.begin(9600);                                //Begin Serial 
  myServo.attach(SERVO_PIN);                          //Attach Servo                                                                 
  myPID.SetMode(AUTOMATIC);                          //Set PID object mode 
  myPID.SetOutputLimits(minOutput,maxOutput);        //Set Output limits
 
    
  }


void loop()
{ 
  for(int x = 0; x < numOfSamples; x++)
  {
     rawDistance = analogRead(2);
     if(x == 0){
      sumOfRaw = rawDistance;
     }
    else{
      sumOfRaw = sumOfRaw + rawDistance;
    }
  }
  avgDistance = sumOfRaw/numOfSamples;
  Serial.print("rawDistance = ");
  Serial.print(rawDistance);
  Serial.print(" avgDistance = ");
  Serial.println(avgDistance);
  // Update PID
  Setpoint = 250;//analogRead(1)/2.5;// make pot less sensative
  // Input is the error signal. So myLevel is an error of 0. So the error is added to the myLevel 
  // and its sign tetermins direction
  //error = avgDistance - Output;
  error = avgDistance;
  Input = error;
  
  myPID.Compute();                               //computes Output in range of 
  ServoOutput = myLevel + (Output/ScaleFactor * myDirection );
  Serial.print("PID INPUT: ");
  Serial.println(Input);
  Serial.print("PID SETPOINT: ");
  Serial.println(Setpoint);
  Serial.print("PID OUTPUT: ");
  Serial.println(Output);
  Serial.print("ServoOutPut: ");
  Serial.println(ServoOutput);
  myServo.write(ServoOutput);  
  //myServo.write(myLevel);
  
  
}




