
#define D4 2
#define D5 14

#include "InterfaceActuatorPWM.h"
#include "LampPWM.h"
#include "VentilatorPWM.h"

VentilatorPWM* actuator = new VentilatorPWM(D5);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  actuator->turnOff();
  Serial.print("Turn off\n");
  delay(1000);
  actuator->turnOn();
  Serial.print("Turn on no parameter\n");
  delay(1000);
  actuator->turnOn(0);
  Serial.print("Turn on\n");
  delay(1000);
  actuator->changeDutycycle(10);
  Serial.print("10\n");
  delay(1000);
  actuator->changeDutycycle(30);
  Serial.print("30\n");
  delay(1000);
  actuator->changeDutycycle(50);
  Serial.print("50\n");
  delay(1000);
  actuator->changeDutycycle(70);
  Serial.print("70\n");
  delay(1000);
  actuator->changeDutycycle(90);
  Serial.print("90\n");
  delay(1000);
}
