#ifndef UNIVERS_HPP
#define UNIVERS_HPP

#include <vector>
#include <map>
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
    Vector gravity; ///< Champ de gravité uniforme (poids)

    bool limite_energie_active = false; ///< Active la limitation d'énergie
    double cible_Ec = 0.0; ///< Energie cinétique cible
    std::map<int, double> cibles_Ec_par_type; ///< Energies cibles par catégorie
    int frequence_limite = 1000; ///< Fréquence de mise à l'échelle

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
     * @param use_potentiel_reflexion Active les forces de parois
     * @param use_gravity Active le champ gravitationnel de poids
     */
    void avancerParticules(double tEnd, double dt, bool use_potentiel_reflexion = false, bool use_gravity = false);

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
     * @param use_potentiel_reflexion Si vrai, applique également les forces de potentiel de réflexion aux parois
     * @param use_gravity Si vrai, applique le champ de gravité uniforme
     * @return Un vecteur contenant les forces associées à chaque particule
     */
    virtual std::vector<Vector> calculerForces(bool use_potentiel_reflexion=false, bool use_gravity=false) = 0;

    /**
     * @brief Met à jour les cellules (si applicable)
     * Méthode virtuelle vide par défaut pour les univers sans cellules
     */
    virtual void mettreAJourCellules(bool use_conditions_limites=true) {}

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

    /**
     * @brief Définit le champ de gravité uniforme
     * @param g Le vecteur gravité (ex: Vector(0, -9.81, 0))
     */
    void setGravity(const Vector& g);

    /**
     * @brief Définit une limite pour l'énergie cinétique
     * @param cible_Ec Energie cinétique cible
     * @param frequence Fréquence de mise à l'échelle (en nombre d'itérations)
     * @param type Catégorie ciblée (par défaut -1 pour toutes les particules)
     */
    void setLimiteEnergie(double cible_Ec, int frequence = 1000, int type = -1);

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