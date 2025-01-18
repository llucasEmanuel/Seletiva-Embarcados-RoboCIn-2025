#include "GyroOdometry.h"

GyroOdometry::GyroOdometry(PinName sda, PinName scl) : mpu_(sda, scl) {
    this->offset_ = 0.;
    this->sensitivity_ = 16.4; // +- 2000 graus/s
}

void GyroOdometry::setup() {
    // Inicializa a MPU6050
    this->mpu_.setup();

    // Obtém o fator de conversão da velocidade angular
    this->sensitivity_ = this->getGyroSensitivity();

    // Faz o cálculo do offset com base nos dados do arquivo apenas na inicialização
    this->offset_ = this->mpu_.getGyroOffset();
}

void GyroOdometry::getAngularVelocity(double *angVelocity) {
    short gyroOut[3];
    // Obtém o output do giroscópio antes de ser processado
    this->getGyroRawOut(gyroOut);

    // Converte da escala LSB para graus/s -> graus/s = LSB / (LSB/(graus/s))
    for (int i = 0; i < 3; i++) {
        angVelocity[i] = ((double) gyroOut[i] / this->sensitivity_);
    }

    // Calibra o resultado do eixo z
    angVelocity[2] -= this->offset_;

    // Converte de graus/s para rad/s
    this->degreesToRadians(angVelocity);
}

double GyroOdometry::getGyroSensitivity() {
    char config;
    // Lê o registrador GYRO_CONFIG e armazena em config
    this->mpu_.getGyroConfig(&config);

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

void GyroOdometry::getGyroRawOut(short *buffer) {
    this->mpu_.getGyroOut(buffer);
}

void GyroOdometry::degreesToRadians(double *angVelocity) {
    const double convFactor = PI / 180.;
    // Multiplica graus/s pelo fator de conversão e obtém rad/s
    for (int i = 0; i < 3; i++) {
        angVelocity[i] = convFactor * angVelocity[i];
    }
}
