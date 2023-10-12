#include "UserControl.h"

using namespace motor;

UserControl::UserControl(IStepper& stepper, userinput::IUserButton& button,
  command::ICommandRegistry& commandRegistry) :
  mStepper(stepper)
{
  button.RegisterCallback(this);
  commandRegistry.RegisterHandler(this);
}

void UserControl::OnButtonPress()
{
  mStepper.Move(1);
}

command::ComponentName UserControl::Name() const
{
  return command::ComponentName::stepper;
}

bool UserControl::Run(command::ICommandData& command)
{
  if (command.CommandIs(StepCommandName) && command.GetParameterCount() == 1)
  {
    uint32_t steps = 0;
    if (!command.GetUint(StepsParameterName, steps))
      return false;

    mStepper.Move(steps);
    return true;
  }

  if (command.CommandIs(SetCommandName) && command.GetParameterCount() == 1)
  {
    uint32_t stepsPerSec = 0;
    if (!command.GetUint(StepsPerSecParameterName, stepsPerSec))
      return false;

    mStepper.SetStepsPerSecond(stepsPerSec);
    return true;
  }

  if (command.CommandIs(RunCommandName) && command.GetParameterCount() == 1)
  {
    uint32_t drpm = 0;
    if (!command.GetUint(DrpmParameterName, drpm))
      return false;

    mStepper.Run(drpm);
    return true;
  }

  if (command.CommandIs(StopCommandName) && command.GetParameterCount() == 0)
  {
    mStepper.Stop();
    return true;
  }

  if (command.CommandIs(StopHiZCommandName) && command.GetParameterCount() == 0)
  {
    mStepper.StopHiZ();
    return true;
  }

  if (command.CommandIs(SetRampRateCommandName)
    && command.GetParameterCount() == 1)
  {
    uint32_t drpmPerSec = 0;
    if (!command.GetUint(DrpmPerSecParameterName, drpmPerSec))
      return false;

    mStepper.SetRampRate(drpmPerSec);
    return true;
  }

  return false;
}