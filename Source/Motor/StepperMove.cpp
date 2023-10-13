#include "StepperMove.h"

#include <cmath>

using namespace ::motor;

StepperMove::StepperMove(IStepperDriver& stepperDriver,
  StepperUtility& stepperUtility) :
  mStepperDriver(stepperDriver),
  mStepperUtility(stepperUtility)
{
}

void StepperMove::Move(const int32_t steps)
{
  if (steps == 0)
    return;

  mDirection = steps > 0 ? Direction::Forward : Direction::Reverse;
  mNextStepTick = mTimerTick;
  mStepsPending = abs(steps);
}

void StepperMove::SetStepsPerSecond(const size_t steps)
{
  mStepsPerSecond = steps;
}

size_t StepperMove::GetStepsPerSecond() const
{
  return mStepsPerSecond;
}

void StepperMove::OnTimerTick(const size_t timerTick)
{
  mTimerTick = timerTick;

  if (mStepsPending == 0 || mTimerTick < mNextStepTick)
    return;

  mStepperDriver.Step(mDirection);
  --mStepsPending;
  mNextStepTick = mStepperUtility.GetNextStepTick(mStepsPerSecond, mTimerTick);
}

bool StepperMove::Moving() const
{
  return mStepsPending > 0;
}