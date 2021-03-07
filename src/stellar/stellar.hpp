//
// Created by yihan on 5/8/19.
//

#pragma once
/*---------------------------------------------------------------------------*\
        Class stellar Declaration
\*---------------------------------------------------------------------------*/
namespace hub::stellar {

    enum class StarType { BH, NS, STAR, WD };

    template <typename Scalar>
    auto stellar_radius(StarType T, Scalar mass) {
        if (T == StarType::BH) {
            return 2 * consts::G * mass / consts::C / consts::C;
        } else if (T == StarType::NS) {
            return 10 * unit::km;
        } else if (T == StarType::STAR) {
            return unit::Rs * pow(mass / unit::Ms, 0.75);
        } else if (T == StarType::WD) {
            return 0.01 * unit::Rs * pow(unit::Ms / mass, 1.0 / 3);
        }
    }
}  // namespace hub::stellar

/*---------------------------------------------------------------------------*\
        Class stellar Implementation
\*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*\
        Help function
\*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*\
        Help macros
\*---------------------------------------------------------------------------*/
