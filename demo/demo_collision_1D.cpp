#include <iostream>
#include <cmath>
#include "UniversLJ.hpp"
#include "Particule.hpp"
#include "Vector.hpp"



int main() {

    const double sigma   = 1.0;
    const double epsilon = 5.0;
    const double r_cut   = 2.5 * sigma;
    const double dt      = 0.00005;
    const double tEnd    = 3.0;
    const double m       = 1.0;

    const double d = std::pow(2.0, 1.0 / 6.0) * sigma;

    const double Lx = 60.0;

    // UniversLJ::UniversLJ(dimension, l_d, r_cut, epsilon, sigma)
    UniversLJ u(1, Vector(Lx), r_cut, epsilon, sigma);
    u.setConditionsLimites(Univers::ConditionLimite::ABSORPTION,
                           Univers::ConditionLimite::ABSORPTION);


    const int    n_A     = 10; // nombre de particules dans le groupe A
    const int    n_B     = 10; // nombre de particules dans le groupe B
    const double vitesse = 5.0; // vitesse d'approche : les deux groupes s'approchent à la même vitesse



    // [-------GroupeA------]  centre  [-------GroupeB------]
    // marge = 3*d
    // debut du groupe A = xA0 = centre - (taille de A + marge + moitié groupe B)
    // const double xA0 = Lx / 2.0 - (n_A + n_B / 2.0 + 3.0) * d;
    // debut du groupe B = xB0 = centre + (marge + moitié groupe B)
    // const double xB0 = Lx / 2.0 + (n_B / 2.0 + 3.0) * d;
    const double xA0 = Lx / 2.0 - (n_A + 3.0) * d;
    const double xB0 = Lx / 2.0 + (3.0) * d;


    int id = 0;

    for (int i = 0; i < n_A; i++) {
        Vector pos(xA0 + i * d, 0.0, 0.0);
        Vector vel(+vitesse, 0.0, 0.0);
        u.ajouterParticule(Particule(pos, vel, m, id++, 0, Vector(0,0,0)));
    }

    for (int i = 0; i < n_B; i++) {
        Vector pos(xB0 + i * d, 0.0, 0.0);
        Vector vel(-vitesse, 0.0, 0.0);
        u.ajouterParticule(Particule(pos, vel, m, id++, 1, Vector(0,0,0)));
    }


    u.initialiserCellules();

    const double E0 = u.energieCinetique() + u.energiePotentielle();
    std::cout << "Energie initiale   : " << E0 << "\n";

    u.avancerParticules(tEnd, dt);

    const double Ef = u.energieCinetique() + u.energiePotentielle();
    std::cout << "Energie finale     : " << Ef << "\n";
    std::cout << "Variation absolue  : " << (Ef - E0) << "\n";
    std::cout << "Variation relative : " << 100.0 * (Ef - E0) / std::abs(E0) << " %\n";

    return 0;
}