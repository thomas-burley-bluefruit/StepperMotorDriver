#include "FullStepSequence.h"
#include "mock.InterruptTimer10Khz.h"
#include "Stepper.h"
#include "gmock/gmock.h"

#include <algorithm>

using namespace ::driver;
using namespace ::motor;
using namespace ::testing;

class StepperRampingTests : public Test
{
public:
  StepperRampingTests() :
    mStepper(mInterruptTimer)
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

  void Ramp(const int32_t drpmTo)
  {
    const int32_t speedDifference = mStepper.GetRunSpeedDrpm() - drpmTo;
    const size_t expectedTicksTilRampEnd =
      abs(static_cast<float>(speedDifference)
        / mStepper.GetRampRateDrpmPerSecond())
      * static_cast<int32_t>(mInterruptTimer.GetInterruptRateHz());

    mStepper.Run(drpmTo);

    SendTimerTicks(expectedTicksTilRampEnd);
  }

  MockInterruptTimer10Khz mInterruptTimer;
  Stepper mStepper;
};

TEST_F(StepperRampingTests, speed_increase_is_ramped_at_default_rate)
{
  // Given
  const auto rampRateDrpmPerSecond = mStepper.GetRampRateDrpmPerSecond();

  const size_t startingSpeedDrpm = 0;
  const size_t targetSpeedDrpm = 1000;
  const size_t speedDifference = targetSpeedDrpm - startingSpeedDrpm;
  const size_t expectedSecondsTilRampEnd =
    speedDifference / rampRateDrpmPerSecond;

  size_t currentSpeedDrpm = startingSpeedDrpm;

  // When
  mStepper.Run(targetSpeedDrpm);

  // Then
  const size_t ticksPerSecond = mInterruptTimer.GetInterruptRateHz();

  for (size_t i = 0; i < expectedSecondsTilRampEnd; ++i)
  {
    SendTimerTicks(ticksPerSecond);
    currentSpeedDrpm += rampRateDrpmPerSecond;
    ASSERT_EQ(currentSpeedDrpm, mStepper.GetRunSpeedDrpm());
  }
}

TEST_F(StepperRampingTests, speed_increase_is_ramped_at_set_rate)
{
  // Given
  const auto rampRateDrpmPerSecond = 100;
  mStepper.SetRampRate(100);

  const size_t startingSpeedDrpm = 0;
  const size_t targetSpeedDrpm = 1000;
  const size_t speedDifference = targetSpeedDrpm - startingSpeedDrpm;
  const size_t expectedSecondsTilRampEnd =
    speedDifference / rampRateDrpmPerSecond;

  size_t currentSpeedDrpm = startingSpeedDrpm;

  // When
  mStepper.Run(targetSpeedDrpm);

  // Then
  const size_t ticksPerSecond = mInterruptTimer.GetInterruptRateHz();

  for (size_t i = 0; i < expectedSecondsTilRampEnd; ++i)
  {
    SendTimerTicks(ticksPerSecond);
    currentSpeedDrpm += rampRateDrpmPerSecond;
    ASSERT_EQ(currentSpeedDrpm, mStepper.GetRunSpeedDrpm());
  }
}

TEST_F(StepperRampingTests, speed_plateaus_after_ramping_to_target)
{
  // Given
  const auto rampRateDrpmPerSecond = mStepper.GetRampRateDrpmPerSecond();

  const size_t startingSpeedDrpm = 0;
  const size_t targetSpeedDrpm = 1000;
  const float speedDifference = targetSpeedDrpm - startingSpeedDrpm;
  const size_t expectedTicksTilRampEnd =
    (speedDifference / rampRateDrpmPerSecond)
    * mInterruptTimer.GetInterruptRateHz();

  mStepper.Run(targetSpeedDrpm);

  // When: speed ramps to target
  SendTimerTicks(expectedTicksTilRampEnd);
  ASSERT_EQ(targetSpeedDrpm, mStepper.GetRunSpeedDrpm());

  // Then: speed stays at target
  SendTimerTicks(mInterruptTimer.GetInterruptRateHz() * 10);
  ASSERT_EQ(targetSpeedDrpm, mStepper.GetRunSpeedDrpm());
}

