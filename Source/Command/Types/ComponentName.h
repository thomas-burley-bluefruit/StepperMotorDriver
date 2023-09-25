#pragma once
#include <cstddef>
#include <cstdint>

namespace command
{

enum class ComponentName : uint8_t
{
  none,
  test,
  stepper,
  count
};

bool StringToComponentName(const char* string, ComponentName& outCommand);
const char* ComponentNameToString(const ComponentName& command);

}
