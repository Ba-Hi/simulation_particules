#ifndef VECTOR_HPP
#define VECTOR_HPP



/**
 * @brief Représente Vecteur 3D (x, y, z)
 * 
 * Chaque vecteur a sa composante x, y et z
 */


class Vector{
private :
    double x_; ///< Coordonée x du vecteur
    double y_; ///< Coordonée y du vecteur
    double z_; ///< Coordonée z du vecteur

public :

    /**
     * @brief Constructeur de Vector : construit un vecteur avec ses coordonées initiales
     * 
     * @param x_init Position x initiale
     * @param y_init Position y initiale (défaut 0.0 pour 1D)
     * @param z_init Position z initiale (défaut 0.0 pour 2D)
     */
    Vector(double x_init, double y_init = 0.0, double z_init = 0.0);

    /**
     * @brief Getter de x
     * 
     * @return La valeur de x
     */
    double x() const { return x_; }

    /**
     * @brief Getter de y
     * 
     * @return La valeur de y
     */
    double y() const { return y_; }

    /**
     * @brief Getter de z
     * 
     * @return La valeur de z
     */
    double z() const { return z_; }

    /**
     * @brief Additionne deux vecteurs
     * @param autre Le vecteur à ajouter
     * @return Un nouveau vecteur résultant de la somme
     */
    Vector operator+(const Vector& autre) const;

    /**
     * @brief Soustrait deux vecteurs
     * @param autre Le vecteur à soustraire
     * @return Un nouveau vecteur résultant de la différence
     */
    Vector operator-(const Vector& autre) const;

    /**
     * @brief Multiplie un vecteur par un scalaire
     * @param scalaire Le facteur de multiplication
     * @return Un nouveau vecteur résultant du produit
     */
    Vector operator*(double scalaire) const;

    /**
     * @brief Divise le vecteur par un scalaire
     * @param scalaire Le diviseur
     * @return Un nouveau vecteur résultant de la division
     * @note Le scalaire ne doit pas être nul
     * @warning Division par zéro
     */
    Vector operator/(double scalaire) const;

    /**
     * @brief Opérateur d'affectation
     * @param source Le vecteur à copier
     * @return Une référence vers le vecteur courant après affectation
     */
    Vector& operator=(const Vector& source);

    /**
     * @brief Norme euclidienne d'un vecteur
     * @return La norme du vecteur
     */
    double norm() const;

    /**
     * @brief Norme carrée euclidienne d'un vecteur
     * @return La norme carrée du vecteur
     */
    double norm2() const;

    /**
     * @brief Afficher les coordonnées du vecteur
     */
    void afficher() const;


    /**
     * @brief Vérifie si deux vecteurs sont égaux
     * @param v Autre vecteur
     * @return Boolean : True si les deux vecteurs sont égaus False sinon
     * @note Le scalaire ne doit pas être nul
     * @warning Division par zéro
     */
    bool operator==(const Vector& v) const;

    /**
     * @brief Ajoute un vecteur au vecteur courant
     * @param v Le vecteur à ajouter
     * @return Une référence vers le vecteur courant modifié
     */
    Vector& operator+=(const Vector& v);

    /**
     * @brief Soustrait un vecteur du vecteur courant
     * @param v Le vecteur à soustraire
     * @return Une référence vers le vecteur courant modifié
     */
    Vector& operator-=(const Vector& v);

    /**
     * @brief Multiplie le vecteur par un scalaire
     * @param scalar Le facteur de multiplication
     * @return Une référence vers le vecteur courant modifié
     */
    Vector& operator*=(double scalar);

    /**
     * @brief Divisie le vecteur par un scalaire
     * @param scalar le facteur de division
     * @return Une référence vers le vecteur courant modifié
     */
    Vector& operator/=(double scalar);

    /**
     * @brief Multiplie un scalaire par un vecteur
     * @param scalar Le facteur de multiplication
     * @param v Le vecteur à multiplier
     * @return Un nouveau vecteur résultant du produit
     */
    friend Vector operator*(double scalar, const Vector& v);
    
    /**
     * @brief Divise un scalaire par un vecteur
     * @param scalar Le scalaire
     * @param v Le vecteur
     * @return Un nouveau vecteur résultant de la division
     * @warning Division par zéro
     */
    friend Vector operator/(double scalar, const Vector& v);



    /**
     * @brief Calcule la distance entre deux vecteurs
     * @param autre Le vecteur avec lequel calculer la distance
     * @return La distance euclidienne entre les deux vecteurs
     */
    double distance(const Vector& autre) const;
};


#endif