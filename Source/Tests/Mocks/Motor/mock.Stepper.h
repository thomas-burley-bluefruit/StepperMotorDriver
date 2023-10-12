#pragma once

#include "IStepper.h"

namespace motor
{

class MockStepper final : public IStepper
{
public:
  void Move(const size_t steps) override
  {
    MoveCalled = true;
    MoveSteps = steps;
  }

  void Run(const int32_t drpm) override
  {
    RunCalled = true;
    RunDrpm = drpm;
  }

  bool Running() const override
  {
    return false;
  }

  void Stop() override
  {
    StopCalled = true;
  }

  void StopHiZ() override
  {
    StopHiZCalled = true;
  }

  void SetStepsPerSecond(const size_t steps) override
  {
    SetStepsPerSecondCalled = true;
    SetStepsPerSecondSteps = steps;
  }

  size_t GetStepsPerSecond() const override
  {
    return 0;
  }

  void SetRampRate(const size_t drpmSquared) override
  {
    SetRampRateCalled = true;
    SetRampRateDrpm = drpmSquared;
  }

  size_t GetRampRateDrpmPerSecond() const override
  {
    return 0;
  }

  bool MoveCalled = false;
  size_t MoveSteps = 0;
  bool RunCalled = false;
  int32_t RunDrpm = 0;
  bool StopCalled = false;
  bool StopHiZCalled = false;
  bool SetStepsPerSecondCalled = false;
  size_t SetStepsPerSecondSteps = 0;
  bool SetRampRateCalled = false;
  size_t SetRampRateDrpm = 0;
};

}