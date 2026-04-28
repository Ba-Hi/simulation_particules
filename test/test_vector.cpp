#include "Vector.hpp"
#include <gtest/gtest.h>

TEST(VectorTest, Construction) {
    Vector v{1.0, 2.0, 3.0};
    EXPECT_DOUBLE_EQ(v.x(), 1.0);
    EXPECT_DOUBLE_EQ(v.y(), 2.0);
    EXPECT_DOUBLE_EQ(v.z(), 3.0);
}

TEST(VectorTest, Addition) {
    Vector v1{1.0, 2.0, 3.0};
    Vector v2{4.0, 5.0, 6.0};
    Vector v3 = v1 + v2;
    EXPECT_EQ(v3, Vector(5.0, 7.0, 9.0));
}

TEST(VectorTest, Subtraction) {
    Vector v1{1.0, 2.0, 3.0};
    Vector v2{4.0, 5.0, 6.0};
    Vector v4 = v1 - v2;
    EXPECT_EQ(v4, Vector(-3.0, -3.0, -3.0));
}

TEST(VectorTest, ScalarMultiplicationLeft) {
    Vector v1{1.0, 2.0, 3.0};
    Vector v5 = 2.0 * v1;
    EXPECT_EQ(v5, Vector(2.0, 4.0, 6.0));
}

TEST(VectorTest, ScalarMultiplicationRight) {
    Vector v1{1.0, 2.0, 3.0};
    Vector v5 = v1 * 2.0;
    EXPECT_EQ(v5, Vector(2.0, 4.0, 6.0));
}

TEST(VectorTest, ScalarDivision) {
    Vector v1{1.0, 2.0, 3.0};
    Vector v8 = 2 / v1;
    EXPECT_EQ(v8, Vector(0.5, 1.0, 1.5));
}

TEST(VectorTest, ScalarDivisionRight) {
    Vector v1{1.0, 2.0, 3.0};
    Vector v8 = v1 / 2.0;
    EXPECT_EQ(v8, Vector(0.5, 1.0, 1.5));
}

TEST(VectorTest, DivisionByZeroThrows) {
    Vector v1{1.0, 2.0, 3.0};
    EXPECT_THROW(0.0 / v1, std::runtime_error);
}

TEST(VectorTest, DivisionByZeroRightThrows) {
    Vector v1{1.0, 2.0, 3.0};
    EXPECT_THROW(v1 / 0.0, std::runtime_error);
}

TEST(VectorTest, CopyConstructor) {
    Vector v1{1.0, 2.0, 3.0};
    Vector v7 = v1;
    EXPECT_DOUBLE_EQ(v7.x(), v1.x());
    EXPECT_DOUBLE_EQ(v7.y(), v1.y());
    EXPECT_DOUBLE_EQ(v7.z(), v1.z());
}

TEST(VectorTest, AssignmentOperator) {
    Vector v1{1.0, 2.0, 3.0};
    Vector v9{0.0, 0.0, 0.0};
    v9 = v1;
    EXPECT_DOUBLE_EQ(v9.x(), v1.x());
    EXPECT_DOUBLE_EQ(v9.y(), v1.y());
    EXPECT_DOUBLE_EQ(v9.z(), v1.z());
}

TEST(VectorTest, SelfAssignment) {
    Vector v1{1.0, 2.0, 3.0};
    v1 = v1; // Self-assignment
    EXPECT_DOUBLE_EQ(v1.x(), 1.0);
    EXPECT_DOUBLE_EQ(v1.y(), 2.0);
    EXPECT_DOUBLE_EQ(v1.z(), 3.0);

}

TEST(VectorTest, PlusEquals) {
    Vector v1{1.0, 2.0, 3.0};
    Vector v2{4.0, 5.0, 6.0};
    v1 += v2;
    EXPECT_EQ(v1, Vector(5.0, 7.0, 9.0));
}

TEST(VectorTest, MinusEquals) {
    Vector v1{1.0, 2.0, 3.0};
    Vector v2{4.0, 5.0, 6.0};
    v1 -= v2;
    EXPECT_EQ(v1, Vector(-3.0, -3.0, -3.0));
}

TEST(VectorTest, TimesEquals) {
    Vector v1{1.0, 2.0, 3.0};
    v1 *= 2.0;
    EXPECT_EQ(v1, Vector(2.0, 4.0, 6.0));
}

TEST(VectorTest, DivideEquals) {
    Vector v1{1.0, 2.0, 3.0};
    v1 /= 2.0;
    EXPECT_EQ(v1, Vector(0.5, 1.0, 1.5));
}

TEST(VectorTest, DivideEqualsByZeroThrows) {
    Vector v1{1.0, 2.0, 3.0};
    EXPECT_THROW(v1 /= 0.0, std::runtime_error);
}

TEST(VectorTest, Equality) {
    Vector v1{1.0, 2.0, 3.0};
    Vector v2{1.0, 2.0, 3.0};
    Vector v3{4.0, 5.0, 6.0};
    EXPECT_TRUE(v1 == v2);
    EXPECT_FALSE(v1 == v3);
}


