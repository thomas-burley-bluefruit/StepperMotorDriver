#include "Initialisation.h"
#include "GpioDriver.h"
#include "InterruptTimer.h"
#include "L298n.h"
#include "UserButton.h"

void Initialisation::Initialise() {}

driver::IGpioDriver& Initialisation::GetGpioDriver()
{
  static driver::GpioDriver gpioDriver;
  return gpioDriver;
}

driver::IInterruptTimer& Initialisation::GetInterruptTimer()
{
  static driver::InterruptTimer interruptTimer;
  interruptTimer.Init();
  return interruptTimer;
}

userinput::IUserButton& Initialisation::GetUserButton()
{
  static userinput::UserButton userButton(GetGpioDriver(), GetInterruptTimer());
  return userButton;
}

motor::IDualChannelMotorDriver& Initialisation::GetDualChannelMotorDriver()
{
  static motor::L298n l298n(GetGpioDriver());
  return l298n;
}
