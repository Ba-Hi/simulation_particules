#include "gtest/gtest.h"
#include "UniversLJ.hpp"
#include "Particule.hpp"
#include "Vector.hpp"
#include <cmath>

// Initialisation des cellules 

TEST(UniversTp4Test, NombreCellules2D) {
    // Lx=10, Ly=10, r_cut=2.5 → floor(10/2.5)=4 → 4*4=16 cellules
    UniversLJ u(2, Vector(10.0, 10.0, 0.0), 2.5, 1.0, 1.0);
    u.initialiserCellules();
    EXPECT_EQ(u.getNbCellules(), 16);
}

TEST(UniversTp4Test, VoisinesInterieure2D) {
    // Cellule intérieure (1,1) doit avoir 9 voisines (elle-même incluse)
    UniversLJ u(2, Vector(10.0, 10.0, 0.0), 2.5, 1.0, 1.0);
    u.initialiserCellules();
    EXPECT_EQ(u.getCellule(1, 1, 0).getVoisines().size(), 9);
}

TEST(UniversTp4Test, VoisinesCoin2D) {
    // Cellule coin (0,0) doit avoir 4 voisines
    UniversLJ u(2, Vector(10.0, 10.0, 0.0), 2.5, 1.0, 1.0);
    u.initialiserCellules();
    EXPECT_EQ(u.getCellule(0, 0, 0).getVoisines().size(), 4);
}

// mettreAJourCellules

TEST(UniversTp4Test, ParticuleDansBonneCellule) {
    UniversLJ u(2, Vector(10.0, 10.0, 0.0), 2.5, 1.0, 1.0);
    u.ajouterParticule(Particule(Vector(1.0, 1.0, 0.0), Vector(0,0,0), 1.0, 0, 0, Vector(0,0,0)));
    u.ajouterParticule(Particule(Vector(3.0, 3.0, 0.0), Vector(0,0,0), 1.0, 1, 0, Vector(0,0,0)));
    u.initialiserCellules();
    EXPECT_EQ(u.getCellule(0, 0, 0).getParticuleList().size(), 1);
    EXPECT_EQ(u.getCellule(1, 1, 0).getParticuleList().size(), 1);
}

TEST(UniversTp4Test, MiseAJourApresDeplacement) {
    UniversLJ u(2, Vector(10.0, 10.0, 0.0), 2.5, 1.0, 1.0);
    u.ajouterParticule(Particule(Vector(1.0, 1.0, 0.0), Vector(0,0,0), 1.0, 0, 0, Vector(0,0,0)));
    u.initialiserCellules();
    // Déplacer la particule vers la cellule (1,0)
    u.getParticules()[0].setPosition(Vector(3.0, 1.0, 0.0));
    u.mettreAJourCellules();
    EXPECT_EQ(u.getCellule(0, 0, 0).getParticuleList().size(), 0);
    EXPECT_EQ(u.getCellule(1, 0, 0).getParticuleList().size(), 1);
}

TEST(UniversTp4Test, ConditionLimiteReflexion) {
    UniversLJ u(2, Vector(10.0, 10.0, 0.0), 2.5, 1.0, 1.0);
    u.setConditionsLimites(Univers::ConditionLimite::REFLEXION,
                            Univers::ConditionLimite::REFLEXION);
    u.ajouterParticule(Particule(Vector(-1.0, 5.0, 0.0), Vector(-2.0, 0.0, 0.0), 1.0, 0, 0, Vector(0,0,0)));
    u.ajouterParticule(Particule(Vector(5.0, 11.0, 0.0), Vector(0.0, 2.0, 0.0), 1.0, 1, 0, Vector(0,0,0)));
    u.initialiserCellules();

    EXPECT_NEAR(u.getParticules()[0].getPosition().x(), 1.0, 1e-9);
    EXPECT_GT(u.getParticules()[0].getVitesse().x(), 0.0);
    EXPECT_NEAR(u.getParticules()[1].getPosition().y(), 9.0, 1e-9);
    EXPECT_LT(u.getParticules()[1].getVitesse().y(), 0.0);
}

TEST(UniversTp4Test, ConditionLimiteAbsorption) {
    UniversLJ u(2, Vector(10.0, 10.0, 0.0), 2.5, 1.0, 1.0);
    u.setConditionsLimites(Univers::ConditionLimite::ABSORPTION,
                            Univers::ConditionLimite::ABSORPTION);
    u.ajouterParticule(Particule(Vector(1.0, 1.0, 0.0), Vector(0,0,0), 1.0, 0, 0, Vector(0,0,0)));
    u.ajouterParticule(Particule(Vector(-1.0, 5.0, 0.0), Vector(0.0, 0.0, 0.0), 1.0, 1, 0, Vector(0,0,0)));
    u.initialiserCellules();

    EXPECT_EQ(u.getParticules().size(), 1);
    EXPECT_NEAR(u.getParticules()[0].getPosition().x(), 1.0, 1e-9);
}

