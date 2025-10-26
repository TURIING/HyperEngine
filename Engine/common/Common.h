//
// Created by turiing on 2025/8/15.
//

#ifndef COMMON_H
#define COMMON_H

#include <memory>
#include <vector>
#include <map>
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
#include "HyperGpu.h"

#include "Singleton.h"
#include "LogManager.h"
#include "BaseDefine.h"
#include "RefObject.h"
#include "NoCopyable.h"
#include "Time.hpp"
#include "Export.hpp"

#endif //COMMON_H
