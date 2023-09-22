#include "Initialisation.h"
#include "GpioDriver.h"
#include "InterruptTimer.h"
#include "L298n.h"
#include "Stepper.h"
#include "UserButton.h"

void Initialisation::Initialise()
{
  (void)GetUserControl();
  GetStepper().Init();
}

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

motor::IDualChannelMotorDriver& Initialisation::GetDualChannelMotorDriver()
{
  static motor::L298n l298n(GetGpioDriver());
  return l298n;
}

motor::IStepper& Initialisation::GetStepper()
{
  static motor::Stepper stepper(GetDualChannelMotorDriver());
  return stepper;
}

motor::UserControl& Initialisation::GetUserControl()
{
  static motor::UserControl userControl(GetStepper(), GetUserButton());
  return userControl;
}

userinput::IUserButton& Initialisation::GetUserButton()
{
  static userinput::UserButton userButton(GetGpioDriver(), GetInterruptTimer());
  return userButton;
}
