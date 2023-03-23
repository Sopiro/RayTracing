#pragma once

#include "common.h"

#include "bitmap.h"
#include "camera.h"
#include "constant_medium.h"
#include "material.h"
#include "model.h"
#include "pdf.h"
#include "postprocess.h"
#include "ray.h"
#include "scene.h"
#include "sphere.h"
#include "transform.h"
#include "triangle.h"

#include <omp.h>

#define IMPORTANCE_SAMPLING 1

namespace spt
{

Color ComputeRayColor(const Scene& scene, const Ray& ray, int32 bounce_count);
Color PathTrace(const Scene& scene, Ray ray, int32 bounce_count);

} // namespace spt
