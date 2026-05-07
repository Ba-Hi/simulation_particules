# Simulation de Particules - Projet C++

## Description
Ce projet implémente un moteur de simulation de particules en N-dimensions (principalement 1D, 2D et 3D) développé en C++. Il utilise l'algorithme d'intégration numérique de **Störmer-Verlet** pour modéliser la dynamique de systèmes physiques complexes, incluant :
- **L'interaction gravitationnelle** (simulation de systèmes planétaires, ex: le Système Solaire).
- **Le potentiel de Lennard-Jones** (dynamique moléculaire, simulation de collisions, gaz et ondes de choc).

Le projet met un accent fort sur l'**optimisation spatiale et algorithmique**, avec des fonctionnalités avancées comme la méthode des cellules liées pour réduire la complexité algorithmique des grandes simulations.

## Fonctionnalités Principales
- **Intégrateur Störmer-Verlet** : Avancement temporel stable et conservatif pour la dynamique moléculaire.
- **Modèles Physiques** : 
  - `UniversGravitationnel` : Loi de la gravitation universelle de Newton.
  - `UniversLJ` : Potentiel de Lennard-Jones avec gestion de distance de coupure ($r_{cut}$).
- **Optimisations des Performances** :
  - Découpage spatial via maillage (`Cellule`) et listes de voisinage limitant le calcul aux cellules adjacentes.
  - Exploitation de la 3ème loi de Newton ($\mathbf{F}_{ji} = -\mathbf{F}_{ij}$) limitant de moitié les évaluations de force.
  - Pré-allocation mémoire (`std::vector::reserve`) et gestion sans copie (in-place) lors de l'application des conditions aux limites.
  - Support de flags d'optimisation (`-O3`, `-march=native`, `-ffast-math`).
- **Conditions aux limites paramétrables** : Périodiques, Absorption et Réflexion (via rebond cinématique ou murs "doux" par potentiel).
- **Régulation de la vitesse** : Implémentation d'une limite d'énergie cinétique par mise à l'échelle des vitesses pour limiter l'énergie cinétique de sous-groupes de particules.
- **Forces Extérieures** : Ajout d'un champ gravitationnel uniforme (poids).
- **Gestion des Erreurs** : Hiérarchie complète d'exceptions (`PhysicsException`, `ConfigurationException`, etc.) pour prévenir les configurations invalides.
- **Export et Visualisation** : Génération de fichiers au format VTK (`.pvd`, `.vtp`) pour une visualisation 3D interactive, ainsi que des exports CSV traditionnels.

## Structure du Projet
- `include/` : Fichiers d'en-tête (.hpp) définissant l'API orientée objet (`Univers`, `Particule`, `Vector`, `Cellule`, etc.).
- `src/` : Fichiers sources (.cpp) contenant les implémentations des classes.
- `test/` : Tests unitaires et physiques (validations de conservation d'énergie, etc.) basés sur **GoogleTest**.
- `demo/` : Exemples exécutables complets (collisions 2D/3D, dynamique du système solaire).
- `_reponses/` : Fichiers Markdown contenant l'analyse technique, l'étude des performances (via `perf` ou `gprof`) et les réponses aux labs.

## Compilation et Installation

### Prérequis
- CMake (>= 3.16)
- Compilateur supportant le standard **C++17** (GCC, Clang)
- GoogleTest (pour la compilation des tests automatiques)
- Doxygen (pour la documentation)
- **ParaView** (recommandé pour visualiser les rendus d'animation physique)

### Instructions de build
```bash
# 1. Créer le répertoire de build
mkdir build && cd build

# 2. Configurer le projet avec CMake
cmake ..

# 3. Compiler
make
# ou 
make -j$(nproc) 
```


### Utilisation des Tests

Le projet utilise GoogleTest. Pour lancer les tests après la compilation :

```bash
# lancer tous les tests à la fois
ctest
# Ou lancer les binaires directement
./test/test_univers
./test/test_particule
```

### Contrôle de la mémoire avec Valgrind

Pour vérifier l'absence de fuites mémoire et les erreurs d'accès, vous pouvez exécuter les binaires de test avec Valgrind. Par exemple :

```bash
valgrind --leak-check=full --show-leak-kinds=all ./build/test/test_univers
```

Un script `valgrind_all_tests.sh` est également fourni pour lancer Valgrind sur tous les tests compilés.

### Exemples de démos

Après compilation, plusieurs exécutables de démonstration sont disponibles dans `build/demo/` :

```bash
./build/demo/demo_systeme_solaire
./build/demo/demo_collision_2D
./build/demo/demo_collision_3D
./build/demo/demo_goutte_collision
./build/demo/demo_comparaison_murs
```
, etc.

Pour visualiser les résultats, ouvrez le fichier `.pvd` généré dans le répertoire de build avec ParaView :

```bash
paraview simulation.pvd
```

### Documentation
Pour générer la documentation HTML via Doxygen, depuis le répertoire de build:

```bash
make doc
```

qui génère `docu/html/index.html` contenant la documentation oxygen.

Ou bien depuis la racine du projet :

```bash
doxygen Doxyfile
```
