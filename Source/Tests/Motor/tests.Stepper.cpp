#include "FullStepSequence.h"
#include "mock.DualChannelMotorDriver.h"
#include "mock.InterruptTimer10Khz.h"
#include "Stepper.h"
#include "gmock/gmock.h"

using namespace ::driver;
using namespace ::motor;
using namespace ::testing;

class StepperTests : public Test
{
public:
  StepperTests() :
    mStepper(mMotorDriver, mInterruptTimer)
  {
  }

protected:
  void SendTimerTicks(size_t ticks)
  {
    for (size_t i = 0; i < ticks; ++i)
      mStepper.OnTimerInterrupt();
  }

  size_t DrpmToStepsPerSec(const size_t speedDrpm)
  {
    return ((speedDrpm / 10) / 60) * 200;
  }

  MockDualChannelMotorDriver mMotorDriver;
  MockInterruptTimer10Khz mInterruptTimer;
  Stepper mStepper;
};

TEST_F(StepperTests, registers_with_interrupt_timer_on_construction)
{
  ASSERT_TRUE(mInterruptTimer.RegisterCallbackCalled);
  ASSERT_EQ(&mStepper, mInterruptTimer.Callback);
}

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
  mStepper.Move(1);

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
    mStepper.Move(1);

    // Then
    ASSERT_EQ(2, mMotorDriver.SetDirectionCalls.CallCount());
    ASSERT_TRUE(mMotorDriver.SetDirectionCalls.CalledWithParams(
      {Channel::A, sFullStepSequence[expectedSequencePos].ChannelADirection}));
    ASSERT_TRUE(mMotorDriver.SetDirectionCalls.CalledWithParams(
      {Channel::B, sFullStepSequence[expectedSequencePos].ChannelBDirection}));

    mMotorDriver.Reset();
  }
}

TEST_F(StepperTests, double_step_performs_second_step_on_the_timer_tick_after_the_period_between_steps_has_elapsed)
{
  // Given
  mStepper.Init();
  mMotorDriver.Reset();

  std::vector<MockDualChannelMotorDriver::SetDirectionParams>
    expectedSetDirectionCalls;

  size_t expectedSequencePos = 1;
  expectedSetDirectionCalls.push_back({.channel = Channel::A,
    .direction = sFullStepSequence[expectedSequencePos].ChannelADirection});
  expectedSetDirectionCalls.push_back({.channel = Channel::B,
    .direction = sFullStepSequence[expectedSequencePos].ChannelBDirection});

  // When
  mStepper.Move(2);

  // First step performed immediately:
  ASSERT_TRUE(
    mMotorDriver.SetDirectionCalls.CallSequenceEq(expectedSetDirectionCalls));

  const size_t timerRateHz = 10'000;
  const size_t expectedTimerTicksToNextStep =
    timerRateHz / Stepper::DefaultStepsPerSecond;

  SendTimerTicks(expectedTimerTicksToNextStep - 1);

  // No further calls have been made:
  ASSERT_TRUE(
    mMotorDriver.SetDirectionCalls.CallSequenceEq(expectedSetDirectionCalls));

  SendTimerTicks(1);

  expectedSequencePos = 2;
  expectedSetDirectionCalls.push_back({.channel = Channel::A,
    .direction = sFullStepSequence[expectedSequencePos].ChannelADirection});
  expectedSetDirectionCalls.push_back({.channel = Channel::B,
    .direction = sFullStepSequence[expectedSequencePos].ChannelBDirection});

  // Then
  ASSERT_TRUE(
    mMotorDriver.SetDirectionCalls.CallSequenceEq(expectedSetDirectionCalls));
}

