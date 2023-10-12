#include "mock.DualChannelMotorDriver.h"
#include "StepperDriver.h"
#include "gmock/gmock.h"

using namespace ::motor;

class StepperDriverTests : public testing::Test
{
public:
  StepperDriverTests() :
    mStepperDriver(mMotorDriver)
  {
  }

protected:
  driver::MockDualChannelMotorDriver mMotorDriver;
  StepperDriver mStepperDriver;
};

TEST_F(StepperDriverTests, single_forward_step_sets_energises_channels_to_first_step_in_sequence_and_then_steps_to_the_next)
{
  // Given
  std::vector<driver::MockDualChannelMotorDriver::SetDirectionParams>
    expectedDriverCalls = {
      {.channel = driver::Channel::A,
       .direction = sFullStepSequence[0].ChannelADirection},
      {.channel = driver::Channel::B,
       .direction = sFullStepSequence[0].ChannelBDirection},
      {.channel = driver::Channel::A,
       .direction = sFullStepSequence[1].ChannelADirection},
      {.channel = driver::Channel::B,
       .direction = sFullStepSequence[1].ChannelBDirection},
  };

  // When
  mStepperDriver.Step(Direction::Forward);

  // Then
  ASSERT_EQ(expectedDriverCalls.size(),
    mMotorDriver.SetDirectionCalls.CallCount());
  ASSERT_TRUE(
    mMotorDriver.SetDirectionCalls.CallSequenceEq(expectedDriverCalls));
}

TEST_F(StepperDriverTests, subsequent_forward_steps_cycle_through_step_sequence)
{
  // Given
  mStepperDriver.Step(Direction::Forward);
  mMotorDriver.Reset();
  const size_t expectedStartingSequencePos = 2; // First step consumes two

  for (size_t i = expectedStartingSequencePos;
       i < FullStepSequence::StepsInSequence * 2; ++i)
  {
    const auto expectedSequencePos = i % FullStepSequence::StepsInSequence;

    // When
    mStepperDriver.Step(Direction::Forward);

    // Then
    ASSERT_EQ(2, mMotorDriver.SetDirectionCalls.CallCount());
    ASSERT_TRUE(
      mMotorDriver.SetDirectionCalls.CalledWithParams({driver::Channel::A,
        sFullStepSequence[expectedSequencePos].ChannelADirection}));
    ASSERT_TRUE(
      mMotorDriver.SetDirectionCalls.CalledWithParams({driver::Channel::B,
        sFullStepSequence[expectedSequencePos].ChannelBDirection}));

    mMotorDriver.Reset();
  }
}

TEST_F(StepperDriverTests, single_reverse_step_sets_energises_channels_to_first_step_in_sequence_and_then_steps_to_the_last)
{
  // Given
  int lastIndex = FullStepSequence::StepsInSequence - 1;

  std::vector<driver::MockDualChannelMotorDriver::SetDirectionParams>
    expectedDriverCalls = {
      {.channel = driver::Channel::A,
       .direction = sFullStepSequence[0].ChannelADirection        },
      {.channel = driver::Channel::B,
       .direction = sFullStepSequence[0].ChannelBDirection        },
      {.channel = driver::Channel::A,
       .direction = sFullStepSequence[lastIndex].ChannelADirection},
      {.channel = driver::Channel::B,
       .direction = sFullStepSequence[lastIndex].ChannelBDirection}
  };

  // When
  mStepperDriver.Step(Direction::Reverse);

  // Then
  ASSERT_EQ(expectedDriverCalls.size(),
    mMotorDriver.SetDirectionCalls.CallCount());
  ASSERT_TRUE(
    mMotorDriver.SetDirectionCalls.CallSequenceEq(expectedDriverCalls));
}

TEST_F(StepperDriverTests, subsequent_reverse_steps_cycle_backwards_through_step_sequence)
{
  // Given
  mStepperDriver.Step(Direction::Reverse);
  mMotorDriver.Reset();
  size_t sequencePos = 3; // First step consumes two

  for (size_t i = 0; i < FullStepSequence::StepsInSequence * 2; ++i)
  {
    sequencePos = (sequencePos - 1) % FullStepSequence::StepsInSequence;

    // When
    mStepperDriver.Step(Direction::Reverse);

    // Then
    ASSERT_EQ(2, mMotorDriver.SetDirectionCalls.CallCount());
    ASSERT_TRUE(mMotorDriver.SetDirectionCalls.CalledWithParams(
      {driver::Channel::A, sFullStepSequence[sequencePos].ChannelADirection}));
    ASSERT_TRUE(mMotorDriver.SetDirectionCalls.CalledWithParams(
      {driver::Channel::B, sFullStepSequence[sequencePos].ChannelBDirection}));

    mMotorDriver.Reset();
  }
}

TEST_F(StepperDriverTests, StopHiZ_deenergises_both_channels)
{
  // Given, when
  mStepperDriver.StopHiZ();

  // Then
  ASSERT_EQ(2, mMotorDriver.SetDirectionCalls.CallCount());
  ASSERT_TRUE(mMotorDriver.SetDirectionCalls.CalledWithParams(
    {driver::Channel::A, driver::Direction::Off}));
  ASSERT_TRUE(mMotorDriver.SetDirectionCalls.CalledWithParams(
    {driver::Channel::B, driver::Direction::Off}));
}

TEST_F(StepperDriverTests, next_step_after_StopHiZ_reenergises_channels_before_stepping)
{
  // Given
  const size_t initialSteps = 3;
  for (size_t i = 0; i < initialSteps; ++i)
    mStepperDriver.Step(Direction::Forward);

  // And then StopHiZ
  mStepperDriver.StopHiZ();
  mMotorDriver.Reset();

  // When: next step occurs
  mStepperDriver.Step(Direction::Forward);

  // Then
  const size_t nextStep =
    (initialSteps + 1) % FullStepSequence::StepsInSequence;

  std::vector<driver::MockDualChannelMotorDriver::SetDirectionParams>
    expectedDriverCalls = {
      {.channel = driver::Channel::A,
       .direction = sFullStepSequence[initialSteps].ChannelADirection},
      {.channel = driver::Channel::B,
       .direction = sFullStepSequence[initialSteps].ChannelBDirection},
      {.channel = driver::Channel::A,
       .direction = sFullStepSequence[nextStep].ChannelADirection    },
      {.channel = driver::Channel::B,
       .direction = sFullStepSequence[nextStep].ChannelBDirection    }
  };

  // Then
  ASSERT_EQ(expectedDriverCalls.size(),
    mMotorDriver.SetDirectionCalls.CallCount());
  ASSERT_TRUE(
    mMotorDriver.SetDirectionCalls.CallSequenceEq(expectedDriverCalls));
}