#include "gtest/gtest.h"
#include <vector>
#include "Particule.hpp"
#include "Vector.hpp"
#include "UniversGravitationnel.hpp"
#include <cstdlib>

#include <cmath>
#include <iostream>
#include <chrono> // Pour la mesure du temps

TEST(UniversTest, CreationEmptyUnivers) {
    UniversGravitationnel u(3);
    EXPECT_EQ(u.getDimension(), 3);
    EXPECT_EQ(u.getNombreParticules(), 0);
    EXPECT_TRUE(u.getParticules().empty());
}

TEST(UniversTest, CreationUnivers) {
    UniversGravitationnel u(3, pow(2, 15));
    EXPECT_EQ(u.getNombreParticules(), 0);
    EXPECT_GE(u.getParticules().capacity(), pow(2, 15));
}

// Ajout de particules et vérification du nombre de particules
TEST(UniversTest, AddOneParticule) {
    UniversGravitationnel u(3);
    Particule p(Vector(1,2,3), Vector(0,0,0), 1.0, 0, 0, Vector(0,0,0));
    u.ajouterParticule(p);
    EXPECT_EQ(u.getNombreParticules(), 1);
    EXPECT_EQ(u.getParticules().size(), 1);
}

TEST(UniversTest, AddMultipleParticules) {
    UniversGravitationnel u(3);
    for (int i = 0; i < 100; ++i)
        u.ajouterParticule(Particule(Vector(i,0,0), Vector(0,0,0), 1.0, i, 0, Vector(0,0,0)));
    EXPECT_EQ(u.getNombreParticules(), 100);
}

TEST(UniversTest, CorrectAddParticulesPosition) {
    UniversGravitationnel u(3);
    Particule p(Vector(1,2,3), Vector(0,0,0), 1.0, 0, 0, Vector(0,0,0));
    u.ajouterParticule(p);
    EXPECT_DOUBLE_EQ(u.getParticules()[0].getPosition().x(), 1.0);
    EXPECT_DOUBLE_EQ(u.getParticules()[0].getPosition().y(), 2.0);
    EXPECT_DOUBLE_EQ(u.getParticules()[0].getPosition().z(), 3.0);
}

// Reserve performance test

TEST(UniversTest, ReserveIsFaster) {
    int n = 32768;

    auto t0 = std::chrono::high_resolution_clock::now();
    UniversGravitationnel u1(3, 0);
    for (int i = 0; i < n; ++i)
        u1.ajouterParticule(Particule(Vector(0,0,0), Vector(0,0,0), 1.0, i, 0, Vector(0,0,0)));
    double sans = std::chrono::duration<double>(
        std::chrono::high_resolution_clock::now() - t0).count();

    auto t2 = std::chrono::high_resolution_clock::now();
    UniversGravitationnel u2(3, n);
    for (int i = 0; i < n; ++i)
        u2.ajouterParticule(Particule(Vector(0,0,0), Vector(0,0,0), 1.0, i, 0, Vector(0,0,0)));
    double avec = std::chrono::duration<double>(
        std::chrono::high_resolution_clock::now() - t2).count();

    std::cout << "Sans reserve: " << sans << "s  Avec reserve: " << avec << "s\n";
    EXPECT_LT(avec, sans);
}

// Forces test

TEST(UniversTest, ForcesEmptyUnivers) {
    UniversGravitationnel u(3);
    EXPECT_TRUE(u.calculerForces().empty());
}

TEST(UniversTest, ForcesOneParticule) {
    UniversGravitationnel u(3);
    Particule p(Vector(0,0,0), Vector(0,0,0), 1.0, 0, 0, Vector(0,0,0));
    u.ajouterParticule(p);
    std::vector<Vector> forces = u.calculerForces();
    EXPECT_EQ(forces.size(), 1);
    EXPECT_DOUBLE_EQ(forces[0].x(), 0.0);
    EXPECT_DOUBLE_EQ(forces[0].y(), 0.0);
    EXPECT_DOUBLE_EQ(forces[0].z(), 0.0);
}