TEST_F(StepperTests, multiple_steps_performed_before_going_idle)
{
  // Given
  mStepper.Init();
  mMotorDriver.Reset();

  std::vector<MockDualChannelMotorDriver::SetDirectionParams>
    expectedSetDirectionCalls;

  size_t expectedSequencePos = 1;
  expectedSetDirectionCalls.push_back({.channel = Channel::A,
    .direction = sFullStepSequence[expectedSequencePos].ChannelADirection});
  expectedSetDirectionCalls.push_back({.channel = Channel::B,
    .direction = sFullStepSequence[expectedSequencePos].ChannelBDirection});

  // When
  const size_t expectedSteps = 5;
  mStepper.Move(expectedSteps);

  // First step performed immediately:
  ASSERT_TRUE(
    mMotorDriver.SetDirectionCalls.CallSequenceEq(expectedSetDirectionCalls));

  const size_t expectedTimerTicksToNextStep =
    mInterruptTimer.GetInterruptRateHz() / Stepper::DefaultStepsPerSecond;

  for (size_t i = 0; i < expectedSteps - 1; ++i)
  {
    SendTimerTicks(expectedTimerTicksToNextStep);
    expectedSequencePos =
      (expectedSequencePos + 1) % FullStepSequence::StepsInSequence;

    expectedSetDirectionCalls.push_back({.channel = Channel::A,
      .direction = sFullStepSequence[expectedSequencePos].ChannelADirection});
    expectedSetDirectionCalls.push_back({.channel = Channel::B,
      .direction = sFullStepSequence[expectedSequencePos].ChannelBDirection});

    // Then
    ASSERT_TRUE(
      mMotorDriver.SetDirectionCalls.CallSequenceEq(expectedSetDirectionCalls));
  }

  // Then: no further steps made
  SendTimerTicks(expectedTimerTicksToNextStep * 2);
  ASSERT_TRUE(
    mMotorDriver.SetDirectionCalls.CallSequenceEq(expectedSetDirectionCalls));
}

TEST_F(StepperTests, run_runs_motor_at_specified_drpm)
{
  // Given
  mStepper.Init();
  mMotorDriver.Reset();

  const size_t speedDrpm = 1000;
  const size_t expectedStepsPerSecond = DrpmToStepsPerSec(speedDrpm);
  const size_t expectedTimerTicksPerStep =
    mInterruptTimer.GetInterruptRateHz() / expectedStepsPerSecond;

  // When
  mStepper.Run(speedDrpm);

  // Then
  ASSERT_EQ(expectedStepsPerSecond, mStepper.GetStepsPerSecond());
  ASSERT_TRUE(mStepper.Running());

  const size_t stepsToCheck = Stepper::StepsPerRotation * 10;
  const size_t motorDriverCallsPerStep = 2;
  for (size_t i = 0; i < stepsToCheck; ++i)
  {
    SendTimerTicks(expectedTimerTicksPerStep);
    ASSERT_EQ(motorDriverCallsPerStep * (i + 1),
      mMotorDriver.SetDirectionCalls.CallCount());
  }
}

TEST_F(StepperTests, stop_stops_motor)
{
  // Given
  mStepper.Init();
  mMotorDriver.Reset();

  const size_t speedDrpm = 1000;
  const size_t expectedStepsPerSecond = DrpmToStepsPerSec(speedDrpm);
  const size_t expectedTimerTicksPerStep =
    mInterruptTimer.GetInterruptRateHz() / expectedStepsPerSecond;

  mStepper.Run(speedDrpm);
  ASSERT_TRUE(mStepper.Running());
  const size_t motorDriverCallsPerStep = 2;
  SendTimerTicks(expectedTimerTicksPerStep);
  ASSERT_EQ(motorDriverCallsPerStep,
    mMotorDriver.SetDirectionCalls.CallCount());

  // When
  mStepper.Stop();
  SendTimerTicks(expectedTimerTicksPerStep);

  // Then
  ASSERT_FALSE(mStepper.Running());
  ASSERT_EQ(motorDriverCallsPerStep,
    mMotorDriver.SetDirectionCalls.CallCount());
}