TEST_F(StepperRampingTests, speed_decrease_is_ramped_at_default_rate)
{
  // Given
  const auto rampRateDrpmPerSecond = mStepper.GetRampRateDrpmPerSecond();

  const int32_t startingSpeedDrpm = 1000;
  mStepper.EnableRamping(false);
  mStepper.Run(startingSpeedDrpm);
  mStepper.OnTimerInterrupt();
  ASSERT_EQ(startingSpeedDrpm, mStepper.GetRunSpeedDrpm());

  mStepper.EnableRamping(true);

  const int32_t targetSpeedDrpm = 0;
  const int32_t speedDifference = targetSpeedDrpm - startingSpeedDrpm;
  const size_t expectedSecondsTilRampEnd =
    abs(speedDifference / static_cast<int32_t>(rampRateDrpmPerSecond));

  int32_t currentSpeedDrpm = startingSpeedDrpm;

  // When
  mStepper.Run(targetSpeedDrpm);

  // Then
  const size_t ticksPerSecond = mInterruptTimer.GetInterruptRateHz();

  for (size_t i = 0; i < expectedSecondsTilRampEnd; ++i)
  {
    SendTimerTicks(ticksPerSecond);
    currentSpeedDrpm =
      std::max(static_cast<int32_t>(currentSpeedDrpm - rampRateDrpmPerSecond),
        targetSpeedDrpm);
    ASSERT_EQ(currentSpeedDrpm, mStepper.GetRunSpeedDrpm());
  }
}

TEST_F(StepperRampingTests, speed_decrease_past_zero_boundary)
{
  // Given
  const auto rampRateDrpmPerSecond = mStepper.GetRampRateDrpmPerSecond();

  const int32_t startingSpeedDrpm = 1000;
  mStepper.EnableRamping(false);
  mStepper.Run(startingSpeedDrpm);
  mStepper.OnTimerInterrupt();
  ASSERT_EQ(startingSpeedDrpm, mStepper.GetRunSpeedDrpm());

  mStepper.EnableRamping(true);

  const int32_t targetSpeedDrpm = -1000;
  const int32_t speedDifference = targetSpeedDrpm - startingSpeedDrpm;
  const size_t expectedSecondsTilRampEnd =
    abs(speedDifference / static_cast<int32_t>(rampRateDrpmPerSecond));

  int32_t currentSpeedDrpm = startingSpeedDrpm;

  // When
  mStepper.Run(targetSpeedDrpm);

  // Then
  const size_t ticksPerSecond = mInterruptTimer.GetInterruptRateHz();

  for (size_t i = 0; i < expectedSecondsTilRampEnd; ++i)
  {
    SendTimerTicks(ticksPerSecond);
    currentSpeedDrpm =
      std::max(static_cast<int32_t>(currentSpeedDrpm - rampRateDrpmPerSecond),
        targetSpeedDrpm);
    ASSERT_EQ(currentSpeedDrpm, mStepper.GetRunSpeedDrpm());
  }
}

TEST_F(StepperRampingTests, sequence_of_speeds_while_running)
{
  int32_t targetSpeed = 1000;
  Ramp(targetSpeed);
  ASSERT_EQ(targetSpeed, mStepper.GetRunSpeedDrpm());

  targetSpeed = 1500;
  Ramp(targetSpeed);
  ASSERT_EQ(targetSpeed, mStepper.GetRunSpeedDrpm());

  targetSpeed = 1200;
  Ramp(targetSpeed);
  ASSERT_EQ(targetSpeed, mStepper.GetRunSpeedDrpm());

  targetSpeed = -1000;
  Ramp(targetSpeed);
  ASSERT_EQ(targetSpeed, mStepper.GetRunSpeedDrpm());
}