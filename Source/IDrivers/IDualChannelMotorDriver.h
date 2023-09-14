#pragma once

namespace motor
{

enum class Channel
{
  A,
  B
};

enum class Polarity
{
  Positive,
  Negative,
  Off,
  Count
};

class IDualChannelMotorDriver
{
public:
  virtual void SetChannelPolarity(Channel channel, Polarity polarity) = 0;
  virtual ~IDualChannelMotorDriver() = default;
};

}