#include <iostream>
#include <fstream>
#include <cmath>

// Encontrada ao analisar o maior valor absoluto do delta_theta
#define TOLERANCE 9.64e-05// 9.63549e-05 = maior valor absoluto

int main() {
    std::ifstream w_file("test/results/w_input.txt");
    std::ofstream theta_file("test/results/theta_output.txt");

    if (!w_file || !theta_file) {
        std::cerr << "error opening file\n";
        return 1;
    }

    // Calculado no get_data 
    const double offset = -0.42024403513892683;

    double prev_w = 0, w;
    double theta = 0;

    while (w_file >> w) {
        // calibra o offset
        w -= offset;

        // converte pra rad
        w *= M_PI/180.;
        
        double delta_theta = ((w + prev_w )/2.) * 0.005;

        // A variação precisa ser maior ou igual que a tolerância para ser relevante
        if (fabs(delta_theta) >= TOLERANCE) {
            theta += delta_theta;
            if (theta > M_PI) {
                theta -= 2 * M_PI; // Ajusta para o intervalo [-pi, pi]
            } else if (theta < -M_PI) {
                theta += 2 * M_PI; // Ajusta para o intervalo [-pi, pi]
            }
        }

        // guarda o valor de w para a proxima iteração
        prev_w = w;

        theta_file << theta << '\n';
    }

    theta_file.close();
    w_file.close();

    return 0;
}