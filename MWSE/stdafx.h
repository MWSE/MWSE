#pragma once

// Standard template libraries.
#include <algorithm>
#include <exception>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <list>
#include <map>
#include <mutex>
#include <numeric>
#include <ostream>
#include <queue>
#include <random>
#include <sstream>
#include <stddef.h>
#include <stdexcept>
#include <stdio.h>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <future>
#include <regex>
#include <charconv>

// Required C includes.
#include <cmath>
#include <cstdint>
#include <cstring>
#include <ctime>

// Required Windows APIs.
#include <windows.h>
#include <wtypes.h>
#include <psapi.h>
#include <dbghelp.h>
#include <d3d8.h>

// 3rd party library: span-lite
// URL: https://github.com/martinmoene/span-lite
#define span_CONFIG_SELECT_SPAN span_SPAN_NONSTD
#include <nonstd/span.hpp>

// DirectInput/Sound requirements.
#define DIRECTINPUT_VERSION 0x0800
#include <control.h>
#include <strmif.h>
#include <dinput.h>
#include <dsound.h>

// Evil devil-spawn legacy boost requirements.
#include <boost/regex.hpp>

// Core lua binding library.
#include <sol/sol.hpp>

// Evil YAML library we are forced to use.
#define YAML_CPP_STATIC_DEFINE 
#include <yaml-cpp/yaml.h>

// Undefine anything we don't care about otherwise.
#undef far
#undef near
#undef PlaySound