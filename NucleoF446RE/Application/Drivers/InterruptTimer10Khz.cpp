#include "InterruptTimer10Khz.h"
#include "tim.h"

#include <cassert>

using namespace ::driver;

static ITimerInterruptReceiver* sCallback = nullptr;

void InterruptTimer10Khz::Init()
{
  const auto result = HAL_TIM_Base_Start_IT(&htim6);
  assert(result == HAL_OK);
}

void InterruptTimer10Khz::RegisterCallback(ITimerInterruptReceiver* callback)
{
  sCallback = callback;
}

extern "C" void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim)
{
  if (htim->Instance != htim6.Instance || sCallback == nullptr)
    return;

  sCallback->OnTimerInterrupt();
}
