#include "gtest/gtest.h"
#include "Particule.hpp"
#include "Vector.hpp"
#include <cmath>

//Constructors

TEST(ParticuleTest, ConstructorSetsFields) {
    Particule p(Vector(1,2,3), Vector(4,5,6), 2.5, 7, 1, Vector(0.1, 0.2, 0.3));

    EXPECT_DOUBLE_EQ(p.getPosition().x(), 1.0);
    EXPECT_DOUBLE_EQ(p.getPosition().y(), 2.0);
    EXPECT_DOUBLE_EQ(p.getPosition().z(), 3.0);

    EXPECT_DOUBLE_EQ(p.getVitesse().x(), 4.0);
    EXPECT_DOUBLE_EQ(p.getVitesse().y(), 5.0);
    EXPECT_DOUBLE_EQ(p.getVitesse().z(), 6.0);

    EXPECT_DOUBLE_EQ(p.getMasse(), 2.5);

    EXPECT_DOUBLE_EQ(p.getForce().x(), 0.1);
    EXPECT_DOUBLE_EQ(p.getForce().y(), 0.2);
    EXPECT_DOUBLE_EQ(p.getForce().z(), 0.3);

    EXPECT_EQ(p.getType(), 1);
}

//Setters

TEST(ParticuleTest, SetPosition) {
    Particule p(Vector(0,0,0), Vector(0,0,0), 1.0, 0, 0, Vector(0,0,0));
    p.setPosition(Vector(3, 4, 5));
    EXPECT_DOUBLE_EQ(p.getPosition().x(), 3.0);
    EXPECT_DOUBLE_EQ(p.getPosition().y(), 4.0);
    EXPECT_DOUBLE_EQ(p.getPosition().z(), 5.0);
}

TEST(ParticuleTest, SetVitesse) {
    Particule p(Vector(0,0,0), Vector(0,0,0), 1.0, 0, 0, Vector(0,0,0));
    p.setVitesse(Vector(1, 2, 3));
    EXPECT_DOUBLE_EQ(p.getVitesse().x(), 1.0);
    EXPECT_DOUBLE_EQ(p.getVitesse().y(), 2.0);
    EXPECT_DOUBLE_EQ(p.getVitesse().z(), 3.0);
}

TEST(ParticuleTest, SetMasse) {
    Particule p(Vector(0,0,0), Vector(0,0,0), 1.0, 0, 0, Vector(0,0,0));
    p.setMasse(5.0);
    EXPECT_DOUBLE_EQ(p.getMasse(), 5.0);
}

TEST(ParticuleTest, SetForce) {
    Particule p(Vector(0,0,0), Vector(0,0,0), 1.0, 0, 0, Vector(0,0,0));
    p.setForce(Vector(7, 8, 9));
    EXPECT_DOUBLE_EQ(p.getForce().x(), 7.0);
    EXPECT_DOUBLE_EQ(p.getForce().y(), 8.0);
    EXPECT_DOUBLE_EQ(p.getForce().z(), 9.0);
}

TEST(ParticuleTest, SetType) {
    Particule p(Vector(0,0,0), Vector(0,0,0), 1.0, 0, 0, Vector(0,0,0));
    p.setType(3);
    EXPECT_EQ(p.getType(), 3);
}

//set chaining

TEST(ParticuleTest, SetterChaining) {
    Particule p(Vector(0,0,0), Vector(0,0,0), 1.0, 0, 0, Vector(0,0,0));
    p.setPosition(Vector(1,2,3))
     .setVitesse(Vector(4,5,6))
     .setMasse(9.0)
     .setForce(Vector(7,8,9))
     .setType(2);

    EXPECT_DOUBLE_EQ(p.getPosition().x(), 1.0);
    EXPECT_DOUBLE_EQ(p.getVitesse().x(),  4.0);
    EXPECT_DOUBLE_EQ(p.getMasse(),         9.0);
    EXPECT_DOUBLE_EQ(p.getForce().x(),    7.0);
    EXPECT_EQ(p.getType(), 2);
}

//initialiser

TEST(ParticuleTest, InitialiserEmptyList) {
    std::vector<Particule> list;
    auto forces = initialiser(list);
    EXPECT_TRUE(forces.empty());
}

TEST(ParticuleTest, InitialiserOneParticule) {
    std::vector<Particule> list = {
        Particule(Vector(0,0,0), Vector(0,0,0), 1.0, 0, 0, Vector(0,0,0))
    };
    auto forces = initialiser(list);
    EXPECT_EQ(forces.size(), 1);
    EXPECT_DOUBLE_EQ(forces[0].x(), 0.0);
    EXPECT_DOUBLE_EQ(forces[0].y(), 0.0);
    EXPECT_DOUBLE_EQ(forces[0].z(), 0.0);
}

TEST(ParticuleTest, InitialiserForceSizeMatchesParticules) {
    std::vector<Particule> list;
    for (int i = 0; i < 5; ++i)
        list.push_back(Particule(Vector(i,0,0), Vector(0,0,0), 1.0, i, 0, Vector(0,0,0)));
    auto forces = initialiser(list);
    EXPECT_EQ(forces.size(), list.size());
}

TEST(ParticuleTest, InitialiserForceSymmetry) {
    std::vector<Particule> list = {
        Particule(Vector(0,0,0), Vector(0,0,0), 1.0, 0, 0, Vector(0,0,0)),
        Particule(Vector(1,0,0), Vector(0,0,0), 1.0, 1, 0, Vector(0,0,0)),
    };
    auto forces = initialiser(list);
    EXPECT_NEAR(forces[0].x() + forces[1].x(), 0.0, 1e-10);
    EXPECT_NEAR(forces[0].y() + forces[1].y(), 0.0, 1e-10);
    EXPECT_NEAR(forces[0].z() + forces[1].z(), 0.0, 1e-10);
}

TEST(ParticuleTest, InitialiserForceDirection) {
    // p0 at origin, p1 to the right → p0 attracted rightward (+x)
    std::vector<Particule> list = {
        Particule(Vector(0,0,0), Vector(0,0,0), 1.0, 0, 0, Vector(0,0,0)),
        Particule(Vector(1,0,0), Vector(0,0,0), 1.0, 1, 0, Vector(0,0,0)),
    };
    auto forces = initialiser(list);
    EXPECT_GT(forces[0].x(), 0.0);   // p0 pulled toward p1 (+x)
    EXPECT_LT(forces[1].x(), 0.0);   // p1 pulled toward p0 (-x)
}

TEST(ParticuleTest, InitialiserCoincidentParticules) {
    // Two particles at same position — should not crash (dist = 0 guard)
    std::vector<Particule> list = {
        Particule(Vector(0,0,0), Vector(0,0,0), 1.0, 0, 0, Vector(0,0,0)),
        Particule(Vector(0,0,0), Vector(0,0,0), 1.0, 1, 0, Vector(0,0,0)),
    };
    EXPECT_NO_THROW(initialiser(list));
}

//algo_stromer_verlet

TEST(ParticuleTest, VerletPositionChanges) {
    std::vector<Particule> list = {
        Particule(Vector(0,0,0),  Vector(1,0,0), 1.0, 0, 0, Vector(0,0,0)),
        Particule(Vector(100,0,0), Vector(0,0,0), 1.0, 1, 0, Vector(0,0,0)),
    };
    std::vector<Vector> f0 = initialiser(list);
    double before = list[0].getPosition().x();
    algo_stromer_verlet(list, f0, 0.1, 0.01);
    EXPECT_NE(list[0].getPosition().x(), before);
}
