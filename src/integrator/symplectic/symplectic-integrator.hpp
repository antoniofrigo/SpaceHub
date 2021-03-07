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
 * @file symplectic-integrator.hpp
 *
 * Header file.
 */
#pragma once

#include "../../spacehub-concepts.hpp"
/**
 * @namespace hub::integrator
 * Documentation for hub
 */
namespace hub::integrator {

    /*---------------------------------------------------------------------------*\
    Class Symplectic2nd Declaration
\*---------------------------------------------------------------------------*/
    /**
     * @brief Second symplectic method
     *
     * @tparam TypeSystem Type class of SpaceHub
     */
    template <typename TypeSystem>
    class Symplectic2nd {
       public:
        SPACEHUB_USING_TYPE_SYSTEM_OF(TypeSystem);

        /**
         * @brief Order of the integrator
         *
         */
        static constexpr size_t order{2};

        /**
         * @brief Integrate the particle system with given step_size
         *
         * @tparam ParticleSys Any types satisfy the particle system concept.
         * @param[in,out] system Particle system needs to be integrated.
         * @param[in] step_size Single step step size.
         */
        template <CONCEPT_PARTICLE_SYSTEM ParticleSys>
        void integrate(ParticleSys &system, typename ParticleSys::Scalar step_size);
    };

    /**
     * @brief Drift-Kick-Drift Leapfrog method.
     *
     * @tparam TypeSystem Type class of SpaceHub.
     */
    template <typename TypeSystem>
    using LeapFrogDKD = Symplectic2nd<TypeSystem>;

    template <typename TypeSystem>
    class LeapFrogKDK {
       public:
        SPACEHUB_USING_TYPE_SYSTEM_OF(TypeSystem);
        /**
         * Order of the integrator.
         */
        static constexpr size_t order{2};

        /**
         * @brief Integrate the particle system with given step_size
         *
         * @tparam ParticleSys Any types satisfy the particle system concept.
         * @param[in,out] system Particle system needs to be integrated.
         * @param[in] step_size Single step step size.
         */
        template <CONCEPT_PARTICLE_SYSTEM ParticleSys>
        void integrate(ParticleSys &system, typename ParticleSys::Scalar step_size);
    };
    /*---------------------------------------------------------------------------*\
        Class Symplectic4th Declaration
    \*---------------------------------------------------------------------------*/
    /**
     * Fourth order symplectic method.
     */
    template <typename TypeSystem>
    class Symplectic4th {
       public:
        SPACEHUB_USING_TYPE_SYSTEM_OF(TypeSystem);
        /**
         * Order of the integrator.
         */
        static constexpr size_t order{4};

        /**
         * @brief Integrate the particle system with given step_size
         *
         * @tparam ParticleSys Any types satisfy the particle system concept.
         * @param[in,out] system Particle system needs to be integrated.
         * @param[in] step_size Single step step size.
         */
        template <CONCEPT_PARTICLE_SYSTEM ParticleSys>
        void integrate(ParticleSys &system, typename ParticleSys::Scalar step_size);
    };

    /*---------------------------------------------------------------------------*\
        Class Symplectic6th Declaration
    \*---------------------------------------------------------------------------*/
    /**
     * Sixth order symplectic method.
     */
    template <typename TypeSystem>
    class Symplectic6th {
       public:
        SPACEHUB_USING_TYPE_SYSTEM_OF(TypeSystem);
        /**
         * Order of the integrator.
         */
        static constexpr size_t order{6};

        /**
         * @brief Integrate the particle system with given step_size
         *
         * @tparam ParticleSys Any types satisfy the particle system concept.
         * @param[in,out] system Particle system needs to be integrated.
         * @param[in] step_size Single step step size.
         */
        template <CONCEPT_PARTICLE_SYSTEM ParticleSys>
        void integrate(ParticleSys &system, typename ParticleSys::Scalar step_size);
    };

    /*---------------------------------------------------------------------------*\
        Class Symplectic8th Declaration
    \*---------------------------------------------------------------------------*/
    /**
     * Eighth order symplectic method.
     */
    template <typename TypeSystem>
    class Symplectic8th {
       public:
        SPACEHUB_USING_TYPE_SYSTEM_OF(TypeSystem);
        /**
         * Order of the integrator.
         */
        static constexpr size_t order{8};

        /**
         * @brief Integrate the particle system with given step_size
         *
         * @tparam ParticleSys Any types satisfy the particle system concept.
         * @param[in,out] system Particle system needs to be integrated.
         * @param[in] step_size Single step step size.
         */
        template <CONCEPT_PARTICLE_SYSTEM ParticleSys>
        void integrate(ParticleSys &system, typename ParticleSys::Scalar step_size);
    };

    /*---------------------------------------------------------------------------*\
        Class Symplectic10th Declaration
    \*---------------------------------------------------------------------------*/
    /**
     * Tenth order symplectic method.
     */
    template <typename TypeSystem>
    class Symplectic10th {
       public:
        SPACEHUB_USING_TYPE_SYSTEM_OF(TypeSystem);
        /**
         * Order of the integrator.
         */
        static constexpr size_t order{10};

