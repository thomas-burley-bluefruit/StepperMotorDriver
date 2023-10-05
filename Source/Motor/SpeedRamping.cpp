#include "SpeedRamping.h"

using namespace ::motor;

void SpeedRamping::Init(const float startSpeed, const float targetSpeed,
  const size_t tick10Khz)
{
  mStartSpeed = startSpeed;
  mTargetSpeed = targetSpeed;
  mSpeedRange = mTargetSpeed - mStartSpeed;
  mStartTick = tick10Khz;
  mSlopeEndTick = mStartTick
    + ((static_cast<float>(mSpeedRange) / mRampRatePerSecond) * 10'000);
}

void SpeedRamping::Update(const size_t tick10Khz)
{
  const float rampPosition = (static_cast<float>(tick10Khz) - mStartTick)
    / (static_cast<float>(mSlopeEndTick) - mStartTick);
  mCurrentSpeed = mStartSpeed + rampPosition * mSpeedRange;
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
  return mCurrentSpeed < mTargetSpeed;
}