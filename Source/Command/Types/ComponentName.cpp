#include "ComponentName.h"

#include <cassert>
#include <cstring>

using namespace ::command;

static constexpr size_t MaxComponentNameLength = 16;

const char* ComponentNameStrings[static_cast<size_t>(ComponentName::count)] {
  "none", "test", "stepper"};

bool command::StringToComponentName(const char* string,
  ComponentName& outCommand)
{
  for (size_t i = 0; i < static_cast<size_t>(ComponentName::count); ++i)
  {
    if (strncmp(string, ComponentNameStrings[i], MaxComponentNameLength) == 0)
    {
      outCommand = static_cast<ComponentName>(i);
      return true;
    }
  }

  return false;
}

const char* command::ComponentNameToString(const ComponentName& command)
{
  assert(command < ComponentName::count);

  return ComponentNameStrings[static_cast<size_t>(command)];
}
