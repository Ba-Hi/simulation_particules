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

    const double tEnd = 15.0;
    const double dt = 0.0005;

    const double sigma   = 1.0;
    const double epsilon = 1.0;
    const double r_cut   = 2.5 * sigma;
    const double m       = 1.0;
    const double d = std::pow(2.0, 1.0/6.0) * sigma;

    UniversLJ u(2, Vector(Lx, Ly), r_cut, epsilon, sigma);
    u.setGravity(Vector(0.0, -12)); // champ de gravité vers le bas

    // reflexion
    u.setConditionsLimites(
        Univers::ConditionLimite::REFLEXION,
        Univers::ConditionLimite::REFLEXION
    );

    int id = 0;

    // 1. Le bain (rendu Fluide)
    // On augmente l'espacement de 5% pour créer un "volume libre" 
    // transformant le bloc de glace compact en un véritable liquide.
    double d_bain = 1.20 * d;
    int count_n2 = Forme::creerRectangleLimitee(u, 2.0 * d_bain, 2.0 * d_bain, Lx - 2.0 * d_bain, N2, d_bain, m, 0, id);

    double hauteur_bain = std::ceil(N2 / ((Lx - 4.0*d_bain)/d_bain)) * d_bain;
    
    // 2. La goutte (Projectile)
    double radius = std::sqrt(N1 / M_PI) * d; // Rayon approximatif pour contenir N1 particules
    double cx = Lx / 2.0;
    double cy = 2.0 * d_bain + hauteur_bain + 20.0 * d + radius; 
    // Vitesse d'impact drastiquement augmentée pour transpercer la surface
    int count_n1 = Forme::creerCercle(u, cx, cy, radius, d, m, 1, id, Vector(0.0, -40.0, 0.0));

    const double Ec_bain = 0.005 * count_n2;
    const double Ec_goutte = 0.005 * count_n1;
    
    // On relaxe la fréquence du thermostat du bain (5000 au lieu de 1000) 
    // pour ne pas dissiper l'onde de choc et laisser la couronne se lever !
    u.setLimiteEnergie(Ec_bain, 5000, 0); 
    u.setLimiteEnergie(Ec_goutte, 1000, 1); // Limite pour la goutte (type 1)


    u.initialiserCellules();
    std::cout << "Simulation a commencé\n";
    // u.avancerParticules(tEnd, dt, use_potentiel_reflexion=true, use_gravity=true);
    u.avancerParticules(tEnd, dt, true, true);
    std::cout << "Simulation terminée\n";
    return 0;
}