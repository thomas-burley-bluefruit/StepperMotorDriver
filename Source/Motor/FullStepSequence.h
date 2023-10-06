#pragma once

#include "IDualChannelMotorDriver.h"

#include <cassert>
#include <cstddef>

namespace motor
{

struct StepState
{
  driver::Direction ChannelADirection = driver::Direction::Off;
  driver::Direction ChannelBDirection = driver::Direction::Off;
};

class FullStepSequence
{
public:
  static constexpr size_t StepsInSequence = 4;

  const StepState& operator[](size_t index) const
  {
    assert(index < StepsInSequence);
    return Sequence[index];
  }

private:
  static constexpr StepState Sequence[StepsInSequence] = {
    {driver::Direction::Forward, driver::Direction::Forward},
    {driver::Direction::Reverse, driver::Direction::Forward},
    {driver::Direction::Reverse, driver::Direction::Reverse},
    {driver::Direction::Forward, driver::Direction::Reverse},
  };
} static const sFullStepSequence;

}
