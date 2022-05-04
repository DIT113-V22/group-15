#include <Smartcar.h>

ArduinoRuntime arduinoRuntime;
BrushedMotor leftMotor{arduinoRuntime, smartcarlib::pins::v2::leftMotorPins};
BrushedMotor rightMotor{arduinoRuntime, smartcarlib::pins::v2::rightMotorPins};
DifferentialControl control{leftMotor, rightMotor};

SimpleCar car(control);
const auto frontIR_PIN = 0;
const auto leftIR_PIN = 1;
const auto rightIR_PIN = 2;
const auto backIR_PIN = 3;


const int triggerPin           = 6; // D6
const int echoPin              = 7; // D7
const unsigned int maxDistance = 100;

SR04 front(arduinoRuntime, triggerPin, echoPin, maxDistance);
GP2Y0A02 sideFrontIR(arduinoRuntime, frontIR_PIN);
GP2Y0A02 backIR(arduinoRuntime, backIR_PIN );
GP2Y0A02 leftIR(arduinoRuntime, leftIR_PIN );
GP2Y0A02 rightIR(arduinoRuntime, rightIR_PIN); 

void setup()
{
  // Move the car with 50% of its full speed
  car.setSpeed(50);
}

void loop()
{
  const auto front_distance = front.getDistance();
  const auto distanceIR = sideFrontIR.getDistance();
  const auto distanceBackIR = backIR.getDistance();
  const auto distanceLeftIR = leftIR.getDistance();
  const auto distanceRightIR = rightIR.getDistance();

// If FRONT and LEFT
  if (front_distance > 25 && front_distance < 70 && distanceLeftIR < 50 && distanceLeftIR > 25 )
  {
    car.setSpeed(0);
    delay(2000);
    car.setAngle(45);
    delay(2000);
    car.setSpeed(30);
    delay(2000);
    car.setAngle(0);
    
  }
  /*If FRONT, RIGHT and left sensor meet value */
  if (front_distance > 25 && front_distance < 50 && distanceLeftIR > 15 && distanceLeftIR < 50 && distanceRightIR > 15 && distanceRightIR < 50)
  {
    car.setSpeed(0);
    delay(2000);
    car.setSpeed(-50);
    delay(2000);
    car.setAngle(45);
    delay(2000);
    car.setAngle(0);
    delay(500);
    car.setSpeed(100);
  }
  //If front, right sensor active
  if (front_distance > 25 && front_distance < 50 && distanceRightIR > 15 && distanceRightIR < 50)
  {
    car.setSpeed(0);
    delay(2000);
    car.setAngle(-45);
    delay(2000);
    car.setSpeed(30);
    delay(2000);
    car.setAngle(0);
    delay(1000);
    car.setSpeed(90);
  }


  // Obstecle FRONT
  if(front_distance > 25 && front_distance < 70 && distanceIR > 25 && distanceIR < 70 )
  {
     car.setSpeed(0);
    delay(2000);
    car.setSpeed(-50);
    delay(2000);
    car.setAngle(75);
    delay(1000);
    car.setAngle(0);
    delay(2000);
    car.setSpeed(50);
  }
  if (distanceBackIR > 25 && distanceBackIR < 70)
  {
    car.setSpeed(0);
    delay(2000);
    car.setSpeed(50);
    delay(2000);
    car.setAngle(60);
    delay(1000);
    car.setAngle(0);
  }





#ifdef SMCE
  // Avoid over-using the CPU if we are running in the emulator
  delay(1);
#endif
}
