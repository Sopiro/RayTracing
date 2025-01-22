#include "bulbit/materials.h"
#include "bulbit/scene.h"

using namespace bulbit;

inline const Material* CreateRandomUnrealMaterial(Scene& scene)
{
    // clang-format off
    Spectrum basecolor = Spectrum(Rand(0.0f, 1.0f), Rand(0.0f, 1.0f), Rand(0.0f, 1.0f)) * 0.7f;
    return CreateUnrealMaterial(scene, 
        basecolor,
        Rand() > 0.5f ? 1.0f : 0.0f,
        (Float)std::sqrt(Rand(0.1f, 1.0f)),
        (Float)std::sqrt(Rand(0.1f, 1.0f)),
        basecolor * (Rand() < 0.08f ? Rand(0.0f, 0.3f) : 0.0f)
    );
    // clang-format on
}
