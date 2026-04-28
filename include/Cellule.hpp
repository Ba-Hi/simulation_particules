#ifndef CELLULE_HPP
#define CELLULE_HPP

#include "Particule.hpp"
#include "Vector.hpp"
#include <vector>

/**
* @class Cellule
* @brief Représente une cellule dans un espace discrétisé contenant des particules.
*
* Une cellule est définie par ses coordonnées (i, j, k) dans une grille 3D,
* sa taille, une liste de particules qu'elle contient, ainsi qu'une liste
* de cellules voisines.
*/
  
class Cellule {

private :
  double tailleCellule; ///< Taille de la cellule
  int i; ///< Coordonnée i de la cellule dans la grille
  int j; ///< Coordonnée j de la cellule dans la grille
  int k; ///< Coordonnée k de la cellule dans la grille
  std::vector<Particule*> particuleList; ///< Liste des particules contenues dans la cellule
  std::vector<Cellule*> voisines; ///< Liste des pointeurs vers les cellules voisines

public :

    /**
     * @brief Constructeur de la cellule
     * 
     * @param tailleCellule Taille de la cellule
     * @param i Coordonnée i
     * @param j Coordonnée j
      * @param k Coordonnée k
     */
    Cellule(const double tailleCellule, int i, int j, int k);

    /***
     * @brief Getter du centre de la cellule
     * @return Vecteur représentant la position du centre de la cellule
     */
    Vector centreCellule() const;

    /**
     * @brief Retourne la taille de la cellule
     * @return Taille de la cellule
     */
    double getTailleCellule() const; 

    /**
     * @brief Accède à la liste des particules
     * @return Référence vers le vecteur de particules
     */
    std::vector<Particule*>& getParticuleList();

    /**
     * @brief Accède à la liste des cellules voisines
     * @return Référence vers le vecteur des cellules voisines
     */
    std::vector<Cellule*>& getVoisines();

    /**
     * @brief Définit la taille de la cellule
     * @param taille Nouvelle taille
     * @return Référence vers la cellule courante
     */
    Cellule& setTailleCellule(const double& taille);

    /**
     * @brief Définit la liste des particules
     * @param listPart Nouvelle liste de particules
     * @return Référence vers la cellule courante
     */
    Cellule& setListParticule(const std::vector<Particule*>& listPart);

    /**
     * @brief Calcule la position de la cellule dans l'espace
     * @return Vecteur représentant la position de la cellule
     */
    Vector getPositionCellule() const;

    /**
     * @brief Ajoute une cellule voisine
     * @param voisine Pointeur vers la cellule voisine
     */
    void ajouterVoisine(Cellule* voisine);

    /**
     * @brief Ajoute une particule à la cellule
     * @param p Particule à ajouter
     */
    void ajouterParticule(Particule* p);

    /**
     * @brief Vide la liste des particules de la cellule
     */
    void viderParticules();
};

#endif
