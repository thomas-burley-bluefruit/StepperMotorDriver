#pragma once

#include <cstddef>

namespace motor
{

class IStepper
{
public:
  virtual void Init() = 0;
  virtual void Move(const size_t steps) = 0;
  virtual void SetStepsPerSecond(const size_t steps) = 0;
  virtual size_t GetStepsPerSecond() = 0;
  virtual ~IStepper() = default;
};

}