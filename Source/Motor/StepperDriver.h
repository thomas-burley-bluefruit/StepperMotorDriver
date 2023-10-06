#pragma once

#include "FullStepSequence.h"
#include "IDualChannelMotorDriver.h"
#include "IStepperDriver.h"

namespace motor
{

class StepperDriver final : public IStepperDriver
{
public:
  StepperDriver(driver::IDualChannelMotorDriver& motorDriver);
  void Step(const Direction direction) override;
  void StopHiZ() override;

private:
  void SetStepState(const StepState& state) const;
  void Energise();

private:
  driver::IDualChannelMotorDriver& mMotorDriver;

  size_t mSequencePos = 0;
  bool mEnergised = false;
};

}