#pragma once

#include <cstddef>
#include <cstdint>

namespace motor
{

class IStepper
{
public:
  virtual void Move(const size_t steps) = 0;
  virtual void Run(const int32_t drpm) = 0;
  virtual bool Running() const = 0;
  virtual void Stop() = 0;
  virtual void StopHiZ() = 0;
  virtual void SetStepsPerSecond(const size_t steps) = 0;
  virtual size_t GetStepsPerSecond() const = 0;
  virtual void SetRampRate(const size_t drpmSquared) = 0;
  virtual size_t GetRampRateDrpmPerSecond() const = 0;
  virtual ~IStepper() = default;
};

}