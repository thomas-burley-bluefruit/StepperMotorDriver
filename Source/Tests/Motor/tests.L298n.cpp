#include "L298n.h"
#include "mock.GpioDriver.h"
#include "gmock/gmock.h"

using namespace ::driver;
using namespace ::motor;

class L298nTests : public ::testing::Test
{
public:
  L298nTests() :
    mL298n(mGpio)
  {
  }

protected:
  MockGpioDriver mGpio;
  L298n mL298n;
};

TEST_F(L298nTests, Setting_channel_to_be_non_energised_sets_both_channel_pins_low)
{
  // Given, when
  mL298n.SetChannelDirection(Channel::A, Direction::Off);

  // Then
  ASSERT_EQ(2, mGpio.SetCalls.CallCount());

  const MockGpioDriver::SetParams channelA1Call = {.pin = L298n::ChannelA1,
    .state = false};
  ASSERT_TRUE(mGpio.SetCalls.CalledWithParams(channelA1Call));

  const MockGpioDriver::SetParams channelA2Call = {.pin = L298n::ChannelA2,
    .state = false};
  ASSERT_TRUE(mGpio.SetCalls.CalledWithParams(channelA2Call));

  mGpio.Reset();

  // Given, when
  mL298n.SetChannelDirection(Channel::B, Direction::Off);

  // Then
  ASSERT_EQ(2, mGpio.SetCalls.CallCount());

  const MockGpioDriver::SetParams channelB1Call = {.pin = L298n::ChannelB1,
    .state = false};
  ASSERT_TRUE(mGpio.SetCalls.CalledWithParams(channelB1Call));

  const MockGpioDriver::SetParams channelB2Call = {.pin = L298n::ChannelB2,
    .state = false};
  ASSERT_TRUE(mGpio.SetCalls.CalledWithParams(channelB2Call));
}

TEST_F(L298nTests, Setting_channel_to_be_positive_polarity)
{
  // Given, when
  mL298n.SetChannelDirection(Channel::A, Direction::Forward);

  // Then
  ASSERT_EQ(2, mGpio.SetCalls.CallCount());

  const MockGpioDriver::SetParams channelA1Call = {.pin = L298n::ChannelA1,
    .state = true};
  ASSERT_TRUE(mGpio.SetCalls.CalledWithParams(channelA1Call));

  const MockGpioDriver::SetParams channelA2Call = {.pin = L298n::ChannelA2,
    .state = false};
  ASSERT_TRUE(mGpio.SetCalls.CalledWithParams(channelA2Call));

  mGpio.Reset();

  // Given, when
  mL298n.SetChannelDirection(Channel::B, Direction::Forward);

  // Then
  ASSERT_EQ(2, mGpio.SetCalls.CallCount());

  const MockGpioDriver::SetParams channelB1Call = {.pin = L298n::ChannelB1,
    .state = true};
  ASSERT_TRUE(mGpio.SetCalls.CalledWithParams(channelB1Call));

  const MockGpioDriver::SetParams channelB2Call = {.pin = L298n::ChannelB2,
    .state = false};
  ASSERT_TRUE(mGpio.SetCalls.CalledWithParams(channelB2Call));
}

TEST_F(L298nTests, Setting_channel_to_be_negative_polarity)
{
  // Given, when
  mL298n.SetChannelDirection(Channel::A, Direction::Reverse);

  // Then
  ASSERT_EQ(2, mGpio.SetCalls.CallCount());

  const MockGpioDriver::SetParams channelA1Call = {.pin = L298n::ChannelA1,
    .state = false};
  ASSERT_TRUE(mGpio.SetCalls.CalledWithParams(channelA1Call));

  const MockGpioDriver::SetParams channelA2Call = {.pin = L298n::ChannelA2,
    .state = true};
  ASSERT_TRUE(mGpio.SetCalls.CalledWithParams(channelA2Call));

  mGpio.Reset();

  // Given, when
  mL298n.SetChannelDirection(Channel::B, Direction::Reverse);

  // Then
  ASSERT_EQ(2, mGpio.SetCalls.CallCount());

  const MockGpioDriver::SetParams channelB1Call = {.pin = L298n::ChannelB1,
    .state = false};
  ASSERT_TRUE(mGpio.SetCalls.CalledWithParams(channelB1Call));

  const MockGpioDriver::SetParams channelB2Call = {.pin = L298n::ChannelB2,
    .state = true};
  ASSERT_TRUE(mGpio.SetCalls.CalledWithParams(channelB2Call));
}