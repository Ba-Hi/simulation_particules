#ifndef UNIVERS_HPP
#define UNIVERS_HPP

#include <vector>
#include "Particule.hpp"
#include "Vector.hpp"


/**
 * @brief Classe abstraite représentant un univers contenant un ensemble de particules
 * 
 * Cette classe gère une collection de particules dans un espace de dimension donnée.
 * Elle définit l'interface commune pour différents types d'univers (gravitationnel, Lennard-Jones, etc.).
 */

class Univers
{
protected:
    int dimension; ///< Dimension de l'espace (1D, 2D, 3D)
    std::vector<Particule> particuleList; ///< Liste des particules
    int n_particules; ///< Nombre de particules

public:
    enum class ConditionLimite {
        REFLEXION,
        ABSORPTION,
        PERIODIQUE
    };

    /**
     * @brief Constructeur de l'univers
     * @param dim Dimension de l'espace
     * @param reserveCount Nombre de particules à réserver (optionnel)
     */
    Univers(int dim, int reserveCount = 0);

    /**
     * @brief Destructeur virtuel
     */
    virtual ~Univers() = default;

    /**
     * @brief Ajoute une particule à l'univers
     * @param p La particule à ajouter
     */
    void ajouterParticule(const Particule& p);

    /**
     * @brief Fait évoluer les particules dans le temps avec l'algorithme Störmer-Verlet
     * @param tEnd Temps final de la simulation
     * @param dt Pas de temps
     */
    void avancerParticules(double tEnd, double dt);

    /**
     * @brief Getter de la dimension
     * @return La dimension de l'univers
     */
    int getDimension() const;

    /**
     * @brief Getter du nombre de particules
     * @return Nombre de particules
     */
    int getNombreParticules() const;

    /**
     * @brief Getter de la liste des particules
     * @return Référence à la liste de particules
     */
    const std::vector<Particule>& getParticules() const;

    /**
     * @brief Getter non-const de la liste des particules
     * @return Référence non-const à la liste de particules
     */
    std::vector<Particule>& getParticules();

    /**
     * @brief Calcule les forces appliquées sur chaque particule
     * @return Un vecteur contenant les forces associées à chaque particule
     */
    virtual std::vector<Vector> calculerForces() = 0;

    /**
     * @brief Met à jour les cellules (si applicable)
     * Méthode virtuelle vide par défaut pour les univers sans cellules
     */
    virtual void mettreAJourCellules() {}

    /**
     * @brief Modifie la dimension.
     * @param dim Nouvelle dimension.
     * @return Référence sur l'objet courant.
     */
    Univers& setDimension(int dim);

    /**
     * @brief Modifie le nombre de particule.
     * @param n nombre de particules
     * @return Référence sur l'objet courant.
     */
    Univers& setNombreParticules(int n);

    /**
     * @brief Modifie la liste des particules.
     * @param particules nouvelle liste de particules
     * @return Référence sur l'objet courant.
     */
    Univers& setParticules(const std::vector<Particule>& particules);

private:
    /**
     * @brief Gère la sauvegarde VTK pour un pas de simulation donné
     * 
     * Si le pas courant correspond à une étape de sauvegarde, enregistre
     * les données VTK et accumule le step et le temps dans les vecteurs.
     * 
     * @param step Numéro du pas courant
     * @param t Temps courant
     * @param save_every Fréquence de sauvegarde (tous les N pas)
     * @param vtk_steps Vecteur accumulateur des steps sauvegardés
     * @param vtk_times Vecteur accumulateur des temps sauvegardés
     */
    void sauvegardeVTK(int step, double t, int save_every,
                      std::vector<int>& vtk_steps,
                      std::vector<double>& vtk_times);

    /**
     * @brief Finalise la sauvegarde VTK en créant le fichier PVD
     * 
     * Appelle savePVD avec les steps et times accumulés.
     * 
     * @param vtk_steps Vecteur des steps sauvegardés
     * @param vtk_times Vecteur des temps sauvegardés
     */
    void finaliseSauvegardeVTK(const std::vector<int>& vtk_steps,
                              const std::vector<double>& vtk_times);
};

#endif