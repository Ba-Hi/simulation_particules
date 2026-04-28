#ifndef PARTICULE_HPP
#define PARTICULE_HPP

#include "Vector.hpp"
#include <vector>

/**
 * @brief Représente une particule dans un espace 3D avec position (x, y, z)
 * 
 * Chaque particule possède une position, une vitesse, une force appliquée,
 * une masse, un identifiant unique et une catégorie.
 */

class Particule
{
private:
    Vector position; ///< Position de la particule en 3D (x, y, z)
    Vector vitesse; ///< Vitesse de la particule (v_x, v_y, v_z)
    Vector force; ///< Force appliquée sur la particule (F_x, F_y, F_z)
    double masse; ///< Masse de la particule
    int identifiant; ///< Identifiant de la particule
    int categorie; ///< Catégorie de la particule


public:

    /**
     * @brief Constructeur de Particule : construit une particule avec ses paramètres initiaux
     * 
     * @param position    Position initiale de la particule.
     * @param vitesse     Vitesse initiale de la particule.
     * @param masse       Masse de la particule (doit être strictement positive).
     * @param identifiant Identifiant unique de la particule.
     * @param categorie   Catégorie de la particule.
     * @param force       Force initiale appliquée sur la particule.
     */
    Particule(const Vector& position, const Vector& vitesse, double masse,
              int identifiant, int categorie, const Vector& force);

    
    /**
     * @brief Modifie la position de la particule.
     * @param newPosition Nouvelle position.
     * @return Référence sur l'objet courant.
     */
    Particule& setPosition(const Vector& newPosition);

    /**
     * @brief Retourne la position de la particule.
     * @return Vecteur position en référence.
     */
    const Vector& getPosition() const;

    /**
     * @brief Modifie la vitesse de la particule.
     * @param newVitesse Nouvelle vitesse.
     * @return Référence sur l'objet courant.
     */
    Particule& setVitesse(const Vector& newVitesse);


    /**
     * @brief Retourne la vitesse de la particule.
     * @return Vecteur vitesse en référence.
     */
    const Vector& getVitesse() const;

    /**
     * @brief Modifie la masse de la particule.
     * @param newMasse Nouvelle masse (doit être strictement positive).
     * @return Référence sur l'objet courant.
     */
    Particule& setMasse(double newMasse);

    /**
     * @brief Retourne la masse de la particule.
     * @return La masse.
     */
    const double getMasse() const;

    /**
     * @brief Modifie la force appliquée sur la particule.
     * @param newForce Nouvelle force.
     * @return Référence sur l'objet courant.
     */
    Particule& setForce(const Vector& newForce);

    /**
     * @brief Retourne la force appliquée sur la particule.
     * @return Vecteur force
     */
    const Vector& getForce() const;

    /**
     * @brief Modifie la catégorie de la particule.
     * @param newType Nouveau type/catégorie.
     * @return Référence sur l'objet courant.
     */
    Particule& setType(int newType);


    /**
     * @brief Retourne la catégorie de la particule.
     * @return La catégorie.
     */
    const int getType() const;
};

/**
 * @brief Initialise les forces d'une liste de particules.
 *
 * Calcule et retourne le vecteur des forces initiales à appliquer
 * à chaque particule, utilisé comme point de départ de l'algorithme
 * de Störmer-Verlet.
 *
 * @param particleList Liste des particules à initialiser.
 * @return Vecteur des forces initiales (F_i) : force appliquée sur la ième particule.
 */
std::vector<Vector> initialiser(std::vector<Particule>& particleList);


/**
 * @brief Simule l'évolution des particules via l'algorithme de Störmer-Verlet.
 *
 * Met à jour positions, vitesses et forces de chaque particule pas à pas
 * jusqu'au temps @p tEnd, avec un pas de temps @p dt.
 *
 * @param particleList Liste des particules à simuler.
 * @param forces_fold  Forces calculées au pas de temps précédent.
 * @param tEnd         Temps de fin de la simulation.
 * @param dt           Pas de temps (doit être strictement positif).
 */
void algo_stromer_verlet(std::vector<Particule>& particleList,
                         std::vector<Vector> forces_fold,
                         double tEnd,
                         double dt);

#endif