
#ifndef CONSTITERATOR_H
#define CONSTITERATOR_H

#include "../dev-tools.h"
#include "ode-iterator.h"
namespace SpaceH::OdeIterator{

    template<typename Integrator>
    class ConstOdeIterator : public OdeIterator<ConstOdeIterator<Integrator>> {
    public:
        template <typename T>
        auto impl_iterate(ParticleSystem<T>& particles, typename T::Scalar macro_step_size) -> typename T::Scalar {
            integrator_.integrate(particles, macro_step_size);
            return macro_step_size;
        }
    private:
        Integrator integrator_;
    };
}
#endif