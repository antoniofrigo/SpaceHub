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
 * @file interaction.hpp
 *
 * Header file.
 */
#pragma once

#include "../core-computation.hpp"
#include "../spacehub-concepts.hpp"
namespace hub::force {

    /*---------------------------------------------------------------------------*\
        Class Interactions Declaration
    \*---------------------------------------------------------------------------*/
    /**
     * @brief Interaction adapter. Put all interactions together.
     *
     * @tparam InternalForce Internal force type
     * @tparam ExtraForce External force types
     */
    template <CONCEPT_FORCE InternalForce, CONCEPT_FORCE... ExtraForce>
    class Interactions {
       public:
        /**
         * @brief Any external velocity dependent force?
         *
         */
        static constexpr bool ext_vel_dep{(... || ExtraForce::vel_dependent)};

        /**
         * @brief Any external velocity independent force?
         *
         */
        static constexpr bool ext_vel_indep{(... || !ExtraForce::vel_dependent)};

        /**
         * Evaluate the total acceleration of the current state of a given particle system.
         *
         * @tparam Particles Type of the particle system.
         *
         * @param[in] particles The particle system need to be evaluated.
         * @param[out] acceleration The output of the evaluated acceleration.
         */
        template <CONCEPT_PARTICLES_DATA Particles>
        static void eval_acc(Particles const &particles, typename Particles::VectorArray &acceleration);

        /**
         * Evaluate the external acceleration of the current state of a given particle system.
         *
         *
         * @tparam Particles Type of the particle system.
         * @param[in] particles The particle system need to be evaluated.
         * @param[out] acceleration The output of the evaluated acceleration.
         */
        template <CONCEPT_PARTICLES_DATA Particles>
        static void eval_extra_acc(Particles const &particles, typename Particles::VectorArray &acceleration);

        /**
         * Evaluate the external velocity dependent acceleration of the current state of a given particle system.
         *
         * @tparam Particles Type of the particle system.
         *
         * @param[in] particles The particle system need to be evaluated.
         * @param[out] acceleration The output of the evaluated acceleration.
         */
        template <CONCEPT_PARTICLES_DATA Particles>
        static void eval_extra_vel_dep_acc(Particles const &particles, typename Particles::VectorArray &acceleration);

        /**
         * Evaluate the external velocity independent acceleration of the current state of a given particle system.
         *
         * @tparam Particles Type of the particle system.
         *
         * @param[in] particles The particle system need to be evaluated.
         * @param[out] acceleration The output of the evaluated acceleration.
         */
        template <CONCEPT_PARTICLES_DATA Particles>
        static void eval_extra_vel_indep_acc(Particles const &particles, typename Particles::VectorArray &acceleration);

        /**
         * Evaluate the internal newtonian acceleration of the current state of a given particle system.
         *
         * @tparam Particles Type of the particle system.
         *
         * @param[in] particles The particle system need to be evaluated.
         * @param[out] acceleration The output of the evaluated acceleration.
         */
        template <CONCEPT_PARTICLES_DATA Particles>
        static void eval_newtonian_acc(Particles const &particles, typename Particles::VectorArray &acceleration);
    };

    /**
     * @brief Acceleration data set.
     *
     * @tparam Interactions Interaction types that provide constexpr member 'ext_vel_indep' and 'ext_vel_dep' to
     * determine the data layout
     * @tparam VectorArray 3D Vector array type
     */
    template <typename Interactions, typename VectorArray>
    class InteractionData {
       public:
        // Constructors
        SPACEHUB_MAKE_CONSTRUCTORS(InteractionData, default, default, default, default, default);

        /**
         * @brief Construct a new Interaction Data object
         *
         * @param[in] size Number of Particles.
         */
        explicit InteractionData(size_t size);

        // Public methods
        /**
         * @brief 3D vector array to store the total acceleration.
         *
         */
        SPACEHUB_ARRAY_ACCESSOR(VectorArray, acc, acc_);

        /**
         * @brief 3D vector array to store the Newtonian acceleration.
         *
         */
        SPACEHUB_ARRAY_ACCESSOR(VectorArray, newtonian_acc, newtonian_acc_);

        /**
         * @brief 3D vector array to store the total velocity independent acceleration.
         *
         */
        SPACEHUB_ARRAY_ACCESSOR(VectorArray, tot_vel_indep_acc, tot_vel_indep_acc_);

        /**
         * @brief 3D vector array to store the velocity independent acceleration.(if no external velocity independent
         * appears, this becomes empty type).
         *
         */
        SPACEHUB_ARRAY_ACCESSOR(VectorArray, ext_vel_indep_acc, ext_vel_indep_acc_);

        /**
         * @brief 3D vector array to store the velocity dependent acceleration.(if no external velocity dependent
         * appears, this becomes empty type).
         *
         */
        SPACEHUB_ARRAY_ACCESSOR(VectorArray, ext_vel_dep_acc, ext_vel_dep_acc_);

