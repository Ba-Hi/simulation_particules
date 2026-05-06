#include "UniversGravitationnel.hpp"
#include "Force.hpp"

UniversGravitationnel::UniversGravitationnel(int dim, int reserveCount)
    : Univers(dim, reserveCount) {}

std::vector<Vector> UniversGravitationnel::calculerForces(bool use_potentiel_reflexion, bool use_gravity) {
    
    size_t n = particuleList.size();
    std::vector<Vector> forces(n, Vector(0.0, 0.0, 0.0));

    for (size_t i = 0; i < n; ++i) {
        for (size_t j = i+1; j < n; ++j) {
            Vector forceIJ = Force::gravitation(particuleList[i], particuleList[j]);
            forces[i] = forces[i] + forceIJ;
            forces[j] = forces[j] - forceIJ;
        }
    }

    if (use_gravity) {
        for (size_t i = 0; i < n; ++i) {
            forces[i] += Force::poids(particuleList[i], gravity);
        }
    }
    return forces;
}