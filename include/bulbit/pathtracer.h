#pragma once

#include "scene.h"

namespace bulbit
{

// Unidirectional path tracer
Spectrum PathTrace(const Scene& scene, Ray primary_ray, int32 max_bounces);

} // namespace bulbit
