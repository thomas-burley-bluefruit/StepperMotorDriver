#include "Stepper.h"
#include <cmath>

using namespace ::motor;
using namespace ::driver;

Stepper::Stepper(IStepperDriver& stepperDriver,
  driver::IInterruptTimer10Khz& interruptTimer10Khz) :
  mStepperDriver(stepperDriver),
  mStepperUtility(interruptTimer10Khz.GetInterruptRateHz(),
    stepperDriver.GetStepsPerRotation()),
  mStepperMove(mStepperDriver, mStepperUtility),
  mStepperRun(mStepperDriver, mStepperUtility)
{
  interruptTimer10Khz.RegisterCallback(this);
}

void Stepper::EnableRamping(const bool enable)
{
  mStepperRun.EnableRamping(enable);
}

int32_t Stepper::GetRunSpeedDrpm() const
{
  return mStepperRun.GetRunSpeedDrpm();
}

size_t Stepper::GetStepsPerRotation() const
{
  return mStepperDriver.GetStepsPerRotation();
}

void Stepper::Move(const size_t steps)
{
  if (Running())
    return;

  mStepperMove.Move(steps);
}

void Stepper::Run(const int32_t drpm)
{
  if (mStepperMove.Moving())
    return;

  mStepperRun.Run(drpm);
}

bool Stepper::Running() const
{
  return mStepperRun.Running();
}

void Stepper::Stop()
{
  mStepperRun.Stop();
}

void Stepper::StopHiZ()
{
  mStepperRun.Stop();
  mStepperDriver.StopHiZ();
}

void Stepper::SetStepsPerSecond(const size_t steps)
{
  mStepperMove.SetStepsPerSecond(steps);
}

size_t Stepper::GetStepsPerSecond() const
{
  return mStepperMove.GetStepsPerSecond();
}

void Stepper::SetRampRate(const size_t drpmSquared)
{
  mStepperRun.SetRampRate(drpmSquared);
}

size_t Stepper::GetRampRateDrpmPerSecond() const
{
  return mStepperRun.GetRampRateDrpmPerSecond();
}

void Stepper::OnTimerInterrupt()
{
  ++mTimerTick;

  mStepperMove.OnTimerTick(mTimerTick);
  mStepperRun.OnTimerTick(mTimerTick);
}
