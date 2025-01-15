#include <GyroOdometry.h>

// Instanciação do módulo de odometria do giroscópio
GyroOdometry gyroOdm;

int main() {
    // Inicializa a odometria do giroscópiO
    gyroOdm.setup();

    // Armazena as velocidades angulares de cada eixo
    double angVelocity[3] = {0., 0., 0.};

    // Variação angular com base na velocidade angular do eixo z
    double angVariation = 0.;

    // Converte o GYRO_FREQ em double
    const double sampleTimeSec = std::chrono::duration<double>(GYRO_FREQ).count();

    // TODO: Tirar a espera ocupada quando a flag é falsa
    // Talvez fazer a thread dormir dps de executar o corpo do loop e tirar a interrupção
    
    // Loop de execução
    while (true) {
        // Só executa se a flag estiver true (a cada 10ms -> frequência fixa)
        if (gyroOdm.getGyroFlag() == true) {

            // Variável que armazena o valor da velocidade angular em z do sample anterior  
            double prevAngVelocity = angVelocity[2];

            // Atualiza a velocidade angular (rad/s) de cada eixo e inverte a gyroFlag_
            gyroOdm.getAngularVelocity(angVelocity);

            // Calcula a velocidade média entre os samples para ter um resultado mais preciso
            double avgAngVelocity = (prevAngVelocity + angVelocity[2]) / 2.;

            // Calcula a variação angular considerando o intervalo de 10ms entre os samples
            angVariation += (avgAngVelocity * sampleTimeSec);
        }
    }

    return 0;
}
