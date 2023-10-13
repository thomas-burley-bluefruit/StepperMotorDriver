#include "StepperUtility.h"

#include <cmath>

using namespace ::motor;

StepperUtility::StepperUtility(const size_t interruptRateHz,
  const size_t stepsPerRotation) :
  InterruptRateHz(interruptRateHz),
  StepsPerRotation(stepsPerRotation)
{
}

size_t StepperUtility::GetNextStepTick(const float stepsPerSecond,
  const size_t lastStepTick)
{
  if (stepsPerSecond == 0.0f)
    return 0;
  const size_t ticksPerStep = InterruptRateHz / stepsPerSecond;
  return lastStepTick + ticksPerStep;
}

float StepperUtility::DrpmToStepsPerSecond(const int32_t drpm) const
{
  return ((drpm / 10.0f) / 60.0f) * StepsPerRotation;
}

int32_t StepperUtility::StepsPerSecondToDrpm(const float steps) const
{
  return static_cast<int32_t>(
    roundf(((steps * 10.0f) * 60) / StepsPerRotation));
}
