#include "Initialisation.h"
#include "CommandInterpreter.h"
#include "CommandRegistry.h"
#include "GpioDriver.h"
#include "InterruptTimer1Khz.h"
#include "L298n.h"
#include "Stepper.h"
#include "TerminalIn.h"
#include "TerminalOut.h"
#include "UartDriver.h"
#include "UserButton.h"

void Initialisation::Initialise()
{
  (void)GetUserControl();
  (void)GetTerminalIn();
  GetStepper().Init();
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

driver::IUartDriver& Initialisation::GetUartDriver()
{
  static driver::UartDriver uartDriver;
  return uartDriver;
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
