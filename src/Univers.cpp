#include "Univers.hpp"
#include <iostream>
#include <filesystem>
#include "ExportVTK.hpp"
#include "ExportCsv.hpp"
#include <cmath>

Univers::Univers(int dim, int reserveCount)
    : dimension(dim), n_particules(0), gravity(0.0, 0.0, 0.0) {
    particuleList.reserve(reserveCount);
}

int Univers::getDimension() const {
    return dimension;
}

int Univers::getNombreParticules() const {
    return n_particules;
}

const std::vector<Particule>& Univers::getParticules() const {
    return particuleList;
}

std::vector<Particule>& Univers::getParticules() {
    return particuleList;
}

Univers& Univers::setDimension(int dim) {
    dimension = dim;
    return *this;
}

Univers& Univers::setNombreParticules(int n) {
    n_particules = n;
    return *this;
}

Univers& Univers::setParticules(const std::vector<Particule>& particules) {
    particuleList = particules;
    n_particules = particuleList.size();
    return *this;
}

void Univers::ajouterParticule(const Particule& p) {
    particuleList.push_back(p);
    n_particules = particuleList.size();
}

void Univers::setGravity(const Vector& g) {
    gravity = g;
}

void Univers::setLimiteEnergie(double cible_Ec, int frequence, int type) {
    this->limite_energie_active = true;
    this->frequence_limite = frequence;
    if (type == -1) {
        this->cible_Ec = cible_Ec;
    } else {
        this->cibles_Ec_par_type[type] = cible_Ec;
    }
}

void Univers::sauvegardeVTK(int step, double t, int save_every,
                           std::vector<int>& vtk_steps,
                           std::vector<double>& vtk_times) {
    if (step % save_every == 0) {
        saveVTK(particuleList, step, t);
        vtk_steps.push_back(step);
        vtk_times.push_back(t);
    }
}

void Univers::finaliseSauvegardeVTK(const std::vector<int>& vtk_steps,
                                    const std::vector<double>& vtk_times) {
    savePVD(vtk_steps, vtk_times);
}

