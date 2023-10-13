#pragma once

#include "IStepperDriver.h"
#include "SpeedRamping.h"
#include "StepperUtility.h"

#include <cstddef>

namespace motor
{

class StepperRun
{
public:
  StepperRun(IStepperDriver& stepperDriver, StepperUtility& stepperUtility);
  void Run(const int32_t drpm);
  bool Running() const;
  void Stop();
  int32_t GetRunSpeedDrpm() const;
  void EnableRamping(const bool enable);
  void SetRampRate(const size_t drpmPerSecond);
  size_t GetRampRateDrpmPerSecond() const;
  void OnTimerTick(const size_t timerTick);

  static constexpr size_t DefaultRampRateStepsPerSecondSquared = 500;

private:
  IStepperDriver& mStepperDriver;
  StepperUtility& mStepperUtility;
  SpeedRamping mSpeedRamping;

  bool mStopped = true;
  float mStepsPerSecond = 0;
  size_t mTimerTick = 0;
  size_t mNextStepTick = 0;
  size_t mLastStepTick = 0;
  bool mRampingEnabled = true;
  Direction mDirection = Direction::Forward;
};

}