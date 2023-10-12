#include "Initialisation.h"
#include "CommandInterpreter.h"
#include "CommandRegistry.h"
#include "GpioDriver.h"
#include "InterruptTimer10Khz.h"
#include "InterruptTimer1Khz.h"
#include "L298n.h"
#include "Stepper.h"
#include "StepperDriver.h"
#include "TerminalIn.h"
#include "TerminalOut.h"
#include "UartDriver.h"
#include "UserButton.h"

void Initialisation::Initialise()
{
  (void)GetUserControl();
  (void)GetTerminalIn();
  GetTerminalIn().Start();
}

terminal::ITerminalIn& Initialisation::GetTerminalIn()
{
  static terminal::TerminalIn terminalIn(GetUartDriver(),
    GetCommandInterpreter());
  return terminalIn;
}

command::ICommandInterpreter& Initialisation::GetCommandInterpreter()
{
  static command::CommandInterpreter commandInterpreter(GetTerminalOut(),
    GetCommandRegistry());
  return commandInterpreter;
}

command::ICommandRegistry& Initialisation::GetCommandRegistry()
{
  static command::CommandRegistry commandRegistry;
  return commandRegistry;
}

driver::IGpioDriver& Initialisation::GetGpioDriver()
{
  static driver::GpioDriver gpioDriver;
  return gpioDriver;
}

driver::IInterruptTimer1Khz& Initialisation::GetInterruptTimer1Khz()
{
  static driver::InterruptTimer1Khz interruptTimer1Khz;
  interruptTimer1Khz.Init();
  return interruptTimer1Khz;
}

driver::IInterruptTimer10Khz& Initialisation::GetInterruptTimer10Khz()
{
  static driver::InterruptTimer10Khz interruptTimer10Khz;
  interruptTimer10Khz.Init();
  return interruptTimer10Khz;
}

driver::IUartDriver& Initialisation::GetUartDriver()
{
  static driver::UartDriver uartDriver;
  return uartDriver;
}

driver::IDualChannelMotorDriver& Initialisation::GetDualChannelMotorDriver()
{
  static driver::L298n l298n(GetGpioDriver());
  return l298n;
}

motor::IStepper& Initialisation::GetStepper()
{
  static motor::Stepper stepper(GetStepperDriver(), GetInterruptTimer10Khz());
  return stepper;
}

motor::IStepperDriver& Initialisation::GetStepperDriver()
{
  static motor::StepperDriver stepperDriver(GetDualChannelMotorDriver());
  return stepperDriver;
}

motor::UserControl& Initialisation::GetUserControl()
{
  static motor::UserControl userControl(GetStepper(), GetUserButton(),
    GetCommandRegistry());
  return userControl;
}

terminal::ITerminalOut& Initialisation::GetTerminalOut()
{
  static terminal::TerminalOut terminalOut(GetUartDriver());
  return terminalOut;
}

userinput::IUserButton& Initialisation::GetUserButton()
{
  static userinput::UserButton userButton(GetGpioDriver(),
    GetInterruptTimer1Khz());
  return userButton;
}
