## Lab 4 : Découpage de l'espace


### Question 1 : Potentiel de Lennard-Jones

Le potentiel de Lennard-Jones entre deux particules séparées d'une distance $r$ est :

$$V(r) = 4\varepsilon \left[ \left(\frac{\sigma}{r}\right)^{12} - \left(\frac{\sigma}{r}\right)^{6} \right]$$

avec $\varepsilon = 1$ et $\sigma = 1$.

Le potentiel présente deux régimes distincts :
- **Répulsif** ($r < 2^{1/6}\sigma \approx 1.122$) : le terme en $r^{-12}$ domine, la force repousse les particules
- **Attractif** ($r > 2^{1/6}\sigma$) : le terme en $r^{-6}$ domine, les particules s'attirent
- **Minimum** en $r^* = 2^{1/6}\sigma$ : $V(r^*) = -\varepsilon$, la force est nulle

À partir d'une distance de coupure $r_\text{cut} = 2.5\sigma$, le potentiel vaut environ $-0.016\varepsilon$ et peut être négligé :

$$V(r) = \begin{cases} 4\varepsilon\left[\left(\frac{\sigma}{r}\right)^{12} - \left(\frac{\sigma}{r}\right)^{6}\right] & r \leq r_\text{cut} \\ 0 & r > r_\text{cut} \end{cases}$$


### Question 2 : Classe `Univers`

La classe `Univers` est une **classe abstraite** qui encapsule l'ensemble des paramètres physiques
et de simulation. Elle est héritée par `UniversLJ` et `UniversGravitationnel`.

Les paramètres physiques spécifiques au potentiel LJ sont dans la sous-classe `UniversLJ`



### Question 3 — Cellules et voisinage

### Nombre de cellules par direction

$$n_x = \left\lfloor \frac{L_x}{r_\text{cut}} \right\rfloor, \quad n_y = \left\lfloor \frac{L_y}{r_\text{cut}} \right\rfloor, \quad n_z = \left\lfloor \frac{L_z}{r_\text{cut}} \right\rfloor$$

En choisissant $r_\text{cut}$ comme taille de cellule, on garantit que les particules d'une cellule
n'interagissent qu'avec les particules des **27 cellules voisines au maximum** (3D).

La classe `Cellule` contient :
- `std::vector<Particule*> particuleList` : pointeurs vers les particules (pas de copie)
- `std::vector<Cellule*> voisines` calculées une seule fois à l'initialisation
- Ses coordonnées `(i, j, k)` dans la grille


### Question 4 : Calcul des forces avec cellules liées

La complexité passe de $O(N^2)$ à $O(N)$ pour des distributions uniformes.

Pour chaque paire de cellules voisines, on calcule les interactions uniquement si
$r_{ij} \leq r_\text{cut}$. La symétrie de Newton ($\mathbf{F}_{ji} = -\mathbf{F}_{ij}$)
est exploitée pour ne traiter chaque paire qu'une seule fois (`cj > &ci`).


### Question 5 : Mise à jour des cellules

À chaque pas de temps, après déplacement des particules, on réattribue chaque particule
à la cellule qui contient sa position courante. On applique d'abord les conditions aux limites (tp6),
puis on vide toutes les cellules et on les repeuple.

`void UniversLJ::mettreAJourCellules() `

### Question 6 : Simulation de collision


- **Phase 1** ($t \approx 0$-$1$) : le carré rouge se déplace vers le rectangle, les deux objets sont rigides (répulsion LJ)
- **Phase 2** (impact) : forte compression, onde de choc, les particules du carré pénètrent le rectangle
- **Phase 3** (dispersion) : les deux objets se fragmentent et les particules se dispersent dans le domaine
- Les particules qui sortent du domaine sont absorbées (condition `ABSORPTION`)



**-- Optimisations pour la performance**


1. Flags de compilation :

| Flag | Effet |
|---|---|
| `-O3` | Inline, NRVO, optimisations agressives |
| `-march=native` | Instructions AVX2/FMA : 4 doubles en parallèle |
| `-ffast-math` | Réassociation des flottants, suppression des gardes NaN/Inf |
| `-funroll-loops` | Déroulage des boucles sur les 3 composantes x, y, z |


2. Exploitation de la symétrie de Newton dans `calculerForces()` (gain ×2)


3. Suppression de la copie dans `appliquerConditionsLimites()`

L'implémentation initiale construisait un vecteur `survivantes` complet à chaque pas de temps,
copiant toutes les particules puis réassignant `particuleList`. On l'a modifiée après pour qu'il n'y a aucune copie supplémentaire (modif in-place)

4. Décomposition scalaire dans les boucles de force

Au lieu de construire des `Vector` temporaires pour chaque composante, on travaille
directement avec des `double` :

Cela réduit fortement la pression sur le constructeur `Vector(double, double, double)`,
qui apparaissait à **13 milliards d'appels** dans le profil sans `-O3`.


5. Cache des listes dans les boucles

Les appels à `getParticuleList()` et `getVoisines()` à l'intérieur des boucles imbriquées
sont mis en cache une seule fois par cellule :

```cpp
const auto& parts   = ci.getParticuleList(); // une seule fois
const auto& voisins = cj->getParticuleList(); // une seule fois
```

6. Les options de compilations (voir lab3)

En prenant en compte les conditions aux limites et les optimisations de performances, l'execution du code `./demo/demo_collision_2D` passe de 26min à 4m17s !

- Remarques sur les performances

L’analyse avec perf montre que la fonction calculerForces() concentre environ 66% du temps d’exécution, ce qui en fait le principal goulot d’étranglement.

Le programme est globalement bien optimisé côté calcul (IPC ~1.8–2.0), mais reste limité par :
- des accès mémoire coûteux (cache misses),
- des branches mal prédites (~6-7% de branch misses),
- et des indirections via pointeurs et objets Vector.

On observe aussi que le CPU passe une partie du temps à attendre les données mémoire (backend bound).

Resultat de perf stat :

```
 1,237,291,373,275      cpu_atom/instructions/           #    1.47  insn per cycle              (0.15%)
 2,112,222,143,109      cpu_core/instructions/           #    1.88  insn per cycle              (99.83%)
   842,119,295,192      cpu_atom/cycles/                 #    3.549 GHz                         (0.15%)
 1,120,706,637,781      cpu_core/cycles/                 #    4.723 GHz                         (99.83%)
   126,072,813,943      cpu_atom/branches/               #  531.361 M/sec                       (0.15%)
   221,992,588,105      cpu_core/branches/               #  935.635 M/sec                       (99.83%)
     7,955,687,725      cpu_atom/branch-misses/          #    6.31% of all branches             (0.15%)
    14,962,123,978      cpu_core/branch-misses/          #    6.74% of all branches             (99.83%)
 #     29.6 %  tma_backend_bound      
                                                  #     20.2 %  tma_bad_speculation    
                                                  #      4.1 %  tma_frontend_bound     
                                                  #     46.2 %  tma_retiring             (99.83%)
 #     37.2 %  tma_bad_speculation    
                                                  #     29.7 %  tma_retiring             (0.15%)
 #     21.8 %  tma_backend_bound      
                                                  #     11.3 %  tma_frontend_bound       (0.15%)

     237.350997730 seconds time elapsed

     237.163028000 seconds user
       0.102986000 seconds sys
```