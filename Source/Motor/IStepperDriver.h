#pragma once

#include <cstddef>

namespace motor
{

enum class Direction
{
  Forward,
  Reverse
};

class IStepperDriver
{
public:
  virtual void Step(const Direction direction) = 0;
  virtual void StopHiZ() = 0;
  virtual size_t GetStepsPerRotation() const = 0;
  virtual ~IStepperDriver() = default;
};

}
