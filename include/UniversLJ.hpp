#ifndef UNIVERS_LJ_HPP
#define UNIVERS_LJ_HPP

#include <vector>
#include "Univers.hpp"
#include "Cellule.hpp"
#include "Vector.hpp"

/**
 * @brief Univers Lennard-Jones utilisant la méthode des cellules liées (linked-cell)
 * 
 * Cette classe hérite de Univers et implémente calculerForces() avec le potentiel de Lennard-Jones
 * en utilisant une optimisation par cellules pour les grandes simulations.
 */
class UniversLJ : public Univers
{
private:
    Vector l_d;
    double r_cut;
    double epsilon;
    double sigma;
    int nx, ny, nz;
    Univers::ConditionLimite conditionX;
    Univers::ConditionLimite conditionY;
    Univers::ConditionLimite conditionZ;
    std::vector<Cellule> celluleList;

public:
    /**
     * @brief Constructeur avec paramètres par défaut
     * @param dimension Dimension de l'espace
     * @param l_d Dimensions du domaine
     * @param r_cut Distance de coupure
     */
    UniversLJ(const int& dimension, const Vector& l_d, const double& r_cut);

    /**
     * @brief Constructeur avec epsilon et sigma personnalisés
     * @param dimension Dimension de l'espace
     * @param l_d Dimensions du domaine
     * @param r_cut Distance de coupure
     * @param epsilon Paramètre epsilon du potentiel LJ
     * @param sigma Paramètre sigma du potentiel LJ
     */
    UniversLJ(const int& dimension, const Vector& l_d, const double& r_cut, double epsilon, double sigma);

    /**
     * @brief Initialise les cellules du domaine
     */
    void initialiserCellules();

    /**
     * @brief Définit la condition aux limites en x (pour dim 1)
     */
    void setConditionsLimites(Univers::ConditionLimite cx);

    /**
     * @brief Définit les conditions aux limites en x et y (pour dim 2)
     */
    void setConditionsLimites(Univers::ConditionLimite cx,
                              Univers::ConditionLimite cy);

    /**
     * @brief Définit les conditions aux limites en x, y et z (pour dim 3)
     */
    void setConditionsLimites(Univers::ConditionLimite cx,
                              Univers::ConditionLimite cy,
                              Univers::ConditionLimite cz);

    /**
     * @brief Calcule les forces de Lennard-Jones en utilisant les cellules liées
     * @return Un vecteur contenant les forces associées à chaque particule
     */
    std::vector<Vector> calculerForces() override;

    /**
     * @brief Met à jour l'appartenance des particules aux cellules
     */
    void mettreAJourCellules() override;

    /**
     * @brief Getter pour accéder à une cellule spécifique
     * @param i Indice x de la cellule
     * @param j Indice y de la cellule
     * @param k Indice z de la cellule
     * @return Référence à la cellule
     */
    Cellule& getCellule(int i, int j, int k);

    /**
     * @brief Getter pour le nombre total de cellules
     * @return Nombre de cellules
     */
    size_t getNbCellules() const;


private:
    /**
     * @brief Applique les conditions aux limites actives aux particules
     */
    void appliquerConditionsLimites(Univers::ConditionLimite cx,
                                    Univers::ConditionLimite cy,
                                    Univers::ConditionLimite cz);

    /**
     * @brief Convertit les indices 3D en indice 1D pour l'accès au tableau
     * @param i Indice x
     * @param j Indice y
     * @param k Indice z
     * @return Indice 1D
     */
    int indice1D(int i, int j, int k);

public:
    /**
     * @brief Calcule l'énergie cinétique du système
     * @return Énergie cinétique totale
     */
    double energieCinetique() const;

    /**
     * @brief Calcule l'énergie potentielle du système
     * @return Énergie potentielle totale
     */
    double energiePotentielle() const;
};



#endif