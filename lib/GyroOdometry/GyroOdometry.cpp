#include "GyroOdometry.h"

GyroOdometry::GyroOdometry(PinName sda, PinName scl) : mpu_(sda, scl) {
    this->gyroFlag_ = false;
}

void GyroOdometry::setup() {
    // Inicializa a MPU6050
    this->mpu_.setup();

    // Gera interrupção que altera flag de coleta de dados do giroscópio a cada 10ms
    this->gyroTicker_.attach(callback(this, &GyroOdometry::changeFlag), GYRO_FREQ); // Checar se roda com 10s ou 10ms
}

void GyroOdometry::getAngularVelocity(double *angVelocity) {
    // Obtém a sensibilidade do giroscópio
    const double sensitivity = this->getGyroSensitivity();

    short gyroOut[3];
    this->getGyroCorrectedOut(gyroOut); 

    // Converte da escala LSB para graus/s -> graus/s = LSB / (LSB/(graus/s))
    for (int i = 0; i < 3; i++) {
        angVelocity[i] = ((double) gyroOut[i] / sensitivity);
    }

    // Converte de graus/s para rad/s
    this->degreesToRadians(angVelocity);

    // Reseta a flag para garantir que o métodos só executa 10ms depois
    this->gyroFlag_ = false;
}

bool GyroOdometry::getGyroFlag() {
    return this->gyroFlag_;
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


void GyroOdometry::calibrateGyroOut(short *gyroOut) {
    // ...
}

void GyroOdometry::getGyroCorrectedOut(short *buffer) {
    // Lê o output bruto
    this->getGyroRawOut(buffer);
    this->calibrateGyroOut(buffer);
}


void GyroOdometry::degreesToRadians(double *angVelocity) {
    const double convFactor = PI / 180.;
    // Multiplica graus/s pelo fator de conversão e obtém rad/s
    for (int i = 0; i < 3; i++) {
        angVelocity[i] = convFactor * angVelocity[i];
    }
}

void GyroOdometry::changeFlag() {
    this->gyroFlag_ = !this->gyroFlag_;
}