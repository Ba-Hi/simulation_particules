#include "UniversLJ.hpp"
#include "Particule.hpp"
#include "Vector.hpp"
#include "ExportVTK.hpp"
#include "CreerForme.hpp"
#include <iostream>
#include <cmath>

int main() {

    const double Lx = 300.0;
    const double Ly = 180.0;

    const int N1 = 395;
    const int N2 = 17227;

    const double tEnd = 29.5;
    const double dt = 0.0005;

    const double sigma   = 1.0;
    const double epsilon = 1.0;
    const double r_cut   = 2.5 * sigma;
    const double m       = 1.0;
    const double d       = std::pow(2.0, 1.0/6.0) * sigma;
    const double d_bain  = 1.05 * d; // plus de dist pour liquide plus fluide

    UniversLJ u(2, Vector(Lx, Ly), r_cut, epsilon, sigma);
    u.setGravity(Vector(0.0, -12)); // champ de gravité vers le bas

    // reflexion
    u.setConditionsLimites(
        Univers::ConditionLimite::REFLEXION,
        Univers::ConditionLimite::REFLEXION
    );

    int id = 0;

    //bain
    int count_n2 = Forme::creerRectangleLimitee(u, 2.0 * d_bain, 2.0 * d_bain, Lx - 2.0 * d_bain, N2, d_bain, m, 0, id);

    double hauteur_bain = std::ceil(N2 / ((Lx - 4.0*d_bain)/d_bain)) * d_bain;
    
    //goutte
    double radius = std::sqrt(N1 / M_PI) * d; // La goutte reste très dense et compacte (d)
    double cx = Lx / 2.0;
    double cy = 2.0 * d_bain + hauteur_bain + 20.0 * d_bain + radius;
    
    int count_n1 = Forme::creerCercle(u, cx, cy, radius, d, m, 1, id, Vector(0.0, -10.0, 0.0));

    // limite vitesse
    const double Ec_D = 0.005 * (N1+N2);
    
    // On limite que le bain pour le stabiliser 
    u.setLimiteEnergie(Ec_D, 1000, 0); 
    // u.setLimiteEnergie(Ec_D, 1000, 1);

    u.initialiserCellules();
    
    std::cout << "Démarrage de la simulation\n";
    
    u.avancerParticules(tEnd, dt, true, true);
    
    std::cout << "Simulation terminée\n";
    return 0;
}