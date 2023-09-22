#include "CommandInterpreter.h"

#include <cctype>
#include <climits>
#include <cstring>

using namespace ::terminal;
using namespace ::command;

CommandInterpreter::CommandInterpreter(ITerminalOut& terminal,
  ICommandRegistry& commandRegistry) :
  mTerminal(terminal),
  mCommandRegistry(commandRegistry)
{
}

void CommandInterpreter::Interpret(CommandData::RawInput& rawInput,
  size_t length)
{
  mCommandData = CommandData();
  mCommandData.GetRawInput() = rawInput;
  mParameterCount = 0;
  mInputLength = length;

  if (!FindKeyValuePairs())
    return PrintFailMessage();

  if (!ParseComponentAndCommand())
    return PrintFailMessage();

  mCommandRegistry.Run(mCommandData);
}

bool CommandInterpreter::FindKeyValuePairs()
{
  auto& rawInput = mCommandData.GetRawInput();

  KeyValuePair pair {};
  size_t currentKeyIndex = 0;

  for (size_t i = 0; i < mInputLength; ++i)
  {
    char& currentChar = rawInput[i];

    if (currentChar == ' ')
      return false;

    if (currentChar == KeyTerminator)
    {
      currentChar = '\0';

      pair.KeyIndex = currentKeyIndex;
      pair.ValueIndex = i + 1;
      continue;
    }

    if (currentChar == ParameterSeparator || currentChar == CommandTerminator)
    {
      currentChar = '\0';

      if (!mCommandData.AddKeyValuePair(pair))
        return false;

      currentKeyIndex = i + 1;
    }
  }

  return mCommandData.GetKeyValuePairCount() != 0;
}

bool CommandInterpreter::ParseComponentAndCommand()
{
  const char* componentNameString = "";
  if (!mCommandData.GetString(CommandKeys::Component, componentNameString))
    return false;

  ComponentName component;
  if (!StringToComponentName(componentNameString, component))
    return false;

  mCommandData.SetComponentName(component);

  if (!mCommandData.ValidateCommand())
    return false;

  return true;
}

CommandData& CommandInterpreter::GetCommandData()
{
  return mCommandData;
}

void CommandInterpreter::PrintFailMessage() const
{
  mTerminal.TextOut(CommandBadMessage);
}
