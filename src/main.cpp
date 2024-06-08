#include "stdafx.h"
#include "application.h"
#include <cstring>

int main(int argc, char *argv[])
{
  std::string pathArg = std::getenv("PATH");
  Application app(pathArg);

  app.m_run();

  return 0;
}
