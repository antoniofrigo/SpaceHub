
#ifndef OWN_MATH_H
#define OWN_MATH_H

#include "dev-tools.hpp"
#include <math.h>
namespace space {
    /** @brief Self min()*/
    template<typename T1, typename T2>
    inline const T2 min(const T1 &x, const T2 &y) {
        return x > y ? y : x;
    }

    /** @brief Self max()*/
    template<typename T1, typename T2>
    inline const T2 max(const T1 &x, const T2 &y) {
        return y > x ? y : x;
    }

    /** @brief Self abs()*/
    template<typename T>
    inline const T abs(const T &x) {
        return x > 0 ? x : -x;
    }

    template <typename T>
    inline T in_range(T low, T x, T high) {
        T tmp = low > x ? low : x;
        return tmp > high ? high : tmp;
    }
    /**
     *
     * @tparam T
     * @param x
     * @return
     */
    template<typename T>
    inline constexpr T stepfunction(T x) {
        return static_cast<T>(x > 0);
    }

    /**
     *
     * @tparam T
     * @param x
     * @return
     */
    template<typename T>
    inline constexpr T sign(T x) {
        return -1 + 2 * static_cast<T>(x > 0);
    }

    /**
     *
     * @tparam Dtype
     */
    template<typename Dtype>
    struct epsilon {
        using value_type = typename space::get_value_type<Dtype>::type;
        constexpr static value_type value = std::numeric_limits<value_type>::epsilon();
    };

    template <typename T>
    constexpr T epsilon_v = epsilon<T>::value;

    template <typename T>
    inline bool iseq(T x, T y){
        return fabs(x-y) < epsilon_v<T>;
    }

    /**
     *
     * @tparam Dtype
     */
    template<typename Dtype>
    struct max_value {
        using value_type = typename space::get_value_type<Dtype>::type;
        constexpr static value_type value = std::numeric_limits<value_type>::max();
    };

    /**
     *
     * @tparam Dtype
     */
    template<typename Dtype>
    struct big_value {
        using value_type = typename space::get_value_type<Dtype>::type;
        constexpr static value_type value = 0.1 * std::numeric_limits<value_type>::max();
    };

    template<typename T>
    inline T KarmackFastInverseSquareRoot(T x) {
        return 1 / sqrt(x);
    }

    template<>
    inline float KarmackFastInverseSquareRoot<float>(float x) {
        float xhalf = 0.5f * x;
        int i = *(int *) &x;
        //i = 0x5f3759df - (i >> 1);
        i = 0x5f375a86 - (i >> 1);
        x = *(float *) &i;
        x = x * (1.5f - xhalf * x * x);
        //x = x*(1.5f - xhalf*x*x);
        return x;
    }

    template<>
    inline double KarmackFastInverseSquareRoot<double>(double x) {
        double xhalf = 0.5f * x;
        long long i = *(long long *) &x;
        i = 0x5fe6eb50c7aa19f9 - (i >> 1);
        x = *(double *) &i;
        x = x * (1.5f - xhalf * x * x);
        //x = x*(1.5f - xhalf*x*x);
        return x;
    }

    /**
     *
     * @tparam Fun
     * @param f
     * @return
     */
    template<typename Fun>
    decltype(std::declval<Fun>()(0)) root_dichotom(Fun f) {
        using Scalar = decltype(f(0));
        Scalar up = space::big_value<Scalar>::value;
        Scalar low = -up;

        for (; fabs((up - low) / up) > space::epsilon<Scalar>::value;) {
            Scalar mid = 0.5 * (up + low);
            if (f(mid) > 0)
                up = mid;
            else
                low = mid;
        }
        return 0.5 * (up + low);
    }
}
#endif