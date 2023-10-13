#pragma once

#include <cstddef>
#include <cstdint>

namespace motor
{

class StepperUtility
{
public:
  StepperUtility(const size_t interruptRateHz, const size_t stepsPerRotation);
  size_t GetNextStepTick(const float stepsPerSecond, const size_t lastStepTick);
  float DrpmToStepsPerSecond(const int32_t drpm) const;
  int32_t StepsPerSecondToDrpm(const float steps) const;

private:
  const size_t InterruptRateHz;
  const size_t StepsPerRotation;
};

}