#ifndef UNIVERS_GRAVITATIONNEL_HPP
#define UNIVERS_GRAVITATIONNEL_HPP

#include "Univers.hpp"

/**
 * @brief Univers gravitationnel implémentant la loi de gravitation universelle F = G*m1*m2/r²
 * 
 * Cette classe hérite de Univers et implémente calculerForces() avec la loi gravitationnelle.
 */
class UniversGravitationnel : public Univers
{
public:
    /**
     * @brief Constructeur
     * @param dim Dimension de l'espace
     * @param reserveCount Nombre de particules à réserver (optionnel)
     */
    UniversGravitationnel(int dim, int reserveCount = 0);

    /**
     * @brief Calcule les forces gravitationnelles entre toutes les paires de particules
     * @return Un vecteur contenant les forces associées à chaque particule
     */
    std::vector<Vector> calculerForces() override;
};

#endif