#pragma once

#include "IStepper.h"

namespace motor
{

class MockStepper final : public IStepper
{
public:
  void Init() override {}

  void Move(const size_t steps) override
  {
    MoveCalled = true;
    MoveSteps = steps;
  }

  void Run(const size_t drpm) override
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

  void SetStepsPerSecond(const size_t steps) override
  {
    SetStepsPerSecondCalled = true;
    SetStepsPerSecondSteps = steps;
  }

  size_t GetStepsPerSecond() override
  {
    return 0;
  }

  bool MoveCalled = false;
  size_t MoveSteps = 0;
  bool RunCalled = false;
  size_t RunDrpm = 0;
  bool StopCalled = false;
  bool SetStepsPerSecondCalled = false;
  size_t SetStepsPerSecondSteps = 0;
};

}