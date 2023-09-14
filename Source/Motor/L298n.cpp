#include "L298n.h"

#include <cassert>

using namespace ::motor;

L298n::L298n(driver::IGpioDriver& gpio) :
  mGpio(gpio)
{
}

void L298n::SetChannelPolarity(Channel channel, Polarity polarity)
{
  SetChannelLevels(channel, LevelsForPolarity[static_cast<size_t>(polarity)]);
}

void L298n::SetChannelLevels(Channel channel, ChannelInputLevels levels)
{
  switch (channel)
  {
  case Channel::A:
    mGpio.Set(ChannelA1, levels.Input1);
    mGpio.Set(ChannelA2, levels.Input2);
    break;
  case Channel::B:
    mGpio.Set(ChannelB1, levels.Input1);
    mGpio.Set(ChannelB2, levels.Input2);
    break;
  default:
    bool handledChannel = false;
    assert(handledChannel);
  }
}
