#pragma once

#include "IDualChannelMotorDriver.h"
#include "IInterruptTimer10Khz.h"
#include "IStepper.h"
#include "IStepperDriver.h"
#include "SpeedRamping.h"
#include "StepperMove.h"
#include "StepperRun.h"
#include "StepperUtility.h"

namespace motor
{

class Stepper final : public IStepper, public driver::ITimerInterruptReceiver
{
public:
  Stepper(IStepperDriver& stepperDriver,
    driver::IInterruptTimer10Khz& interruptTimer10Khz);

  void EnableRamping(const bool enable);
  int32_t GetRunSpeedDrpm() const;
  size_t GetStepsPerRotation() const;

  // IStepper
  void Run(const int32_t drpm) override;
  bool Running() const override;
  void Stop() override;
  void StopHiZ() override;
  void Move(const size_t steps) override;
  void SetStepsPerSecond(const size_t steps) override;
  size_t GetStepsPerSecond() const override;
  void SetRampRate(const size_t drpmPerSecond) override;
  size_t GetRampRateDrpmPerSecond() const override;

  // ITimerInterruptReceiver
  void OnTimerInterrupt() override;

private:
  IStepperDriver& mStepperDriver;
  StepperUtility mStepperUtility;
  StepperMove mStepperMove;
  StepperRun mStepperRun;

  size_t mTimerTick = 0;
};

}