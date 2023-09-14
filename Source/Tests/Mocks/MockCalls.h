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

private:
  std::vector<T> mParams;
};

}