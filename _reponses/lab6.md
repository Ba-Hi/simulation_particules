## Lab 6 : Raffinement du modèle et conditions aux limites

### Question 1 : Implémentation des conditions aux limites

Le code implémente trois types de conditions aux limites (de nature cinématique) dans la méthode `UniversLJ::appliquerConditionsLimites`, activées via un appel à `mettreAJourCellules(true)` :
- **Réflexion (`REFLEXION`)** : Lorsqu'une particule franchit une paroi, sa position est corrigée par rapport au mur franchi et la composante de sa vitesse perpendiculaire au mur est inversée.
- **Absorption (`ABSORPTION`)** : Utilisation de l'algorithme `std::remove_if` combiné à une lambda pour détecter et supprimer définitivement de la simulation (`particuleList.erase`) toute particule sortant du domaine.
- **Périodique (`PERIODIQUE`)** : Utilisation de l'opérateur modulo (`std::fmod`) pour transporter une particule sortant d'un bord directement sur le bord opposé du domaine, sans modifier sa trajectoire ou sa vitesse.

### Question 2 : Test des conditions aux limites

Ces différentes conditions font l'objet de tests unitaires dans `test/test_univers_tp4.cpp`

### Question 3 : Implémentation du potentiel de paroi

La condition réflexive a également été implémentée via un potentiel de paroi pour modéliser une vraie interaction physique (qu'on appelera apres : murs doux).
- L'activation se fait via le booléen `use_potentiel_reflexion` passé à la méthode `UniversLJ::calculerForces()`. Si activé, celle-ci appelle `appliquerForcesParoi()`.
- Pour chaque particule se trouvant à une distance $d < r_{cut}$ d'un mur, la méthode `calculerForceParoi(d)` évalue la force de répulsion dérivée du potentiel de Lennard-Jones et l'ajoute à la force totale exercée sur la particule.

### Question 4 : Comparaison des deux implémentations réflexives

La simulation comparative (`demo/demo_comparaison_murs.cpp`) permet d'observer l'impact de chaque méthode de réflexion. Le résultat est illustré ci-dessous :

![image](./comparaison_murs.png)


- **Réflexion Cinématique (Mur dur, en rouge pointillé)** :
  - rebond géométrique abstrait.
  - La trajectoire percute exactement l'axe $x=0$ sous forme de V.
  - La vitesse s'inverse instantanément via une discontinuité mathématique (de $-8.0$ à $+8.0$).

- **Réflexion par Potentiel (Mur doux, en bleu)** :
  - rebond physique dynamique.
  - La particule ralentit progressivement sous l'effet de la force de paroi avant même d'atteindre le mur. Sa trajectoire est courbée.

### Question 5 : Ajout du potentiel gravitationnel (poids)

Le champ gravitationnel uniforme a été intégré de manière générique dans la classe de base `Univers` :
- Un attribut `Vector gravity` a été ajouté, configurable avec `setGravity(const Vector& g)`.
- Le calcul purement physique est isolé dans la classe utilitaire via `Force::poids(p, gravity)`, appliquant strictement $\mathbf{F} = m\mathbf{g}$. (On a après deplacé tous les calculs de forces dans cette classe utilitaire à ce stade)
- La fonction `calculerForces()` des classes dérivées accepte un paramètre booléen `use_gravity`. Lorsqu'il est activé, ce champ agit comme une force extérieure qui s'additionne aux autres interactions inter-particulaires (LJ) lors de l'intégration numérique.

### Question 6 : Simulation de collision et limitation de l'énergie cinétique

**Implémentation de la régulation de vitesse :**
- La méthode `Univers::setLimiteEnergie(cible_Ec, frequence, type)` permet d'activer cette régulation.
- Toutes les 1000 itérations, l'énergie cinétique courante $E_c$ est calculée.
- Si l'énergie dépasse la cible ($E_c > E_c^D$), les vitesses des particules sont re-mises à l'échelle en étant multipliées par le facteur correctif $\beta = \sqrt{\frac{E_c^D}{E_c}}$.

**Quelques modifications :**
1. **Régulation par sous-groupes** : L'énergie cible est appliquée séparément à la goutte comme système entier (Type 1) et au bain (Type 0) `Univers::setLimiteEnergie(cible_Ec, frequence, type)` prend comme paramètre la categorie . Si on utilisait une seule cible globale, l'énorme vitesse d'impact de la petite goutte se retrouverait lissée dans l'immobilité du grand bain, annulant la collision.
2. **Distance interparticules bain** : L'espacement des particules du bain a été très légèrement augmenté (`d_bain = 1.05 * d` : 5% de plus) par rapport à `d` donné au tp4 pour transforme un arrangement initialement solide en un état fluide pour laisser la compressibilité nécessaire pour laisser pénétrer la goutte.

La configuration de cette simulation est isolée dans `demo_goutte_collision.cpp`

### Question 7 : Mécanisme de gestion des erreurs

Nous avons mis en place une hiérarchie d'exceptions personnalisées dans `include/Exceptions.hpp`:
- `SimulationException` : Classe de base.
- `ConfigurationException` : Erreurs de configuration (ex: dimension de l'univers invalide).
- `PhysicsException` : Erreurs physiques (ex: masse négative ou nulle).
- `MathException` : Erreurs mathématiques (ex: division par zéro dans `Vector`).
- `IOException` : Erreurs d'entrées/sorties (ex: échec d'ouverture d'un fichier VTK).
Ces exceptions remplacent les `std::invalid_argument` et `std::runtime_error` génériques précédemment utilisés dans `UniversLJ`, `Particule`, `Vector` et `ExportVTK`.

### Question 8 : Forces et faiblesses de ce mécanisme

**Forces :**
- **Typage fort** : Permet au code appelant d'attraper (`catch`) un type d'erreur spécifique et d'agir en conséquence
- **Débogage** : Les messages d'erreur sont préfixés par leur catégorie (ex: "Erreur Physique : La masse..."), ce qui facilite l'identification immédiate du problème

**Faiblesses :**
- **Coût en performance** :l'utilisation de `throw` brise les optimisations du compilateur. Par exemple, vérifier la division par zéro dans `Vector::operator/=` (appelé des millions de fois) peut avoir un très léger impact sur les performances globales, comparé à une stratégie d'erreur désactivable
