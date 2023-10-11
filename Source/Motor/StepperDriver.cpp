#include "StepperDriver.h"

using namespace ::motor;

StepperDriver::StepperDriver(driver::IDualChannelMotorDriver& motorDriver) :
  mMotorDriver(motorDriver)
{
}

void StepperDriver::Step(const Direction direction)
{
  if (!mEnergised)
    Energise();

  mSequencePos = (mSequencePos + (direction == Direction::Forward ? 1 : -1))
    % FullStepSequence::StepsInSequence;
  SetStepState(sFullStepSequence[mSequencePos]);
}

void StepperDriver::StopHiZ()
{
  SetStepState({driver::Direction::Off, driver::Direction::Off});
  mEnergised = false;
}

size_t StepperDriver::GetStepsPerRotation() const
{
  return StepsPerRotation;
}

void StepperDriver::SetStepState(const StepState& state) const
{
  mMotorDriver.SetChannelDirection(driver::Channel::A, state.ChannelADirection);
  mMotorDriver.SetChannelDirection(driver::Channel::B, state.ChannelBDirection);
}

void StepperDriver::Energise()
{
  SetStepState(sFullStepSequence[mSequencePos]);
  mEnergised = true;
}
