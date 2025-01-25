#include <iostream>
#include <fstream>
#include <cmath>

// Encontrada ao analisar o maior valor absoluto do delta_theta
#define TOLERANCE 9.64e-05// 9.63549e-05 = maior valor absoluto

int main() {
    std::ifstream file("calibration/data/steady_w.txt");
    if (!file) {
        std::cerr << "error opening file\n";
        return 1;
    }

    // Calculado no get_data.py
    const double offset = -0.42024403513892683;

    double prev_w = 0, w;
    double theta = 0;

    while (file >> w) {
        // calibra o offset
        w -= offset;

        // converte pra rad
        w *= M_PI/180.;
        
        double delta_theta = ((w + prev_w )/2.) * 0.005;

        // A variação precisa ser maior ou igual que a tolerância para ser relevante
        if (fabs(delta_theta) >= TOLERANCE) {
            theta += delta_theta;
            std::cout << "AAAAAAAAAAAAA: ";
            return 1;
        }

        // guarda o valor de w para a proxima iteração
        prev_w = w;

        std::cout << theta << '\n';
    }

    file.close();

    return 0;
}