TEST(UniversTest, ForcesTwoParticules) {
    UniversGravitationnel u(3);
    u.ajouterParticule(Particule(Vector(0,0,0), Vector(0,0,0), 1.0, 0, 0, Vector(0,0,0)));
    u.ajouterParticule(Particule(Vector(1,0,0), Vector(0,0,0), 1.0, 1, 0, Vector(0,0,0)));
    auto forces = u.calculerForces();

    // G * m1 * m2 / r² = 1 * 1 * 1 / 1²
    double G = 1; // On considère G = 1 au lieu de 6.67430e-11;
    EXPECT_NEAR(forces[0].x(), G, 1e-15);   // attracted toward pj (+x)
    EXPECT_NEAR(forces[1].x(), -G, 1e-15);  // attracted toward pi (-x)
}

TEST(UniversTest, ForceSymetry) {
    UniversGravitationnel u(3);
    u.ajouterParticule(Particule(Vector(0,0,0), Vector(0,0,0), 1.0, 0, 0, Vector(0,0,0)));
    u.ajouterParticule(Particule(Vector(1,0,0), Vector(0,0,0), 1.0, 1, 0, Vector(0,0,0)));
    auto forces = u.calculerForces();
    EXPECT_EQ(forces.size(), 2);
    EXPECT_NEAR(forces[0].x() + forces[1].x(), 0.0, 1e-10);
    EXPECT_NEAR(forces[0].y() + forces[1].y(), 0.0, 1e-10);
    EXPECT_NEAR(forces[0].z() + forces[1].z(), 0.0, 1e-10);
}

TEST(UniversTest, CorrectForceSize) {
    UniversGravitationnel u(3);
    for (int i = 0; i < 5; ++i)
        u.ajouterParticule(Particule(Vector(i,0,0), Vector(0,0,0), 1.0, i, 0, Vector(0,0,0)));
    EXPECT_EQ(u.calculerForces().size(), 5);
}

// Avancer particules test

TEST(UniversTest, AvancerParticulesEmptyUnivers) {
    UniversGravitationnel u(3);
    EXPECT_NO_THROW(u.avancerParticules(1.0, 0.1));
}












// SPEEEEEED

/*
int main() {
    UniversGravitationnel u(3, pow(2, 15));

    // creation de 2^15 particules uniformément distribuées sur le cube [0; 1] × [0; 1] × [0; 1].
    for (int i = 0; i < pow(2, 15); ++i) {
        double x = static_cast<double>(rand()) / RAND_MAX;
        double y = static_cast<double>(rand()) / RAND_MAX;
        double z = static_cast<double>(rand()) / RAND_MAX;
        Vector position(x, y, z);
        Vector vitesse(0.0, 0.0, 0.0);
        double masse = 1.0;
        int identifiant = i;
        int categorie = 0;
        Vector force(0.0, 0.0, 0.0);

        Particule p(position, vitesse, masse, identifiant, categorie, force);
        u.ajouterParticule(p);
    }

    std::cout << "UniversGravitationnel créé avec " << u.getNombreParticules() << " particules." << std::endl;

    int n = 32768; // 2^15


    // sans reserve
    UniversGravitationnel u1(3, 0); 
    auto start1 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < n; ++i) {
        u1.ajouterParticule(Particule(Vector(0,0,0), Vector(0,0,0), 1.0, i, 0, Vector(0,0,0)));
    }
    auto end1 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff1 = end1 - start1;
    std::cout << "Temps SANS reserve: " << diff1.count() << " s" << std::endl;


    // avec reserve
    UniversGravitationnel u2(3, n); 
    auto start2 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < n; ++i) {
        u2.ajouterParticule(Particule(Vector(0,0,0), Vector(0,0,0), 1.0, i, 0, Vector(0,0,0)));
    }
    auto end2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff2 = end2 - start2;
    std::cout << "Temps AVEC reserve: " << diff2.count() << " s" << std::endl;



    for (int k = 1; k<8; k++) {
        auto start1 = std::chrono::high_resolution_clock::now();
        int n = pow(8, k);
        UniversGravitationnel u(3, n);
        for (int i = 0; i < n; ++i) {
            u1.ajouterParticule(Particule(Vector(0,0,0), Vector(0,0,0), 1.0, i, 0, Vector(0,0,0)));
        }
        auto end1 = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> diff1 = end1 - start1;
        std::cout << "Temps pour :" << n << " particules est " << diff1.count() << " s" << std::endl;

    }

    return 0;
}*/
