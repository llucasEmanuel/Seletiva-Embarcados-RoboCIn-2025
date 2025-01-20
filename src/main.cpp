#include <GyroOdometry.h>
#include <cstdio>

// Instanciação da odometria do giroscópio
GyroOdometry gyroOdm;

int main() {
    // Inicializa a odometria do giroscópiO
    gyroOdm.setup();

    // Loop de execução
    while (true) {
        // Atualiza as grandezas da odometria
        gyroOdm.update();

        // Armazena a velocidade angular de z
        double angVelocityZ = gyroOdm.getAngularVelocityZ();

        // Variação angular com base na velocidade angular do eixo z
        double angVariation = gyroOdm.getAngularVariation();

        printf("Angular Velocity Z: %lf\tAngular Variation: %lf\n", angVelocityZ, angVariation);

        // Faz a thread dormir por FETCH_TIME para manter a frequência fixa
        ThisThread::sleep_for(FETCH_TIME); // Evita espera ocupada do loop
    }

    return 0;
}