TEST(UniversTp4Test, ConditionLimitePeriodique) {
    UniversLJ u(2, Vector(10.0, 10.0, 0.0), 2.5, 1.0, 1.1);
    u.setConditionsLimites(Univers::ConditionLimite::PERIODIQUE,
                            Univers::ConditionLimite::PERIODIQUE);
    u.ajouterParticule(Particule(Vector(-1.0, 5.0, 0.0), Vector(0.0, 0.0, 0.0), 1.0, 0, 0, Vector(0,0,0)));
    u.ajouterParticule(Particule(Vector(10.3, 7.0, 0.0), Vector(0.0, 0.0, 0.0), 1.0, 1, 0, Vector(0,0,0)));
    u.initialiserCellules();

    EXPECT_NEAR(u.getParticules()[0].getPosition().x(), 9.0, 1e-9);
    EXPECT_NEAR(u.getParticules()[1].getPosition().x(), 0.3, 1e-9);
}

// Forces de Lennard-Jones

TEST(UniversTp4Test, ForceNulleAuMinimum) {
    // r = 2^(1/6)*sigma → force nulle (minimum du potentiel LJ)
    double r_star = std::pow(2.0, 1.0/6.0);
    UniversLJ u(2, Vector(20.0, 20.0, 0.0), 2.5, 1.0, 1.0);
    u.ajouterParticule(Particule(Vector(10.0,        10.0, 0.0), Vector(0,0,0), 1.0, 0, 0, Vector(0,0,0)));
    u.ajouterParticule(Particule(Vector(10.0+r_star, 10.0, 0.0), Vector(0,0,0), 1.0, 1, 0, Vector(0,0,0)));
    u.initialiserCellules();
    auto forces = u.calculerForces();
    EXPECT_NEAR(forces[0].x(), 0.0, 1e-10);
    EXPECT_NEAR(forces[0].y(), 0.0, 1e-10);
    EXPECT_NEAR(forces[0].z(), 0.0, 1e-10);
}

TEST(UniversTp4Test, ForceRepulsiveProche) {
    // r < r* → répulsion : force sur p0 dans le sens -x
    UniversLJ u(2, Vector(20.0, 20.0, 0.0), 2.5, 1.0, 1.0);
    u.ajouterParticule(Particule(Vector(10.0,       10.0, 0.0), Vector(0,0,0), 1.0, 0, 0, Vector(0,0,0)));
    u.ajouterParticule(Particule(Vector(10.0 + 0.9, 10.0, 0.0), Vector(0,0,0), 1.0, 1, 0, Vector(0,0,0)));
    u.initialiserCellules();
    auto forces = u.calculerForces();
    std::cout << "Force p0: " << forces[0].x() << ", " << forces[0].y() << std::endl;
    std::cout << "Force p1: " << forces[1].x() << ", " << forces[1].y() << std::endl;
    EXPECT_LT(forces[0].x(), 0.0);
}

TEST(UniversTp4Test, ForceAttractiveLoin) {
    // r* < r < r_cut → attraction : force sur p0 dans le sens +x
    UniversLJ u(2, Vector(20.0, 20.0, 0.0), 2.5, 1.0, 1.0);
    u.ajouterParticule(Particule(Vector(10.0,       10.0, 0.0), Vector(0,0,0), 1.0, 0, 0, Vector(0,0,0)));
    u.ajouterParticule(Particule(Vector(10.0 + 1.5, 10.0, 0.0), Vector(0,0,0), 1.0, 1, 0, Vector(0,0,0)));
    u.initialiserCellules();
    auto forces = u.calculerForces();
    std::cout << "Force p0: " << forces[0].x() << ", " << forces[0].y() << std::endl;
    std::cout << "Force p1: " << forces[1].x() << ", " << forces[1].y() << std::endl;
    EXPECT_GT(forces[0].x(), 0.0);
}

TEST(UniversTp4Test, ForceNulleAuDela_rcut) {
    // r > r_cut → force nulle
    UniversLJ u(2, Vector(20.0, 20.0, 0.0), 2.5, 1.0, 1.0);
    u.ajouterParticule(Particule(Vector(10.0,       10.0, 0.0), Vector(0,0,0), 1.0, 0, 0, Vector(0,0,0)));
    u.ajouterParticule(Particule(Vector(10.0 + 3.0, 10.0, 0.0), Vector(0,0,0), 1.0, 1, 0, Vector(0,0,0)));
    u.initialiserCellules();
    auto forces = u.calculerForces();
    EXPECT_NEAR(forces[0].x(), 0.0, 1e-10);
}

TEST(UniversTp4Test, Newton3emeLoi) {
    // F_ij + F_ji = 0
    UniversLJ u(2, Vector(20.0, 20.0, 0.0), 2.5, 1.0, 1.0);
    u.ajouterParticule(Particule(Vector(10.0,       10.0, 0.0), Vector(0,0,0), 1.0, 0, 0, Vector(0,0,0)));
    u.ajouterParticule(Particule(Vector(10.0 + 1.5, 10.0, 0.0), Vector(0,0,0), 1.0, 1, 0, Vector(0,0,0)));
    u.initialiserCellules();
    auto forces = u.calculerForces();
    EXPECT_NEAR(forces[0].x() + forces[1].x(), 0.0, 1e-10);
}