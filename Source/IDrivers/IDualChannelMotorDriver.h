#pragma once

namespace driver
{

enum class Channel
{
  A,
  B
};

enum class Direction
{
  Forward,
  Reverse,
  Off,
  Count
};

class IDualChannelMotorDriver
{
public:
  virtual void SetChannelDirection(Channel channel, Direction polarity) = 0;
  virtual Direction GetChannelDirection(Channel channel) = 0;
  virtual ~IDualChannelMotorDriver() = default;
};

}