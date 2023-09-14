#pragma once

#include "IGpioDriver.h"
#include "MockCalls.h"

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
    SetCalls.Add({pin, state});
  }

  void Reset()
  {
    *this = {};
  }

  mutable bool GetCalled = false;
  bool GetReturn = false;

  struct SetParams
  {
    GpioPin pin;
    bool state;

    bool operator==(const SetParams& rhs) const
    {
      return this->pin == rhs.pin && this->state == rhs.state;
    }
  };

  mutable test::utility::CallsWithParams<SetParams> SetCalls;
};

}