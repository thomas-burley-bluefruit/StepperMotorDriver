#pragma once

#include <cstddef>

namespace motor
{

class IStepper
{
public:
  virtual void Init() = 0;
  virtual void Step(size_t steps) = 0;
  virtual ~IStepper() = default;
};

}