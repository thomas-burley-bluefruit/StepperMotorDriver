#pragma once

#include "IStepperDriver.h"
#include "MockCalls.h"

namespace motor
{

class MockStepperDriver final : public IStepperDriver
{
public:
  void Step(const Direction direction)
  {
    StepCalls.Add(direction);
  }

  void StopHiZ()
  {
    StopHiZCalled = true;
  }

  test::utility::CallsWithParams<Direction> StepCalls;
  bool StopHiZCalled = false;
};

}
