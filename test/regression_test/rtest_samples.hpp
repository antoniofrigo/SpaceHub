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
#pragma once

#include <tuple>

#include "../../src/spaceHub.hpp"

auto two_body(double e = 0) {
    using Particle = typename hub::methods::DefaultMethod<>::Particle;
    using namespace hub;
    using namespace hub::unit;
    using namespace hub::orbit;

    Particle sun{1_Ms}, earth{1_Me};
    auto orbit = EllipOrbit(sun.mass, earth.mass, 1_AU, e, 0, 0, 0, 0);

    move_particles(orbit, earth);

    move_to_COM_frame(sun, earth);

    return std::vector{sun, earth};
}

auto outer_solar() {
    using Particle = typename hub::methods::DefaultMethod<>::Particle;
    using namespace hub;
    using namespace hub::unit;
    using namespace hub::orbit;

    Particle sun{1.00000597682,        -4.06428567034226e-3, -6.08813756435987e-3, -1.66162304225834e-6,
                 +6.69048890636161e-6, -6.33922479583593e-6, -3.13202145590767e-9};
    Particle jup{1. / 1047.355,       +3.40546614227466e+0, +3.62978190075864e+0,   +3.42386261766577e-2,
                 -0.3254242234844626, 0.32078376079804843,  -0.00015504584274327233};
    Particle sat{1. / 3501.6,          +6.60801554403466e+0, +6.38084674585064e+0, -1.36145963724542e-1,
                 -0.24261807906125546, 0.23236917361652312,  0.0009720111543216125};
    Particle ura{1. / 22869.,         +1.11636331405597e+1, +1.60373479057256e+1,  +3.61783279369958e-1,
                 -0.1894447922304644, 0.12000768830940597,  -0.0012655376696374542};
    Particle nep{1. / 19314.,          -3.01777243405203e+1, +1.91155314998064e+0, -1.53887595621042e-1,
                 -0.01264216568441132, -0.18100181375010865, 0.0020831452402001265};
    Particle plu{7.4074074e-09,        -2.13858977531573e+1, +3.20719104739886e+1, +2.49245689556096e+0,
                 -0.10285755114348066, -0.12017192726456442, 0.038256490292647924};

    move_to_COM_frame(sun, jup, sat, ura, nep);

    return std::vector{sun, jup, sat, ura, nep};
}

auto earth_system() {
    using Particle = typename hub::methods::DefaultMethod<>::Particle;
    using namespace hub;
    using namespace hub::unit;
    using namespace hub::orbit;

    Particle sun{1_Ms}, earth{1_Me}, moon{1_Mmoon};

    auto moon_orbit = EllipOrbit(earth.mass, moon.mass, 384748_km, 0.0549006, 5.15_deg, 0, 0, 0);

    move_particles(moon_orbit, moon);

    auto orbit = EllipOrbit(sun.mass, earth.mass + moon.mass, 1_AU, 0, 0, 0, 0, 0);

    move_particles(orbit, earth, moon);

    move_to_COM_frame(sun, earth, moon);

    return std::vector{sun, earth, moon};
}

auto harmonic_system(size_t N) {
    using Particle = typename hub::methods::DefaultMethod<>::Particle;
    using namespace hub;
    using namespace hub::unit;
    using namespace hub::orbit;

    Particle sun{1_Ms};
    std::vector<Particle> particles;
    particles.reserve(N + 1);
    particles.emplace_back(sun);

    for (size_t i = 0; i < N; ++i) {
        Particle test{1_Mmoon};
        auto orbit = EllipOrbit(sun.mass, test.mass, (i + 1) * 1_AU, 0.0, 0, 0, 0, (i % 2) * consts::pi);

        move_particles(orbit, test);

        particles.emplace_back(test);
    }

    move_to_COM_frame(particles);

    return particles;
}

auto kozai() {
    using Particle = typename hub::methods::DefaultMethod<>::Particle;
    using namespace hub;
    using namespace hub::unit;
    using namespace hub::orbit;

    Particle m1{1_Ms}, m2{0.5_Ms}, m3{1_Ms};

    auto in_orbit = EllipOrbit(m1.mass, m2.mass, 10_AU, 0.001, 88_deg, 180_deg, 0_deg, 0);

    move_particles(in_orbit, m2);

    move_to_COM_frame(m1, m2);

    auto out_orbit = EllipOrbit(m1.mass + m2.mass, m3.mass, 100_AU, 0.5, 9_deg, 0_deg, 0_deg, 0);

    move_particles(out_orbit, m3);

    move_to_COM_frame(m1, m2, m3);

    return std::vector{m1, m2, m3};
}