        /**
         * @brief Integrate the particle system with given step_size
         *
         * @tparam ParticleSys Any types satisfy the particle system concept.
         * @param[in,out] system Particle system needs to be integrated.
         * @param[in] step_size Single step step size.
         */
        template <CONCEPT_PARTICLE_SYSTEM ParticleSys>
        void integrate(ParticleSys &system, typename ParticleSys::Scalar step_size);
    };

    /*---------------------------------------------------------------------------*\
         Class Symplectic2nd Implementation
    \*---------------------------------------------------------------------------*/
    template <typename TypeSystem>
    template <CONCEPT_PARTICLE_SYSTEM ParticleSys>
    void Symplectic2nd<TypeSystem>::integrate(ParticleSys &system, typename ParticleSys::Scalar step_size) {
        system.drift(0.5 * step_size);
        system.kick(step_size);
        system.drift(0.5 * step_size);
    }

    template <typename TypeSystem>
    template <CONCEPT_PARTICLE_SYSTEM ParticleSys>
    void LeapFrogKDK<TypeSystem>::integrate(ParticleSys &system, typename ParticleSys::Scalar step_size) {
        system.kick(0.5 * step_size);
        system.drift(step_size);
        system.kick(0.5 * step_size);
    }

    /*---------------------------------------------------------------------------*\
         Class Symplectic4th Implementation
    \*---------------------------------------------------------------------------*/
    template <typename TypeSystem>
    template <CONCEPT_PARTICLE_SYSTEM ParticleSys>
    void Symplectic4th<TypeSystem>::integrate(ParticleSys &system, typename ParticleSys::Scalar step_size) {
        system.drift(6.7560359597983000E-1 * step_size);
        system.kick(1.3512071919596600E0 * step_size);
        system.drift(-1.7560359597983000E-1 * step_size);
        system.kick(-1.7024143839193200E0 * step_size);
        system.drift(-1.7560359597983000E-1 * step_size);
        system.kick(1.3512071919596600E0 * step_size);
        system.drift(6.7560359597983000E-1 * step_size);
    }

    /*---------------------------------------------------------------------------*\
         Class Symplectic6th Implementation
    \*---------------------------------------------------------------------------*/
    template <typename TypeSystem>
    template <CONCEPT_PARTICLE_SYSTEM ParticleSys>
    void Symplectic6th<TypeSystem>::integrate(ParticleSys &system, typename ParticleSys::Scalar step_size) {
        /*unroll loop manually*/
        system.drift(3.9225680523877998E-1 * step_size);
        system.kick(7.8451361047755996E-1 * step_size);
        system.drift(5.1004341191845848E-1 * step_size);
        system.kick(2.3557321335935699E-1 * step_size);
        system.drift(-4.7105338540975655E-1 * step_size);
        system.kick(-1.1776799841788701E0 * step_size);
        system.drift(6.8753168252518093E-2 * step_size);
        system.kick(1.3151863206839063E0 * step_size);
        system.drift(6.8753168252518093E-2 * step_size);
        system.kick(-1.1776799841788701E0 * step_size);
        system.drift(-4.7105338540975655E-1 * step_size);
        system.kick(2.3557321335935699E-1 * step_size);
        system.drift(5.1004341191845848E-1 * step_size);
        system.kick(7.8451361047755996E-1 * step_size);
        system.drift(3.9225680523877998E-1 * step_size);
    }

    /*---------------------------------------------------------------------------*\
         Class Symplectic8th Implementation
    \*---------------------------------------------------------------------------*/
    template <typename TypeSystem>
    template <CONCEPT_PARTICLE_SYSTEM ParticleSys>
    void Symplectic8th<TypeSystem>::integrate(ParticleSys &system, typename ParticleSys::Scalar step_size) {
        /*unroll loop manually*/
        system.drift(5.21213104349955048E-1 * step_size);
        system.kick(1.04242620869991010E0 * step_size);
        system.drift(1.43131625920352512E0 * step_size);
        system.kick(1.82020630970713992E0 * step_size);
        system.drift(9.88973118915378424E-1 * step_size);
        system.kick(1.57739928123617007E-1 * step_size);
        system.drift(1.29888362714548355E0 * step_size);
        system.kick(2.44002732616735019E0 * step_size);
        system.drift(1.21642871598513458E0 * step_size);
        system.kick(-7.16989419708119989E-3 * step_size);
        system.drift(-1.22708085895116059E0 * step_size);
        system.kick(-2.44699182370524015E0 * step_size);
        system.drift(-2.03140778260310517E0 * step_size);
        system.kick(-1.61582374150096997E0 * step_size);
        system.drift(-1.69832618404521085E0 * step_size);
        system.kick(-1.78082862658945151E0 * step_size);
        system.drift(-1.69832618404521085E0 * step_size);
        system.kick(-1.61582374150096997E0 * step_size);
        system.drift(-2.03140778260310517E0 * step_size);
        system.kick(-2.44699182370524015E0 * step_size);
        system.drift(-1.22708085895116059E0 * step_size);
        system.kick(-7.16989419708119989E-3 * step_size);
        system.drift(1.21642871598513458E0 * step_size);
        system.kick(2.44002732616735019E0 * step_size);
        system.drift(1.29888362714548355E0 * step_size);
        system.kick(1.57739928123617007E-1 * step_size);
        system.drift(9.88973118915378424E-1 * step_size);
        system.kick(1.82020630970713992E0 * step_size);
        system.drift(1.43131625920352512E0 * step_size);
        system.kick(1.04242620869991010E0 * step_size);
        system.drift(5.21213104349955048E-1 * step_size);
    }

