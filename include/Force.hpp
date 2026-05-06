#ifndef FORCE_HPP
#define FORCE_HPP

#include "Particule.hpp"
#include "Vector.hpp"
#include <cmath>

/**
 * @brief Classe utilitaire gérant toutes les formules physiques de forces
 * 
 * Isoler ces calculs permet de séparer la responsabilité des calculs physiques 
 * de la responsabilité algorithmique du parcours 
 * de l'espace (doubles boucles, cellules).
 */

class Force {
public:
    /**
     * @brief Calcule la force gravitationnelle exercée par p2 sur p1
     * @param p1 Première particule
     * @param p2 Deuxième particule
     * @return Vecteur de force exercée sur p1 par p2
     */
    static inline Vector gravitation(const Particule& p1, const Particule& p2) {
        Vector diff = p2.getPosition() - p1.getPosition();
        double distSq = diff.norm2();
        if (distSq == 0.0) return Vector(0.0, 0.0, 0.0);
        
        double invDistCubed = 1.0 / (distSq * diff.norm());
        double scalar = p1.getMasse() * p2.getMasse() * invDistCubed;
        return diff * scalar;
    }

    /**
     * @brief Calcule la force de Lennard-Jones exercée par p2 sur p1
     * @param p1 Première particule
     * @param p2 Deuxième particule
     * @param epsilon Paramètre epsilon du potentiel LJ
     * @param sigma Paramètre sigma du potentiel LJ
     * @param r_cut2 Distance de coupure au carré
     * @return Vecteur de force exercée sur p1 par p2
     */
    static inline Vector lennardJones(const Particule& p1, const Particule& p2, double epsilon, double sigma, double r_cut2) {
        Vector diff = p2.getPosition() - p1.getPosition();
        double dist2 = diff.norm2() + 1e-12; // Évite la division par zéro
        
        if (dist2 > r_cut2) return Vector(0.0, 0.0, 0.0);

        double sr2 = (sigma * sigma) / dist2;
        double sr6 = sr2 * sr2 * sr2;
        double f = (24.0 * epsilon) * sr6 * (1.0 - 2.0 * sr6) / dist2;
        return diff * f;
    }

    /**
     * @brief Calcule la force de répulsion d'une paroi selon Lennard-Jones
     * @param distance Distance à la paroi
     * @param epsilon Paramètre epsilon du potentiel LJ
     * @param sigma Paramètre sigma du potentiel LJ
     * @return Force de répulsion exercée par la paroi
     */
    static inline double paroiLennardJones(double distance, double epsilon, double sigma) {
        if (distance <= 0.0) return 0.0;
        const double s2r2 = (sigma * sigma) / (4.0 * distance * distance);
        const double sr6 = s2r2 * s2r2 * s2r2;
        return (24.0 * epsilon / distance) * sr6 * (2.0 * sr6 - 1.0);
    }

    /**
     * @brief Calcule la force du poids exercée par un champ de gravité uniforme
     * @param p Particule pour laquelle calculer la force de poids
     * @param gravity Champ de gravité uniforme
     * @return Force de poids exercée sur la particule
     */
    static inline Vector poids(const Particule& p, const Vector& gravity) {
        return gravity * p.getMasse();
    }
};

#endif