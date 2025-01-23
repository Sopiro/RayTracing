#include "texture_builder.h"

namespace bulbit
{

ConstantTexture<Float>* CreateFloatConstantTexture(Scene& scene, Float value)
{
    return scene.CreateTexture<ConstantTexture, Float>(value);
}

ConstantTexture<Spectrum>* CreateSpectrumConstantTexture(Scene& scene, const Spectrum& value)
{
    return scene.CreateTexture<ConstantTexture, Spectrum>(value);
}

ConstantTexture<Spectrum>* CreateSpectrumConstantTexture(Scene& scene, Float value)
{
    return scene.CreateTexture<ConstantTexture, Spectrum>(Spectrum{ value });
}

ConstantTexture<Spectrum>* CreateSpectrumConstantTexture(Scene& scene, Float r, Float g, Float b)
{
    return scene.CreateTexture<ConstantTexture, Spectrum>(Spectrum{ r, g, b });
}

ImageTexture<Float>* CreateFloatImageTexture(Scene& scene, std::string filename, int32 channel, bool non_color)
{
    return scene.CreateTexture<ImageTexture, Float>(ReadImage1(filename, channel, non_color));
}

ImageTexture<Spectrum>* CreateSpectrumImageTexture(Scene& scene, std::string filename, bool non_color)
{
    return scene.CreateTexture<ImageTexture, Spectrum>(ReadImage3(filename, non_color));
}

CheckerTexture<Float>* CreateFloatCheckerTexture(Scene& scene, Float a, Float b, const Point2& resolution)
{
    return scene.CreateTexture<CheckerTexture, Float>(
        CreateFloatConstantTexture(scene, a), CreateFloatConstantTexture(scene, b), resolution
    );
}

CheckerTexture<Float>* CreateFloatCheckerTexture(
    Scene& scene, const Texture<Float>* a, const Texture<Float>* b, const Point2& resolution
)
{
    return scene.CreateTexture<CheckerTexture, Float>(a, b, resolution);
}

CheckerTexture<Spectrum>* CreateSpectrumCheckerTexture(
    Scene& scene, const Spectrum& a, const Spectrum& b, const Point2& resolution
)
{
    return scene.CreateTexture<CheckerTexture, Spectrum>(
        CreateSpectrumConstantTexture(scene, a), CreateSpectrumConstantTexture(scene, b), resolution
    );
}

CheckerTexture<Spectrum>* CreateSpectrumCheckerTexture(
    Scene& scene, const Texture<Spectrum>* a, const Texture<Spectrum>* b, const Point2& resolution
)
{
    return scene.CreateTexture<CheckerTexture, Spectrum>(a, b, resolution);
}

} // namespace bulbit
