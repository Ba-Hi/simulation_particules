#include "UniversLJ.hpp"
#include <iostream>
#include <cmath>
#include <algorithm>
#include "ExportCsv.hpp"
#include "Force.hpp"
#include "Exceptions.hpp"



UniversLJ::UniversLJ(const int& dimension, const Vector& l_d, const double& r_cut)
            : 
            Univers(dimension),
            l_d(l_d),
            r_cut(r_cut),
            epsilon(1.0),
            sigma(1.0),
            nx(dimension >= 1 ? (int)(l_d.x() / r_cut) : 1),
            ny(dimension >= 2 ? (int)(l_d.y() / r_cut) : 1),
            nz(dimension == 3 ? (int)(l_d.z() / r_cut) : 1),
            conditionX(Univers::ConditionLimite::ABSORPTION),
            conditionY(Univers::ConditionLimite::ABSORPTION),
            conditionZ(Univers::ConditionLimite::ABSORPTION),
            celluleList(){
    if (dimension > 3 || dimension < 1) {
        throw ConfigurationException("La dimension doit être 1, 2 ou 3.");
    }
    if (r_cut <= 0.0) {
        throw ConfigurationException("Le rayon de coupure (r_cut) doit être strictement positif.");
    }
    if (l_d.x() <= 0.0 || (dimension >= 2 && l_d.y() <= 0.0) || (dimension == 3 && l_d.z() <= 0.0)) {
        throw ConfigurationException("Les dimensions du domaine (l_d) doivent être strictement positives.");
    }
}

UniversLJ::UniversLJ(const int& dimension, const Vector& l_d, const double& r_cut, double epsilon, double sigma)
            : 
            Univers(dimension),
            l_d(l_d),
            r_cut(r_cut),
            epsilon(epsilon),
            sigma(sigma),
            nx(dimension >= 1 ? (int)(l_d.x() / r_cut) : 1),
            ny(dimension >= 2 ? (int)(l_d.y() / r_cut) : 1),
            // nx((int)(l_d.x() / r_cut)),
            // ny((int)(l_d.y() / r_cut)),
            nz(dimension == 3 ? (int)(l_d.z() / r_cut) : 1),
            conditionX(Univers::ConditionLimite::ABSORPTION),
            conditionY(Univers::ConditionLimite::ABSORPTION),
            conditionZ(Univers::ConditionLimite::ABSORPTION),
            celluleList(){
    if (dimension > 3 || dimension < 1) {
        throw ConfigurationException("La dimension doit être 1, 2 ou 3.");
    }
    if (r_cut <= 0.0) {
        throw ConfigurationException("Le rayon de coupure (r_cut) doit être strictement positif.");
    }
    if (l_d.x() <= 0.0 || (dimension >= 2 && l_d.y() <= 0.0) || (dimension == 3 && l_d.z() <= 0.0)) {
        throw ConfigurationException("Les dimensions du domaine (l_d) doivent être strictement positives.");
    }
}

// definir le vecteur gravity :


void UniversLJ::initialiserCellules() {
    celluleList.clear();

    int nbCellules = 0;
    if (dimension == 1) {
        nbCellules = nx;
    }
    else if (dimension == 2) {
        nbCellules = nx * ny;
    }
    else {
        nbCellules = nx * ny * nz;
    }

    celluleList.reserve(nbCellules);

    // Création des cellules :
    for (int i=0; i<nx; i++){
        for (int j=0; j<ny; j++){
            for (int k=0; k<nz; k++){
                celluleList.emplace_back(r_cut, i, j, k);
            }
        }
    }

    // Création des voisins :
    for (int i=0; i<nx; i++){
        for (int j=0; j<ny; j++){
            for (int k=0; k<nz; k++){
                Cellule& c = celluleList[indice1D(i, j, k)];
                for (int dk = -1; dk <= 1; dk++)
                    for (int dj = -1; dj <= 1; dj++)
                        for (int di = -1; di <= 1; di++) {
                            int ni = i + di, nj = j + dj, nk = k + dk;
                            if (ni >= 0 && ni < nx &&
                                nj >= 0 && nj < ny &&
                                nk >= 0 && nk < nz) c.ajouterVoisine(&celluleList[indice1D(ni, nj, nk)]);
               }
            }
        }
    }

    mettreAJourCellules();
}

