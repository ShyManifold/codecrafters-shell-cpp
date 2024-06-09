#include "stdafx.h"
#include "application.h"
#include <cstring>

int main(int argc, char *argv[])
{
  std::string pathArg = std::getenv("PATH");
  Application app(pathArg);

  for (size_t i = 0; i < argc; i++)
  {
    std::cout << argv[i] << std::endl;
  }

  app.m_run();

  return 0;
}
