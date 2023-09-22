#include "UserControl.h"

using namespace motor;

UserControl::UserControl(IStepper& stepper, userinput::IUserButton& button) :
  mStepper(stepper)
{
  button.RegisterCallback(this);
}

void UserControl::OnButtonPress()
{
  mStepper.Step(1);
}
