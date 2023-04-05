#pragma once

#include "common.h"

#include "bitmap.h"
#include "camera.h"
#include "constant_medium.h"
#include "dielectric.h"
#include "diffuse_light.h"
#include "hittable_pdf.h"
#include "image_texture.h"
#include "isotropic.h"
#include "lambertian.h"
#include "material.h"
#include "metal.h"
#include "microfacet.h"
#include "mixture_pdf.h"
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

#define SAMPLE_ALL_LIGHTS 0

namespace spt
{

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
extern void GGXVNDFSamplingTest(Scene&);

// Unidirectional path tracer
Color PathTrace(const Scene& scene, Ray ray, size_t bounce_count);

} // namespace spt
