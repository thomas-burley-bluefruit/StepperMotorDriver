#include "mock.ButtonPressCallback.h"
#include "mock.GpioDriver.h"
#include "mock.InterruptTimer.h"
#include "UserButton.h"
#include "gmock/gmock.h"

#include <vector>

using namespace ::driver;
using namespace ::userinput;
using ::std::vector;

class UserButtonTests : public testing::Test
{
public:
  UserButtonTests() :
    mUserButton(mGpio, mInterruptTimer)
  {
  }

protected:
  MockGpioDriver mGpio;
  MockInterruptTimer mInterruptTimer;
  UserButton mUserButton;

  void TriggerButtonPress()
  {
    mGpio.GetReturn = false;
    for (int i = 0; i < UserButton::MinimumOnTimeMs; ++i)
    {
      mUserButton.OnExternalInterrupt();
    }
  }

  void ReleaseButton()
  {
    mGpio.GetReturn = true;
    mUserButton.OnExternalInterrupt();
  }

  void SatisfyDebouncePeriod()
  {
    for (size_t i = 0; i < UserButton::MinimumIntervalMs; ++i)
      mUserButton.OnExternalInterrupt();
  }
};

TEST_F(UserButtonTests, Constructor_registers_callback_with_interrupt_timer)
{
  ASSERT_TRUE(mInterruptTimer.RegisterCallbackCalled);
  ASSERT_EQ(&mUserButton, mInterruptTimer.Callback);
}

TEST_F(UserButtonTests,
       Registered_callback_is_called_when_button_gpio_is_reset_for_minimum_counts_required)
{
  // Given
  MockButtonPressCallback callbackSpy;
  mUserButton.RegisterCallback(&callbackSpy);
  mGpio.GetReturn = false;

  // When
  for (int i = 0; i < UserButton::MinimumOnTimeMs - 1; ++i)
  {
    mUserButton.OnExternalInterrupt();
    ASSERT_FALSE(callbackSpy.OnButtonPressCalled);
  }

  mUserButton.OnExternalInterrupt();

  // Then
  ASSERT_TRUE(callbackSpy.OnButtonPressCalled);
}

TEST_F(UserButtonTests,
       Registered_callback_is_not_called_when_button_gpio_is_set_for_minimum_counts_required)
{
  // Given
  MockButtonPressCallback callbackSpy;
  mUserButton.RegisterCallback(&callbackSpy);
  mGpio.GetReturn = true;

  // When, then
  for (int i = 0; i < UserButton::MinimumOnTimeMs; ++i)
  {
    mUserButton.OnExternalInterrupt();
    ASSERT_FALSE(callbackSpy.OnButtonPressCalled);
  }
}

TEST_F(UserButtonTests,
       On_time_is_reset_when_button_gpio_goes_to_set)
{
  // Given
  MockButtonPressCallback callbackSpy;
  mUserButton.RegisterCallback(&callbackSpy);
  mGpio.GetReturn = false;

  for (int i = 0; i < UserButton::MinimumOnTimeMs - 1; ++i)
  {
    mUserButton.OnExternalInterrupt();
    ASSERT_FALSE(callbackSpy.OnButtonPressCalled);
  }

  // When
  mGpio.GetReturn = true;
  mUserButton.OnExternalInterrupt();
  mGpio.GetReturn = false;
  mUserButton.OnExternalInterrupt();

  // Then
  ASSERT_FALSE(callbackSpy.OnButtonPressCalled);
}

TEST_F(UserButtonTests, Button_is_debounced)
{
  // Given
  MockButtonPressCallback callbackSpy;
  mUserButton.RegisterCallback(&callbackSpy);

  TriggerButtonPress();
  ReleaseButton();
  ASSERT_TRUE(callbackSpy.OnButtonPressCalled);
  callbackSpy.OnButtonPressCalled = false;

  // When
  mGpio.GetReturn = false;
  for (int i = 0; i < UserButton::MinimumIntervalMs - 1; ++i)
  {
    mUserButton.OnExternalInterrupt();
    ASSERT_FALSE(callbackSpy.OnButtonPressCalled);
  }

  // Then
  mUserButton.OnExternalInterrupt();
  ASSERT_TRUE(callbackSpy.OnButtonPressCalled);
}

TEST_F(UserButtonTests, Button_needs_to_be_released_before_callback_will_fire_again)
{
  // Given
  MockButtonPressCallback callbackSpy;
  mUserButton.RegisterCallback(&callbackSpy);

  TriggerButtonPress();
  ASSERT_TRUE(callbackSpy.OnButtonPressCalled);
  callbackSpy.OnButtonPressCalled = false;

  // When
  SatisfyDebouncePeriod();
  TriggerButtonPress();

  // Then
  ASSERT_FALSE(callbackSpy.OnButtonPressCalled);

  // When
  ReleaseButton();
  SatisfyDebouncePeriod();
  TriggerButtonPress();

  // Then
  ASSERT_TRUE(callbackSpy.OnButtonPressCalled);
}

TEST_F(UserButtonTests, Multiple_callbacks_can_be_registered)
{
  // Given
  std::vector<MockButtonPressCallback> callbacks(UserButton::MaxCallbacks,
    MockButtonPressCallback());

  for (auto& callback : callbacks)
    mUserButton.RegisterCallback(&callback);

  // When
  TriggerButtonPress();

  // Then
  for (auto& callback : callbacks)
    ASSERT_TRUE(callback.OnButtonPressCalled);
}

TEST_F(UserButtonTests, Can_only_register_max_callbacks)
{
  // Given
  MockButtonPressCallback callback;

  // When
  for (int i = 0; i < UserButton::MaxCallbacks; ++i)
    mUserButton.RegisterCallback(&callback);

  // Then
  ASSERT_DEATH(mUserButton.RegisterCallback(&callback), "");
}
