#pragma once

#include "IButtonPressCallback.h"
#include "ICommandHandler.h"
#include "ICommandRegistry.h"
#include "IStepper.h"
#include "IUserButton.h"

namespace motor
{

class UserControl final :
  public userinput::IButtonPressCallback,
  public command::ICommandHandler
{
public:
  UserControl(IStepper& stepper, userinput::IUserButton& button,
    command::ICommandRegistry& commandRegistry);

  // IButtonPressCallback
  void OnButtonPress() override;

  // ICommandHandler
  command::ComponentName Name() const override;
  bool Run(command::ICommandData& command) override;

  static constexpr char const* RunCommandName = "run";
  static constexpr char const* StopCommandName = "stop";
  static constexpr char const* SetCommandName = "set";
  static constexpr char const* StepCommandName = "step";
  static constexpr char const* DrpmParameterName = "drpm";
  static constexpr char const* StepsParameterName = "steps";
  static constexpr char const* StepsPerSecParameterName = "stepspersec";

private:
  IStepper& mStepper;
};

}