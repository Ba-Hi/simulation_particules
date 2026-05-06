#include <iostream>
#include <fstream>
#include <cmath>
#include "UniversLJ.hpp"
#include "Particule.hpp"
#include "Vector.hpp"

int main() {
    const double dt = 0.0001;
    const double tEnd = 0.6;
    const double Lx = 10.0;
    const double m = 1.0;

    // Univers 1 : Cinématique
    UniversLJ u_cin(1, Vector(Lx), 2.5, 1.0, 1.0);
    u_cin.setConditionsLimites(Univers::ConditionLimite::REFLEXION);
    u_cin.ajouterParticule(Particule(Vector(3.0, 0.0, 0.0), Vector(-8.0, 0.0, 0.0), m, 0, 0, Vector(0,0,0)));
    u_cin.initialiserCellules();

    // Univers 2 : Potentiel
    UniversLJ u_pot(1, Vector(Lx), 2.5, 1.0, 1.0);
    u_pot.setConditionsLimites(Univers::ConditionLimite::ABSORPTION); // absorption pour voir la différence clairement
    u_pot.ajouterParticule(Particule(Vector(3.0, 0.0, 0.0), Vector(-8.0, 0.0, 0.0), m, 0, 0, Vector(0,0,0)));
    u_pot.initialiserCellules();

    // enregistrer le csv dans ./demo/comparaison_murs.csv pas dans le build
    std::ofstream file("comparaison_murs.csv");
    file << "t,x_cin,v_cin,x_pot,v_pot,E_cin_total,E_pot_total\n";

    double t = 0.0;
    auto f_cin = u_cin.calculerForces(false);
    auto f_pot = u_pot.calculerForces(true);

    // Boucle Störmer-Verlet manuelle 
    while (t < tEnd) {

        double E_cin_total = u_cin.energieCinetique() + u_cin.energiePotentielle();
        double E_pot_total = u_pot.energieCinetique() + u_pot.energiePotentielle();
        
        file << t << ","
             << u_cin.getParticules()[0].getPosition().x() << ","
             << u_cin.getParticules()[0].getVitesse().x() << ","
             << u_pot.getParticules()[0].getPosition().x() << ","
             << u_pot.getParticules()[0].getVitesse().x() << ","
             << E_cin_total << "," << E_pot_total << "\n";

        t += dt;

        // cin : calculerForces = false
        {
            auto& p = u_cin.getParticules()[0];
            p.setPosition(p.getPosition() + p.getVitesse() * dt + f_cin[0] * (0.5 * dt * dt / m));
            u_cin.mettreAJourCellules(true);
            auto f_cin_new = u_cin.calculerForces(false);
            p.setVitesse(p.getVitesse() + (f_cin_new[0] + f_cin[0]) * (0.5 * dt / m));
            f_cin = f_cin_new;
        }

        // pot : calculerForces = true
        {
            // Cas où la particule serait absorbée
            if (!u_pot.getParticules().empty()) {
                auto& p = u_pot.getParticules()[0];
                p.setPosition(p.getPosition() + p.getVitesse() * dt + f_pot[0] * (0.5 * dt * dt / m));
                u_pot.mettreAJourCellules(true);
                auto f_pot_new = u_pot.calculerForces(true);
                
                // Si absorbée après la mise à jour, on ne met plus à jour la vitesse
                if (!u_pot.getParticules().empty()) {
                    p.setVitesse(p.getVitesse() + (f_pot_new[0] + f_pot[0]) * (0.5 * dt / m));
                }
                f_pot = f_pot_new;
            }
        }
    }
    
    file.close();
    std::cout << "Simulation comparaison des conditions limites terminée !" << std::endl;
    return 0;
}