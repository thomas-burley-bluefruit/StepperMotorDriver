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

  // IStepper
  void Init() override;
  void Step(size_t steps) override;

private:
  void SetStepState(const StepState& state) const;

private:
  IDualChannelMotorDriver& mMotorDriver;
  size_t mSequencePos = 0;
};

}