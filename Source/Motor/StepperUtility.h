#pragma once

#include <cstddef>

namespace motor
{

class StepperUtility
{
public:
  StepperUtility(const size_t interruptRateHz, const size_t stepsPerRotation);
  size_t GetNextStepTick(const size_t stepsPerSecond, const size_t currentTick);
  float DrpmToStepsPerSecond(const float drpm) const;
  float StepsPerSecondToDrpm(const float steps) const;

private:
  const size_t InterruptRateHz;
  const size_t StepsPerRotation;
};

}