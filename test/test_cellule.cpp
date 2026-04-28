#include "gtest/gtest.h"
#include "Cellule.hpp"
#include "Particule.hpp"
#include "Vector.hpp"

// Centre de cellule
TEST(CelluleTest, CentreCellule) {
    Cellule c(2.5, 1, 2, 0);
    Vector centre = c.centreCellule();
    EXPECT_NEAR(centre.x(), 3.75, 1e-8);
    EXPECT_NEAR(centre.y(), 6.25, 1e-8);
    EXPECT_NEAR(centre.z(), 1.25, 1e-8);
}

// Ajout de particules via pointeur
TEST(CelluleTest, AjouterParticule) {
    Cellule c(2.5, 0, 0, 0);
    Particule p(Vector(1,1,0), Vector(0,0,0), 1.0, 0, 0, Vector(0,0,0));
    c.ajouterParticule(&p);
    EXPECT_EQ(c.getParticuleList().size(), 1);
}

// Modification via pointeur répercutée sur la particule originale
TEST(CelluleTest, ModificationViaPointeur) {
    Cellule c(2.5, 0, 0, 0);
    Particule p(Vector(1,1,0), Vector(0,0,0), 1.0, 0, 0, Vector(0,0,0));
    c.ajouterParticule(&p);
    c.getParticuleList()[0]->setForce(Vector(1.0, 2.0, 3.0));
    EXPECT_DOUBLE_EQ(p.getForce().x(), 1.0);
    EXPECT_DOUBLE_EQ(p.getForce().y(), 2.0);
    EXPECT_DOUBLE_EQ(p.getForce().z(), 3.0);
}

// Vider les particules
TEST(CelluleTest, ViderParticules) {
    Cellule c(2.5, 0, 0, 0);
    Particule p(Vector(1,1,0), Vector(0,0,0), 1.0, 0, 0, Vector(0,0,0));
    c.ajouterParticule(&p);
    c.viderParticules();
    EXPECT_TRUE(c.getParticuleList().empty());
}

// Ajout de voisines
TEST(CelluleTest, AjouterVoisine) {
    Cellule c1(2.5, 0, 0, 0);
    Cellule c2(2.5, 1, 0, 0);
    c1.ajouterVoisine(&c2);
    EXPECT_EQ(c1.getVoisines().size(), 1);
    EXPECT_EQ(c1.getVoisines()[0], &c2);
}