int UniversLJ::indice1D(int i, int j, int k){
    if (i < 0 || i >= nx || j < 0 || j >= ny || k < 0 || k >= nz) {
        throw MathException("Indice de cellule hors limites de la grille.");
    }
    return i + nx * (j + ny * k);

}

Cellule& UniversLJ::getCellule(int i, int j, int k) {
    return celluleList[indice1D(i, j, k)];
}

size_t UniversLJ::getNbCellules() const {
    return celluleList.size();
}


std::vector<Vector> UniversLJ::calculerForces(bool use_potentiel_reflexion, bool use_gravity) {

    const size_t N = particuleList.size();
    std::vector<Vector> forces(N, Vector(0.0, 0.0, 0.0));

    const double r_cut2 = r_cut * r_cut;

    // Helper to convert pointer
    auto idx = [&](Particule* p) -> size_t {
        return static_cast<size_t>(p - &particuleList[0]);
    };

    for (int i = 0; i < nx; i++)
    for (int j = 0; j < ny; j++)
    for (int k = 0; k < nz; k++) {

        Cellule& ci = celluleList[indice1D(i, j, k)];
        const auto& parts = ci.getParticuleList();

        // intracell
        for (size_t a = 0; a < parts.size(); ++a) {
            Particule* pi_ptr = parts[a];

            for (size_t b = a + 1; b < parts.size(); ++b) {
                Particule* pj_ptr = parts[b];
                Vector fij = Force::lennardJones(*pi_ptr, *pj_ptr, epsilon, sigma, r_cut2);

                forces[idx(pi_ptr)] += fij;
                forces[idx(pj_ptr)] -= fij;
            }
        }

        // intercell
        for (Cellule* cj : ci.getVoisines()) {

            if (cj <= &ci) continue; // avoids double count

            const auto& voisins = cj->getParticuleList();

            for (Particule* pi_ptr : parts) {

                for (Particule* pj_ptr : voisins) {
                    Vector fij = Force::lennardJones(*pi_ptr, *pj_ptr, epsilon, sigma, r_cut2);

                    forces[idx(pi_ptr)] += fij;
                    forces[idx(pj_ptr)] -= fij;
                }
            }
        }
    }

    if (use_gravity) {
        for (size_t i = 0; i < N; ++i) {
            forces[i] += Force::poids(particuleList[i], gravity);
        }
    }

    // Update particle forces
    for (size_t i = 0; i < N; ++i) {
        particuleList[i].setForce(forces[i]);
    }

    if (use_potentiel_reflexion) {
        appliquerForcesParoi();
        for (size_t i = 0; i < N; ++i) {
            forces[i] = particuleList[i].getForce();
        }
    }

    return forces;
}


void UniversLJ::setConditionsLimites(Univers::ConditionLimite cx) {
    conditionX = cx;
}

void UniversLJ::setConditionsLimites(Univers::ConditionLimite cx, Univers::ConditionLimite cy) {
    conditionX = cx;
    conditionY = cy;
}

void UniversLJ::setConditionsLimites(Univers::ConditionLimite cx, Univers::ConditionLimite cy, Univers::ConditionLimite cz) {
    conditionX = cx;
    conditionY = cy;
    conditionZ = cz;
}

void UniversLJ::mettreAJourCellules(bool use_conditions_limites) {
    if (use_conditions_limites) {
        appliquerConditionsLimites(conditionX, conditionY, conditionZ);
    }


    for (Cellule& c : celluleList)
        c.viderParticules();

    for (Particule& p : particuleList) {
        Vector pos = p.getPosition();
        int i = (int)(pos.x() / r_cut);
        int j = (int)(pos.y() / r_cut);
        int k = (dimension == 3) ? (int)(pos.z() / r_cut) : 0;

        i = std::max(0, std::min(i, nx - 1));
        j = std::max(0, std::min(j, ny - 1));
        k = std::max(0, std::min(k, nz - 1));

        celluleList[indice1D(i, j, k)].ajouterParticule(&p);
    }
}



