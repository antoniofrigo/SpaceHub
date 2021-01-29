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
    the terms of the MIT License. SpaceHub is distributed in the hope that it
    will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
    of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the MIT License
    for more details. You should have received a copy of the MIT License along
    with SpaceHub.
\*---------------------------------------------------------------------------*/
#include <thread>

#include "../../src/spaceHub.hpp"
#include "rtest_samples.hpp"

USING_NAMESPACE_SPACEHUB_ALL;

template <typename simulation>
void run(std::string const &sim_name) {
    auto solar = outer_solar<simulation>();

    basic_error_test<simulation>("outer-solar-" + sim_name, 11862_year, 1e-15, solar);

    /* auto [rtol, error] = error_scale<simulation>(3e-16, 1e-8, 100_year, earth_sys);

     std::fstream err_stream{"earth-system-" + sim_name + ".scale", std::ios::out};

     err_stream << rtol << '\n' << error;*/
}

int main(int argc, char **argv) {
    using type = Types<double>;

    using rtype = Types<double>;

    using force = interactions::Interactions<interactions::NewtonianGrav>;

    using particles = PointParticles<type>;

    using sim_sys = SimpleSystem<particles, force>;

    using regu_sys = RegularizedSystem<particles, force, ReguType::LogH>;

    using chain_sys = ChainSystem<particles, force>;

    using arch_sys = ARchainSystem<particles, force, ReguType::LogH>;

    using base_integrator = LeapFrogDKD<rtype>;
    //    using iter = ConstOdeIterator<Symplectic2nd>;

    using err_estimator = WorstOffender<rtype>;

    using step_controller = PIDController<rtype>;

    using iter = BurlishStoer<base_integrator, err_estimator, step_controller>;

    using ias15_iter = IAS15<integrator::GaussDadau<rtype>, IAS15Error<rtype>, step_controller>;

    // using ias15_iter = IAS15<integrator::GaussDadau<rtype>, err_estimator, step_controller>;

    using space_iter = BisecOdeIterator<integrator::Symplectic6th<rtype>, WorstOffender<rtype>, step_controller>;

    /*run<Simulator<sim_sys, iter>>("sim");

    run<Simulator<regu_sys, iter>>("regu");

    run<Simulator<chain_sys, iter>>("chain");*/

    // run<Simulator<arch_sys, iter>>("arch");

    run<Simulator<sim_sys, ias15_iter>>("ias15");

    /*run<Simulator<chain_sys, ias15_iter>>("chain_ias15");

    run<Simulator<regu_sys, ias15_iter>>("regu_ias15");*/

    // run<Simulator<arch_sys, ias15_iter>>("arch_ias15");

    // run<Simulator<arch_sys, space_iter>>("space");

    return 0;
}
