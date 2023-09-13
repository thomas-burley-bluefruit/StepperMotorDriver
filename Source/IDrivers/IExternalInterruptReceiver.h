#pragma once

namespace driver
{

  class IExternalInterruptReceiver
  {
  public:
    virtual void OnExternalInterrupt() = 0;
    ~IExternalInterruptReceiver() = default;
  };

}
