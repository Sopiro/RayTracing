#pragma once

#include <array>
#include <assert.h>
#include <fstream>
#include <functional>
#include <iostream>
#include <limits>
#include <memory>
#include <memory_resource>
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

using Resource = std::pmr::monotonic_buffer_resource;
using PoolResource = std::pmr::unsynchronized_pool_resource;
using Allocator = std::pmr::polymorphic_allocator<std::byte>;