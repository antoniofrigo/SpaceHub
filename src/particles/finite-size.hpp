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
 * @file finite-size.hpp
 *
 * Header file.
 */
#pragma once

#include "../IO.hpp"
#include "point-particles.hpp"

namespace hub::particles {

    template <typename Vec3>
    struct SizeParticle : public PointParticle<Vec3> {
       public:
        using Base = PointParticle<Vec3>;
        // Type members
        /**
         * @brief Floating point like type.
         */
        using Scalar = typename Vec3::value_type;

        /**
         * @brief 3D vector.
         */
        using Vector = Vec3;

        // Constructors
        SPACEHUB_MAKE_CONSTRUCTORS(SizeParticle, default, default, default, default, default);

        /**
         * @brief Construct a new Particle object
         *
         * @param[in] m Mass of the particle.
         * @param[in] r Radius of the particle.
         * @param[in] p Position of the particle.
         * @param[in] v Velocity of the particle.
         */
        SizeParticle(Scalar m, Scalar r, Vector p, Vector v) : PointParticle<Vector>{m, p, v}, radius{r} {}

        /**
         * @brief Construct a new Particle object
         *
         * @param[in] m Mass of the particle.
         * @param[in] r Radius of the particle.
         * @param[in] px X component of the position.
         * @param[in] py Y component of the position.
         * @param[in] pz Z component of the position.
         * @param[in] vx X component of the velocity.
         * @param[in] vy Y component of the velocity.
         * @param[in] vz Z component of the velocity.
         */
        SizeParticle(Scalar m, Scalar r, Scalar px = 0, Scalar py = 0, Scalar pz = 0, Scalar vx = 0, Scalar vy = 0,
                     Scalar vz = 0)
                : PointParticle<Vector>{m, px, py, pz, vx, vy, vz}, radius{r} {}

        friend std::ostream &operator<<(std::ostream &os, SizeParticle const &particle) {
            hub::print_csv(os, particle.mass, particle.radius, particle.pos, particle.vel);
            return os;
        }

        friend std::istream &operator>>(std::istream &is, SizeParticle &particle) {
            hub::input(is, particle.mass, particle.radius, particle.pos, particle.vel);
            return is;
        }

        // Public members
        /**
         * @brief Radius of the particle.
         */
        Scalar radius;
    };

    /*---------------------------------------------------------------------------*\
        Class SizeParticles Declaration
    \*---------------------------------------------------------------------------*/
    /**
     * @brief Finite size particle.
     *
     * @tparam TypeSystem The type system in spaceHub(hub::Types).
     */
    template <typename TypeSystem>
    class SizeParticles {
       public:
        // Type members
        SPACEHUB_USING_TYPE_SYSTEM_OF(TypeSystem);

        using Particle = SizeParticle<Vector>;
        /*---------------------------------------------------------------------------*\
        Sub-Class Particle Declaration and Implementation
        \*---------------------------------------------------------------------------*/
        /**
         * @brief Embedded Particle of SOA particles `SizeParticles`.
         *
         */

        // Constructors
        SPACEHUB_MAKE_CONSTRUCTORS(SizeParticles, default, default, default, default, default);

        /**
         * @brief Construct a new Size Particles object from std::ranges(Container).
         *
         * @tparam STL std::ranges(Container)
         * @param[in] t Initial time of the the particle group.
         * @param[in] particle_set Input particle set.
         */
        template <CONCEPT_PARTICLE_CONTAINER STL>
        SizeParticles(Scalar time, STL const &particles_set);

        // Public methods
        SPACEHUB_ARRAY_ACCESSOR(ScalarArray, radius, radius_);

        SPACEHUB_ARRAY_ACCESSOR(ScalarArray, mass, mass_);

        SPACEHUB_ARRAY_ACCESSOR(IdxArray, idn, idn_);

        SPACEHUB_STD_ACCESSOR(StateScalar, time, time_);

        SPACEHUB_ARRAY_ACCESSOR(StateVectorArray, pos, pos_);

        SPACEHUB_ARRAY_ACCESSOR(StateVectorArray, vel, vel_);

        void resize(size_t new_sz);

        void reserve(size_t new_cap);

        void emplace_back(Particle const &new_particle);

           size_t number() const;

           size_t capacity() const;

        void clear();

          std::string column_names() const;

        template <typename U>
        friend std::ostream &operator<<(std::ostream &os, SizeParticles<U> const &ps);

        template <typename U>
        friend std::istream &operator>>(std::istream &is, SizeParticles<U> &ps);

       private:
        // Private members
        StateVectorArray pos_;

        StateVectorArray vel_;

        ScalarArray mass_;

        ScalarArray radius_;

        IdxArray idn_;

        StateScalar time_{0};

        size_t active_num_{0};
    };
}  // namespace hub::particles

namespace hub::particles {
    /*---------------------------------------------------------------------------*\
        Class SizeParticles Implementation
    \*---------------------------------------------------------------------------*/
    template <typename TypeSystem>
    template <CONCEPT_PARTICLE_CONTAINER STL>
    SizeParticles<TypeSystem>::SizeParticles(Scalar time, const STL &particles_set) {
        size_t input_num = particles_set.size();
        this->reserve(input_num);
        size_t id = 0;
        for (auto &p : particles_set) {
            pos_.emplace_back(p.pos);
            vel_.emplace_back(p.vel);
            mass_.emplace_back(p.mass);
            radius_.emplace_back(p.radius);
            idn_.emplace_back(id++);
        }
        time_ = time;
        active_num_ = input_num;
    }

    template <typename TypeSystem>
    size_t SizeParticles<TypeSystem>::number() const {
        return active_num_;
    }

    template <typename TypeSystem>
    size_t SizeParticles<TypeSystem>::capacity() const {
        return idn_.capacity();
    }

    template <typename TypeSystem>
    void SizeParticles<TypeSystem>::reserve(size_t new_cap) {
        hub::reserve_all(new_cap, pos_, vel_, mass_, radius_, idn_);
    }

    template <typename TypeSystem>
    void SizeParticles<TypeSystem>::clear() {
        hub::clear_all(pos_, vel_, mass_, radius_, idn_);
        active_num_ = 0;
    }

    template <typename TypeSystem>
    void SizeParticles<TypeSystem>::resize(size_t new_sz) {
        hub::resize_all(new_sz, pos_, vel_, mass_, radius_, idn_);
        active_num_ = new_sz;
    }

    template <typename TypeSystem>
    void SizeParticles<TypeSystem>::emplace_back(typename SizeParticles<TypeSystem>::Particle const &new_particle) {
        pos_.emplace_back(new_particle.pos);
        vel_.emplace_back(new_particle.vel);
        mass_.emplace_back(new_particle.mass);
        radius_.emplace_back(new_particle.radius);
        idn_.emplace_back(this->number());
        active_num_++;
    }

    template <typename TypeSystem>
    std::string SizeParticles<TypeSystem>::column_names() const {
        return "time,id,mass,radius,px,py,pz,vx,vy,vz";
    }

    template <typename TypeSystem>
    std::ostream &operator<<(std::ostream &os, SizeParticles<TypeSystem> const &ps) {
        size_t num = ps.number();
        for (size_t i = 0; i < num; ++i) {
            hub::print(os, ps.time(), ',', ps.idn(i), ',', ps.mass(i), ',', ps.radius(i), ',', ps.pos(i), ',',
                       ps.vel(i), '\n');
        }
        return os;
    }
}  // namespace hub::particles
