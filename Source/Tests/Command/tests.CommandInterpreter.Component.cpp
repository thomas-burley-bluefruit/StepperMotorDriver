#include "CommandInterpreter.h"
#include "CommandInterpreterTestObject.h"
#include "CommandTestUtilities.h"
#include "ComponentName.h"
#include "mock.CommandRegistry.h"
#include "mock.TerminalOut.h"
#include "gmock/gmock.h"

#include <array>
#include <format>
#include <ios>
#include <sstream>
#include <string>

using namespace ::command;
using namespace ::terminal;
using namespace ::testing;

class CommandInterpreterComponentTests : public ::testing::Test
{
public:
  CommandInterpreterComponentTests() :
    mCommandInterpreter(mTerminal, mCommandRegistry)
  {
  }

protected:
  MockTerminalOut mTerminal;
  MockCommandRegistry mCommandRegistry;
  CommandInterpreterTestObject mCommandInterpreter;
};

TEST_F(CommandInterpreterComponentTests, valid_component_name_is_parsed_correctly)
{
  // Given
  const auto expectedComponentName = ComponentName::stepper;

  const auto commandString = std::format("component={},command=bleh\n",
    ComponentNameToString(expectedComponentName));
  auto command = CreateRawCommand(commandString);

  // When
  mCommandInterpreter.Interpret(command, commandString.length());

  // Then
  ASSERT_EQ(mCommandInterpreter.GetCommandData().GetComponentName(),
    expectedComponentName);
}

TEST_F(CommandInterpreterComponentTests, interpret_fails_for_invalid_component_name)
{
  // Given
  std::string commandString =
    "target=12,sender=123,unique=456,component=invalid,command=bleh\n";
  auto command = CreateRawCommand(commandString);

  // When
  mCommandInterpreter.Interpret(command, commandString.length());

  // Then
  ASSERT_THAT(mTerminal.TextOutStrings, HasSubstr("COMMAND:BAD\n"));
}

TEST_F(CommandInterpreterComponentTests, interpret_fails_for_missing_component_name)
{
  // Given
  std::string commandString = "target=12,sender=123,unique=456,command=bleh\n";
  auto command = CreateRawCommand(commandString);

  // When
  mCommandInterpreter.Interpret(command, commandString.length());

  // Then
  ASSERT_THAT(mTerminal.TextOutStrings, HasSubstr("COMMAND:BAD\n"));
}

TEST_F(CommandInterpreterComponentTests, command_is_parsed_correctly)
{
  // Given
  const auto expectedCommand = "command";
  const auto commandString =
    std::format("component=stepper,command={}\n", expectedCommand);
  auto command = CreateRawCommand(commandString);

  // When
  mCommandInterpreter.Interpret(command, commandString.length());

  // Then
  ASSERT_STREQ(mCommandInterpreter.GetCommandData().GetCommand(),
    expectedCommand);
}

TEST_F(CommandInterpreterComponentTests, interpret_fails_for_missing_command)
{
  // Given
  std::string commandString =
    "target=12,sender=123,unique=456,component=stepper\n";
  auto command = CreateRawCommand(commandString);

  // When
  mCommandInterpreter.Interpret(command, commandString.length());

  // Then
  ASSERT_THAT(mTerminal.TextOutStrings, HasSubstr("COMMAND:BAD\n"));
}
