#include "GyroOdometry.h"

GyroOdometry::GyroOdometry(PinName sda, PinName scl) : mpu_(sda, scl) {
    this->offset_ = 0.;
    this->sensitivity_ = 16.4; // +- 2000 graus/s
    for (int i = 0; i < 3; i++)
        this->angVelocity_[i] = 0.;
    this->angVariation_ = 0.;
}

void GyroOdometry::setup() {
    // Inicializa a MPU6050 com as configurações padrões
    this->mpu_.setup();

    // Obtém o fator de conversão da velocidade angular
    this->sensitivity_ = this->getGyroSensitivity();

    // Faz o cálculo do offset com base nos dados do arquivo apenas na inicialização
    this->offset_ = this->mpu_.getGyroOffset();
}

void GyroOdometry::update() {
    // Salva o valor do sample anterior
    double prevAngVelocity = this->angVelocity_[2];

    // Atualiza o array de velocidades angulares
    this->updateAngularVelocity();

    // Calcula a média das velocidades usando o sample anterior e o atual para ter um resultado mais preciso
    double avgAngVelocityZ = (prevAngVelocity + this->angVelocity_[2]) / 2.;

    // Converte o tempo entre os samples (5ms) de milisegundos para segundos
    const double fetchTimeSec = std::chrono::duration<double>(FETCH_TIME).count();

    // Calcula a variação do ângulo do fetch anterior até esse
    double deltaTheta = avgAngVelocityZ * fetchTimeSec;

    // deltaTheta precisa ser maior ou igual que a tolerância para ser relevante
    if (fabs(deltaTheta) >= GYRO_TOLERANCE) {
        this->angVariation_ += deltaTheta;
        // Limita a variação angular a [-PI, +PI]
        if (this->angVariation_ > PI) {
            this->angVariation_ -= 2 * PI;
        } else if (this->angVariation_ < -PI) {
            this->angVariation_ += 2 * PI;
        }
    }
}

double GyroOdometry::getAngularVelocityZ() {
    return this->angVelocity_[2];
}

double GyroOdometry::getAngularVariation() {
    return this->angVariation_;
}


void GyroOdometry::updateAngularVelocity() {
    short gyroOut[3];
    // Obtém o output do giroscópio antes de ser processado
    this->getGyroRawOut(gyroOut);

    // Converte da escala LSB para graus/s -> graus/s = LSB / (LSB/(graus/s))
    for (int i = 0; i < 3; i++) {
        this->angVelocity_[i] = ((double) gyroOut[i] / this->sensitivity_);
    }

    // Calibra o resultado do eixo z
    this->angVelocity_[2] -= this->offset_;

    // Converte de graus/s para rad/s
    this->degreesToRadians();
}

double GyroOdometry::getGyroSensitivity() {
    char gyroConfig;
    // Lê o registrador GYRO_CONFIG e armazena em gyroConfig
    this->mpu_.getGyroConfig(&gyroConfig);

    // Seleciona apenas os bits 3 e 4 do registrador
    char gyroConfig34 = gyroConfig & 0b00011000;
    // Faz o shift do valor para isolar o fs_sel
    char fs_sel = gyroConfig34 >> 3;    

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

void GyroOdometry::degreesToRadians() {
    const double convFactor = PI / 180.;
    // Multiplica graus/s pelo fator de conversão e obtém rad/s
    for (int i = 0; i < 3; i++) {
        this->angVelocity_[i] = convFactor * this->angVelocity_[i];
    }
}
