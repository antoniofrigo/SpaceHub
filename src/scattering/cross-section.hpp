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
/**
 * @file cross-section.hpp
 *
 * Header file.
 */
#ifndef SPACEHUB_CROSS_SECTION_HPP
#define SPACEHUB_CROSS_SECTION_HPP

#include "../orbits/operations.hpp"
#include "../orbits/orbits.hpp"
#include "../rand-generator.hpp"
#include "../vector/vector3.hpp"

/**
 * @namespace space::scattering
 * namespace for scattering
 */
namespace space::scattering {

/*---------------------------------------------------------------------------*\
    Functions Declaration
\*---------------------------------------------------------------------------*/
template <typename Scalar>
auto critical_vel(Scalar m_redu, Scalar E_stay, Scalar E_incident);

template <typename Scalar>
auto b_max(Scalar v_c, Scalar v_inf, Scalar a_max);

template <typename Scalar>
auto random_incident(Scalar m_stay, Scalar m_incident, Scalar v_inf, Scalar b_max, Scalar r);

template <typename Cluster1, typename Cluster2, typename Scalar>
auto random_incident(Cluster1&& stay_cluster, Cluster2&& incident_cluster, Scalar v_inf, Scalar tidal_factor);
/*---------------------------------------------------------------------------*\
    Functions Implementation
\*---------------------------------------------------------------------------*/
template <typename Scalar>
auto critical_vel(Scalar m_redu, Scalar E_stay, Scalar E_incident) {
  return sqrt(-2 * (E_stay + E_incident) / m_redu);
}

template <typename Scalar>
auto b_max(Scalar v_c, Scalar v_inf, Scalar a_max) {
  return a_max * (4 * v_c / v_inf + 3);
}

template <typename Scalar>
auto random_incident(Scalar m_stay, Scalar m_incident, Scalar v_inf, Scalar b_max, Scalar r) {
  using Vector = Vec3<Scalar>;
  auto b = sqrt(random::Uniform(0, b_max * b_max));
  auto w = random::Uniform(0, 2 * consts::pi);
  auto orbit = orbit::HyperOrbit(m_stay, m_incident, v_inf, b, r, w, 0, 0);
  Vector pos, vel;
  orbit::orbit_to_coord(orbit, pos, vel);
  return std::make_tuple(pos, vel);
}

template <typename Cluster1, typename Cluster2, typename Scalar>
auto random_incident(Cluster1&& stay_cluster, Cluster2&& incident_cluster, Scalar v_inf, Scalar tidal_factor) {
  auto M_stay = orbit::M_tot(stay_cluster);
  auto M_incident = orbit::M_tot(incident_cluster);
  auto M_reduce = orbit::M_rdc(M_stay, M_incident);
  auto v_c = critical_vel(M_reduce, orbit::E_tot(stay_cluster), orbit::E_tot(incident_cluster));
  auto R1 = orbit::cluster_size(stay_cluster);
  auto R2 = orbit::cluster_size(incident_cluster);
  auto R_max = math::max(R1, R2);
  auto b_upper = b_max(v_c, v_inf, R_max);
  auto r_start = orbit::tidal_radius(tidal_factor, stay_cluster, incident_cluster, R2);
  return random_incident(M_stay, M_incident, v_inf, b_upper, r_start);
}

}  // namespace space::scattering

#endif  // SPACEHUB_CROSS_SECTION_HPP
