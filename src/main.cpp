#include <GyroOdometry.h>

// Instanciação do módulo de odometria do giroscópio
GyroOdometry gyroOdm;

int main() {
    // Inicializa a odometria do giroscópiO
    gyroOdm.setup();

    // Armazena as velocidades angulares de cada eixo
    double angVelocity[3] = {0., 0., 0.};

    // Loop de execução
    while (true) {
        // Só executa se a flag estiver true (a cada 10ms -> frequência fixa)
        if (gyroOdm.getGyroFlag() == true) {
            // Calcula a velocidade angular (rad/s) de cada eixo
            gyroOdm.getAngularVelocity(angVelocity);
        }
    }

    return 0;
}
