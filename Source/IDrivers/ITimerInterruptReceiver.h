#pragma once

namespace driver
{

class ITimerInterruptReceiver
{
public:
  virtual void OnTimerInterrupt() = 0;
  ~ITimerInterruptReceiver() = default;
};

}
