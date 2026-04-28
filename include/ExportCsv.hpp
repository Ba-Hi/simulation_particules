#pragma once
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include "Particule.hpp"
 
// Appends one line per timestep: t x0 y0 x1 y1 x2 y2 ...
inline void saveCSV(const std::vector<Particule>& particles,
                    std::ofstream& file,
                    double t) {
    file << t;
    for (const auto& p : particles)
        file << " " << p.getPosition().x()
             << " " << p.getPosition().y();
    file << "\n";
}