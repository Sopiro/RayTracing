#pragma once

#include "pdf.h"

namespace spt
{

class MixturePDF : public PDF
{
public:
    MixturePDF(PDF* pdf1, PDF* pdf2);

    virtual Vec3 Generate() const override;
    virtual f64 Evaluate(const Vec3& wi) const override;

public:
    PDF* p1;
    PDF* p2;
};

inline MixturePDF::MixturePDF(PDF* pdf1, PDF* pdf2)
    : p1{ pdf1 }
    , p2{ pdf2 }
{
}

inline Vec3 MixturePDF::Generate() const
{
    if (Rand() > 0.5)
    {
        return p1->Generate();
    }
    else
    {
        return p2->Generate();
    }
}

inline f64 MixturePDF::Evaluate(const Vec3& wi) const
{
    // Mixing two pdfs
    return 0.5 * p1->Evaluate(wi) + 0.5 * p2->Evaluate(wi);
}

} // namespace spt
