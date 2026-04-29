#include "gtest/gtest.h"
#include "UniversLJ.hpp"
#include "Particule.hpp"
#include "Vector.hpp"
#include <cmath>

#include <iostream>


TEST(UniversLJ1DTest, CreationUnivers1D) {
    UniversLJ u(1, Vector(10), 2.5, 1.0, 1.0);
    EXPECT_EQ(u.getDimension(), 1);
    EXPECT_EQ(u.getNombreParticules(), 0);
    EXPECT_TRUE(u.getParticules().empty());
}


TEST(UniversLJ1DTest, NombreCellules1D) {
    // Lx=10, r_cut=2.5 : floor(10/2.5)=4 cellules
    UniversLJ u(1, Vector(10.0, 0.0, 0.0), 2.5, 1.0, 1.0);
    u.initialiserCellules();
    EXPECT_EQ(u.getNbCellules(), 4);
}

TEST(UniversLJ1DTest, VoisinesCelluleInterieure1D) {
    // Cellule intérieure (1,0,0) : voisines à gauche, elle-même, à droite = 3
    UniversLJ u(1, Vector(10.0, 0.0, 0.0), 2.5, 1.0, 1.0);
    u.initialiserCellules();
    EXPECT_EQ(u.getCellule(1, 0, 0).getVoisines().size(), 3);
}

TEST(UniversLJ1DTest, VoisinesCelluleBord1D) {
    // Cellule bord (0,0,0) : elle-même + droite = 2
    UniversLJ u(1, Vector(10.0, 0.0, 0.0), 2.5, 1.0, 1.0);
    u.initialiserCellules();
    EXPECT_EQ(u.getCellule(0, 0, 0).getVoisines().size(), 2);
}


TEST(UniversLJ1DTest, ParticuleDansBonneCellule1D) {
    // r_cut=2.5 : cellule 0 = [0, 2.5[, cellule 1 = [2.5, 5.0[
    UniversLJ u(1, Vector(10.0, 0.0, 0.0), 2.5, 1.0, 1.0);
    u.ajouterParticule(Particule(Vector(1.0, 0.0, 0.0), Vector(0,0,0), 1.0, 0, 0, Vector(0,0,0)));
    u.ajouterParticule(Particule(Vector(3.0, 0.0, 0.0), Vector(0,0,0), 1.0, 1, 0, Vector(0,0,0)));
    u.initialiserCellules();
    EXPECT_EQ(u.getCellule(0, 0, 0).getParticuleList().size(), 1);
    EXPECT_EQ(u.getCellule(1, 0, 0).getParticuleList().size(), 1);
}


TEST(UniversLJ1DTest, MiseAJourApresDeplacement1D) {
    //
    UniversLJ u(1, Vector(10.0, 0.0, 0.0), 2.5, 1.0, 1.0);
    u.ajouterParticule(Particule(Vector(1.0, 0.0, 0.0), Vector(0,0,0), 1.0, 0, 0, Vector(0,0,0)));
    u.initialiserCellules();
    u.getParticules()[0].setPosition(Vector(3.5, 0.0, 0.0));
    u.mettreAJourCellules();
    EXPECT_EQ(u.getCellule(0, 0, 0).getParticuleList().size(), 0);
    EXPECT_EQ(u.getCellule(1, 0, 0).getParticuleList().size(), 1);
}


// conditions limites
TEST(UniversLJ1DTest, AbsorptionHorsLimite1D) {
    UniversLJ u(1, Vector(10.0, 0.0, 0.0), 2.5, 1.0, 1.0);
    u.setConditionsLimites(Univers::ConditionLimite::ABSORPTION,
                           Univers::ConditionLimite::ABSORPTION);
    u.ajouterParticule(Particule(Vector(5.0, 0.0, 0.0), Vector(0,0,0), 1.0, 0, 0, Vector(0,0,0)));
    u.ajouterParticule(Particule(Vector(-2.0, 0.0, 0.0), Vector(0,0,0), 1.0, 1, 0, Vector(0,0,0)));
    u.initialiserCellules();
    EXPECT_EQ(u.getParticules().size(), 1);
}

TEST(UniversLJ1DTest, ReflexionBordGauche1D) {
    UniversLJ u(1, Vector(10.0, 0.0, 0.0), 2.5, 1.0, 1.0);
    u.setConditionsLimites(Univers::ConditionLimite::REFLEXION,
                           Univers::ConditionLimite::REFLEXION);
    u.ajouterParticule(Particule(Vector(-1.0, 0.0, 0.0), Vector(-3.0, 0.0, 0.0), 1.0, 0, 0, Vector(0,0,0)));
    u.initialiserCellules();
    EXPECT_NEAR(u.getParticules()[0].getPosition().x(), 1.0, 1e-9);
    EXPECT_GT(u.getParticules()[0].getVitesse().x(), 0.0);
}

TEST(UniversLJ1DTest, PeriodiqueX1D) {
    UniversLJ u(1, Vector(10.0, 0.0, 0.0), 2.5, 1.0, 1.0);
    u.setConditionsLimites(Univers::ConditionLimite::PERIODIQUE,
                           Univers::ConditionLimite::PERIODIQUE);
    u.ajouterParticule(Particule(Vector(-1.0, 0.0, 0.0), Vector(0,0,0), 1.0, 0, 0, Vector(0,0,0)));
    u.ajouterParticule(Particule(Vector(10.5, 0.0, 0.0), Vector(0,0,0), 1.0, 1, 0, Vector(0,0,0)));
    u.initialiserCellules();
    EXPECT_NEAR(u.getParticules()[0].getPosition().x(), 9.0, 1e-9);
    EXPECT_NEAR(u.getParticules()[1].getPosition().x(), 0.5, 1e-9);
}


