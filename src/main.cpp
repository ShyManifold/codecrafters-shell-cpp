#include "stdafx.h"
#include "application.h"
#include <cstring>

std::string g_getEnvironmentVariable(const char *envVariable);

int main(int argc, char *argv[])
{
  std::string pathArg = g_getEnvironmentVariable("PATH");
  Application app(pathArg);

  app.m_run();

  return 0;
}

std::string g_getEnvironmentVariable(const char *envVariable)
{
#ifdef _WIN32
  char *value;
  size_t len;

  // Determine the required buffer size
  if (_dupenv_s(&value, &len, envVariable) != 0)
  {
    std::cout << "Failed to retrieve environment variable" << std::endl;
    return {};
  }

  char *buffer = (char *)malloc(len * sizeof(char));
  if (buffer == NULL)
  {
    std::cout << "Failed to allocate memory" << std::endl;
    return {};
  }

  if (_dupenv_s(&buffer, &len, envVariable) != 0)
  {
    std::cout << "Failed to retrieve environment variable" << std::endl;
    free(buffer);
    return {};
  }
  std::string result = buffer;
  free(buffer);
  return result;
#else
  const char *buffer = std::getenv(envVariable);
  if (buffer == nullptr)
  {
    std::cout << "Failed to retrieve environment variable" << std::endl;
    return "";
  }
  return std::string(buffer);
#endif
}
