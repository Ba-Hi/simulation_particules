#include "UniversGravitationnel.hpp"

UniversGravitationnel::UniversGravitationnel(int dim, int reserveCount)
    : Univers(dim, reserveCount) {}

std::vector<Vector> UniversGravitationnel::calculerForces() {
    
    size_t n = particuleList.size();
    std::vector<Vector> forces(n, Vector(0.0, 0.0, 0.0));

    for (size_t i = 0; i < n; ++i) {
        for (size_t j = i+1; j < n; ++j) {
            const Vector& pi = particuleList[i].getPosition();
            const Vector& pj = particuleList[j].getPosition();
            Vector diff = pj - pi;
            double distSq = diff.norm2();
            if (distSq == 0.0) continue;
            double invDistCubed = 1.0 / (distSq * diff.norm());
            double scalar = particuleList[i].getMasse() * particuleList[j].getMasse() * invDistCubed;
            Vector forceIJ = diff * scalar;
            forces[i] = forces[i] + forceIJ;
            forces[j] = forces[j] - forceIJ;
        }
    }
    return forces;
}