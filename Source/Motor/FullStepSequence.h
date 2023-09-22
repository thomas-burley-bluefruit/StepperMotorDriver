#pragma once

#include "IDualChannelMotorDriver.h"

#include <cassert>
#include <cstddef>

namespace motor
{

struct StepState
{
  Direction ChannelADirection = Direction::Off;
  Direction ChannelBDirection = Direction::Off;
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
    {Direction::Forward, Direction::Forward},
    {Direction::Reverse, Direction::Forward},
    {Direction::Reverse, Direction::Reverse},
    {Direction::Forward, Direction::Reverse},
  };
} static const sFullStepSequence;

}
