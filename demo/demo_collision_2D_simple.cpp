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
    const double tEnd    = 3.5;
    const double m       = 1.0;

    const double d = std::pow(2.0, 1.0/6.0) * sigma;

    const double Lx = 100.0;
    const double Ly = 80.0;

    UniversLJ u(2, Vector(Lx, Ly, 0.0), r_cut, epsilon, sigma);

    // SMALL RECTANGLE: 20×10
    int rect_w = 20;
    int rect_h = 10;

    double rect_x0 = (Lx - rect_w * d) / 2.0;
    double rect_y0 = 5.0;

    int id = 0;
    for (int j = 0; j < rect_h; j++) {
        for (int i = 0; i < rect_w; i++) {
            Vector pos(rect_x0 + i * d, rect_y0 + j * d, 0.0);
            Vector vel(0.0, 0.0, 0.0);
            u.ajouterParticule(Particule(pos, vel, m, id++, 0, Vector(0,0,0)));
        }
    }

    // SMALL SQUARE: 10×10
    int sq_size = 10;

    double rect_top = rect_y0 + (rect_h - 1) * d;
    double gap      = 3 * d;
    double sq_y0    = rect_top + gap;

    double sq_x0 = rect_x0 + (rect_w - sq_size) / 2.0 * d;

    for (int j = 0; j < sq_size; j++) {
        for (int i = 0; i < sq_size; i++) {
            Vector pos(sq_x0 + i * d, sq_y0 + j * d, 0.0);
            Vector vel(0.0, -10.0, 0.0);
            u.ajouterParticule(Particule(pos, vel, m, id++, 1, Vector(0,0,0)));
        }
    }

    std::cout << "Nombre de particules : " << u.getParticules().size() << "\n";
    std::cout << "Simulation de t=0 à t=" << tEnd << "\n";

    u.initialiserCellules();

    double enerique_initiale = u.energieCinetique() + u.energiePotentielle();
    u.avancerParticules(tEnd, dt);
    double enerique_finale = u.energieCinetique() + u.energiePotentielle();
    std::cout << "Done.\n";
    std::cout << "Energie initiale : " << enerique_initiale << "\n";
    std::cout << "Energie finale : " << enerique_finale << "\n";
    std::cout << "Variation d'énergie : " << (enerique_finale - enerique_initiale) << "\n";

    return 0;
}
