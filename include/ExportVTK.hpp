#pragma once
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include "Particule.hpp"

/**
 * @brief Sauvegarde les particules dans un fichier VTK XML (.vtp)
 *        lisible par Paraview/Visit.
 * 
 * @param particles Liste des particules
 * @param step      Numéro du pas de temps (utilisé pour le nom du fichier)
 * @param t         Temps courant
 */
inline void saveVTK(const std::vector<Particule>& particles,
                    int step,
                    double t) {

    // Nom du fichier : output_0000.vtp, output_0001.vtp, ...
    std::ostringstream filename;
    filename << "output_" << std::setw(4) << std::setfill('0') << step << ".vtp";

    std::ofstream file(filename.str());
    if (!file) throw std::runtime_error("Impossible d'ouvrir " + filename.str());

    int N = particles.size();

    file << "<?xml version=\"1.0\"?>\n";
    file << "<VTKFile type=\"PolyData\" version=\"0.1\" byte_order=\"LittleEndian\">\n";
    file << "  <PolyData>\n";
    file << "    <Piece NumberOfPoints=\"" << N << "\" NumberOfVerts=\"0\" "
         << "NumberOfLines=\"0\" NumberOfStrips=\"0\" NumberOfPolys=\"0\">\n";

    file << "      <Points>\n";
    file << "        <DataArray type=\"Float32\" NumberOfComponents=\"3\" format=\"ascii\">\n";
    for (const auto& p : particles) {
        file << "          "
             << p.getPosition().x() << " "
             << p.getPosition().y() << " "
             << p.getPosition().z() << "\n";
    }
    file << "        </DataArray>\n";
    file << "      </Points>\n";

    file << "      <PointData>\n";

    // Vitesse
    file << "        <DataArray type=\"Float32\" Name=\"Velocity\" "
         << "NumberOfComponents=\"3\" format=\"ascii\">\n";
    for (const auto& p : particles) {
        file << "          "
             << p.getVitesse().x() << " "
             << p.getVitesse().y() << " "
             << p.getVitesse().z() << "\n";
    }
    file << "        </DataArray>\n";

    // Masse
    file << "        <DataArray type=\"Float32\" Name=\"Masse\" format=\"ascii\">\n";
    for (const auto& p : particles)
        file << "          " << p.getMasse() << "\n";
    file << "        </DataArray>\n";

    // Catégorie (pour colorer par type dans Paraview)
    file << "        <DataArray type=\"Int32\" Name=\"Categorie\" format=\"ascii\">\n";
    for (const auto& p : particles)
        file << "          " << p.getType() << "\n";
    file << "        </DataArray>\n";

    file << "      </PointData>\n";

    file << "    </Piece>\n";
    file << "  </PolyData>\n";
    file << "</VTKFile>\n";
}

/**
 * @brief Génère un fichier .pvd qui regroupe tous les fichiers VTK
 *        en une série temporelle lisible par Paraview.
 * 
 * @param steps  Liste des pas de temps sauvegardés
 * @param times  Temps correspondant à chaque pas
 */
inline void savePVD(const std::vector<int>& steps,
                    const std::vector<double>& times) {

    std::ofstream file("simulation.pvd");
    file << "<?xml version=\"1.0\"?>\n";
    file << "<VTKFile type=\"Collection\" version=\"0.1\">\n";
    file << "  <Collection>\n";
    for (size_t i = 0; i < steps.size(); ++i) {
        std::ostringstream fname;
        fname << "output_" << std::setw(4) << std::setfill('0') << steps[i] << ".vtp";
        file << "    <DataSet timestep=\"" << times[i]
             << "\" file=\"" << fname.str() << "\"/>\n";
    }
    file << "  </Collection>\n";
    file << "</VTKFile>\n";
}