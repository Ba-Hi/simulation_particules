#include <iostream>
#include <fstream>
#include <cmath>
#include "UniversLJ.hpp"
#include "Particule.hpp"
#include "Vector.hpp"

int main() {

    const double sigma   = 1.0;
    const double epsilon = 1.0;
    const double r_cut   = 2.5 * sigma;
    const double dt      = 0.0001;
    const double tEnd    = 5.0;
    const double m       = 1.0;

    const double d = std::pow(2.0, 1.0/6.0) * sigma;

    UniversLJ u(2, Vector(30.0, 30.0, 0.0), r_cut, epsilon, sigma);
    u.initialiserCellules();

    // Groupe A : 3 particules fixes
    for (int i = 0; i < 3; i++) {
        Vector pos(10.0 + i * d, 10.0, 0.0);
        u.ajouterParticule(Particule(pos, Vector(0,0,0), m, i, 0, Vector(0,0,0)));
    }

    // Groupe B : 1 particule tombante
    u.ajouterParticule(Particule(
        Vector(10.0 + d, 14.0, 0.0),
        Vector(0.0, -3.0, 0.0),
        m, 3, 1, Vector(0,0,0)));

    double enerique_initiale = u.energieCinetique() + u.energiePotentielle();
    u.avancerParticules(tEnd, dt);
    double enerique_finale = u.energieCinetique() + u.energiePotentielle();

    std::cout << "Energie initiale : " << enerique_initiale << "\n";
    std::cout << "Energie finale : " << enerique_finale << "\n";
    std::cout << "Variation d'énergie : " << (enerique_finale - enerique_initiale) << "\n";


    return 0;
}