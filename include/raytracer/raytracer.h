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

Color ComputeRayColor(
    const Ray& ray, const Hittable& scene, std::shared_ptr<Hittable>& lights, const Color& sky_color, int32 depth);

Color PathTrace(Ray ray, const Hittable& scene, std::shared_ptr<Hittable>& lights, const Color& sky_color, int32 bounce_count);

} // namespace spt
