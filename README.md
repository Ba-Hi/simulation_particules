# Simulation de Particules - Projet C++

## Description
Ce projet implémente un système de simulation de particules en 3D utilisant l'algorithme d'intégration de **Störmer-Verlet**. Il permet de gérer des univers de particules, de calculer des forces gravitationnelles ou d'interaction, et d'optimiser les performances via des structures de données comme les grilles de cellules (TP4).

## Structure du Projet
- `include/` : Fichiers d'en-tête (.hpp)
- `src/` : Code source (.cpp)
- `test/` : Tests unitaires utilisant GoogleTest
- `demo/` : Exemples d'utilisation de la bibliothèque

## Analyse UML (Concepts Métier)

### Cas d'Utilisation
L'utilisateur peut configurer l'univers, ajouter des particules et lancer une simulation temporelle. Les données peuvent être chargées ou sauvegardées via le système de fichiers.

### Modèle du Domaine (Classes d'analyse)
- **Univers** : Gère la collection globale et l'évolution temporelle.
- **Particule** : Entité physique définie par sa masse et sa cinématique.
- **Vector** : Outil mathématique pour les calculs en 3D.
- **Cellule** : Optimisation spatiale pour le calcul des interactions locales.

## Compilation et Installation

### Prérequis
- CMake (>= 3.16)
- Compilateur supportant le C++17 (GCC, Clang)
- Doxygen (pour la documentation)

### Instructions de build
```bash
# 1. Créer le répertoire de build
mkdir build && cd build

# 2. Configurer le projet avec CMake
cmake ..

# 3. Compiler
make
```


### Utilisation des Tests

Le projet utilise GoogleTest. Pour lancer les tests après la compilation :

```bash

ctest
# Ou lancer les binaires directement
./test/test_univers
./test/test_particule
```

### Documentation
Pour générer la documentation HTML via Doxygen :

```bash

doxygen Doxyfile

```