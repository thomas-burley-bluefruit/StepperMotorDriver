#pragma once

#include "ICommandData.h"
#include <map>
#include <string>

namespace command
{

class MockCommandData final : public ICommandData
{
public:
  bool GetString(const char* key, const char*& outString) const override
  {
    if (Pairs.find(key) == Pairs.end())
      return false;

    std::string valueString = Pairs.at(key);
    outString = valueString.c_str();
    return true;
  }

  bool GetUint(const char* key, uint32_t& outInt) const override
  {
    if (Pairs.find(key) == Pairs.end())
      return false;

    std::string valueString = Pairs.at(key);
    outInt = std::stoul(valueString);
    return true;
  }

  ComponentName GetComponentName() const override
  {
    return GetComponentNameReturnValue;
  }

  const char* GetCommand() const override
  {
    return GetCommandReturnValue.c_str();
  }

  bool CommandIs(const char* command) const override
  {
    return std::string(command) == GetCommandReturnValue;
  }

  size_t GetParameterCount() const override
  {
    return GetParameterCountReturnValue;
  }

  std::map<std::string, std::string> Pairs {};
  bool GetStringReturnValue = false;
  std::string GetStringOutput;
  bool GetUintReturnValue = false;
  uint32_t GetUintOutput = 0;
  ComponentName GetComponentNameReturnValue = ComponentName::none;
  std::string GetCommandReturnValue;
  size_t GetParameterCountReturnValue = 0;
};
}
