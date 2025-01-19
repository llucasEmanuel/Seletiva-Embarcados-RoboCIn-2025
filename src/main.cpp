#include <GyroOdometry.h>

// Instanciação da odometria do giroscópio
GyroOdometry gyroOdm;

int main() {
    // Inicializa a odometria do giroscópiO
    gyroOdm.setup();

    // Armazena as velocidades angulares de cada eixo
    double angVelocity[3] = {0., 0., 0.};

    // Variação angular com base na velocidade angular do eixo z
    double angVariation = 0.;

    // Converte o FETCH_TIME(5ms) em segundos
    const double fetchTimeSec = std::chrono::duration<double>(FETCH_TIME).count();
    
    // Loop de execução
    while (true) {
        // Variável que armazena o valor da velocidade angular em z do sample anterior  
        double prevAngVelocityZ = angVelocity[2];

        // Atualiza a velocidade angular (rad/s) de cada eixo
        gyroOdm.getAngularVelocity(angVelocity);

        // Calcula a velocidade angular média entre o sample atual e o anterior para um resultado mais preciso
        double avgAngVelocityZ = (prevAngVelocityZ + angVelocity[2]) / 2.;

        // Calcula a variação angular considerando o intervalo de FETCH_TIME entre os samples
        angVariation += (avgAngVelocityZ * fetchTimeSec);

        // Faz a thread dormir por FETCH_TIME para manter a frequência de leitura fixa
        ThisThread::sleep_for(FETCH_TIME); // Evita espera ocupada do loop
    }

    return 0;
}
