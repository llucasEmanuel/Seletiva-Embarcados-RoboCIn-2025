#include <GyroOdometry.h>

// Instanciação do módulo de odometria do giroscópio
GyroOdometry gyroOdm;

int main() {
    // Inicializa a odometria do giroscópiO
    gyroOdm.setup();

    // Armazena as velocidades angulares de cada eixo
    double angVelocity[3] = {0., 0., 0.};

    // Variação angular com base na velocidade angular do eixo z
    double angVariation = 0;

    // Loop de execução
    while (true) {
        // Só executa se a flag estiver true (a cada 10ms -> frequência fixa)
        if (gyroOdm.getGyroFlag() == true) {
            // Calcula a velocidade angular (rad/s) de cada eixo
            gyroOdm.getAngularVelocity(angVelocity);

            
            // O método de calcular a variação angular deve ser chamado após o cálculo da velocidade angular
            // Dessa forma, ele conesgue usar os valores do sample mais recente e ter um resultado atualizado
            // Quanto à gyroFlag_, ela deve ser setada como 'false' após os dois terminarem
        }
    }

    return 0;
}
