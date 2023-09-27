#pragma once

#include "ICommandInterpreter.h"
#include "ICommandRegistry.h"
#include "IDualChannelMotorDriver.h"
#include "IGpioDriver.h"
#include "IInterruptTimer10Khz.h"
#include "IInterruptTimer1Khz.h"
#include "IStepper.h"
#include "ITerminalIn.h"
#include "ITerminalOut.h"
#include "IUartDriver.h"
#include "IUserButton.h"
#include "UserControl.h"

class Initialisation
{
public:
  static void Initialise();
  static terminal::ITerminalIn& GetTerminalIn();

private:
  static command::ICommandInterpreter& GetCommandInterpreter();
  static command::ICommandRegistry& GetCommandRegistry();
  static driver::IGpioDriver& GetGpioDriver();
  static driver::IInterruptTimer1Khz& GetInterruptTimer1Khz();
  static driver::IInterruptTimer10Khz& GetInterruptTimer10Khz();
  static driver::IUartDriver& GetUartDriver();
  static motor::IDualChannelMotorDriver& GetDualChannelMotorDriver();
  static motor::IStepper& GetStepper();
  static motor::UserControl& GetUserControl();
  static terminal::ITerminalOut& GetTerminalOut();
  static userinput::IUserButton& GetUserButton();
};
