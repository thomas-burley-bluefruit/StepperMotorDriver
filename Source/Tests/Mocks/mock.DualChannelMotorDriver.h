#pragma once

#include "IDualChannelMotorDriver.h"
#include "MockCalls.h"

namespace motor
{

class MockDualChannelMotorDriver final : public IDualChannelMotorDriver
{
public:
  void SetChannelDirection(Channel channel, Direction polarity)
  {
    SetDirectionCalls.Add({channel, polarity});
  }

  Direction GetChannelDirection(Channel channel)
  {
    return Direction::Off;
  }

  void Reset()
  {
    *this = {};
  }

  struct SetDirectionParams
  {
    Channel channel;
    Direction direction;

    bool operator==(const SetDirectionParams& rhs) const
    {
      return this->channel == rhs.channel && this->direction == rhs.direction;
    }
  };

  test::utility::CallsWithParams<SetDirectionParams> SetDirectionCalls;
};

}