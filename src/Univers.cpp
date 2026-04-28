#include "Univers.hpp"
#include <iostream>
#include <filesystem>
#include "ExportVTK.hpp"
#include "ExportCsv.hpp"

Univers::Univers(int dim, int reserveCount)
    : dimension(dim), n_particules(0) {
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

void Univers::avancerParticules(double tEnd, double dt) {
    if (particuleList.empty()) return;

    long unsigned int N = particuleList.size();

    
    std::vector<double> inv_masses(N);
    for (size_t i = 0; i < N; i++)
        inv_masses[i] = 1.0 / particuleList[i].getMasse();

    std::vector<Vector> forces = calculerForces();
    std::vector<Vector> forces_old = forces;
    

    double t = 0.0;
    int step = 0;
    const int save_every = (tEnd / dt) / 200; // 200 frames au total


    std::vector<int>    vtk_steps;
    std::vector<double> vtk_times;

    while (t < tEnd) {
        t += dt;
        step++;

        
        std::swap(forces, forces_old);

        for (size_t i = 0; i < N; ++i) {
            Particule& p = particuleList[i];
            Vector accel = forces[i] * inv_masses[i];
            p.setPosition(p.getPosition() + p.getVitesse()*dt + accel*(0.5*dt*dt));
        }

        mettreAJourCellules();
        forces = calculerForces();

        for (size_t i = 0; i < N; ++i) {
            Particule& p = particuleList[i];
            p.setVitesse(p.getVitesse()
                + (forces[i] + forces_old[i]) * (0.5*dt * inv_masses[i]));
        }

        sauvegardeVTK(step, t, save_every, vtk_steps, vtk_times);
    }

    finaliseSauvegardeVTK(vtk_steps, vtk_times); 

    std::cout << "Simulation terminée\n";
}