#include "Stepper.h"

using namespace ::motor;

Stepper::Stepper(IDualChannelMotorDriver& motorDriver,
  driver::IInterruptTimer10Khz& interruptTimer10Khz) :
  mMotorDriver(motorDriver),
  InterruptRateHz(interruptTimer10Khz.GetInterruptRateHz())
{
  interruptTimer10Khz.RegisterCallback(this);
}

void Stepper::Init()
{
  SetStepState(sFullStepSequence[0]);
}

void Stepper::Move(const size_t steps)
{
  if (steps == 0)
    return;

  Step();
  CalculateNextStepTick();
  mStepsPending = steps - 1;
}

void Stepper::SetStepsPerSecond(const size_t steps)
{
  mStepsPerSecond = steps;
}

size_t Stepper::GetStepsPerSecond()
{
  return mStepsPerSecond;
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

void Stepper::CalculateNextStepTick()
{
  const size_t ticksPerStep = InterruptRateHz / mStepsPerSecond;
  mNextStepTick = mTimerTick + ticksPerStep;
}

void Stepper::OnTimerInterrupt()
{
  if (++mTimerTick < mNextStepTick || mStepsPending == 0)
    return;

  Step();
  --mStepsPending;
  CalculateNextStepTick();
}
