#include "FullStepSequence.h"
#include "mock.DualChannelMotorDriver.h"
#include "Stepper.h"
#include "gmock/gmock.h"

using namespace ::motor;
using namespace ::testing;

class StepperTests : public Test
{
public:
  StepperTests() :
    mStepper(mMotorDriver)
  {
  }

protected:
  MockDualChannelMotorDriver mMotorDriver;
  Stepper mStepper;
};

TEST_F(StepperTests, init_sets_motor_channels_to_start_of_full_step_sequence)
{
  // Given, when
  mStepper.Init();

  // Then
  ASSERT_EQ(2, mMotorDriver.SetDirectionCalls.CallCount());
  ASSERT_TRUE(mMotorDriver.SetDirectionCalls.CalledWithParams(
    {Channel::A, sFullStepSequence[0].ChannelADirection}));
  ASSERT_TRUE(mMotorDriver.SetDirectionCalls.CalledWithParams(
    {Channel::B, sFullStepSequence[0].ChannelBDirection}));
}

TEST_F(StepperTests, single_step_sets_motor_channels_to_second_step_in_sequence)
{
  // Given
  mStepper.Init();
  mMotorDriver.Reset();

  // When
  mStepper.Step(1);

  // Then
  ASSERT_EQ(2, mMotorDriver.SetDirectionCalls.CallCount());
  ASSERT_TRUE(mMotorDriver.SetDirectionCalls.CalledWithParams(
    {Channel::A, sFullStepSequence[1].ChannelADirection}));
  ASSERT_TRUE(mMotorDriver.SetDirectionCalls.CalledWithParams(
    {Channel::B, sFullStepSequence[1].ChannelBDirection}));
}

TEST_F(StepperTests, subsequent_single_steps_cycle_through_step_sequence)
{
  // Given
  mStepper.Init();
  mMotorDriver.Reset();

  for (size_t i = 1; i < FullStepSequence::StepsInSequence * 2; ++i)
  {
    const auto expectedSequencePos = i % FullStepSequence::StepsInSequence;

    // When
    mStepper.Step(1);

    // Then
    ASSERT_EQ(2, mMotorDriver.SetDirectionCalls.CallCount());
    ASSERT_TRUE(mMotorDriver.SetDirectionCalls.CalledWithParams(
      {Channel::A, sFullStepSequence[expectedSequencePos].ChannelADirection}));
    ASSERT_TRUE(mMotorDriver.SetDirectionCalls.CalledWithParams(
      {Channel::B, sFullStepSequence[expectedSequencePos].ChannelBDirection}));

    mMotorDriver.Reset();
  }
}