#pragma once

#include <vector>

namespace test::utility
{

template <typename T>
class CallsWithParams
{
public:
  void Add(const T params)
  {
    mParams.push_back(params);
  }

  size_t CallCount()
  {
    return mParams.size();
  }

  bool CalledWithParams(const T params)
  {
    for (const auto& p : mParams)
    {
      if (p == params)
        return true;
    }
    return false;
  }

  bool CallSequenceEq(std::vector<T> calls)
  {
    return calls == mParams;
  }

private:
  std::vector<T> mParams;
};

}