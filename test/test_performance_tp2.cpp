#include <iostream>
#include <vector>
#include <chrono>
#include <cmath>
#include <functional>
#include <cstdlib>
#include "Particule.hpp"
#include "Vector.hpp"
#include "UniversGravitationnel.hpp"

// fonction lambda pour mesurer le temps d'exécution d'une d'un programme f
double mesurer(std::function<void()> f) {
    auto start = std::chrono::steady_clock::now();
    f();
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    return elapsed_seconds.count();
}


// insertion sans reserve
double insertion_sans_reserve(int n) {
    return mesurer([&]() {
        UniversGravitationnel u(3);
        for (int i = 0; i < n; ++i)
            u.ajouterParticule(Particule(
                Vector((double)rand()/RAND_MAX,
                       (double)rand()/RAND_MAX,
                       (double)rand()/RAND_MAX),
                Vector(0,0,0), 1.0, i, 0, Vector(0,0,0)));
    });
}

// insertion avec reserve
double insertion_avec_reserve(int n) {
    return mesurer([&]() {
        UniversGravitationnel u(3, n);
        for (int i = 0; i < n; ++i)
            u.ajouterParticule(Particule(
                Vector((double)rand()/RAND_MAX,
                       (double)rand()/RAND_MAX,
                       (double)rand()/RAND_MAX),
                Vector(0,0,0), 1.0, i, 0, Vector(0,0,0)));
    });
}

int main() {
    std::vector<int> tailles = {64, 128, 256, 512, 1024, 2048, 4096};

    for (int n : tailles) {
        double sans  = insertion_sans_reserve(n);
        double avec  = insertion_avec_reserve(n);
        double ratio = (avec > 0) ? sans / avec : 0;

        std::cout << "N = " << n
                  << " // Sans reserve: " << sans << " s"
                  << " // Avec reserve: " << avec << " s"
                  << " // Ratio (sans/avec): " << ratio
                  << "\n";
    }

    return 0;
}