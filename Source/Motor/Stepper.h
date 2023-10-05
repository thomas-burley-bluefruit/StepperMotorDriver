#pragma once

#include "FullStepSequence.h"
#include "IDualChannelMotorDriver.h"
#include "IInterruptTimer10Khz.h"
#include "IStepper.h"
#include "SpeedRamping.h"

namespace motor
{

enum class StepperState
{
  Stopped,
  Ramping,
  Running,
  Moving
};

class Stepper final : public IStepper, public driver::ITimerInterruptReceiver
{
public:
  Stepper(IDualChannelMotorDriver& motorDriver,
    driver::IInterruptTimer10Khz& interruptTimer10Khz);

  void EnableRamping(const bool enable);
  size_t GetRunSpeedDrpm() const;

  // IStepper
  void Init() override;
  void Run(const int32_t drpm) override;
  bool Running() const override;
  void Stop() override;
  void Move(const size_t steps) override;
  void SetStepsPerSecond(const size_t steps) override;
  size_t GetStepsPerSecond() override;
  void SetRampRate(const size_t drpmPerSecond) override;
  size_t GetRampRateDrpmPerSecond() const override;

  // ITimerInterruptReceiver
  void OnTimerInterrupt() override;

  static constexpr size_t StepsPerRotation = 200;
  static constexpr size_t DefaultStepsPerSecond = 500;
  static constexpr size_t DefaultRampRateStepsPerSecondSquared = 150;

private:
  void Step();
  void SetStepState(const StepState& state) const;
  void CalculateNextStepTick(const size_t stepsPerSecond);
  float DrpmToStepsPerSecond(const float drpm) const;
  float StepsPerSecondToDrpm(const float steps) const;

private:
  IDualChannelMotorDriver& mMotorDriver;
  SpeedRamping mSpeedRamping;

  StepperState mState = StepperState::Stopped;
  const size_t InterruptRateHz;
  size_t mSequencePos = 0;
  size_t mMovingStepsPerSecond = DefaultStepsPerSecond;
  float mRunningStepsPerSecond = 0;
  size_t mTimerTick = 0;
  size_t mNextStepTick = 0;
  size_t mStepsPending = 0;
  bool mRampingEnabled = true;
};

}