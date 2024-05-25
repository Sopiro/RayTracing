#pragma once

#include <array>
#include <assert.h>
#include <cstring>
#include <functional>
#include <iostream>
#include <limits>
#include <memory>
#include <memory_resource>
#include <span>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

#include "math.h"
#include "math_util.h"
#include "types.h"

using Resource = std::pmr::monotonic_buffer_resource;
using PoolResource = std::pmr::unsynchronized_pool_resource;
using Allocator = std::pmr::polymorphic_allocator<std::byte>;