## Lab 3 : utilisation des opérateurs

### Question 1 : Classe `Vector` pour un vecteur 3D

Nous avons créé une classe `Vector` permettant de manipuler un vecteur de dimension 3. Chaque vecteur est défini par ses trois composantes réelles $(x, y, z)$.

Le constructeur accepte des valeurs par défaut pour $y$ et $z$, ce qui permet d'utiliser la même classe pour des vecteurs 1D, 2D et 3D, par exemple `Vector(5.0)` pour un vecteur 1D.

### Question 2 : Constructeurs et opérateurs

**Opérateurs arithmétiques** (retournent un nouveau `Vector`) :

```cpp
Vector operator+(const Vector& autre) const;
Vector operator-(const Vector& autre) const;
Vector operator*(double scalaire) const;
Vector operator/(double scalaire) const;
friend Vector operator*(double scalar, const Vector& v); // scalaire * vecteur
```

**Opérateurs d'affectation composée** (modifient le vecteur en place) :

```cpp
Vector& operator+=(const Vector& v);
Vector& operator-=(const Vector& v);
Vector& operator*=(double scalar);
Vector& operator/=(double scalar);
Vector& operator=(const Vector& source);
```

**Méthodes utilitaires** :

```cpp
double norm()  const; // norme euclidienne : sqrt(x² + y² + z²)
double norm2() const; // norme au carré (évite un sqrt inutile)
bool operator==(const Vector& v) const;
double distance(const Vector& autre) const;
```

### Question 3 : Tests unitaires

Les tests couvrent chaque méthode à l'aide de **Google Test** (`test_vector.cpp`) :


### Question 4 : Utilisation de `Vector` dans `Particule`

La classe `Particule` a été modifiée pour remplacer les représentations scalaires séparées par des objets `Vector`

### Question 5 : Classe `Univers`

La classe abstraite `Univers` encapsule la simulation :

L'algorithme d'intégration **Störmer-Verlet** est implémenté dans `avancerParticules()` :

La classe `UniversGravitationnel` hérite de `Univers` et implémente `calculerForces()` avec la loi de gravitation universelle. La classe `UniversLJ` l'implémente avec le potentiel de Lennard-Jones.


### Question 6 : Création d'un univers avec $(2^5)^3$ particules

On crée $2^{15} = 32\,768$ particules uniformément réparties sur le cube $[0,1]^3$ : `./test/test_performance tp2`

Le paramètre `reserveCount` du constructeur appelle `particuleList.reserve(n)` pour pré-allouer la mémoire et éviter les réallocations successives lors de l'insertion.

### Question 7 : Test de performance en insertion

Le fichier `test_performance_tp2.cpp` compare l'insertion **avec et sans** `reserve()` :

**Résultats observés** : la version avec `reserve()` est systématiquement plus rapide.
Sans `reserve`, le `std::vector` réalloue sa capacité par doublement successif ($1 \to 2 \to 4 \to \ldots$), ce qui entraîne $O(\log N)$ copies complètes du tableau. Avec `reserve(N)`, une seule allocation est faite, et chaque insertion est $O(1)$ amorti sans aucune copie.

### Question 8 : Test de performance du calcul des interactions

Le calcul des forces gravitationnelles est en $O(N^2)$ : pour $N$ particules, on évalue $\frac{N(N-1)}{2}$ paires.

Le temps de calcul suit bien une loi quadratique : doubler $N$ multiplie le temps par 4.

### Question 9 : Modification

La modification la plus simple consiste à appliquer la 3ème loi de Newton $\mathbf{F}_{ji} = -\mathbf{F}_{ij}$, sans recalcul.

Au lieu de la double boucle complète :

On n'itère que sur les paires uniques $i < j$ :

```cpp
for (size_t i = 0; i < N; ++i)
    for (size_t j = i+1; j < N; ++j) {
        Vector fij = calculer_force(i, j);
        forces[i] += fij;
        forces[j] -= fij; // Newton : F_ji = -F_ij
    }
```

C'est exactement ce qui est implémenté dans `UniversGravitationnel::calculerForces()` et `UniversLJ::calculerForces()`.


### Question 10 : Autres simplifications possibles

**a) Flags de compilation**

Activer les optimisations du compilateur (`-O2` ou `-O3`) permet au compilateur d'inliner les getters, vectoriser les boucles et éliminer les copies inutiles.

**b) Précalcul des inverses de masse**

Dans l'intégrateur, `1.0 / masse` est calculé à chaque pas de temps pour chaque particule. Le précalcul une seule fois dans un vecteur `inv_masses` est déjà appliqué dans `Univers::avancerParticules()`
