#pragma once

#include "IDualChannelMotorDriver.h"
#include "IGpioDriver.h"

namespace motor
{

class L298n final : public IDualChannelMotorDriver
{
public:
  L298n(driver::IGpioDriver& gpio);

  // IDualChannelMotorDriver
  void SetChannelPolarity(Channel channel, Polarity polarity);

  static constexpr driver::GpioPort GpioPort = driver::GpioPort::PortB;
  static constexpr driver::GpioPin ChannelA1 {.port = GpioPort, .pin = 2};
  static constexpr driver::GpioPin ChannelA2 {.port = GpioPort, .pin = 1};
  static constexpr driver::GpioPin ChannelB1 {.port = GpioPort, .pin = 15};
  static constexpr driver::GpioPin ChannelB2 {.port = GpioPort, .pin = 14};

private:
  driver::IGpioDriver& mGpio;

  struct ChannelInputLevels
  {
    bool Input1;
    bool Input2;
  };

  static constexpr ChannelInputLevels
    LevelsForPolarity[static_cast<size_t>(Polarity::Count)] = {
      {true,  false}, // Positive
      {false, true }, // Negative
      {false, false}  // Off
  };

private:
  void SetChannelLevels(Channel channel, ChannelInputLevels levels);
};

}