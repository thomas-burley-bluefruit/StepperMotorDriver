#pragma once

#include "ICommandRegistry.h"

#include <array>

namespace command
{

class CommandRegistry final : public ICommandRegistry
{
public:
  static constexpr size_t MaxCommandHandlers = 8;

  void RegisterHandler(ICommandHandler* handler) override;
  bool Run(ICommandData& command) override;

private:
  size_t mHandlerCount = 0;
  std::array<ICommandHandler*, MaxCommandHandlers> mHandlers {nullptr};
};

}