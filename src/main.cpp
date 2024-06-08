#include "stdafx.h"
#include "application.h"
#include <cstring>

int main(int argc, char *argv[])
{
  std::string pathArg = ".";

  for (int i = 1; i < argc; ++i)
  {
    // Check if the argument starts with "PATH="
    if (strncmp(argv[i], "PATH=", 5) == 0)
    {
      pathArg = std::string(argv[i] + 6,strlen(argv[i]) - 7); // Skip "PATH=" prefix
    }
  }


  Application app(pathArg);

  app.m_run();

  return 0;
}
