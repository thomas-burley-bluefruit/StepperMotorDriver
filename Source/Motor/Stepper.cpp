#include "Stepper.h"
#include <cmath>

using namespace ::motor;

Stepper::Stepper(IDualChannelMotorDriver& motorDriver,
  driver::IInterruptTimer10Khz& interruptTimer10Khz) :
  mMotorDriver(motorDriver),
  InterruptRateHz(interruptTimer10Khz.GetInterruptRateHz())
{
  interruptTimer10Khz.RegisterCallback(this);
  mSpeedRamping.SetRampRate(DefaultRampRateStepsPerSecondSquared);
}

void Stepper::Init()
{
  SetStepState(sFullStepSequence[0]);
}

void Stepper::EnableRamping(const bool enable)
{
  mRampingEnabled = enable;
}

size_t Stepper::GetRunSpeedDrpm() const
{
  return static_cast<size_t>(
    roundf(StepsPerSecondToDrpm(mRunningStepsPerSecond)));
}

void Stepper::Move(const size_t steps)
{
  if (steps == 0)
    return;

  Step();

  if (steps == 1)
    return;

  CalculateNextStepTick(mMovingStepsPerSecond);
  mStepsPending = steps - 1;
  mState = StepperState::Moving;
}

void Stepper::Run(const int32_t drpm)
{
  if (mRampingEnabled)
  {
    mState = StepperState::Ramping;
    mSpeedRamping.Init(mRunningStepsPerSecond, DrpmToStepsPerSecond(drpm),
      mTimerTick);
  }
  else
  {
    mRunningStepsPerSecond = DrpmToStepsPerSecond(drpm);
    CalculateNextStepTick(static_cast<size_t>(roundf(mRunningStepsPerSecond)));
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
  mMovingStepsPerSecond = steps;
}

size_t Stepper::GetStepsPerSecond()
{
  return mMovingStepsPerSecond;
}

void Stepper::SetRampRate(const size_t drpmSquared)
{
  mSpeedRamping.SetRampRate(DrpmToStepsPerSecond(drpmSquared));
}

size_t Stepper::GetRampRateDrpmPerSecond() const
{
  return static_cast<size_t>(
    roundf(StepsPerSecondToDrpm(mSpeedRamping.GetRampRate())));
}

void Stepper::Step()
{
  mSequencePos = (mSequencePos + 1) % FullStepSequence::StepsInSequence;
  SetStepState(sFullStepSequence[mSequencePos]);
}

void Stepper::SetStepState(const StepState& state) const
{
  mMotorDriver.SetChannelDirection(Channel::A, state.ChannelADirection);
  mMotorDriver.SetChannelDirection(Channel::B, state.ChannelBDirection);
}

void Stepper::CalculateNextStepTick(const size_t stepsPerSecond)
{
  if (stepsPerSecond == 0)
    return;
  const size_t ticksPerStep = InterruptRateHz / stepsPerSecond;
  mNextStepTick = mTimerTick + ticksPerStep;
}

float Stepper::DrpmToStepsPerSecond(const float drpm) const
{
  return ((drpm / 10.0f) / 60.0f) * StepsPerRotation;
}

float Stepper::StepsPerSecondToDrpm(const float steps) const
{
  return ((steps * 10.0f) * 60) / StepsPerRotation;
}

void Stepper::OnTimerInterrupt()
{
  ++mTimerTick;

  switch (mState)
  {
  case StepperState::Ramping:
    mSpeedRamping.Update(mTimerTick);
    mRunningStepsPerSecond = mSpeedRamping.GetSpeed();

    if (mTimerTick < mNextStepTick)
      break;

    Step();
    CalculateNextStepTick(static_cast<size_t>(roundf(mRunningStepsPerSecond)));
    if (!mSpeedRamping.Ramping())
      mState = StepperState::Running;
    break;

  case StepperState::Moving:
    if (mTimerTick < mNextStepTick)
      break;
    if (mStepsPending-- == 0)
    {
      mState = StepperState::Stopped;
      return;
    }
    Step();
    CalculateNextStepTick(mMovingStepsPerSecond);
    break;

  case StepperState::Running:
    if (mTimerTick < mNextStepTick)
      break;
    CalculateNextStepTick(static_cast<size_t>(roundf(mRunningStepsPerSecond)));
    Step();
    break;

  case StepperState::Stopped:
  default:
    break;
  }
}
