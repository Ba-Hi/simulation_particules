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
    const double Ly = 30.0;
    const double Lz = 30.0;

    UniversLJ u(3, Vector(Lx, Ly, Lz), r_cut, epsilon, sigma);
    u.setConditionsLimites(Univers::ConditionLimite::ABSORPTION,
                           Univers::ConditionLimite::ABSORPTION);

    const int    n       = 5; // taille du cube : n×n×n particules
    const double vitesse = 5.0; //vitesse d'approche

    // Centrage suivant y et z
    const double y0 = (Ly - n * d) / 2.0;
    const double z0 = (Lz - n * d) / 2.0;

    // Cube A : à gauche, se déplace vers +x
    const double xA0 = Lx / 2.0 - (n + 3.0) * d; // marge de 3d avant impact

    // Cube B : à droite, se déplace vers -x
    const double xB0 = Lx / 2.0 + 3.0 * d;

    int id = 0;

    for (int k = 0; k < n; k++) {
        for (int j = 0; j < n; j++) {
            for (int i = 0; i < n; i++) {
                Vector pos(xA0 + i * d, y0 + j * d, z0 + k * d);
                Vector vel(+vitesse, 0.0, 0.0);
                u.ajouterParticule(Particule(pos, vel, m, id++, 0, Vector(0,0,0)));
            }
        }
    }

    for (int k = 0; k < n; k++) {
        for (int j = 0; j < n; j++) {
            for (int i = 0; i < n; i++) {
                Vector pos(xB0 + i * d, y0 + j * d, z0 + k * d);
                Vector vel(-vitesse, 0.0, 0.0);
                u.ajouterParticule(Particule(pos, vel, m, id++, 1, Vector(0,0,0)));
            }
        }
    }

    u.initialiserCellules();

    const double Ec0 = u.energieCinetique();
    const double Ep0 = u.energiePotentielle();
    const double E0  = Ec0 + Ep0;
    std::cout << "E_initiale  = " << E0  << "\n";

    u.avancerParticules(tEnd, dt);

    const double Ecf = u.energieCinetique();
    const double Epf = u.energiePotentielle();
    const double Ef  = Ecf + Epf;
    std::cout << "E_finale  = " << Ef  << "\n";
    std::cout << "Variation relative : " << 100.0 * (Ef - E0) / std::abs(E0) << " %\n";

    return 0;
}