#pragma once

#include <iostream>
#include <sstream>
#include <string>

#include <unordered_map>
#include <functional>
#include <vector>
#include <filesystem>

#ifdef _WIN32
#include <windows.h>
#ifdef UNICODE
typedef std::wstring tstring;
#else
typedef std::string tstring;
#endif
#else
#include <unistd.h>
#include <limits.h>
#endif

namespace fs = std::filesystem;