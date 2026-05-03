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
    const double tEnd    = 19.5;
    const double m       = 1.0;

    // Distance inter-particules
    const double d = std::pow(2.0, 1.0/6.0) * sigma;

    // Rectangle : 160 colonnes * d de large, 40 lignes * d de haut
    // Carré     : 40 colonnes * d de large, 40 lignes * d de haut
    const double Lx = 250.0;

    const double Ly = 150;

    UniversLJ u(2, Vector(Lx, Ly, 0.0), r_cut, epsilon, sigma);

    // Rectangle 160×40
    double rect_x0 = (Lx - 160 * d) / 2.0; // centré en x
    double rect_y0 = 5.0;  // petite marge en bas

    int id = 0;
    for (int j = 0; j < 40; j++) {
        for (int i = 0; i < 160; i++) {
            Vector pos(rect_x0 + i * d, rect_y0 + j * d, 0.0);
            Vector vel(0.0, 0.0, 0.0);
            u.ajouterParticule(Particule(pos, vel, m, id++, 0, Vector(0,0,0)));
        }
    }

    // Carré 40×40
    double rect_top = rect_y0 + 39 * d;
    double gap      = 10*d; // espace entre le rectangle et le carré
    double sq_y0    = rect_top + gap;

    double sq_x0    = rect_x0 + (160 - 40) / 2.0 * d; // centré sur le rectangle

    for (int j = 0; j < 40; j++) {
        for (int i = 0; i < 40; i++) {
            Vector pos(sq_x0 + i * d, sq_y0 + j * d, 0.0);
            Vector vel(0.0, -10.0, 0.0);  // vitesse initiale réduite pour éviter une collision trop violente
            u.ajouterParticule(Particule(pos, vel, m, id++, 1, Vector(0,0,0)));
        }
    }

    u.initialiserCellules();
    u.avancerParticules(tEnd, dt);


    return 0;
}

