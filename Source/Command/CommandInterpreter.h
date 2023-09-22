#pragma once

#include <cstddef>
#include <cstdlib>

#include "CommandData.h"
#include "CommandKeys.h"
#include "ICommandInterpreter.h"
#include "ICommandRegistry.h"
#include "ITerminalOut.h"

namespace command
{

class CommandInterpreter : public ICommandInterpreter
{
public:
  CommandInterpreter(terminal::ITerminalOut& terminal,
    ICommandRegistry& registry);
  void Interpret(CommandData::RawInput& rawInput, size_t length) override;

protected:
  CommandData& GetCommandData();

private:
  CommandData mCommandData {};

  size_t mParameterCount = 0;
  size_t mInputLength = 0;

  static constexpr char KeyTerminator = '=';
  static constexpr char ParameterSeparator = ',';
  static constexpr char CommandTerminator = '\n';

  static constexpr char const* CommandBadMessage = "COMMAND:BAD\n";

  terminal::ITerminalOut& mTerminal;
  ICommandRegistry& mCommandRegistry;

  void PrintFailMessage() const;
  bool FindKeyValuePairs();
  bool ParseComponentAndCommand();
};

}
