
#include "../src/spaceHub.hpp"
using namespace hub;
using namespace unit;
using namespace callback;
using Solver = methods::DefaultMethod<>;
using Particle = Solver::Particle;

int main(int argc, char** argv) {
    Particle p1{1_Ms};
    Particle p2{1_Ms};
    Particle p3{1_Ms};

    auto inner_orb = orbit::Elliptic(p1.mass, p2.mass, 5_AU, 0.001, 1_deg, 2_deg, 3_deg, 4_deg);

    auto outer_orb = orbit::Elliptic(orbit::M_tot(p1, p2), p3.mass, 40_AU, 0.1, 6_deg, 7_deg, 8_deg, 9_deg);

    orbit::move_particles(inner_orb, p2);

    orbit::move_to_COM_frame(p1, p2);

    orbit::move_particles(outer_orb, p3);

    orbit::move_to_COM_frame(p1, p2, p3);

    Solver solver{0, p1, p2, p3};

    Solver::RunArgs args;

    args.add_stop_condition(1000_year);

    /*--------------------------------------------------New-----------------------------------------------------------*/
    // the stop condition callback function have two input parameters, the first is the evolving particle system, the
    // second is the step size(no necessary to be dt, in regularized algorithm this is dh) in the next iteration. The
    // return type must be bool
    auto distance_check = [](auto& particles, auto step_size) -> bool {
        // if the distance between particle 0 and 1 is smaller than 1 AU, stop the integration.
        if (distance(particles.pos(0), particles.pos(1)) < 1_AU) {
            return true;
        } else {
            return false;
        }
    };

    auto stop_callback = [](auto& particles, auto step_size) -> bool {
        // if t > 500 years stop the integration
        if (particles.time() > 500_year) {
            return true;
        } else {
            return false;
        }
    };

    // distance check in each step, otherwise we might miss it.
    args.add_stop_condition(distance_check);  // add callback.

    // time check may not be that strict, check it every 1000 steps.
    args.add_stop_condition(StepSlice(stop_callback, 1000));  // add callback.

    // now we have three stop condition: 1. time upper limit 1000 year; 2. distance check between particle 0(p1) and
    // 1(p2); 3. t > 500 years(so condition 1 will never be triggered);
    /*----------------------------------------------------------------------------------------------------------------*/

    solver.run(args);

    print(std::cout, "simulation of adding stop conditions complete!\n");

    return 0;
}
