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

extern "C" void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim)
{
  if (htim->Instance != htim7.Instance || sCallback == nullptr)
    return;

  sCallback->OnTimerInterrupt();
}
