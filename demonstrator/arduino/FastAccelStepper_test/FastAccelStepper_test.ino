// Driving a stepper motor to a high speed.

#include "FastAccelStepper.h"

// As in StepperDemo for Motor 1 on AVR
#define dirPinStepper 2
#define enablePinStepper 8
#define stepPinStepper 9

int microstep = 800;
//int rev = 1000;
uint32_t _speed;

FastAccelStepperEngine engine = FastAccelStepperEngine();
FastAccelStepper *stepper = NULL;

void setup() {
  Serial.begin(9600);
  engine.init();
  stepper = engine.stepperConnectToPin(stepPinStepper);
  _speed = stepper->getMaxSpeedInHz(); // max speed
  if (stepper) {
    stepper->setDirectionPin(dirPinStepper);
    stepper->setEnablePin(enablePinStepper);
    stepper->setAutoEnable(true);

    stepper->setSpeedInHz(_speed); // steps/s
    stepper->setAcceleration(1000); // < 2000
//    stepper->move(microstep * rev);
    stepper->runForward();
    delay(25 * 1000);
    stepper->stopMove();
  }
}

void loop() {
  Serial.println(stepper->isRunning());
  Serial.println(_speed);
  delay(1000);  
}
