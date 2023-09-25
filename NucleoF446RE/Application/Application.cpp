#include "Application.h"
#include "Initialisation.h"

extern "C" void Application_Start()
{
  Initialisation::Initialise();

  auto& terminalIn = Initialisation::GetTerminalIn();
  while (true)
  {
    terminalIn.Update(0);
  }
}
