#pragma once

#include <memory>
#include <utility>
#include <vector>
#include <array>
#include <map>
#include <string>
#include <string_view>
#include <fstream>
#include <filesystem>

#include <Windows.h>

#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>
#include <imgui.h>
#include <imgui_stdlib.h>

#include <expected/expected.hpp>

#include "InteropString.hpp"
#include "AModule.hpp"
#include "utils.ipp"
