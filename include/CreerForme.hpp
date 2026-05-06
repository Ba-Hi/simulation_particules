#pragma once
#include "Univers.hpp"
#include "Particule.hpp"
#include "Vector.hpp"
#include <cmath>

namespace Forme {

    // Remplit un espace rectangulaire jusqu'à atteindre N_max particules
    inline int creerRectangleLimitee(Univers& u, double x_start, double y_start, double x_max, int N_max, double d, double m, int type, int& id, Vector vel = Vector(0,0,0)) {
        int count = 0;
        double current_x = x_start;
        double current_y = y_start;

        while (count < N_max) {
            u.ajouterParticule(Particule(Vector(current_x, current_y, 0.0), vel, m, id++, type, Vector(0,0,0)));
            count++;
            current_x += d;
            if (current_x > x_max) {
                current_x = x_start;
                current_y += d;
            }
        }
        return count;
    }

    // Remplit un cercle donné par son centre et son rayon
    inline int creerCercle(Univers& u, double cx, double cy, double radius, double d, double m, int type, int& id, Vector vel = Vector(0,0,0)) {
        int count = 0;
        for (double y = -radius; y <= radius; y += d) {
            for (double x = -radius; x <= radius; x += d) {
                if (x * x + y * y <= radius * radius) {
                    u.ajouterParticule(Particule(Vector(cx + x, cy + y, 0.0), vel, m, id++, type, Vector(0,0,0)));
                    count++;
                }
            }
        }
        return count;
    }

}
