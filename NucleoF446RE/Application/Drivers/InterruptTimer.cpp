#include "InterruptTimer.h"
#include "tim.h"

#include <cassert>

using namespace ::driver;

static IExternalInterruptReceiver* sCallback = nullptr;

void InterruptTimer::Init()
{
  const auto result = HAL_TIM_Base_Start_IT(&htim7);
  assert(result == HAL_OK);
}

void InterruptTimer::RegisterCallback(IExternalInterruptReceiver* callback)
{
  sCallback = callback;
}

extern "C" void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim)
{
  if (htim->Instance != htim7.Instance || sCallback == nullptr)
    return;

  sCallback->OnExternalInterrupt();
}
