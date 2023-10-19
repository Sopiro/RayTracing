#pragma once

#include "scene.h"

namespace spt
{

// Unidirectional path tracer
Spectrum PathTrace(const Scene& scene, Ray primary_ray, int32 max_bounces);

} // namespace spt
