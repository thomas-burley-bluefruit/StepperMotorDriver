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
#include <vector>

using namespace ::command;
using namespace ::terminal;
using namespace ::testing;

class CommandInterpreterParametersTests : public ::testing::Test
{
public:
  CommandInterpreterParametersTests() :
    mCommandInterpreter(mTerminal, mCommandRegistry)
  {
  }

protected:
  MockTerminalOut mTerminal;
  MockCommandRegistry mCommandRegistry;
  CommandInterpreterTestObject mCommandInterpreter;
};

TEST_F(CommandInterpreterParametersTests, interpret_fails_when_command_has_too_many_parameters)
{
  // Given
  std::string pair = "k=v,";
  std::string rawCommand;
  for (size_t i = 0; i < CommandData::MaxKeyValuePairs + 1; ++i)
    rawCommand += pair;

  rawCommand += "\n";
  auto command = CreateRawCommand(rawCommand);

  // When
  mCommandInterpreter.Interpret(command, rawCommand.length());

  // Then
  ASSERT_THAT(mTerminal.TextOutStrings, HasSubstr("COMMAND:BAD\n"));
}

TEST_F(CommandInterpreterParametersTests, interpret_fails_when_no_pairs_in_command)
{
  // Given
  std::string commandString = "id\n";
  auto command = CreateRawCommand(commandString);

  // When
  mCommandInterpreter.Interpret(command, commandString.length());

  // Then
  ASSERT_THAT(mTerminal.TextOutStrings, HasSubstr("COMMAND:BAD\n"));
}

TEST_F(CommandInterpreterParametersTests, can_retrieve_first_argument)
{
  // Given
  const auto key = "key";
  const auto expectedArgument = "arg";

  const auto commandString = std::format(
    "component=stepper,command=blah,{}={}\n", key, expectedArgument);
  auto command = CreateRawCommand(commandString);

  // When
  mCommandInterpreter.Interpret(command, commandString.length());

  // Then
  const char* actualArgument = "";
  ASSERT_TRUE(
    mCommandInterpreter.GetCommandData().GetString(key, actualArgument));
  ASSERT_STREQ(actualArgument, expectedArgument);
}

TEST_F(CommandInterpreterParametersTests, can_retrieve_up_to_max_arguments)
{
  // Given
  std::string command = "component=stepper,command=a,";

  for (size_t i = 0; i < CommandData::MaxParameterCount; ++i)
  {
    command += std::format("k{}={}{}", i, i,
      i < (CommandData::MaxParameterCount - 1) ? "," : "");
  }

  command += "\n";

  auto rawCommand = CreateRawCommand(command);

  // When
  mCommandInterpreter.Interpret(rawCommand, command.length());

  // Then
  const auto commandData = mCommandInterpreter.GetCommandData();

  for (size_t i = 0; i < CommandData::MaxParameterCount; ++i)
  {
    const auto key = std::format("k{}", i);
    const auto expectedStringValue = std::format("{}", i);

    const char* actualStringValue = "";
    ASSERT_TRUE(commandData.GetString(key.c_str(), actualStringValue));
    ASSERT_STREQ(actualStringValue, expectedStringValue.c_str());

    uint32_t actualIntValue = 0;
    ASSERT_TRUE(commandData.GetUint(key.c_str(), actualIntValue));
    ASSERT_EQ(i, actualIntValue);
  }
}

TEST_F(CommandInterpreterParametersTests, retrieving_string_as_uint_fails)
{
  // Given
  const auto key = "key";
  const auto argument = "arg";

  const auto commandString =
    std::format("component=stepper,command=blah,{}={}\n", key, argument);
  auto command = CreateRawCommand(commandString);

  // When
  mCommandInterpreter.Interpret(command, commandString.length());

  // Then
  const auto commandData = mCommandInterpreter.GetCommandData();
  uint32_t outInt = 0;
  ASSERT_FALSE(commandData.GetUint(key, outInt));
}

TEST_F(CommandInterpreterParametersTests, retrieving_float_as_uint_fails)
{
  // Given
  const auto key = "key";
  const auto argument = "1.2";

  const auto commandString =
    std::format("component=stepper,command=blah,{}={}\n", key, argument);
  auto command = CreateRawCommand(commandString);

  // When
  mCommandInterpreter.Interpret(command, commandString.length());

  // Then
  const auto commandData = mCommandInterpreter.GetCommandData();
  uint32_t outInt = 0;
  ASSERT_FALSE(commandData.GetUint(key, outInt));
}

