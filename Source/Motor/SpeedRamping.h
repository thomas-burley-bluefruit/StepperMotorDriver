#pragma once

#include <cstddef>
#include <cstdint>

namespace motor
{

class SpeedRamping
{
public:
  void Init(const float startSpeed, const float targetSpeed,
    const size_t tick10Khz);
  void Update(const size_t tick10Khz);
  void SetRampRate(float speedUnitsPerSecond);
  float GetRampRate() const;
  float GetSpeed();
  bool Ramping() const;

private:
  bool mRamping = false;
  size_t mStartTick = 0;
  size_t mSlopeEndTick = 0;
  float mRampRatePerSecond = 0;
  float mStartSpeed = 0;
  float mTargetSpeed = 0;
  float mSpeedRange = 0;
  float mCurrentSpeed = 0;
};

}