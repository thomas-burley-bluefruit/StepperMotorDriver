#pragma once

#include "IDualChannelMotorDriver.h"
#include "IInterruptTimer10Khz.h"
#include "IStepper.h"
#include "IStepperDriver.h"
#include "SpeedRamping.h"
#include "StepperMove.h"
#include "StepperUtility.h"

namespace motor
{

enum class StepperState
{
  Stopped,
  Ramping,
  Running
};

class Stepper final : public IStepper, public driver::ITimerInterruptReceiver
{
public:
  Stepper(IStepperDriver& stepperDriver,
    driver::IInterruptTimer10Khz& interruptTimer10Khz);

  void EnableRamping(const bool enable);
  size_t GetRunSpeedDrpm() const;

  // IStepper
  void Run(const int32_t drpm) override;
  bool Running() const override;
  void Stop() override;
  void Move(const size_t steps) override;
  void SetStepsPerSecond(const size_t steps) override;
  size_t GetStepsPerSecond() const override;
  void SetRampRate(const size_t drpmPerSecond) override;
  size_t GetRampRateDrpmPerSecond() const override;

  // ITimerInterruptReceiver
  void OnTimerInterrupt() override;

  static constexpr size_t StepsPerRotation = 200;
  static constexpr size_t DefaultRampRateStepsPerSecondSquared = 150;

private:
private:
  IStepperDriver& mStepperDriver;
  StepperUtility mStepperUtility;
  SpeedRamping mSpeedRamping;
  StepperMove mStepperMove;

  StepperState mState = StepperState::Stopped;
  float mRunningStepsPerSecond = 0;
  size_t mTimerTick = 0;
  size_t mNextStepTick = 0;
  bool mRampingEnabled = true;
};

}