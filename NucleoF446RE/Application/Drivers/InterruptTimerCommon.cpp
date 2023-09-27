#include "InterruptTimer10Khz.h"
#include "InterruptTimer1Khz.h"
#include "tim.h"

extern "C" void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim)
{
  if (htim->Instance == htim6.Instance)
    return InterruptTimer10Khz_PeriodElapsed();

  if (htim->Instance == htim7.Instance)
    return InterruptTimer1Khz_PeriodElapsed();
}