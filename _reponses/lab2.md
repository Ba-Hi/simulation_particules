## Lab 2 : Simulation de particules

#### Question 1 : Classe `Particule`

Une particule est modélisée comme un objet tridimensionnel dans `src/Particule.cpp` et `include/Particule.hpp`.

###### Attributs

| Attribut | Type | Description |
|---|---|---|
| `position` | `Vector` | Position (x, y, z) |
| `vitesse` | `Vector` | Vitesse (vx, vy, vz) |
| `force` | `Vector` | Force appliquée |
| `masse` | `double` | Masse (strictement positive) |
| `identifiant` | `int` | Identifiant unique |
| `categorie` | `int` | Type/catégorie |

###### Constructeur

Une exception `std::invalid_argument` est levée si la masse est négative ou nulle.

###### Getters / Setters

Chaque attribut dispose d'un getter et d'un setter. Les setters retournent une référence sur l'objet courant pour permettre le **chaînage**


#### Question 2 : Collection de particules

Les particules sont stockées dans un `std::vector<Particule>` au sein de la classe `Univers` (`include/Univers.hpp`).


La méthode `ajouterParticule` permet d'insérer une particule :


Un `reserve` peut être passé au constructeur pour pré-allouer la mémoire et améliorer les performances d'insertion


#### Question 3 : Comparaison des performances

Les tests de performance sont dans `test/test_performance_tp2.cpp`. On mesure le temps d'insertion et de calcul des forces pour des tailles croissantes de collections : 64, 128, 256, ..., particules.

**Résultats observés**

| Nombre de particules ($N$) | Sans `reserve` (s) | Avec `reserve` (s) | Ratio (sans/avec) |
| :--- | :--- | :--- | :--- |
| **64** | $2.1259 \times 10^{-5}$ | $7.9280 \times 10^{-6}$ | 2.68 |
| **128** | $2.6361 \times 10^{-5}$ | $1.5347 \times 10^{-5}$ | 1.72 |
| **256** | $4.4202 \times 10^{-5}$ | $3.0149 \times 10^{-5}$ | 1.47 |
| **512** | $1.0420 \times 10^{-4}$ | $5.9944 \times 10^{-5}$ | 1.74 |
| **1024** | $2.0658 \times 10^{-4}$ | $1.1939 \times 10^{-4}$ | 1.73 |
| **2048** | $4.1380 \times 10^{-4}$ | $2.6572 \times 10^{-4}$ | 1.56 |
| **4096** | $9.2234 \times 10^{-4}$ | $5.9728 \times 10^{-4}$ | 1.54 | 

**Commentaires**

- À partir de **N ≈ 512** les différences s’observent

- Avec l'utilisation de la méthode `reserve`  On observe une amélioration constante de la performance, avec une réduction du temps de traitement comprise entre **35% et 60%** = $$\text{Gain en \%} = \left( 1 - \frac{1}{\text{Ratio}} \right) \times 100$$



#### Question 4 : Algorithme de Störmer-Verlet

L'algorithme de Störmer-Verlet est implémenté dans `Univers::avancerParticules` (`src/Univers.cpp`).


#### Question 5 : Vérification sur le système solaire

L'implémentation est validée sur un système à 4 corps dans `test/test_systeme_solaire.cpp` :

| Corps | Position initiale | Vitesse initiale | Masse |
|---|---|---|---|
| Soleil | (0, 0, 0) | (0, 0, 0) | 1.0 |
| Terre | (0, 1, 0) | (-1, 0, 0) | 3×10⁻⁶ |
| Jupiter | (0, 5.36, 0) | (-0.425, 0, 0) | 9.55×10⁻⁴ |
| Halley | (34.75, 0, 0) | (0, 0.0296, 0) | 10⁻¹⁴ |

La simulation est lancée pour `tEnd = 0.468`  avec `dt = 0.015` :

Les trajectoires obtenues (voir figure ci-dessous) montrent :
- La **Terre** décrit une orbite quasi-circulaire autour du Soleil
- **Jupiter** décrit une orbite elliptique plus large
- La **comète de Halley** suit une orbite très elliptique caractéristique

`test/plot_python/plotting.py` génére la figure ci-dessous en se basant sur les postions des astres dans `output.csv`

![Système solaire](solar_system.png)

Les résultats sont cohérents avec les données astronomiques connues, ce qui valide l'implémentation de l'algorithme.


#### Question 6 : Protection des données de `Particule`

Les attributs de `Particule` sont déclarés `private` dans `include/Particule.hpp`. L'accès en lecture se fait via des **getters `const`**, et la modification via des **setters** qui retournent une référence pour le chaînage :

Les getters retournent des **références constantes** pour éviter toute copie inutile tout en interdisant la modification directe :


Les résultats de simulation sont sauvegardés au format CSV dans `output.csv` (à la racine du projet) via `ExportCsv.hpp`, avec une position par astre par ligne :

```
t x0 y0 x1 y1 x2 y2 x3 y3
0.015 0.0 0.0 -0.015 1.0 ...
```




