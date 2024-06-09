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
#else
#include <unistd.h>
#include <limits.h>
#endif

#ifdef UNICODE
typedef std::wstring tstring;
#else
typedef std::string tstring;
#endif

namespace fs = std::filesystem;