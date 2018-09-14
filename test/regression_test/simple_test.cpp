#include "../../src/spaceHub.h"
#include <iomanip>
using namespace SpaceH;
//using scalar = double;
using scalar = precise_d;
const size_t N = 3;//SpaceH::DYNAMICAL;
using type = SpaceH::TypeClass<scalar, N>;
int main(int argc, char **argv) {

    //using f = KarmackNewtonian<type>;
    //using force = NewtonianForce<type>;
    using force = NewtonianChainForce<type>;
    //using PN = PostNewtonianForce<type,true,false,false>;

    //using sys = Basic<force>;
    //using sys =  GAR<force>;
    using sys = ARchain <force>;
    using simulation = Nbody<sys>;
    //using simulation = Nbody<sys, IAS15, GaussDadau>;
    //using simulation = Nbody<sys, const_iterator, GaussDadau>;
    //using simulation = Nbody<sys, const_iterator, symplectic2th>;

    simulation nbody;

    std::cout << std::setprecision(16);



    simulation::RunArgs args;
    //args.file = "solar_earth.init";
    //args.file = "circular.init";
    //args.file = "elliptic.init";
    args.file = "Kozai.init";
    args.endTime = 30* Unit::YEAR;

    args.registerPreOption(CallBack::DefaultWriter<sys>("circular.dat", args.endTime));
    args.registerPreOption(CallBack::EnergyWriter<sys>("circular.eng", args.endTime));
    args.registerPostOption(CallBack::ShowProgressBar<sys>(args.endTime));

    nbody.run(args);
    return 0;
}
