/*---------------------------------------------------------------------------*\
        .-''''-.         |
       /        \        |
      /_        _\       |  SpaceHub: The Open Source N-body Toolkit
     // \  <>  / \\      |
     |\__\    /__/|      |  Website:  https://yihanwangastro.github.io/SpaceHub/
      \    ||    /       |
        \  __  /         |  Copyright (C) 2019 Yihan Wang
         '.__.'          |
---------------------------------------------------------------------
License
    This file is part of SpaceHub.
    SpaceHub is free software: you can redistribute it and/or modify it under
    the terms of the GPL-3.0 License. SpaceHub is distributed in the hope that it
    will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
    of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GPL-3.0 License
    for more details. You should have received a copy of the GPL-3.0 License along
    with SpaceHub.
\*---------------------------------------------------------------------------*/
/**
 * @file max-ratio-error.hpp
 *
 * Header file.
 */
#pragma once

namespace hub::ode {
    /*---------------------------------------------------------------------------*\
         Class MaxRatioError Declaration
    \*---------------------------------------------------------------------------*/
    /**
     * @brief Error estimator using max diff/ max scale
     *
     * @tparam TypeSystem Type class of SpaceHub
     */
    template <typename TypeSystem>
    class MaxRatioError {
       public:
        // Type member
        SPACEHUB_USING_TYPE_SYSTEM_OF(TypeSystem);

        // Constructors
        SPACEHUB_MAKE_CONSTRUCTORS(MaxRatioError, default, default, default, default, default);

        /**
         * @brief Construct a new Max Ratio Error object
         *
         * @param atol Absolute error tolerance
         * @param rtol Relative error tolerance
         */
        MaxRatioError(Scalar atol, Scalar rtol) : atol_{atol}, rtol_{rtol} {}

        /**
         * @brief Absolute error tolerance
         *
         */
        SPACEHUB_READ_ACCESSOR(Scalar, atol, atol_);

        /**
         * @brief Relative error tolerance
         *
         */
        SPACEHUB_READ_ACCESSOR(Scalar, rtol, rtol_);

        /**
         * @brief Set the absolute error tolerance
         *
         */
        void set_atol(Scalar);

        /**
         * @brief Set the relative error tolerance
         *
         */
        void set_rtol(Scalar);

        /**
         * @brief Estimate the error
         *
         * @tparam Array1 Scalar array type.
         * @tparam Array2 Scalar array type.
         * @param scale Scale used for normalization. The error ~ |diff|/|scale|.
         * @param diff Difference between two results that need convergence check.
         * @return Scalar
         */
        template <typename Array1, typename Array2>
        Scalar error(Array1 const &scale, Array2 const &diff);

        /**
         * @brief Estimate the error
         *
         * @tparam Array1 Scalar array type
         * @tparam Array2 Scalar array type
         * @tparam Array3 Scalar array type
         * @param scale Scale used for normalization. The error ~ |y1-y1`|/|scale|.
         * @param y1 One of the results needs convergence check.
         * @param y1_prime The other results needs convergence check.
         * @return Scalar
         */
        template <typename Array1, typename Array2, typename Array3>
        Scalar error(Array1 const &scale, Array2 const &y1, Array3 const &y1_prime);

       private:
        Scalar atol_{0};

        Scalar rtol_{1e-12};
    };
    /*---------------------------------------------------------------------------*\
         Class MaxRatioError Implementation
    \*---------------------------------------------------------------------------*/
    template <typename TypeSystem>
    void MaxRatioError<TypeSystem>::set_atol(Scalar error) {
        atol_ = error;
    }

    template <typename TypeSystem>
    void MaxRatioError<TypeSystem>::set_rtol(Scalar error) {
        rtol_ = error;
    }

    template <typename TypeSystem>
    template <typename Array1, typename Array2>
    auto MaxRatioError<TypeSystem>::error(const Array1 &scale, const Array2 &diff) -> Scalar {
        size_t const size = scale.size();
        Scalar max_diff = 0;
        Scalar max_scale = 0;
        if constexpr (std::is_same_v<raw_type_t<typename Array1::value_type>, raw_type_t<Scalar>>) {
            for (size_t i = 0; i < size; ++i) {
                max_diff = std::max(max_diff, static_cast<Scalar>(fabs(diff[i])));
                max_scale = std::max(max_scale, static_cast<Scalar>(atol_ + fabs(scale[i]) * rtol_));
            }
        } else if constexpr (std::is_same_v<typename Array1::value_type, Vec3<Scalar>>) {
            for (size_t i = 0; i < size; ++i) {
                max_diff = std::max(max_diff, static_cast<Scalar>(max_abs(diff[i])));
                max_scale = std::max(max_scale, static_cast<Scalar>(atol_ + max_abs(scale[i]) * rtol_));
            }
        } else {
            spacehub_abort("Unsupported array type!");
        }
        return max_diff / max_scale;
    }

    template <typename TypeSystem>
    template <typename Array1, typename Array2, typename Array3>
    auto MaxRatioError<TypeSystem>::error(const Array1 &scale, const Array2 &y1, const Array3 &y1_prime) -> Scalar {
        size_t const size = scale.size();
        Scalar max_diff = 0;
        Scalar max_scale = 0;
        if constexpr (std::is_same_v<typename Array1::value_type, Scalar>) {
            for (size_t i = 0; i < size; ++i) {
                max_diff = std::max(max_diff, static_cast<Scalar>(fabs(y1_prime[i] - y1[i])));
                max_scale = std::max(max_scale, static_cast<Scalar>(atol_ + fabs(scale[i]) * rtol_));
            }
        } else if constexpr (std::is_same_v<typename Array1::value_type, Vec3<Scalar>>) {
            for (size_t i = 0; i < size; ++i) {
                max_diff = std::max(max_diff, static_cast<Scalar>(max_abs(y1_prime[i] - y1[i])));
                max_scale = std::max(max_scale, static_cast<Scalar>(atol_ + max_abs(scale[i]) * rtol_));
            }
        } else {
            spacehub_abort("Unsupported array type!");
        }
        return max_diff / max_scale;
    }
}  // namespace hub::ode
