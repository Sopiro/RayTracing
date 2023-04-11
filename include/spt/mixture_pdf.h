#pragma once

#include "pdf.h"

namespace spt
{

class MixturePDF : public PDF
{
public:
    MixturePDF(PDF* pdf1, PDF* pdf2)
        : p1{ pdf1 }
        , p2{ pdf2 }
    {
    }

    virtual Vec3 Generate() const override
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

    virtual double Evaluate(const Vec3& direction) const override
    {
        // Mixing two pdfs
        return 0.5 * p1->Evaluate(direction) + 0.5 * p2->Evaluate(direction);
    }

public:
    PDF* p1;
    PDF* p2;
};

} // namespace spt
