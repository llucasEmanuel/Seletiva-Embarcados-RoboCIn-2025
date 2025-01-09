#include <MPU6050.h>

#define PI 3.14159265358979323846

double getGyroSensitivity();
void getAngularVelocity(double *angVelocity, short *gyroOutCorrected, const double &sensitivity);
void calibrateGyroOut(short *gyroOutCorrected, short *gyroOut);
void degreesToRadians(double *angVelocity);

// Instanciação da MPU-6050, que se comunicará com a Nucleo f767zi
MPU6050 mpu;

int main() {
    // inicializa os registradores importantes da MPU6050
    mpu.setup();
    // Lê o valor dele
    const double sensitivity = getGyroSensitivity();

    // Armazena os outputs brutos do giroscópio
    short gyroOut[3] = {0, 0, 0};

    // Armazena os outputs calibrados do giroscópio 
    short gyroOutCorrected[3] = {0, 0, 0};

    // Armazena as velocidades angulares de cada eixo
    double angVelocity[3] = {0., 0., 0.};

    // Loop de execução
    while (true) {
        // Obtém os ouputs brutos do giroscópio
        mpu.getGyroOut(gyroOut);
        // Calibra os outbuts brutos limpando o offset
        calibrateGyroOut(gyroOutCorrected, gyroOut);
        // Calcula a velocidade angular (graus/s) de cada eixo
        getAngularVelocity(angVelocity, gyroOutCorrected, sensitivity);
        // Converte a velocidade angular de graus/s para rad/s
        degreesToRadians(angVelocity);
    }

    return 0;
}

double getGyroSensitivity() {
    char config;
    // Lê o registrador GYRO_CONFIG e armazena em config
    mpu.getGyroConfig(&config);

    // Seleciona apenas os bits 3 e 4 do registrador
    char config_3_4 = config & 0b00011000;
    // Faz o shift do valor para isolar o fs_sel
    char fs_sel = config_3_4 >> 3;    

    double sensitivity;
    // Seleciona o valor da sensibilidade de acordo com o fs_sel
    switch (fs_sel) {
        case 0:
            sensitivity = 131.;
            break;
        case 1:
            sensitivity = 65.5;
            break;
        case 2:
            sensitivity = 32.8;
            break;
        case 3:
            sensitivity = 16.4;
            break;
        default:
            break;
    }

    return sensitivity;
}

// Remover o offset da leitura do sensor (deve ser feito a cada leitura) 
// Valor do offset vai ser calculado em outra função
void calibrateGyroOut(short *gyroOutCorrected, short *gyroOut) {

}

void getAngularVelocity(double *angVelocity, short *gyroOutCorrected, const double &sensitivity) {
    // Converte da escala LSB para graus/s -> graus/s = LSB / (LSB/(graus/s))
    for (int i = 0; i < 3; i++) {
        angVelocity[i] = ((double) gyroOutCorrected[i] / sensitivity);
    }
}

void degreesToRadians(double *angVelocity) {
    const double convFactor = PI / 180.;
    // Multiplica graus/s pelo fator de conversão e obtém rad/s
    for (int i = 0; i < 3; i++) {
        angVelocity[i] = convFactor * angVelocity[i];
    }
}
