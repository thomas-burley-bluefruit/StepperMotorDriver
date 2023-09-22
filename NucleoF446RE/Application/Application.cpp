#include "Application.h"
#include "Initialisation.h"

extern "C" void Application_Start()
{
  Initialisation::Initialise();

  while (true)
  {
  }
}
