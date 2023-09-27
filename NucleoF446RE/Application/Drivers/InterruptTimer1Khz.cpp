#include "InterruptTimer1Khz.h"
#include "tim.h"

#include <cassert>

using namespace ::driver;

static ITimerInterruptReceiver* sCallback = nullptr;

void InterruptTimer1Khz::Init()
{
  const auto result = HAL_TIM_Base_Start_IT(&htim7);
  assert(result == HAL_OK);
}

void InterruptTimer1Khz::RegisterCallback(ITimerInterruptReceiver* callback)
{
  sCallback = callback;
}

void InterruptTimer1Khz_PeriodElapsed()
{
  if (sCallback != nullptr)
    sCallback->OnTimerInterrupt();
}
