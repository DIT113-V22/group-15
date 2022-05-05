#include <vector>

#include <MQTT.h>
#include <WiFi.h>
#ifdef __SMCE__
#include <OV767X.h>
#endif

#include <Smartcar.h>

MQTTClient mqtt;
WiFiClient net;

//Global variable

// Pins decleration...
const auto triggerPin = 6;
const auto echoPin = 7;
const auto frontIR_PIN = 0;
const auto leftIR_PIN = 1;
const auto rightIR_PIN = 2;
const auto backIR_PIN = 3;
const auto maxDistance = 400;


bool obstacle = false;
unsigned int angleToTurn =            1;




const char ssid[] =                 "***";
const char pass[] =                "****";

const auto carSpeed =                50;
const int SPEED_INCREMENT =          20;



unsigned long previousTransmission = 0UL;
unsigned long currentTime = millis();
const auto oneSecond = 1000UL;



const int fSpeed   =                 70;  // 70% of the full speed forward
const int bSpeed   =                -70; // 70% of the full speed backward
const int lDegrees =                -75; // degrees to turn left
const int rDegrees =                 75;  // degrees to turn right


ArduinoRuntime arduinoRuntime;
BrushedMotor leftMotor(arduinoRuntime, smartcarlib::pins::v2::leftMotorPins);
BrushedMotor rightMotor(arduinoRuntime, smartcarlib::pins::v2::rightMotorPins);
DifferentialControl control(leftMotor, rightMotor);


SimpleCar car(control);


SR04 US_front(arduinoRuntime, triggerPin, echoPin, maxDistance);
GP2Y0A02 FrontIR(arduinoRuntime, frontIR_PIN);
GP2Y0A02 backIR(arduinoRuntime, backIR_PIN );
GP2Y0A02 leftIR(arduinoRuntime, leftIR_PIN );
GP2Y0A02 rightIR(arduinoRuntime, rightIR_PIN);



std::vector<char> frameBuffer;



void setup() {

  Serial.begin(9600);


#ifdef __SMCE__
  Camera.begin(QVGA, RGB888, 15);
  frameBuffer.resize(Camera.width() * Camera.height() * Camera.bytesPerPixel());

#else
  WiFi.begin(ssid, pass);
  mqtt.begin(net);
#endif

  
  Serial.println("Connecting to WiFi...");
  auto wifiStatus = WiFi.status();
  while (wifiStatus != WL_CONNECTED && wifiStatus != WL_NO_SHIELD) {
    Serial.println(wifiStatus);
    Serial.print(".");
    delay(1000);
    wifiStatus = WiFi.status();
  }


  Serial.println("Connecting to MQTT broker");
  while (!mqtt.connect("arduino", "public", "public")) {
    Serial.print(".");
    delay(1000);
  }

  
  if (mqtt.connect("arduino", "public", "public")) {
    mqtt.subscribe("/smartcar/control/#", 1);
    mqtt.onMessage([](String topic, String message) {
      if (topic == "/smartcar/control/throttle") {

        car.setSpeed(message.toInt());
      } else if (topic == "/smartcar/control/steering") {

        car.setAngle(message.toInt());
      } else if ( topic == "/smartcar/control/autoDrive") {
        if (message.toInt() == 1) {
          car.setSpeed(50);
          autoDrive();
        }
      }
      else if ( topic == "/smartcar/control/ManuellDrive") {
        if (message.toInt() == 1) {
          manualControl();
        }
      }
      else {
        Serial.println(topic + " " + message);
      }
    });
  }
}

void loop() {

//  autoDrive();
//  manualControl();
  detectObstacle();


  if (mqtt.connected()) {
    mqtt.loop();
    const auto currentTime = millis();

#ifdef __SMCE__
    static auto previousFrame = 0UL;
    if (currentTime - previousFrame >= 65) {
      previousFrame = currentTime;
      Camera.readFrame(frameBuffer.data());
      mqtt.publish("/smartcar/camera", frameBuffer.data(), frameBuffer.size(),
                   false, 0);
    }

#endif

  }

#ifdef __SMCE__
  // Avoid over-using the CPU if we are running in the emulator
  delay(1);
#endif
}




void manualControl()
{ // handle serial input if there is any
  obstacle = false;


  if (Serial.available())
  {
    char input = Serial.read(); // read everything that has been received so far and log down
    // the last entry
    switch (input)
    {
      case 'a': // rotate counter-clockwise going forward
        car.setSpeed(fSpeed);
        car.setAngle(lDegrees);
        break;
      case 'd': // turn clock-wise
        car.setSpeed(fSpeed);
        car.setAngle(rDegrees);
        break;
      case 'w': // go ahead
        car.setSpeed(fSpeed);
        car.setAngle(0);
        break;
      case 's': // go back
        car.setSpeed(bSpeed);
        car.setAngle(0);
        break;
      default: // if you receive something that you don't know, just stop
        car.setSpeed(0);
        car.setAngle(0);
    }
  }
}




void detectObstacle()
{
  auto distance_front_IR = FrontIR.getDistance();
  auto US_front_distance = US_front.getDistance();
  //  if (US_front_distance > 300 && distance_front_IR > 125)
  //  {
  //    car.setSpeed(carSpeed);
  //  }

  
  if (US_front_distance > 100 && US_front_distance < 200 || distance_front_IR > 100 && distance_front_IR < 125)
  {
    Serial.print("Obstacle has been detected");
    car.setSpeed(carSpeed - SPEED_INCREMENT);
  }

  else if (US_front_distance > 0 && US_front_distance < 100 && distance_front_IR > 0 && distance_front_IR < 100)
  {
    
    obstacle = true;
  }
  else
  {
    car.setSpeed(carSpeed); 
  }
}



void autoDrive()
{
  if(obstacle == true)
  {
    static long previousTransmission = 0UL;
    long currentTime = millis();

    const auto oneSecond = 1000UL;

    car.setSpeed(0);
    Serial.println(currentTime);
    if (currentTime - previousTransmission >= oneSecond * 20)
    {

      car.setSpeed(-50);
      delay(1000);
      randomAngle();
      delay(2000);
      car.setAngle(0);
      car.setSpeed(carSpeed);
      previousTransmission = currentTime;
      currentTime = 0UL;
    }
  }




}

void randomAngle()
{
  int angle = angleToTurn * rand() % 180;
  car.setAngle(angle);

}

//void decreaseSpeed() {
//  carSpeed = carSpeed - SPEED_INCREMENT;
//  car.setSpeed(carSpeed);
//}
