#include "stdafx.h"
#include "application.h"
#include <cstring>

int main(int argc, char *argv[])
{
  std::string pathArg;

  bool foundPathVariable = false;

  for (int i = 1; i < argc; ++i)
  {
    // Check if the argument starts with "PATH="
    if (strncmp(argv[i], "PATH=", 5) == 0)
    {
      foundPathVariable = true;
      pathArg = std::string(argv[i] + 5, strlen(argv[i]) - 5); // Skip "PATH=" prefix
    }
  }

  if (!foundPathVariable)
  {
    pathArg = "";
  }
  
  Application app(pathArg);

  app.m_run();

  return 0;
}
