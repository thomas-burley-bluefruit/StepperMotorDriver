#pragma once

namespace motor
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
  virtual void SetChannelPolarity(Channel channel, Direction polarity) = 0;
  virtual Direction GetChannelPolarity(Channel channel) = 0;
  virtual ~IDualChannelMotorDriver() = default;
};

}