#include <iostream>
#include "Vector.hpp"
#include <cmath>
#include "Exceptions.hpp"


Vector::Vector(double x_init, double y_init, double z_init) :
        x_(x_init), y_(y_init), z_(z_init) {}

double Vector::norm2() const {
    return x_ * x_ + y_ * y_ + z_ * z_;
}


double Vector::norm() const {
    return std::sqrt(norm2());
}

Vector Vector::operator+(const Vector& autre) const {
        return Vector(x_ + autre.x(), y_ + autre.y() , z_+autre.z());
    }

Vector Vector::operator-(const Vector& autre) const {
        return Vector(x_ - autre.x(), y_ - autre.y() , z_ - autre.z());
    }

Vector Vector::operator*(double scalaire) const {
    return Vector(x_*scalaire, y_*scalaire, z_*scalaire);
}

Vector Vector::operator/(double scalar) const {
    if (scalar != 0) {
        return Vector(x_ / scalar , y_ / scalar , z_ / scalar);
    }
    else {
        throw MathException("Division d'un vecteur par zéro");
    }
}


Vector &Vector::operator=(const Vector& source){
    if (this != &source) {
    x_ = source.x();
    y_ = source.y();
    z_ = source.z();
    }
    return *this; // chainage
}

void Vector::afficher() const {
    std::cout << "Vector(" << x_ << ", " << y_ << ", " << z_ << ")" << std::endl;
}

Vector operator*(double s, const Vector&p) {
    return Vector(p.x() * s, p.y() * s, p.z() * s);
}

Vector operator/(double s, const Vector&p) {
        if (s != 0) {
        return Vector(p.x() / s, p.y() / s, p.z() / s);
    }
    else {
        throw MathException("Division d'un vecteur par zéro");
    }
    
}

Vector& Vector::operator+=(const Vector& v){
    x_ += v.x(); y_ += v.y(); z_ += v.z();
    return *this;
}

Vector& Vector::operator-=(const Vector& v){
    x_ -= v.x(); y_ -= v.y(); z_ -= v.z();
    return *this;
}
Vector& Vector::operator*=(double scalar){
    x_ *= scalar; y_ *= scalar; z_ *= scalar;
    return *this;
}

Vector& Vector::operator/=(double scalar){
    if (std::abs(scalar) < 1e-30) throw MathException("Division d'un vecteur par zéro");
    x_ /= scalar; y_ /= scalar; z_ /= scalar;
    return *this;
}

bool Vector::operator==(const Vector& v) const{
    return x_ == v.x_ && y_ == v.y_ && z_ == v.z_;
}   

double Vector::distance(const Vector& autre) const {
    double dx = x_ - autre.x();
    double dy = y_ - autre.y();
    double dz = z_ - autre.z();
    return std::sqrt(dx * dx + dy * dy + dz * dz);
}