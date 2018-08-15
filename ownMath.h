
#ifndef OWNMATH_H
#define OWNMATH_H
#include "devTools.h"
namespace SpaceH
{
    template<typename Dtype>
    struct epsilon
    {
        using value_type = typename SpaceH::get_value_type<Dtype>::type;
        constexpr static value_type value = std::numeric_limits<value_type>::epsilon();
    };
    
    template<typename Dtype>
    struct max_value
    {
        using value_type = typename SpaceH::get_value_type<Dtype>::type;
        constexpr static value_type value = std::numeric_limits<value_type>::max();
    };
    
    template<typename Dtype>
    struct big_value
    {
        using value_type = typename SpaceH::get_value_type<Dtype>::type;
        constexpr static value_type value = 0.1*std::numeric_limits<value_type>::max();
    };
    
    template<typename Fun>
    decltype(std::declval<Fun>()(0)) root_dichotom(Fun f)
    {
        using Scalar = decltype(f(0));
        Scalar up = SpaceH::big_value<Scalar>::value;
        Scalar low = -up;
        
        for( ;fabs((up-low)/up) > SpaceH::epsilon<Scalar>::value; )
        {
            Scalar mid = 0.5*(up + low);
            if(f(mid)>0)
                up = mid;
            else
                low = mid;
        }
        return 0.5*(up+low);
    }
    
    template<typename Dtype>
    class Random
    {
        Random() : gen(rd()), Dist(0,1) {}
    public:
        std::random_device rd;// non-deterministic generator
        std::mt19937 gen;
        std::uniform_real_distribution<Dtype> Dist;
        inline Dtype operator()(){ return Dist(gen);}
    };
    
    Random<double> uniform;
}
#endif
