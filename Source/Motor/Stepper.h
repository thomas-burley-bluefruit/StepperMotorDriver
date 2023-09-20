#pragma once

#include "FullStepSequence.h"
#include "IDualChannelMotorDriver.h"
#include "IStepper.h"

namespace motor
{

class Stepper final : public IStepper
{
public:
  Stepper(IDualChannelMotorDriver& motorDriver);
  void Init();

  // IStepper
  void Step(size_t steps) override;

private:
  void SetStepState(const StepState& state) const;

private:
  IDualChannelMotorDriver& mMotorDriver;
  const FullStepSequence mStepSequence;
  size_t mSequencePos = 0;
};

}