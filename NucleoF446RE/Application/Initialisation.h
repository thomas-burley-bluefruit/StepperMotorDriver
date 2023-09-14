#pragma once

#include "IGpioDriver.h"
#include "IInterruptTimer.h"
#include "IUserButton.h"

class Initialisation
{
public:
  static void Initialise();

private:
  static driver::IGpioDriver& GetGpioDriver();
  static driver::IInterruptTimer& GetInterruptTimer();
  static userinput::IUserButton& GetUserButton();
};