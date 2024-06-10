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
  char *value;
  size_t len;

  // Determine the required buffer size
  if (_dupenv_s(&value, &len, envVariable) != 0)
  {
    printf("Failed to retrieve environment variable\n");
    return {};
  }

  char *buffer = (char *)malloc(len * sizeof(char));
  if (buffer == NULL)
  {
    printf("Failed to allocate memory\n");
    return {};
  }

  if (_dupenv_s(&buffer, &len, envVariable) != 0)
  {
    // Handle error
    printf("Failed to retrieve environment variable\n");
    free(buffer);
    return {};
  }

  std::string result = buffer;
  free(buffer);
  return result;
}
