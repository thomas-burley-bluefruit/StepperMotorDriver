#pragma once

#include "IStepperDriver.h"
#include "StepperUtility.h"

#include <cstddef>
#include <cstdint>

namespace motor
{

class StepperMove
{
public:
  StepperMove(IStepperDriver& stepperDriver, StepperUtility& stepperUtility);
  void Move(const int32_t steps);
  void SetStepsPerSecond(const size_t steps);
  size_t GetStepsPerSecond() const;
  void OnTimerTick(const size_t timerTick);

  static constexpr size_t DefaultStepsPerSecond = 500;

private:
  size_t mStepsPerSecond = DefaultStepsPerSecond;
  size_t mStepsPending = 0;
  size_t mNextStepTick = 0;
  size_t mTimerTick = 0;
  Direction mDirection = Direction::Forward;

  IStepperDriver& mStepperDriver;
  StepperUtility& mStepperUtility;
};

}