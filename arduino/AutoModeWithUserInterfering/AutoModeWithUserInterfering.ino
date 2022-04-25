#include <Smartcar.h>

/*
this sketch needs to be improved with millis() function and interrupt to handle multiple events.
*/


const auto carSpeed =                70;
volatile int counter;

const auto frontIR_PIN = 0;
const auto leftIR_PIN = 1;
const auto rightIR_PIN = 2;
const auto backIR_PIN = 3;

const int triggerPin           = 6; // D6
const int echoPin              = 7; // D7
const unsigned int maxDistance = 100;

ArduinoRuntime arduinoRuntime;
BrushedMotor leftMotor{arduinoRuntime, smartcarlib::pins::v2::leftMotorPins};
BrushedMotor rightMotor{arduinoRuntime, smartcarlib::pins::v2::rightMotorPins};
DifferentialControl control{leftMotor, rightMotor};


const auto pulsesPerMeter = 600;

DirectionlessOdometer leftOdometer(
  arduinoRuntime,
  smartcarlib::pins::v2::leftOdometerPin,
[]() {
  leftOdometer.update();
},
pulsesPerMeter);
DirectionlessOdometer rightOdometer(
  arduinoRuntime,
  smartcarlib::pins::v2::rightOdometerPin,
[]() {
  rightOdometer.update();
},
pulsesPerMeter);

DistanceCar car(arduinoRuntime, control, leftOdometer, rightOdometer);


SR04 front(arduinoRuntime, triggerPin, echoPin, maxDistance);
GP2Y0A02 sideFrontIR(arduinoRuntime, frontIR_PIN);
GP2Y0A02 backIR(arduinoRuntime, backIR_PIN );
GP2Y0A02 leftIR(arduinoRuntime, leftIR_PIN );
GP2Y0A02 rightIR(arduinoRuntime, rightIR_PIN);


void setup()
{
  // Move the car with 50% of its full speed
  Serial.begin(9600);
  car.setSpeed(carSpeed);
  delay(3000);

  Serial.println("Do you need some help?");
  Serial.println("Please press 'y' if YES or 'n' if NO");

}

void loop()
{


  conferimAdmin();
  // Obstecle FRONT
  frontObstacle();

  //Obstacle BACK
  backObstacle();

//  // If FRONT and LEFT
//  //frontLeftObstacle();
//  
//  /*If FRONT, RIGHT and left sensor meet value */
//  //FrontLeftRightObstacle();
//
//  //If front, right sensor active
//  //frontRightObstacle();

  

#ifdef SMCE
  // Avoid over-using the CPU if we are running in the emulator
  delay(1);
#endif
}





void backObstacle()
{
  const auto distanceBackIR = backIR.getDistance();
  if (distanceBackIR > 0 && distanceBackIR < 70)
  {
    car.setSpeed(0);
    delay(2000);
    car.setSpeed(carSpeed);
  }

}


void frontObstacle()
{

  String command;
  const auto front_distance = front.getDistance();
  const auto distanceIR = sideFrontIR.getDistance();

  if (front_distance > 0 && front_distance < 60 || distanceIR > 25 && distanceIR < 70 )
  {

    car.setSpeed(0);
    counter++;
    if(counter > 2000 )
    {
       car.setSpeed(-50);
        delay(1000);
        car.setSpeed(0);
        delay(1000);
        car.setSpeed(-50);
        car.setAngle(90);
        delay(2000);
        car.setAngle(0);
        car.setSpeed(carSpeed);
        car.setSpeed(50);
        counter = 0;
    }
      
    }

    /* Give the possibility of user interference after the car is stopped. */

    if (Serial.available())
    {
      command = Serial.readStringUntil('\n');
      command.trim(); //Make less prone to error by triming the white spaces if mistakingly being typed.
      if (command.equals("y"))
      {

        Serial.println("How I can help you?");
        delay(10000);

        Serial.println("Ok! Have a great day");
        delay(3000);
        car.setSpeed(-50);
        delay(1000);
        car.setSpeed(0);
        delay(1000);
        car.setSpeed(-50);
        car.setAngle(90);
        delay(2000);
        car.setAngle(0);
        car.setSpeed(carSpeed);
        car.setSpeed(50);



      }
      else if (command.equals("n"))
      {
        Serial.println("Ok! Have a great day");
        delay(3000);
        car.setSpeed(-50);
        delay(1000);
        car.setSpeed(0);
        delay(1000);

        car.setSpeed(-50);
        car.setAngle(90);
        delay(2000);
        car.setAngle(0);

        car.setSpeed(carSpeed);

        car.setSpeed(50);
      }

      else
      {
        Serial.println("Wrong command!");
        Serial.println("Please type 'y' if YES 'n' if NO");
      }
    }
    
  
#ifdef SMCE
  // Avoid over-using the CPU if we are running in the emulator
  delay(1);
#endif
}





void conferimAdmin()
{
  const auto front_distance = front.getDistance();
  const auto distanceIR = sideFrontIR.getDistance();
  const auto distanceBackIR = backIR.getDistance();
  if(car.getSpeed() == 0 && front_distance > 65 || car.getSpeed() == 0 || distanceIR > 70)
  {
    Serial.println("HELP!");
    Serial.println("HELP!");
    
  }
 
}






//void FrontLeftRightObstacle()
//{
//  const auto front_distance = front.getDistance();
//  const auto distanceLeftIR = leftIR.getDistance();
//  const auto distanceRightIR = rightIR.getDistance();
//  if (front_distance > 0 && front_distance < 70 && distanceLeftIR > 25 && distanceLeftIR < 100 && distanceRightIR > 25 && distanceRightIR < 100)
//  {
//    car.setSpeed(0);
//    delay(2000);
//    car.setSpeed(-50);
//    delay(2000);
//    car.setSpeed(0);
//    delay(2000);
//    car.setAngle(180);
//    car.setSpeed(carSpeed);
//    delay(2000);
//    car.setAngle(0);
//  }
//
//}
//void frontLeftObstacle()
//{
//
//  const auto distanceLeftIR = leftIR.getDistance();
//  const auto front_distance = front.getDistance();
//  if (front_distance > 0 && front_distance < 70 && distanceLeftIR > 25 && distanceLeftIR < 100 )
//  {
//    car.setSpeed(0);
//    delay(2000);
//    car.setSpeed(-50);
//    delay(1000);
//    car.setSpeed(0);
//    delay(1000);
//    car.setAngle(60);
//    car.setSpeed(carSpeed);
//    delay(1700);
//    car.setAngle(0);
//  }
//
//}
//
//void frontRightObstacle()
//{
//  const auto front_distance = front.getDistance();
//  const auto distanceRightIR = rightIR.getDistance();
//  if (front_distance > 0 && front_distance < 70 && distanceRightIR > 25 && distanceRightIR < 100)
//  {
//    car.setSpeed(0);
//    delay(2000);
//    car.setSpeed(-50);
//    delay(1000);
//    car.setSpeed(0);
//    delay(1000);
//    car.setAngle(-65);
//    car.setSpeed(carSpeed);
//    delay(1700);
//    car.setAngle(0);
//
//  }
//
//}
