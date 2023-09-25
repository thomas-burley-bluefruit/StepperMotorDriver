#include "CommandInterpreter.h"
#include "CommandInterpreterTestObject.h"
#include "CommandTestUtilities.h"
#include "ComponentName.h"
#include "mock.CommandRegistry.h"
#include "mock.TerminalOut.h"
#include "gmock/gmock.h"

#include <format>

using namespace ::command;
using namespace ::terminal;
using namespace ::testing;

class CommandInterpreterTests : public ::testing::Test
{
public:
  CommandInterpreterTests() :
    mCommandInterpreter(mTerminal, mCommandRegistry)
  {
  }

protected:
  MockTerminalOut mTerminal;
  MockCommandRegistry mCommandRegistry;
  CommandInterpreterTestObject mCommandInterpreter;
};

TEST_F(CommandInterpreterTests, parsed_command_is_passed_to_command_registry)
{
  // Given
  auto expectedComponent = ComponentName::stepper;
  std::string expectedCommand = "cmd";
  std::string expectedKey = "key";
  std::string expectedArgument = "arg";

  const auto commandString = std::format("component={},command={},{}={}\n",
    ComponentNameToString(expectedComponent), expectedCommand, expectedKey,
    expectedArgument);
  auto command = CreateRawCommand(commandString);

  // When
  mCommandInterpreter.Interpret(command, commandString.length());

  // Then
  ASSERT_TRUE(mCommandRegistry.RunCalled);
  const auto commandRun = mCommandRegistry.CommandRun;

  ASSERT_EQ(expectedComponent, commandRun.GetComponentName());
  ASSERT_STREQ(expectedCommand.c_str(), commandRun.GetCommand());
  const char* actualArgument = "";
  ASSERT_TRUE(commandRun.GetString(expectedKey.c_str(), actualArgument));
  ASSERT_STREQ(expectedArgument.c_str(), actualArgument);
}
