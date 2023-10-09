#include "StepperMove.h"

using namespace ::motor;

StepperMove::StepperMove(IStepperDriver& stepperDriver,
  StepperUtility& stepperUtility) :
  mStepperDriver(stepperDriver),
  mStepperUtility(stepperUtility)
{
}

void StepperMove::Move(const size_t steps)
{
  if (steps == 0)
    return;

  mStepperDriver.Step(Direction::Forward);

  if (steps == 1)
    return;

  mNextStepTick = mStepperUtility.GetNextStepTick(mStepsPerSecond, mTimerTick);
  mStepsPending = steps - 1;
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

  mStepperDriver.Step(Direction::Forward);
  --mStepsPending;
  mNextStepTick = mStepperUtility.GetNextStepTick(mStepsPerSecond, mTimerTick);
}
