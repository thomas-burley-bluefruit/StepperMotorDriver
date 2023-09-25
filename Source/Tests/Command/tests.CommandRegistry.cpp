#include "CommandRegistry.h"
#include "mock.CommandHandler.h"
#include "gmock/gmock.h"

using namespace ::command;

TEST(CommandRegistryTests, can_only_register_up_to_max_handlers)
{
  // Given
  MockCommandHandler handler;
  CommandRegistry registry;

  // When
  for (size_t i = 0; i < CommandRegistry::MaxCommandHandlers; ++i)
  {
    registry.RegisterHandler(&handler);
  }

  // Then
  EXPECT_DEATH(registry.RegisterHandler(&handler), "");
}

TEST(CommandRegistryTests, run_calls_registered_handler_when_component_name_matches)
{
  // Given
  auto componentName = ComponentName::stepper;

  MockCommandHandler handler;
  handler.NameReturnValue = componentName;

  CommandRegistry registry;
  registry.RegisterHandler(&handler);

  CommandData commandData;
  commandData.SetComponentName(componentName);

  // When
  registry.Run(commandData);

  // Then
  ASSERT_TRUE(handler.RunCalled);
}

TEST(CommandRegistryTests, run_does_not_call_registered_handler_if_component_name_does_not_match)
{
  // Given
  auto handlerComponentName = ComponentName::test;
  auto commandComponentName = ComponentName::stepper;

  MockCommandHandler handler;
  handler.NameReturnValue = handlerComponentName;

  CommandRegistry registry;
  registry.RegisterHandler(&handler);

  CommandData commandData;
  commandData.SetComponentName(commandComponentName);

  // When
  registry.Run(commandData);

  // Then
  ASSERT_FALSE(handler.RunCalled);
}

TEST(CommandRegistryTests, run_calls_second_registered_handler_if_component_name_matches)
{
  // Given
  auto firstHandlerComponentName = ComponentName::test;
  MockCommandHandler firstHandler;
  firstHandler.NameReturnValue = firstHandlerComponentName;

  auto secondHandlerComponentName = ComponentName::stepper;
  MockCommandHandler secondHandler;
  secondHandler.NameReturnValue = secondHandlerComponentName;

  CommandRegistry registry;
  registry.RegisterHandler(&firstHandler);
  registry.RegisterHandler(&secondHandler);

  CommandData commandData;
  commandData.SetComponentName(secondHandlerComponentName);

  // When
  registry.Run(commandData);

  // Then
  ASSERT_FALSE(firstHandler.RunCalled);
  ASSERT_TRUE(secondHandler.RunCalled);
}

TEST(CommandRegistryTests, run_returns_result_of_handler_run)
{
  // Given
  auto componentName = ComponentName::stepper;

  MockCommandHandler handler;
  handler.NameReturnValue = componentName;
  handler.RunReturnValue = true;

  CommandRegistry registry;
  registry.RegisterHandler(&handler);

  CommandData commandData;
  commandData.SetComponentName(componentName);

  // When
  bool runReturnValue = registry.Run(commandData);

  // Then
  ASSERT_TRUE(handler.RunCalled);
  ASSERT_EQ(handler.RunReturnValue, runReturnValue);

  // When
  handler.RunReturnValue = false;
  runReturnValue = registry.Run(commandData);

  // Then
  ASSERT_EQ(handler.RunReturnValue, runReturnValue);
}

TEST(CommandRegistryTests, run_returns_false_when_no_handlers_registered)
{
  // Given
  CommandRegistry registry;
  CommandData command;

  // When
  bool runReturnValue = registry.Run(command);

  // Then
  ASSERT_FALSE(runReturnValue);
}

TEST(CommandRegistryTests, run_returns_false_when_no_matching_handlers_registered)
{
  // Given
  auto handlerComponentName = ComponentName::test;
  auto commandComponentName = ComponentName::stepper;

  MockCommandHandler handler;
  handler.NameReturnValue = handlerComponentName;

  CommandRegistry registry;
  registry.RegisterHandler(&handler);

  CommandData commandData;
  commandData.SetComponentName(commandComponentName);

  // When
  bool runReturnValue = registry.Run(commandData);

  // Then
  ASSERT_FALSE(runReturnValue);
}