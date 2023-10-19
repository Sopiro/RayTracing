#pragma once

#include <array>
#include <assert.h>
#include <fstream>
#include <functional>
#include <iostream>
#include <limits>
#include <memory>
#include <sstream>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "math.h"
#include "math_util.h"
#include "random.h"
#include "types.h"

template <typename T>
using Ref = std::shared_ptr<T>;

template <typename T, typename... Args>
inline std::shared_ptr<T> CreateSharedRef(Args&&... args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}
