#pragma once

#include "IStepperDriver.h"
#include "MockCalls.h"

namespace motor
{

class MockStepperDriver final : public IStepperDriver
{
public:
  void Step(const Direction direction) override
  {
    StepCalls.Add(direction);
  }

  void StopHiZ() override
  {
    StopHiZCalled = true;
  }

  size_t GetStepsPerRotation() const override
  {
    return 200;
  }

  test::utility::CallsWithParams<Direction> StepCalls;
  bool StopHiZCalled = false;
};

}
