#include "Cellule.hpp"

Cellule::Cellule(const double tailleCellule, int i, int j, int k)
    : tailleCellule(tailleCellule), i(i), j(j), k(k)
{}

double Cellule::getTailleCellule() const {
    return tailleCellule;
}

std::vector<Particule*>& Cellule::getParticuleList() {
    return particuleList;
}

Cellule& Cellule::setTailleCellule(const double& taille) {
    tailleCellule = taille;
    return *this;
}

Cellule& Cellule::setListParticule(const std::vector<Particule*>& listPart) {
    particuleList = listPart;
    return *this;
}

Vector Cellule::getPositionCellule() const {
    return Vector(i, j, k);
}

Vector Cellule::centreCellule() const {
    return Vector(
        i * tailleCellule + tailleCellule / 2.0,
        j * tailleCellule + tailleCellule / 2.0,
        k * tailleCellule + tailleCellule / 2.0
    );
}

void Cellule::ajouterVoisine(Cellule* voisine) {
    voisines.push_back(voisine);
}

void Cellule::ajouterParticule(Particule* p) {  // pas de const
    particuleList.push_back(p);
}

std::vector<Cellule*>& Cellule::getVoisines() {
    return voisines;
}

void Cellule::viderParticules() {
    particuleList.clear();
}