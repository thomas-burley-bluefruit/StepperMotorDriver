#include "UserButton.h"

#include <cassert>

using namespace ::userinput;

UserButton::UserButton(driver::IGpioDriver& gpio,
  driver::IInterruptTimer& interruptTimer) :
  mGpio(gpio),
  mInterruptTimer(interruptTimer)
{
  mInterruptTimer.RegisterCallback(this);
}

bool UserButton::Debounce()
{
  if ((++mHeldTimeMs < MinimumOnTimeMs)
    || (mTickMs - mLastPressTimeMs) <= MinimumIntervalMs)
    return false;

  mLastPressTimeMs = mTickMs;
  return true;
}

void UserButton::RegisterCallback(IButtonPressCallback* callback)
{
  assert(mCallbackCount < MaxCallbacks);
  mCallbacks[mCallbackCount++] = callback;
}

void UserButton::OnExternalInterrupt()
{
  ++mTickMs;

  if (mGpio.Get(ButtonGpio))
  {
    mHeldTimeMs = 0;
    mHeld = false;
    return;
  }

  if (mHeld || !Debounce())
    return;

  for (uint32_t i = 0; i < mCallbackCount; ++i)
    mCallbacks[i]->OnButtonPress();

  mHeld = true;
}
