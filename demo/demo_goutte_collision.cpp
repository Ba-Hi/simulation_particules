#include "UniversLJ.hpp"
#include "Particule.hpp"
#include "Vector.hpp"
#include "ExportVTK.hpp"
#include "CreerForme.hpp"
#include <iostream>
#include <cmath>

int main() {

    const double Lx = 300.0; // Domaine + large pour étaler le bain en largeur
    const double Ly = 180.0;

    const int N1 = 395;
    const int N2 = 17227;

    const double tEnd = 29.5;
    const double dt = 0.0005;

    const double sigma   = 1.0;
    const double epsilon = 1.0;
    const double r_cut   = 2.5 * sigma;
    const double m       = 1.0;
    const double d = std::pow(2.0, 1.0/6.0) * sigma;

    UniversLJ u(2, Vector(Lx, Ly), r_cut, epsilon, sigma);
    u.setGravity(Vector(0.0, -12.0)); // champ de gravité vers le bas

    // reflexion
    u.setConditionsLimites(
        Univers::ConditionLimite::REFLEXION,
        Univers::ConditionLimite::REFLEXION
    );

    int id = 0;

    // le rectangle (bain)
    int count_n2 = Forme::creerRectangleLimitee(u, 2.0 * d, 2.0 * d, Lx - 2.0 * d, N2, d, m, 0, id);

    // hauteur du bain
    double hauteur_bain = std::ceil(N2 / ((Lx - 4.0*d)/d)) * d;
    
    // cercle (goutte)
    double radius = std::sqrt(N1 / M_PI) * d; // Rayon approximatif pour contenir N1 particules
    double cx = Lx / 2.0;
    double cy = 2.0 * d + hauteur_bain + 15.0 * d + radius; // 15*d d'espacement avec le bain
    int count_n1 = Forme::creerCercle(u, cx, cy, radius, d, m, 1, id, Vector(0.0, -10.0, 0.0));

    const double Ec_D = 0.005 * (count_n1 + count_n2);
    u.setLimiteEnergie(Ec_D, 1000); // Application de la limite


    u.initialiserCellules();
    std::cout << "Simulation a commencé\n";
    // u.avancerParticules(tEnd, dt, use_potentiel_reflexion=true, use_gravity=true);
    u.avancerParticules(tEnd, dt, true, true);
    std::cout << "Simulation terminée\n";
    return 0;

    
}