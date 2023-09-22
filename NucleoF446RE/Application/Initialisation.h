#pragma once

#include "IDualChannelMotorDriver.h"
#include "IGpioDriver.h"
#include "IInterruptTimer.h"
#include "IStepper.h"
#include "IUserButton.h"
#include "UserControl.h"

class Initialisation
{
public:
  static void Initialise();

private:
  static driver::IGpioDriver& GetGpioDriver();
  static driver::IInterruptTimer& GetInterruptTimer();
  static motor::IDualChannelMotorDriver& GetDualChannelMotorDriver();
  static motor::IStepper& GetStepper();
  static motor::UserControl& GetUserControl();
  static userinput::IUserButton& GetUserButton();
};
