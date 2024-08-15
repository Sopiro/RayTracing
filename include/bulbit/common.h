#pragma once

#include <array>
#include <cmath>
#include <cstring>
#include <format>
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

#include "asserts.h"
#include "math.h"

using Resource = std::pmr::monotonic_buffer_resource;
using PoolResource = std::pmr::unsynchronized_pool_resource;
using Allocator = std::pmr::polymorphic_allocator<std::byte>;