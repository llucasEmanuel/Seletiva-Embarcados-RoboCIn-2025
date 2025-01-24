#include <GyroOdometry.h>

// Instanciação da odometria do giroscópio
GyroOdometry gyroOdm;

int main() {
    // Inicializa a odometria do giroscópiO
    bool mpuWorking = gyroOdm.setup();

    // Loop de execução (só executa se a MPU for inicializada corretamente)
    while (mpuWorking) {
        // Atualiza as grandezas da odometria
        gyroOdm.update();

        // Armazena a velocidade angular de z
        double angVelocityZ = gyroOdm.getAngularVelocityZ();

        // Armazena a variação angular com base na velocidade angular de z
        double angVariation = gyroOdm.getAngularVariation();

        printf("Angular Velocity Z: %.21lf\tAngular Variation: %.21lf\n", angVelocityZ, angVariation);

        // Faz a thread dormir por FETCH_TIME(=5ms) para manter a frequência fixa
        ThisThread::sleep_for(FETCH_TIME); // Evita espera ocupada do loop
    }

    return 0;
}
