#pragma once
#include <stdexcept>
#include <string>

/**
 * @brief Classe de base pour toutes les exceptions du simulateur
 */
class SimulationException : public std::runtime_error {
public:
    explicit SimulationException(const std::string& msg) : std::runtime_error(msg) {}
};

/**
 * @brief Exception liée à la configuration du modèle (dimensions, paramètres aberrants)
 */
class ConfigurationException : public SimulationException {
public:
    explicit ConfigurationException(const std::string& msg) 
        : SimulationException("Erreur de Configuration : " + msg) {}
};

/**
 * @brief Exception liée à la physique (masse invalide, comportement non-physique)
 */
class PhysicsException : public SimulationException {
public:
    explicit PhysicsException(const std::string& msg) 
        : SimulationException("Erreur Physique : " + msg) {}
};

/**
 * @brief Exception liée aux mathématiques (division par zéro, NaN)
 */
class MathException : public SimulationException {
public:
    explicit MathException(const std::string& msg) 
        : SimulationException("Erreur Mathématique : " + msg) {}
};

/**
 * @brief Exception liée aux entrées/sorties (échec d'écriture VTK, CSV)
 */
class IOException : public SimulationException {
public:
    explicit IOException(const std::string& msg) 
        : SimulationException("Erreur IO : " + msg) {}
};