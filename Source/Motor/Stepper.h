#pragma once

#include "FullStepSequence.h"
#include "IDualChannelMotorDriver.h"
#include "IInterruptTimer10Khz.h"
#include "IStepper.h"

namespace motor
{

class Stepper final : public IStepper, public driver::ITimerInterruptReceiver
{
public:
  Stepper(IDualChannelMotorDriver& motorDriver,
    driver::IInterruptTimer10Khz& interruptTimer10Khz);

  // IStepper
  void Init() override;
  void Move(const size_t steps) override;
  void SetStepsPerSecond(const size_t steps) override;
  size_t GetStepsPerSecond() override;

  // ITimerInterruptReceiver
  void OnTimerInterrupt() override;

  static constexpr size_t DefaultStepsPerSecond = 3;

private:
  void Step();
  void SetStepState(const StepState& state) const;
  void CalculateNextStepTick();

private:
  IDualChannelMotorDriver& mMotorDriver;
  const size_t InterruptRateHz;
  size_t mSequencePos = 0;
  size_t mStepsPerSecond = DefaultStepsPerSecond;
  size_t mTimerTick = 0;
  size_t mNextStepTick = 0;
  size_t mStepsPending = 0;
};

}