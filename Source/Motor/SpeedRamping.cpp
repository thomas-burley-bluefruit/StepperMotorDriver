#include "SpeedRamping.h"

#include <algorithm>
#include <cmath>

using namespace ::motor;

void SpeedRamping::Init(const float startSpeed, const float targetSpeed,
  const size_t tick10Khz)
{
  mStartSpeed = startSpeed;
  mTargetSpeed = targetSpeed;
  mSpeedRange = mTargetSpeed - mStartSpeed;
  mStartTick = tick10Khz;
  mSlopeEndTick =
    mStartTick + abs(((mSpeedRange / mRampRatePerSecond) * 10'000));
  mRamping = true;
}

void SpeedRamping::Update(const size_t tick10Khz)
{
  const float rampPosition =
    std::min((static_cast<float>(tick10Khz) - mStartTick)
        / (static_cast<float>(mSlopeEndTick) - mStartTick),
      1.0f);
  mCurrentSpeed = mStartSpeed + (rampPosition * mSpeedRange);
  if (mCurrentSpeed == mTargetSpeed)
    mRamping = false;
}

void SpeedRamping::SetRampRate(float speedUnitsPerSecond)
{
  mRampRatePerSecond = speedUnitsPerSecond;
}

float SpeedRamping::GetRampRate() const
{
  return mRampRatePerSecond;
}

float SpeedRamping::GetSpeed()
{
  return mCurrentSpeed;
}

bool SpeedRamping::Ramping() const
{
  return mRamping;
}