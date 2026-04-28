#include <random>
#include <iostream>
#include <vector>
#include <cmath>
#include "Particule.hpp"

// Implementation de la classe Particule en utilisant Vector

Particule::Particule(const Vector& position,
                     const Vector& vitesse,
                     double masse,
                     int identifiant,
                     int categorie,
                     const Vector& force)
    : position(position),
      vitesse(vitesse),
      force(force),
      masse(masse),
      identifiant(identifiant),
      categorie(categorie)
{
    if (masse <= 0)
        throw std::invalid_argument("La masse doit être strictement positive.");
}

// Setters et getters

Particule& Particule::setPosition(const Vector& newPosition) {
    position = newPosition;
    return *this;
}

const Vector& Particule::getPosition() const {
    return position;
}

Particule& Particule::setVitesse(const Vector& newVitesse) {
    vitesse = newVitesse;
    return *this;
}

const Vector& Particule::getVitesse() const {
    return vitesse;
}

Particule& Particule::setMasse(double newMasse) {
    masse = newMasse;
    return *this;
}

const double Particule::getMasse() const {
    return masse;
}

Particule& Particule::setForce(const Vector& newForce) {
    force = newForce;
    return *this;
}

const Vector& Particule::getForce() const {
    return force;
}

Particule& Particule::setType(int newType) {
    categorie = newType;
    return *this;
}

const int Particule::getType() const {
    return categorie;
}


std::vector<Vector> initialiser(std::vector<Particule>& particleList) {
    size_t n = particleList.size();
    std::vector<Vector> forces(n, Vector(0.0, 0.0, 0.0));

    for (size_t i = 0; i < n; ++i) {
        for (size_t j = i+1; j < n; ++j) {
            const Vector& pi = particleList[i].getPosition();
            const Vector& pj = particleList[j].getPosition();
            Vector diff = pj - pi;
            double distSq = diff.norm2();
            if (distSq == 0.0) continue;
            double invDistCubed = 1.0 / (distSq * diff.norm());
            double scalar = particleList[i].getMasse() * particleList[j].getMasse() * invDistCubed;
            Vector forceIJ = diff * scalar;
            forces[i] = forces[i] + forceIJ;
            forces[j] = forces[j] - forceIJ;
        }
    }
    return forces;
}


void algo_stromer_verlet(std::vector<Particule>& particleList,
                         std::vector<Vector> forces_fold,
                         double tEnd,
                         double dt) {
    std::vector<Vector> forces = initialiser(particleList);
    double t = 0.0;

    while (t < tEnd) {
        t += dt;
        for (size_t i = 0; i < particleList.size(); ++i) {
            Particule& p = particleList[i];
            Vector accel = forces[i] * (1.0 / p.getMasse());
            Vector halfStep = p.getVitesse() + accel * (0.5 * dt);
            Vector newPos = p.getPosition() + halfStep * dt;
            p.setPosition(newPos);
            forces_fold[i] = forces[i];
        }
        forces = initialiser(particleList);
        for (size_t i = 0; i < particleList.size(); ++i) {
            Particule& p = particleList[i];
            Vector newVel = p.getVitesse() + (forces[i] + forces_fold[i]) * ((0.5 * dt) / p.getMasse());
            p.setVitesse(newVel);
        }
        // const Vector& pos0 = particleList[0].getPosition();
        // std::cout << "t : " << t << " x : " << pos0.x() << " y : " << pos0.y() << "\n";
    }
}