void Univers::avancerParticules(double tEnd, double dt, bool use_potentiel_reflexion, bool use_gravity) {
    if (particuleList.empty()) return;

    long unsigned int N = particuleList.size();

    
    std::vector<double> inv_masses(N);
    for (size_t i = 0; i < N; i++)
        inv_masses[i] = 1.0 / particuleList[i].getMasse();

    std::vector<Vector> forces = calculerForces(use_potentiel_reflexion, use_gravity);
    std::vector<Vector> forces_old = forces;
    

    double t = 0.0;
    int step = 0;
    const int save_every = (tEnd / dt) / 200; // 200 frames au total
    // int save_every = (tEnd / dt) / 1500; // 1500 frames pour avoir un beau ralenti
    // if (save_every < 1) save_every = 1;


    std::vector<int>    vtk_steps;
    std::vector<double> vtk_times;

    while (t < tEnd) {
        t += dt;
        step++;

        
        std::swap(forces, forces_old);

        for (size_t i = 0; i < N; ++i) {
            Particule& p = particuleList[i]; // une référence pour éviter des copies inutiles

            Vector pos = p.getPosition(); // une copie pour modifier sans affecter l'original
            const Vector& vel = p.getVitesse();
            const double inv_m = inv_masses[i];

            pos += vel * dt;
            pos += forces_old[i] * (0.5*dt*dt*inv_m);
            p.setPosition(pos);
        }

        mettreAJourCellules();
        forces = calculerForces(use_potentiel_reflexion, use_gravity);

        for (size_t i = 0; i < N; ++i) {
            Particule& p = particuleList[i];
            Vector vel = p.getVitesse();
            vel += (forces[i] + forces_old[i]) * (0.5*dt * inv_masses[i]);
            p.setVitesse(vel);
        }

        
        // if (limite_energie_active && step % frequence_limite == 0) {
        //     if (!cibles_Ec_par_type.empty()) {
        //         // 1. Initialisation pour éviter les erreurs de constructeur par défaut
        //         std::map<int, Vector> quantite_mouvement;
        //         std::map<int, double> masse_totale;
        //         std::map<int, Vector> v_com;
        //         std::map<int, double> current_Ec_par_type;
        //         for (const auto& pair : cibles_Ec_par_type) {
        //             quantite_mouvement.insert({pair.first, Vector(0.0, 0.0, 0.0)});
        //             masse_totale.insert({pair.first, 0.0});
        //             v_com.insert({pair.first, Vector(0.0, 0.0, 0.0)});
        //             current_Ec_par_type.insert({pair.first, 0.0});
        //         }

        //         // 2. Calcul du Centre de Masse (vitesse de dérive macroscopique)
        //         for (const auto& p : particuleList) {
        //             int type = p.getType();
        //             if (masse_totale.find(type) != masse_totale.end()) {
        //                 quantite_mouvement.at(type) += p.getVitesse() * p.getMasse();
        //                 masse_totale.at(type) += p.getMasse();
        //             }
        //         }
        //         for (const auto& pair : cibles_Ec_par_type) {
        //             int type = pair.first;
        //             if (masse_totale.at(type) > 0) {
        //                 v_com.at(type) = quantite_mouvement.at(type) / masse_totale.at(type);
        //             }
        //         }
                
        //         // 3. Énergie thermique (vitesse relative au centre de masse)
        //         for (const auto& p : particuleList) {
        //             int type = p.getType();
        //             if (current_Ec_par_type.find(type) != current_Ec_par_type.end()) {
        //                 Vector v_thermique = p.getVitesse() - v_com.at(type);
        //                 current_Ec_par_type.at(type) += 0.5 * p.getMasse() * v_thermique.norm2();
        //             }
        //         }

        //         // 4. Application du thermostat (uniquement si divergence/dépassement)
        //         std::map<int, double> betas;
        //         for (const auto& pair : cibles_Ec_par_type) {
        //             int type = pair.first;
        //             double cible = pair.second;
        //             double current = current_Ec_par_type.at(type);
        //             betas[type] = (current > cible) ? std::sqrt(cible / current) : 1.0;
        //         }
                
        //         for (auto& p : particuleList) {
        //             int type = p.getType();
        //             auto it = betas.find(type);
        //             if (it != betas.end()) {
        //                 Vector v_thermique = p.getVitesse() - v_com.at(type);
        //                 p.setVitesse(v_com.at(type) + v_thermique * it->second);
        //             }
        //         }
        //     } else {
        //         Vector quantite_mouvement(0.0, 0.0, 0.0);
        //         double masse_totale = 0.0;
        //         for (const auto& p : particuleList) {
        //             quantite_mouvement += p.getVitesse() * p.getMasse();
        //             masse_totale += p.getMasse();
        //         }
        //         Vector v_com = (masse_totale > 0) ? (quantite_mouvement / masse_totale) : Vector(0.0, 0.0, 0.0);

        //         double current_Ec = 0.0;
        //         for (const auto& p : particuleList) {
        //             Vector v_thermique = p.getVitesse() - v_com;
        //             current_Ec += 0.5 * p.getMasse() * v_thermique.norm2();
        //         }

        //         if (current_Ec > cible_Ec) {
        //             double beta = std::sqrt(cible_Ec / current_Ec);
        //             for (auto& p : particuleList) {
        //                 Vector v_thermique = p.getVitesse() - v_com;
        //                 p.setVitesse(v_com + v_thermique * beta);
        //             }
        //         }
        //     }
        // }


        // Régulation de l'énergie cinétique par type 
        if (limite_energie_active && step % frequence_limite == 0) {
            // Calcul Ec totale par type
            std::map<int, double> current_Ec_par_type;
            for (const auto& p : particuleList) {
                current_Ec_par_type[p.getType()] +=
                    0.5 * p.getMasse() * p.getVitesse().norm2();
            }

            // rescaling
            for (auto& p : particuleList) {
                int type = p.getType();
                if (cibles_Ec_par_type.find(type) != cibles_Ec_par_type.end()) {
                    double ec = current_Ec_par_type[type];
                    double cible = cibles_Ec_par_type[type];
                    if (ec > cible) { // Limite uniquement si l'énergie dépasse la cible !
                        double beta = std::sqrt(cible / ec);
                        p.setVitesse(p.getVitesse() * beta);
                    }
                }
            }
        }

        sauvegardeVTK(step, t, save_every, vtk_steps, vtk_times);
    }

    finaliseSauvegardeVTK(vtk_steps, vtk_times); 
}