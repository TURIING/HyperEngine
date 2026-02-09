//
// Created by turiing on 2025/8/15.
//

#ifndef COMMON_H
#define COMMON_H

#include <memory>
#include <vector>
#include <map>
#include <set>
#include <unordered_map>
#include <string>
#include <algorithm>
#include <typeindex>
#include <filesystem>
#include <functional>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <rocket.hpp>

#if defined(_WIN32)
#define VK_USE_PLATFORM_WIN32_KHR
#elif defined(__linux__) || defined(__unix__)
#define VK_USE_PLATFORM_XLIB_KHR
#elif defined(__APPLE__)
#define VK_USE_PLATFORM_MACOS_MVK
#else
#endif
#define VK_NO_PROTOTYPES
#include <volk.h>

#include "Singleton.h"
#include "LogManager.h"
#include "BaseDefine.h"
#include "RefObject.h"
#include "NoCopyable.h"
#include "Time.hpp"
#include "Export.hpp"
#include "Utility.h"
#include "TypeInfo.hpp"

#endif //COMMON_H
