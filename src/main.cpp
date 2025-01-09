#include <GyroOdometry.h>

// Instanciação do módulo de odometria do giroscópio
GyroOdometry gyroOdometry;

int main() {
    // Inicializa a odometria do giroscópiO
    gyroOdometry.setup();

    // Armazena as velocidades angulares de cada eixo
    double angVelocity[3] = {0., 0., 0.};

    // Loop de execução
    while (true) {
        // Calcula a velocidade angular (rad/s) de cada eixo
        gyroOdometry.getAngularVelocity(angVelocity);
    }

    return 0;
}
