#include "StepperRun.h"

#include <cmath>

using namespace ::motor;

StepperRun::StepperRun(IStepperDriver& stepperDriver,
  StepperUtility& stepperUtility) :
  mStepperDriver(stepperDriver),
  mStepperUtility(stepperUtility)
{
  mSpeedRamping.SetRampRate(DefaultRampRateStepsPerSecondSquared);
}

void StepperRun::Run(const int32_t drpm)
{
  if (mRampingEnabled)
    mSpeedRamping.Init(mStepsPerSecond,
      mStepperUtility.DrpmToStepsPerSecond(drpm), mTimerTick);
  else
    mStepsPerSecond = mStepperUtility.DrpmToStepsPerSecond(drpm);

  mLastStepTick = mTimerTick;
  mStopped = false;
}

void StepperRun::Stop()
{
  mStopped = true;
  mStepsPerSecond = 0;
}

int32_t StepperRun::GetRunSpeedDrpm() const
{
  return mStepperUtility.StepsPerSecondToDrpm(mStepsPerSecond);
}

void StepperRun::EnableRamping(const bool enable)
{
  mRampingEnabled = enable;
}

void StepperRun::SetRampRate(const size_t drpmSquared)
{
  mSpeedRamping.SetRampRate(mStepperUtility.DrpmToStepsPerSecond(drpmSquared));
}

size_t StepperRun::GetRampRateDrpmPerSecond() const
{
  return static_cast<size_t>(
    roundf(mStepperUtility.StepsPerSecondToDrpm(mSpeedRamping.GetRampRate())));
}

bool StepperRun::Running() const
{
  return !mStopped;
}

void StepperRun::OnTimerTick(const size_t timerTick)
{
  mTimerTick = timerTick;

  if (mStopped)
    return;

  if (mSpeedRamping.Ramping())
  {
    mSpeedRamping.Update(mTimerTick);
    mStepsPerSecond = mSpeedRamping.GetSpeed();
  }

  mNextStepTick =
    mStepperUtility.GetNextStepTick(abs(mStepsPerSecond), mLastStepTick);

  if (mTimerTick < mNextStepTick)
    return;

  const auto direction =
    mStepsPerSecond < 0.0f ? Direction::Reverse : Direction::Forward;

  mStepperDriver.Step(direction);
  mLastStepTick = mTimerTick;
}