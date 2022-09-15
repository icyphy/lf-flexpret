// ConstantSpeed.pde
// -*- mode: C++ -*-
//
// Shows how to run AccelStepper in the simplest,
// fixed speed mode with no accelerations
/// \author  Mike McCauley (mikem@airspayce.com)
// Copyright (C) 2009 Mike McCauley
// $Id: ConstantSpeed.pde,v 1.1 2011/01/05 01:51:01 mikem Exp mikem $
 
#include <AccelStepper.h>

AccelStepper stepper(AccelStepper::DRIVER, 5, 2);

void setup()
{
   stepper.setMaxSpeed(1000);
   stepper.setSpeed(100); // AccelStepper does not recommend setting this above 1000.
}
 
void loop()
{
   stepper.runSpeed();
}