void UniversLJ::appliquerForcesParoi(){
    for (Particule& p : particuleList) {

        Vector pos = p.getPosition();
        double fx = 0.0, fy = 0.0, fz = 0.0;
        double x = pos.x(), y = pos.y(), z = pos.z();

        // Paroi x=0
        if (x < r_cut) {
            double d = x;
            fx += Force::paroiLennardJones(d, epsilon, sigma);
        }
        // Paroi x=Lx
        if (x > l_d.x() - r_cut) {
            double d = l_d.x() - x;
            fx -= Force::paroiLennardJones(d, epsilon, sigma);
        }

        if (dimension >= 2) {
            // Paroi y=0
            if (y < r_cut) {
                double d = y;
                fy += Force::paroiLennardJones(d, epsilon, sigma);
            }
            // Paroi y=Ly
            if (y > l_d.y() - r_cut) {
                double d = l_d.y() - y;
                fy -= Force::paroiLennardJones(d, epsilon, sigma);
            }

            if (dimension == 3) {
                // Paroi z=0
                if (z < r_cut) {
                    double d = z;
                    fz += Force::paroiLennardJones(d, epsilon, sigma);
                }
                // Paroi z=Lz
                if (z > l_d.z() - r_cut) {
                    double d = l_d.z() - z;
                    fz -= Force::paroiLennardJones(d, epsilon, sigma);
                }
            }
        }

        p.setForce(p.getForce() + Vector(fx, fy, fz));
    }
}

void UniversLJ::appliquerConditionsLimites(
        Univers::ConditionLimite cx,
        Univers::ConditionLimite cy,
        Univers::ConditionLimite cz) {

    auto appliquerAxe = [&](double& coord, double limit,
                             Univers::ConditionLimite cond,
                             double& vel) -> bool {
        if (cond == Univers::ConditionLimite::REFLEXION) {
            while (coord < 0.0 || coord >= limit) {
                coord = (coord < 0.0) ? -coord : 2.0*limit - coord;
                vel = -vel;
            }
        } else if (cond == Univers::ConditionLimite::PERIODIQUE) {
            if (limit > 0.0) { coord = std::fmod(coord, limit); if (coord < 0.0) coord += limit; }
        } else if (cond == Univers::ConditionLimite::ABSORPTION) {
            return (coord < 0.0 || coord >= limit);
        }
        return false;
    };

    bool anyAbsorption = (cx == Univers::ConditionLimite::ABSORPTION ||
                          cy == Univers::ConditionLimite::ABSORPTION ||
                          cz == Univers::ConditionLimite::ABSORPTION);

    auto it = std::remove_if(particuleList.begin(), particuleList.end(),
        [&](Particule& p) {
            double x = p.getPosition().x(), y = p.getPosition().y(), z = p.getPosition().z();
            double vx = p.getVitesse().x(), vy = p.getVitesse().y(), vz = p.getVitesse().z();

            bool absorb = appliquerAxe(x, l_d.x(), cx, vx);
            if (dimension >= 2) absorb |= appliquerAxe(y, l_d.y(), cy, vy);
            if (dimension == 3) absorb |= appliquerAxe(z, l_d.z(), cz, vz);

            if (!absorb) {
                p.setPosition(Vector(x, y, z));
                p.setVitesse(Vector(vx, vy, vz));
            }
            return absorb;
        });

    if (anyAbsorption) {
        particuleList.erase(it, particuleList.end());
        n_particules = particuleList.size();
    }
}


double UniversLJ::energieCinetique() const {
    double ec = 0.0;
    for (const auto& p : particuleList)
        ec += 0.5 * p.getMasse() * p.getVitesse().norm2();
    return ec;
}

double UniversLJ::energiePotentielle() const {
    double ep = 0.0;
    for (size_t i = 0; i < particuleList.size(); ++i) {
        for (size_t j = i+1; j < particuleList.size(); ++j) {
            double dist = (particuleList[i].getPosition()
                         - particuleList[j].getPosition()).norm();
            if (dist == 0.0 || dist > r_cut) continue;
            double s_r6 = pow(sigma / dist, 6);
            ep += 4.0 * epsilon * s_r6 * (s_r6 - 1.0);
        }
    }
    return ep;
}
