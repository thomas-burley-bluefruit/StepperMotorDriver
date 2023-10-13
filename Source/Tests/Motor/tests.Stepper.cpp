#include "FullStepSequence.h"
#include "mock.InterruptTimer10Khz.h"
#include "mock.StepperDriver.h"
#include "Stepper.h"
#include "gmock/gmock.h"

using namespace ::driver;
using namespace ::motor;
using namespace ::testing;

class StepperTests : public Test
{
public:
  StepperTests() :
    mStepper(mStepperDriver, mInterruptTimer)
  {
  }

protected:
  void SendTimerTicks(size_t ticks)
  {
    for (size_t i = 0; i < ticks; ++i)
      mStepper.OnTimerInterrupt();
  }

  float DrpmToStepsPerSec(const size_t speedDrpm)
  {
    return ((speedDrpm / 10.0f) / 60.0f) * 200.0f;
  }

  int32_t StepsPerSecToDrpm(const float stepsPerSec)
  {
    return static_cast<int32_t>(roundf(((stepsPerSec * 10.0f) * 60) / 200));
  }

  MockStepperDriver mStepperDriver;
  MockInterruptTimer10Khz mInterruptTimer;
  Stepper mStepper;
};

TEST_F(StepperTests, registers_with_interrupt_timer_on_construction)
{
  ASSERT_TRUE(mInterruptTimer.RegisterCallbackCalled);
  ASSERT_EQ(&mStepper, mInterruptTimer.Callback);
}

TEST_F(StepperTests, moving_1_step_commands_the_stepper_driver_to_step_forward_once)
{
  // Given, when
  mStepper.Move(1);
  mStepper.OnTimerInterrupt();

  // Then
  ASSERT_EQ(1, mStepperDriver.StepCalls.CallCount());
  ASSERT_TRUE(
    mStepperDriver.StepCalls.CalledWithParams(motor::Direction::Forward));
}

TEST_F(StepperTests, moving_negative_1_step_commands_the_stepper_driver_to_step_backward_once)
{
  // Given, when
  mStepper.Move(-1);
  mStepper.OnTimerInterrupt();

  // Then
  ASSERT_EQ(1, mStepperDriver.StepCalls.CallCount());
  ASSERT_TRUE(
    mStepperDriver.StepCalls.CalledWithParams(motor::Direction::Reverse));
}

TEST_F(StepperTests,
double_step_performs_second_step_on_the_timer_tick_after_the_period_between_steps_has_elapsed)
{
  // Given, when
  mStepper.Move(2);
  mStepper.OnTimerInterrupt();

  // First step performed immediately:
  ASSERT_EQ(1, mStepperDriver.StepCalls.CallCount());

  const size_t timerRateHz = 10'000;
  const size_t expectedTimerTicksToNextStep =
    timerRateHz / mStepper.GetStepsPerSecond();

  SendTimerTicks(expectedTimerTicksToNextStep - 1);

  // No further calls have been made:
  ASSERT_EQ(1, mStepperDriver.StepCalls.CallCount());

  SendTimerTicks(1);

  // Then
  ASSERT_EQ(2, mStepperDriver.StepCalls.CallCount());
}

TEST_F(StepperTests, multiple_forward_steps_performed_before_going_idle)
{
  // Given
  const size_t expectedSteps = 5;
  size_t expectedStepsPerformed = 0;

  // When
  mStepper.Move(expectedSteps);

  // First step performed on next tick::
  mStepper.OnTimerInterrupt();
  expectedStepsPerformed++;
  ASSERT_EQ(1, mStepperDriver.StepCalls.CallCount());
  ASSERT_EQ(motor::Direction::Forward,
    mStepperDriver.StepCalls[expectedStepsPerformed - 1]);

  const size_t expectedTimerTicksToNextStep =
    mInterruptTimer.GetInterruptRateHz() / mStepper.GetStepsPerSecond();

  while (expectedStepsPerformed < expectedSteps)
  {
    SendTimerTicks(expectedTimerTicksToNextStep);
    expectedStepsPerformed++;
    ASSERT_EQ(expectedStepsPerformed, mStepperDriver.StepCalls.CallCount());
    ASSERT_EQ(motor::Direction::Forward,
      mStepperDriver.StepCalls[expectedStepsPerformed - 1]);
  }

  // Then: no further steps made
  SendTimerTicks(expectedTimerTicksToNextStep * 2);
  ASSERT_EQ(expectedStepsPerformed, mStepperDriver.StepCalls.CallCount());
}

TEST_F(StepperTests, multiple_reverse_steps_performed_before_going_idle)
{
  // Given
  const size_t expectedSteps = 5;
  size_t expectedStepsPerformed = 0;

  // When
  mStepper.Move(-expectedSteps);

  // First step performed on next tick::
  mStepper.OnTimerInterrupt();
  expectedStepsPerformed++;
  ASSERT_EQ(1, mStepperDriver.StepCalls.CallCount());
  ASSERT_EQ(motor::Direction::Reverse,
    mStepperDriver.StepCalls[expectedStepsPerformed - 1]);

  const size_t expectedTimerTicksToNextStep =
    mInterruptTimer.GetInterruptRateHz() / mStepper.GetStepsPerSecond();

  while (expectedStepsPerformed < expectedSteps)
  {
    SendTimerTicks(expectedTimerTicksToNextStep);
    expectedStepsPerformed++;
    ASSERT_EQ(expectedStepsPerformed, mStepperDriver.StepCalls.CallCount());
    ASSERT_EQ(motor::Direction::Reverse,
      mStepperDriver.StepCalls[expectedStepsPerformed - 1]);
  }

  // Then: no further steps made
  SendTimerTicks(expectedTimerTicksToNextStep * 2);
  ASSERT_EQ(expectedStepsPerformed, mStepperDriver.StepCalls.CallCount());
}

