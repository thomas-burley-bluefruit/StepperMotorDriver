#include "StepperUtility.h"

using namespace ::motor;

StepperUtility::StepperUtility(const size_t interruptRateHz,
  const size_t stepsPerRotation) :
  InterruptRateHz(interruptRateHz),
  StepsPerRotation(stepsPerRotation)
{
}

size_t StepperUtility::GetNextStepTick(const size_t stepsPerSecond,
  const size_t currentTick)
{
  if (stepsPerSecond == 0)
    return 0;
  const size_t ticksPerStep = InterruptRateHz / stepsPerSecond;
  return currentTick + ticksPerStep;
}

float StepperUtility::DrpmToStepsPerSecond(const float drpm) const
{
  return ((drpm / 10.0f) / 60.0f) * StepsPerRotation;
}

float StepperUtility::StepsPerSecondToDrpm(const float steps) const
{
  return ((steps * 10.0f) * 60) / StepsPerRotation;
}
