#pragma once

#include "IGpioDriver.h"

namespace driver
{

  class MockGpioDriver final : public IGpioDriver
  {
  public:
    bool Get(GpioPin pin) const
    {
      GetCalled = true;
      return GetReturn;
    }

    void Set(GpioPin pin, bool state) const
    {
      SetCalled = true;
      SetPin = pin;
      SetState = state;
    }

    mutable bool GetCalled = false;
    bool GetReturn = false;

    mutable bool SetCalled = false;
    mutable GpioPin SetPin{};
    mutable bool SetState = false;
  };

}