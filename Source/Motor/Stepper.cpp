#include "Stepper.h"

using namespace ::motor;

Stepper::Stepper(IDualChannelMotorDriver& motorDriver) :
  mMotorDriver(motorDriver)
{
}

void Stepper::Init()
{
  SetStepState(sFullStepSequence[0]);
}

void Stepper::Step(size_t steps)
{
  mSequencePos = (mSequencePos + 1) % FullStepSequence::StepsInSequence;
  SetStepState(sFullStepSequence[mSequencePos]);
}

void Stepper::SetStepState(const StepState& state) const
{
  mMotorDriver.SetChannelDirection(Channel::A, state.ChannelADirection);
  mMotorDriver.SetChannelDirection(Channel::B, state.ChannelBDirection);
}