    /*---------------------------------------------------------------------------*\
         Class Symplectic10th Implementation
    \*---------------------------------------------------------------------------*/
    template <typename TypeSystem>
    template <CONCEPT_PARTICLE_SYSTEM ParticleSys>
    void Symplectic10th<TypeSystem>::integrate(ParticleSys &system, typename ParticleSys::Scalar step_size) {
        /*unroll loop manually*/
        system.drift(3.0610967201933609e-01 * step_size);
        system.kick(6.1221934403867218e-01 * step_size);
        system.drift(-9.4012698954724694e-02 * step_size);
        system.kick(-8.0024474194812156e-01 * step_size);
        system.drift(-6.6002635995076209e-01 * step_size);
        system.kick(-5.1980797795340250e-01 * step_size);
        system.drift(-1.5240397828727220e-01 * step_size);
        system.kick(2.1500002137885812e-01 * step_size);
        system.drift(-1.1750569210727700e-01 * step_size);
        system.kick(-4.5001140559341213e-01 * step_size);
        system.drift(2.2250778443570857e-01 * step_size);
        system.kick(8.9502697446482926e-01 * step_size);
        system.drift(5.1288848042847668e-01 * step_size);
        system.kick(1.3074998639212410e-01 * step_size);
        system.drift(3.3095796002497074e-01 * step_size);
        system.kick(5.3116593365781739e-01 * step_size);
        system.drift(-6.0050191119721985e-02 * step_size);
        system.kick(-6.5126631589726136e-01 * step_size);
        system.drift(-7.6956706144236287e-01 * step_size);
        system.kick(-8.8786780698746448e-01 * step_size);
        system.drift(-7.6872229417056015e-02 * step_size);
        system.kick(7.3412334815335245e-01 * step_size);
        system.drift(4.2477286784491525e-01 * step_size);
        system.kick(1.1542238753647800e-01 * step_size);
        system.drift(4.3160892192959932e-01 * step_size);
        system.kick(7.4779545632272060e-01 * step_size);
        system.drift(5.5434862753225678e-02 * step_size);
        system.kick(-6.3692573081626924e-01 * step_size);
        system.drift(-1.9288621063874828e-01 * step_size);
        system.kick(2.5115330953877268e-01 * step_size);
        system.drift(3.3904387248169282e-01 * step_size);
        system.kick(4.2693443542461296e-01 * step_size);
        system.drift(3.3904387248169282e-01 * step_size);
        system.kick(2.5115330953877268e-01 * step_size);
        system.drift(-1.9288621063874828e-01 * step_size);
        system.kick(-6.3692573081626924e-01 * step_size);
        system.drift(5.5434862753225678e-02 * step_size);
        system.kick(7.4779545632272060e-01 * step_size);
        system.drift(4.3160892192959932e-01 * step_size);
        system.kick(1.1542238753647800e-01 * step_size);
        system.drift(4.2477286784491525e-01 * step_size);
        system.kick(7.3412334815335245e-01 * step_size);
        system.drift(-7.6872229417056015e-02 * step_size);
        system.kick(-8.8786780698746448e-01 * step_size);
        system.drift(-7.6956706144236287e-01 * step_size);
        system.kick(-6.5126631589726136e-01 * step_size);
        system.drift(-6.0050191119721985e-02 * step_size);
        system.kick(5.3116593365781739e-01 * step_size);
        system.drift(3.3095796002497074e-01 * step_size);
        system.kick(1.3074998639212410e-01 * step_size);
        system.drift(5.1288848042847668e-01 * step_size);
        system.kick(8.9502697446482926e-01 * step_size);
        system.drift(2.2250778443570857e-01 * step_size);
        system.kick(-4.5001140559341213e-01 * step_size);
        system.drift(-1.1750569210727700e-01 * step_size);
        system.kick(2.1500002137885812e-01 * step_size);
        system.drift(-1.5240397828727220e-01 * step_size);
        system.kick(-5.1980797795340250e-01 * step_size);
        system.drift(-6.6002635995076209e-01 * step_size);
        system.kick(-8.0024474194812156e-01 * step_size);
        system.drift(-9.4012698954724694e-02 * step_size);
        system.kick(6.1221934403867218e-01 * step_size);
        system.drift(3.0610967201933609e-01 * step_size);
    }
}  // namespace hub::integrator
