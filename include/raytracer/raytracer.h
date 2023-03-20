#pragma once

#define STBI_MSC_SECURE_CRT
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "common.h"

#include "bitmap.h"
#include "camera.h"
#include "constant_medium.h"
#include "material.h"
#include "model.h"
#include "pdf.h"
#include "ray.h"
#include "scene.h"
#include "sphere.h"
#include "transform.h"
#include "triangle.h"

#include <omp.h>

#define IMPORTANCE_SAMPLING