TEST_F(CommandInterpreterParametersTests, retrieving_mixed_string_as_uint_fails)
{
  // Given
  const auto key = "key";
  const auto argument = "1test";

  const auto commandString =
    std::format("component=stepper,command=blah,{}={}\n", key, argument);
  auto command = CreateRawCommand(commandString);

  // When
  mCommandInterpreter.Interpret(command, commandString.length());

  // Then
  const auto commandData = mCommandInterpreter.GetCommandData();
  uint32_t outInt = 0;
  ASSERT_FALSE(commandData.GetUint(key, outInt));
}

TEST_F(CommandInterpreterParametersTests, interpret_fails_when_command_parameter_name_matches_mandatory_parameter_name)
{
  // Given
  const auto duplicateName = "target";
  auto commandString =
    std::format("{}=1,sender=0,unique=3,component=stepper,command=blah,{}=10\n",
      duplicateName, duplicateName);

  // When
  auto command = CreateRawCommand(commandString);
  mCommandInterpreter.Interpret(command, commandString.length());

  // Then
  ASSERT_THAT(mTerminal.TextOutStrings, HasSubstr("BAD\n"));
}

TEST_F(CommandInterpreterParametersTests, interpret_fails_with_duplicate_command_parameter_names)
{
  // Given
  const auto duplicateParameterName = "p3";
  auto commandString = std::format("component=stepper,"
                                   "command=blah,p1=v,p2=v,{}=v,{}=v\n",
    duplicateParameterName, duplicateParameterName);

  // When
  auto command = CreateRawCommand(commandString);
  mCommandInterpreter.Interpret(command, commandString.length());

  // Then
  ASSERT_THAT(mTerminal.TextOutStrings, HasSubstr("BAD\n"));
}

TEST_F(CommandInterpreterParametersTests, interpret_fails_with_missing_command_parameter_name)
{
  // Given
  std::string commandString = "component=stepper,command=blah,=v\n";
  auto command = CreateRawCommand(commandString);

  // When
  mCommandInterpreter.Interpret(command, commandString.length());

  // Then
  ASSERT_THAT(mTerminal.TextOutStrings, HasSubstr("BAD\n"));

  // Given
  mTerminal = MockTerminalOut();
  commandString = "component=stepper,command=blah,p=v,=v\n";
  command = CreateRawCommand(commandString);

  // When
  mCommandInterpreter.Interpret(command, commandString.length());

  // Then
  ASSERT_THAT(mTerminal.TextOutStrings, HasSubstr("BAD\n"));
}

TEST_F(CommandInterpreterParametersTests, interpret_fails_with_unpaired_command_parameter_name)
{
  // Given
  std::vector<std::string> testCases = {
    "target=1,sender=0,unique=1,component=stepper,command=foo,p\n",
    "target=1,sender=0,unique=1,component=stepper,command=foo,p,p=v\n",
    "target=1,sender=0,unique=1,component=stepper,command=foo,ledOn,\n"};

  for (auto testCase : testCases)
  {
    // When
    auto command = CreateRawCommand(testCase);
    mCommandInterpreter.Interpret(command, testCase.length());

    // Then
    ASSERT_THAT(mTerminal.TextOutStrings, HasSubstr("BAD\n"));
    mTerminal = MockTerminalOut();
  }
}

TEST_F(CommandInterpreterParametersTests, interpret_fails_with_trailing_comma)
{
  // Given
  std::vector<std::string> testCases = {
    "target=1,sender=0,unique=1,component=stepper,command=foo,\n",
    "target=1,sender=0,unique=1,component=stepper,command=foo,p=v,\n",
    "target=1,sender=0,unique=1,component=stepper,command=foo,p=v,p1=,\n"};

  for (auto testCase : testCases)
  {
    // When
    auto command = CreateRawCommand(testCase);
    mCommandInterpreter.Interpret(command, testCase.length());

    // Then
    ASSERT_THAT(mTerminal.TextOutStrings, HasSubstr("BAD\n"));
    mTerminal = MockTerminalOut();
  }
}