// forces
TEST(UniversLJ1DTest, ForceNulleAuMinimum1D) {
    double r_star = std::pow(2.0, 1.0/6.0);
    UniversLJ u(1, Vector(20.0, 0.0, 0.0), 2.5, 1.0, 1.0);
    u.ajouterParticule(Particule(Vector(10.0,          0.0, 0.0), Vector(0,0,0), 1.0, 0, 0, Vector(0,0,0)));
    u.ajouterParticule(Particule(Vector(10.0 + r_star, 0.0, 0.0), Vector(0,0,0), 1.0, 1, 0, Vector(0,0,0)));
    u.initialiserCellules();
    auto forces = u.calculerForces();
    EXPECT_NEAR(forces[0].x(), 0.0, 1e-10);
    EXPECT_NEAR(forces[0].y(), 0.0, 1e-10);
    EXPECT_NEAR(forces[0].z(), 0.0, 1e-10);
}

TEST(UniversLJ1DTest, ForceRepulsiveProche1D) {
    UniversLJ u(1, Vector(20.0, 0.0, 0.0), 2.5, 1.0, 1.0);
    u.ajouterParticule(Particule(Vector(10.0,       0.0, 0.0), Vector(0,0,0), 1.0, 0, 0, Vector(0,0,0)));
    u.ajouterParticule(Particule(Vector(10.0 + 0.9, 0.0, 0.0), Vector(0,0,0), 1.0, 1, 0, Vector(0,0,0)));
    u.initialiserCellules();
    auto forces = u.calculerForces();
    EXPECT_LT(forces[0].x(), 0.0);   // répulsion → p0 poussé vers -x
    EXPECT_GT(forces[1].x(), 0.0);   // p1 poussé vers +x
}

TEST(UniversLJ1DTest, ForceAttractiveLoin1D) {
    UniversLJ u(1, Vector(20.0, 0.0, 0.0), 2.5, 1.0, 1.0);
    u.ajouterParticule(Particule(Vector(10.0,       0.0, 0.0), Vector(0,0,0), 1.0, 0, 0, Vector(0,0,0)));
    u.ajouterParticule(Particule(Vector(10.0 + 1.5, 0.0, 0.0), Vector(0,0,0), 1.0, 1, 0, Vector(0,0,0)));
    u.initialiserCellules();
    auto forces = u.calculerForces();
    EXPECT_GT(forces[0].x(), 0.0);   // attraction → p0 tiré vers +x
}

TEST(UniversLJ1DTest, ForceNulleAuDela_rcut1D) {
    UniversLJ u(1, Vector(20.0, 0.0, 0.0), 2.5, 1.0, 1.0);
    u.ajouterParticule(Particule(Vector(10.0,       0.0, 0.0), Vector(0,0,0), 1.0, 0, 0, Vector(0,0,0)));
    u.ajouterParticule(Particule(Vector(10.0 + 3.0, 0.0, 0.0), Vector(0,0,0), 1.0, 1, 0, Vector(0,0,0)));
    u.initialiserCellules();
    auto forces = u.calculerForces();
    EXPECT_NEAR(forces[0].x(), 0.0, 1e-10);
}

TEST(UniversLJ1DTest, Newton3emeLoi1D) {
    UniversLJ u(1, Vector(20.0, 0.0, 0.0), 2.5, 1.0, 1.0);
    u.ajouterParticule(Particule(Vector(10.0,       0.0, 0.0), Vector(0,0,0), 1.0, 0, 0, Vector(0,0,0)));
    u.ajouterParticule(Particule(Vector(10.0 + 1.5, 0.0, 0.0), Vector(0,0,0), 1.0, 1, 0, Vector(0,0,0)));
    u.initialiserCellules();
    auto forces = u.calculerForces();
    EXPECT_NEAR(forces[0].x() + forces[1].x(), 0.0, 1e-10);
}

// energies
TEST(UniversLJ1DTest, EnergiesCinetique1D) {
    UniversLJ u(1, Vector(20.0, 0.0, 0.0), 2.5, 1.0, 1.0);
    // v = 3 → Ec = 0.5 * 1 * 9 = 4.5
    u.ajouterParticule(Particule(Vector(5.0, 0.0, 0.0), Vector(3.0, 0.0, 0.0), 1.0, 0, 0, Vector(0,0,0)));
    u.initialiserCellules();
    EXPECT_NEAR(u.energieCinetique(), 4.5, 1e-10);
}

TEST(UniversLJ1DTest, EnergiesPotentielleZeroLoin1D) {
    UniversLJ u(1, Vector(40.0, 0.0, 0.0), 2.5, 1.0, 1.0);
    u.ajouterParticule(Particule(Vector(5.0,  0.0, 0.0), Vector(0,0,0), 1.0, 0, 0, Vector(0,0,0)));
    u.ajouterParticule(Particule(Vector(20.0, 0.0, 0.0), Vector(0,0,0), 1.0, 1, 0, Vector(0,0,0)));
    u.initialiserCellules();
    EXPECT_NEAR(u.energiePotentielle(), 0.0, 1e-10);
}

