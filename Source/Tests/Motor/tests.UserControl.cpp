#include "mock.CommandData.h"
#include "mock.CommandRegistry.h"
#include "mock.Stepper.h"
#include "mock.UserButton.h"
#include "UserControl.h"
#include "gmock/gmock.h"

using namespace ::command;
using namespace ::motor;
using namespace ::testing;
using namespace ::userinput;

class UserControlTests : public Test
{
public:
  UserControlTests() :
    mUserControl(mStepper, mUserButton, mCommandRegistry)
  {
  }

protected:
  MockStepper mStepper;
  MockUserButton mUserButton;
  MockCommandRegistry mCommandRegistry;
  UserControl mUserControl;
};

TEST_F(UserControlTests, registers_with_user_button_on_contruction)
{
  ASSERT_TRUE(mUserButton.RegisterCallbackCalled);
  ASSERT_EQ(&mUserControl, mUserButton.Callback);
}

TEST_F(UserControlTests, registers_with_command_handler_on_contruction)
{
  ASSERT_TRUE(mCommandRegistry.RegisterHandlerCalled);
  ASSERT_EQ(&mUserControl, mCommandRegistry.RegisteredHandler);
}

TEST_F(UserControlTests, button_press_moves_motor_one_step)
{
  // Given, when
  mUserControl.OnButtonPress();

  // Then
  ASSERT_TRUE(mStepper.MoveCalled);
  ASSERT_EQ(1, mStepper.MoveSteps);
}

TEST_F(UserControlTests, step_command_moves_motor_n_steps)
{
  // Given
  MockCommandData command;
  command.GetCommandReturnValue = UserControl::StepCommandName;
  const uint32_t expectedSteps = 123;
  command.Pairs[UserControl::StepsParameterName] =
    std::to_string(expectedSteps);
  command.GetParameterCountReturnValue = 1;

  // When
  mUserControl.Run(command);

  // Then
  ASSERT_TRUE(mStepper.MoveCalled);
  ASSERT_EQ(expectedSteps, mStepper.MoveSteps);
}

TEST_F(UserControlTests, set_steps_per_sec_command_sets_sps_on_stepper)
{
  // Given
  MockCommandData command;
  command.GetCommandReturnValue = UserControl::SetCommandName;
  const uint32_t expectedSteps = 123;
  command.Pairs[UserControl::StepsPerSecParameterName] =
    std::to_string(expectedSteps);
  command.GetParameterCountReturnValue = 1;

  // When
  mUserControl.Run(command);

  // Then
  ASSERT_TRUE(mStepper.SetStepsPerSecondCalled);
  ASSERT_EQ(expectedSteps, mStepper.SetStepsPerSecondSteps);
}