TEST_F(StepperTests, run_runs_motor_at_specified_forward_drpm)
{
  // Given
  mStepper.EnableRamping(false);

  const size_t speedDrpm = 1000;
  const auto expectedStepsPerSecond = DrpmToStepsPerSec(speedDrpm);
  const auto expectedTimerTicksPerStep = static_cast<size_t>(
    mInterruptTimer.GetInterruptRateHz() / expectedStepsPerSecond);

  // When
  mStepper.Run(speedDrpm);

  // Then
  ASSERT_EQ(speedDrpm, mStepper.GetRunSpeedDrpm());
  ASSERT_TRUE(mStepper.Running());

  const size_t stepsToCheck = mStepper.GetStepsPerRotation() * 10;
  for (size_t i = 0; i < stepsToCheck; ++i)
  {
    SendTimerTicks(expectedTimerTicksPerStep);
    ASSERT_EQ(i + 1, mStepperDriver.StepCalls.CallCount());
    ASSERT_EQ(motor::Direction::Forward, mStepperDriver.StepCalls[i]);
  }
}

TEST_F(StepperTests, run_runs_motor_at_specified_reverse_drpm)
{
  // Given
  mStepper.EnableRamping(false);

  const int32_t speedDrpm = 1000;
  const auto expectedStepsPerSecond = DrpmToStepsPerSec(speedDrpm);
  const auto expectedTimerTicksPerStep = static_cast<size_t>(
    mInterruptTimer.GetInterruptRateHz() / expectedStepsPerSecond);

  // When
  mStepper.Run(-speedDrpm);

  // Then
  ASSERT_EQ(-speedDrpm, mStepper.GetRunSpeedDrpm());
  ASSERT_TRUE(mStepper.Running());

  const size_t stepsToCheck = mStepper.GetStepsPerRotation() * 10;
  for (size_t i = 0; i < stepsToCheck; ++i)
  {
    SendTimerTicks(expectedTimerTicksPerStep);
    ASSERT_EQ(i + 1, mStepperDriver.StepCalls.CallCount());
    ASSERT_EQ(motor::Direction::Reverse, mStepperDriver.StepCalls[i]);
  }
}

TEST_F(StepperTests, stop_stops_motor)
{
  // Given
  mStepper.EnableRamping(false);

  const size_t speedDrpm = 1000;
  const size_t expectedStepsPerSecond = DrpmToStepsPerSec(speedDrpm);
  const size_t expectedTimerTicksPerStep =
    mInterruptTimer.GetInterruptRateHz() / expectedStepsPerSecond;

  mStepper.Run(speedDrpm);
  ASSERT_TRUE(mStepper.Running());
  SendTimerTicks(expectedTimerTicksPerStep);
  ASSERT_EQ(1, mStepperDriver.StepCalls.CallCount());

  // When
  mStepper.Stop();
  SendTimerTicks(expectedTimerTicksPerStep);

  // Then
  ASSERT_FALSE(mStepper.Running());
  ASSERT_EQ(1, mStepperDriver.StepCalls.CallCount());
}

TEST_F(StepperTests, stop_hiz_stops_running_and_sets_driver_to_hiz)
{
  // Given
  mStepper.Run(1000);
  ASSERT_TRUE(mStepper.Running());

  // When
  mStepper.StopHiZ();

  // Then
  ASSERT_FALSE(mStepper.Running());
  ASSERT_TRUE(mStepperDriver.StopHiZCalled);
}

TEST_F(StepperTests, cannot_move_while_running)
{
  // Given
  mStepper.EnableRamping(false);

  const int32_t speedDrpm = 1000;
  mStepper.Run(speedDrpm);
  ASSERT_TRUE(mStepper.Running());

  const size_t expectedStepsPerSecond = DrpmToStepsPerSec(speedDrpm);
  const size_t expectedTimerTicksPerStep =
    mInterruptTimer.GetInterruptRateHz() / expectedStepsPerSecond;

  // When
  mStepper.Move(1);
  SendTimerTicks(expectedTimerTicksPerStep);

  // Then
  ASSERT_EQ(1, mStepperDriver.StepCalls.CallCount());
}

TEST_F(StepperTests, cannot_run_while_moving)
{
  // Given
  mStepper.Move(100);

  mStepper.EnableRamping(false);

  const int32_t speedDrpm = StepsPerSecToDrpm(mStepper.GetStepsPerSecond());
  const size_t expectedTimerTicksPerStep =
    mInterruptTimer.GetInterruptRateHz() / mStepper.GetStepsPerSecond();

  // When
  mStepper.Run(speedDrpm);
  SendTimerTicks(expectedTimerTicksPerStep);

  // Then
  ASSERT_FALSE(mStepper.Running());
  ASSERT_EQ(1, mStepperDriver.StepCalls.CallCount());
}
