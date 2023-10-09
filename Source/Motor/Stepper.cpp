#include "Stepper.h"
#include <cmath>

using namespace ::motor;
using namespace ::driver;

Stepper::Stepper(IStepperDriver& stepperDriver,
  driver::IInterruptTimer10Khz& interruptTimer10Khz) :
  mStepperDriver(stepperDriver),
  mStepperUtility(interruptTimer10Khz.GetInterruptRateHz(), StepsPerRotation),
  mStepperMove(mStepperDriver, mStepperUtility)
{
  interruptTimer10Khz.RegisterCallback(this);
  mSpeedRamping.SetRampRate(DefaultRampRateStepsPerSecondSquared);
}

void Stepper::EnableRamping(const bool enable)
{
  mRampingEnabled = enable;
}

size_t Stepper::GetRunSpeedDrpm() const
{
  return static_cast<size_t>(
    roundf(mStepperUtility.StepsPerSecondToDrpm(mRunningStepsPerSecond)));
}

void Stepper::Move(const size_t steps)
{
  mStepperMove.Move(steps);
}

void Stepper::Run(const int32_t drpm)
{
  if (mRampingEnabled)
  {
    mState = StepperState::Ramping;
    mSpeedRamping.Init(mRunningStepsPerSecond,
      mStepperUtility.DrpmToStepsPerSecond(drpm), mTimerTick);
  }
  else
  {
    mRunningStepsPerSecond = mStepperUtility.DrpmToStepsPerSecond(drpm);
    mNextStepTick = mStepperUtility.GetNextStepTick(
      static_cast<size_t>(roundf(mRunningStepsPerSecond)), mTimerTick);
    mState = StepperState::Running;
  }
}

bool Stepper::Running() const
{
  return mState == StepperState::Running;
}

void Stepper::Stop()
{
  mState = StepperState::Stopped;
}

void Stepper::SetStepsPerSecond(const size_t steps)
{
  mStepperMove.SetStepsPerSecond(steps);
}

size_t Stepper::GetStepsPerSecond() const
{
  return mStepperMove.GetStepsPerSecond();
}

void Stepper::SetRampRate(const size_t drpmSquared)
{
  mSpeedRamping.SetRampRate(mStepperUtility.DrpmToStepsPerSecond(drpmSquared));
}

size_t Stepper::GetRampRateDrpmPerSecond() const
{
  return static_cast<size_t>(
    roundf(mStepperUtility.StepsPerSecondToDrpm(mSpeedRamping.GetRampRate())));
}

void Stepper::OnTimerInterrupt()
{
  ++mTimerTick;

  mStepperMove.OnTimerTick(mTimerTick);

  switch (mState)
  {
  case StepperState::Ramping:
    mSpeedRamping.Update(mTimerTick);
    mRunningStepsPerSecond = mSpeedRamping.GetSpeed();

    if (mTimerTick < mNextStepTick)
      break;

    mStepperDriver.Step(Direction::Forward);
    mNextStepTick = mStepperUtility.GetNextStepTick(
      static_cast<size_t>(roundf(mRunningStepsPerSecond)), mTimerTick);
    if (!mSpeedRamping.Ramping())
      mState = StepperState::Running;
    break;

  case StepperState::Running:
    if (mTimerTick < mNextStepTick)
      break;
    mNextStepTick = mStepperUtility.GetNextStepTick(
      static_cast<size_t>(roundf(mRunningStepsPerSecond)), mTimerTick);
    mStepperDriver.Step(Direction::Forward);
    break;

  case StepperState::Stopped:
  default:
    break;
  }
}
