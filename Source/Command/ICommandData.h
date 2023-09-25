#pragma once

#include "ComponentName.h"

namespace command
{

class ICommandData
{
public:
  virtual bool GetString(const char* key, const char*& outString) const = 0;
  virtual bool GetUint(const char* key, uint32_t& outInt) const = 0;
  virtual ComponentName GetComponentName() const = 0;
  virtual const char* GetCommand() const = 0;
  virtual bool CommandIs(const char* command) const = 0;
  virtual size_t GetParameterCount() const = 0;
  virtual ~ICommandData() = default;
};

}
