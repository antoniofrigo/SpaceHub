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
 * @file type-class.hpp
 *
 * Header file.
 */
#pragma once
#include <vector>

#include "dev-tools.hpp"
#include "small-vector.hpp"
#include "vector/lazy-vec3.h"
#include "vector/vector3.hpp"

namespace hub {
    template <typename T>
    using SSO_vector = llvm::SmallVector<T, 16>;

    template <typename T>
    using SSO_vec_vector = llvm::SmallVector<T, 5>;
    /**
      Type system that is used across the Space Hub system. This type class provide all basic type i.e `Scalar`,
      `Vector`, `ScalarArray`, `Coord` and etc,.

      @tparam Real The basic scalar type, i.e `float`, `double`, etc.
     */
    template <typename Real, template <typename> typename V = Vec3>
    struct Types {
       public:
        /**
         * @brief
         *
         */
        using TypeSet = Types<Real, V>;

        template <typename T>
        using Container = SSO_vector<T>;

        /**
         * Floating point like type cross the system
         */
        using Scalar = typename raw_type<Real>::type;

        using StateScalar = Real;

        /**
         * 1-d array with value type `Scalar`. Alias of `Container<Scalar>`.
         */
        using ScalarArray = Container<Scalar>;

        using StateScalarArray = Container<StateScalar>;

        /**
         * 1-d array with value type `int`. Alias of `Container<int>`.
         */
        using IntArray = Container<int>;

        /**
         * 1-d array with value type `size_t`. Alias of `Container<size_t>`.
         */
        using IdxArray = Container<size_t>;

        /**
         * 3-d math vector (x, y, z) with `Scalar` type of x, y, z. Use genetic `Vec3` by default, but can be replaced
         * with any other implementation implements interfaces defined in `Vec3`.
         */
        using Vector = V<Scalar>;

        using StateVector = V<StateScalar>;

        /**
         * 1-d array with value type `Vector`, Alias of `Container<Vector>`.
         */
        using VectorArray = SSO_vec_vector<Vector>;

        using StateVectorArray = SSO_vec_vector<StateVector>;
    };

    template <typename Iter, typename VectorArray>
    void load_to_coords(Iter begin, Iter end, VectorArray& var) {
        size_t len = (end - begin) / 3;
        var.resize(len);
        auto iter = begin;
        for (auto& v : var) {
            v.x = *iter++;
            v.y = *iter++;
            v.z = *iter++;
        }
    }

    template <typename Iter, typename VectorArray>
    void advance_coords_to(Iter begin, VectorArray const& var) {
        auto iter = begin;
        for (auto& v : var) {
            *(iter++) += v.x;
            *(iter++) += v.y;
            *(iter++) += v.z;
        }
    }

    template <typename Iter, typename VectorArray, typename Scalar>
    void advance_scaled_coords_to(Iter begin, VectorArray const& var, Scalar scale) {
        auto iter = begin;
        for (auto& v : var) {
            *(iter++) += v.x * scale;
            *(iter++) += v.y * scale;
            *(iter++) += v.z * scale;
        }
    }

    template <typename ScalarArray, typename VectorArray>
    void add_coords_to(ScalarArray& stl, VectorArray const& var) {
        stl.reserve(var.size() * 3 + stl.size());

        for (auto const& v : var) {
            stl.emplace_back(v.x);
            stl.emplace_back(v.y);
            stl.emplace_back(v.z);
        }
    }

    template <typename ScalarArray, typename VectorArray, typename Scalar>
    void add_scaled_coords_to(ScalarArray& stl, VectorArray const& var, Scalar scale) {
        stl.reserve(var.size() * 3 + stl.size());
        for (auto const& v : var) {
            stl.emplace_back(v.x * scale);
            stl.emplace_back(v.y * scale);
            stl.emplace_back(v.z * scale);
        }
    }

}  // namespace hub