       private:
        VectorArray acc_{0};

        VectorArray newtonian_acc_{0};

        VectorArray tot_vel_indep_acc_{0};

        std::conditional_t<Interactions::ext_vel_indep, VectorArray, Empty> ext_vel_indep_acc_;

        std::conditional_t<Interactions::ext_vel_dep, VectorArray, Empty> ext_vel_dep_acc_;
    };
    template <typename Arg, typename... Args>
    struct InvokeVelDepForce {
        template <CONCEPT_PARTICLES_DATA Particles>
        static void add_acc_to(Particles const &particles, typename Particles::VectorArray &acceleration) {
            if constexpr (Arg::vel_dependent) {
                Arg::add_acc_to(particles, acceleration);
            }
            if constexpr (sizeof...(Args) > 0) {
                InvokeVelDepForce<Args...>::add_acc_to(particles, acceleration);
            }
        }
    };

    template <typename Arg, typename... Args>
    struct InvokeVelIndepForce {
        template <CONCEPT_PARTICLES_DATA Particles>
        static void add_acc_to(Particles const &particles, typename Particles::VectorArray &acceleration) {
            if constexpr (!Arg::vel_dependent) {
                Arg::add_acc_to(particles, acceleration);
            }
            if constexpr (sizeof...(Args) > 0) {
                InvokeVelIndepForce<Args...>::add_acc_to(particles, acceleration);
            }
        }
    };
    /*---------------------------------------------------------------------------*\
        Class Interactions Implementation
    \*---------------------------------------------------------------------------*/

    template <CONCEPT_FORCE InternalForce, CONCEPT_FORCE... ExtraForce>
    template <CONCEPT_PARTICLES_DATA Particles>
    void Interactions<InternalForce, ExtraForce...>::eval_acc(const Particles &particles,
                                                              typename Particles::VectorArray &acceleration) {
        calc::array_set_zero(acceleration);
        InternalForce::add_acc_to(particles, acceleration);
        (ExtraForce::add_acc_to(particles, acceleration), ...);
    }

    template <CONCEPT_FORCE InternalForce, CONCEPT_FORCE... ExtraForce>
    template <CONCEPT_PARTICLES_DATA Particles>
    void Interactions<InternalForce, ExtraForce...>::eval_extra_acc(const Particles &particles,
                                                                    typename Particles::VectorArray &acceleration) {
        if constexpr (ext_vel_dep || ext_vel_indep) {
            calc::array_set_zero(acceleration);
        }
        if constexpr (ext_vel_dep) {
            InvokeVelDepForce<ExtraForce...>::add_acc_to(particles, acceleration);
        }
        if constexpr (ext_vel_indep) {
            InvokeVelIndepForce<ExtraForce...>::add_acc_to(particles, acceleration);
        }
    }

    template <CONCEPT_FORCE InternalForce, CONCEPT_FORCE... ExtraForce>
    template <CONCEPT_PARTICLES_DATA Particles>
    void Interactions<InternalForce, ExtraForce...>::eval_extra_vel_dep_acc(
        const Particles &particles, typename Particles::VectorArray &acceleration) {
        if constexpr (ext_vel_dep) {
            calc::array_set_zero(acceleration);
            InvokeVelDepForce<ExtraForce...>::add_acc_to(particles, acceleration);
        }
    }

    template <CONCEPT_FORCE InternalForce, CONCEPT_FORCE... ExtraForce>
    template <CONCEPT_PARTICLES_DATA Particles>
    void Interactions<InternalForce, ExtraForce...>::eval_extra_vel_indep_acc(
        const Particles &particles, typename Particles::VectorArray &acceleration) {
        if constexpr (ext_vel_indep) {
            calc::array_set_zero(acceleration);
            InvokeVelIndepForce<ExtraForce...>::add_acc_to(particles, acceleration);
        }
    }

    template <CONCEPT_FORCE InternalForce, CONCEPT_FORCE... ExtraForce>
    template <CONCEPT_PARTICLES_DATA Particles>
    void Interactions<InternalForce, ExtraForce...>::eval_newtonian_acc(const Particles &particles,
                                                                        typename Particles::VectorArray &acceleration) {
        calc::array_set_zero(acceleration);
        InternalForce::add_acc_to(particles, acceleration);
    }

    /*---------------------------------------------------------------------------*\
            Class InteractionData Implementation
    \*---------------------------------------------------------------------------*/
    template <typename Interactions, typename VectorArray>
    InteractionData<Interactions, VectorArray>::InteractionData(size_t size)
        : acc_(size), newtonian_acc_(size), tot_vel_indep_acc_(size) {
        if constexpr (Interactions::ext_vel_indep) {
            ext_vel_indep_acc_.resize(size);
        }
        if constexpr (Interactions::ext_vel_dep) {
            ext_vel_dep_acc_.resize(size);
        }
    }
}  // namespace hub::force
