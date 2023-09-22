#include "CommandRegistry.h"

#include <cassert>

using namespace ::command;

void CommandRegistry::RegisterHandler(ICommandHandler* handler)
{
  assert(mHandlerCount < MaxCommandHandlers);

  mHandlers[mHandlerCount++] = handler;
}

bool CommandRegistry::Run(ICommandData& command)
{
  const auto component = command.GetComponentName();

  for (size_t i = 0; i < mHandlerCount; ++i)
  {
    if (mHandlers[i]->Name() == component)
      return mHandlers[i]->Run(command);
  }

  return false;
}
