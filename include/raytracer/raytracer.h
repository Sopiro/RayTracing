#pragma once

#include "common.h"

#include "bitmap.h"
#include "camera.h"
#include "constant_medium.h"
#include "dielectric.h"
#include "diffuse_light.h"
#include "image_texture.h"
#include "isotropic.h"
#include "lambertian.h"
#include "material.h"
#include "metal.h"
#include "model.h"
#include "pdf.h"
#include "postprocess.h"
#include "ray.h"
#include "scene.h"
#include "solid_color.h"
#include "sphere.h"
#include "transform.h"
#include "triangle.h"
#include "util.h"

#include <omp.h>

namespace spt
{

constexpr bool importance_sampling = true;

// Test scenes
extern void RandomScene(Scene&);
extern void BVHTest(Scene&);
extern void CornellBox(Scene&);
extern void Sponza(Scene&);
extern void NormalMapping(Scene&);
extern void PBRTest(Scene&);
extern void EnvironmentMap(Scene&);
extern void BRDFSamplingTest(Scene&);
extern void MISTest1(Scene&);
extern void MISTest2(Scene&);
extern void MISTestWak(Scene&);

Color ComputeRayColor(const Scene& scene, const Ray& ray, int32 bounce_count);
Color PathTrace(const Scene& scene, Ray ray, int32 bounce_count);
Color PathTrace2(const Scene& scene, Ray ray, int32 bounce_count);

} // namespace spt
