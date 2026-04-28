#include <iostream>
#include <cmath>
#include "UniversGravitationnel.hpp"
#include "Particule.hpp"
#include "Vector.hpp"

int main() {

    UniversGravitationnel u(3);

    double r_earth   = 1.0;
    double r_jupiter = 5.36;
    double r_halley  = 34.75;

    double v_earth   = 1;
    double v_jupiter = 0.425;
    double v_halley  = 0.0296;
    
    // Soleil
    u.ajouterParticule(Particule(
        Vector(0, 0, 0), Vector(0, 0, 0), 1.0, 0, 0, Vector(0,0,0)));

    // Terre
    u.ajouterParticule(Particule(
        Vector(0, r_earth, 0), Vector(-v_earth, 0, 0), 3.0e-6, 1, 0, Vector(0,0,0)));

    // Jupiter
    u.ajouterParticule(Particule(
        Vector(0, r_jupiter, 0), Vector(-v_jupiter, 0, 0), 9.55e-4, 2, 0, Vector(0,0,0)));

    // Haley
    u.ajouterParticule(Particule(
        Vector(r_halley, 0, 0), Vector(0, v_halley, 0), 1.0e-14, 3, 0, Vector(0,0,0)));

    u.avancerParticules(468.5, 0.015);

    return 0;
}