#include "GyroOdometry.h"
#include <fstream>

GyroOdometry::GyroOdometry(PinName sda, PinName scl) : mpu_(sda, scl) {
    this->offset_ = 0.;
    this->angVariation_ = 0.;
    for (int i = 0; i < 3; i++)
        this->angVelocity_[i] = 0.;
}

bool GyroOdometry::setup() {
    // Inicializa a MPU6050 com as configurações padrões
    bool mpuWorking = this->mpu_.initialize();

    // Seta o sample rate como 8kHz
    this->mpu_.setDLPFConfig(0); // Seta o DLPF_CFG como 0 
    this->mpu_.setSampleRate(0x00); // Seta o SMPLRT_DIV como 0

    // Faz o cálculo do offset com base nos dados do arquivo apenas na inicialização
    this->offset_ = this->getGyroOffset();

    return mpuWorking;
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
    double gyroOut[3];
    // Lê o output do giroscópio já em grau/s (já foi convertido de LSB/grau/s)
    this->mpu_.readGyro(gyroOut);

    printf("offset: %lf\n", this->offset_);

    // Calibra o resultado do eixo z
    this->angVelocity_[2] -= this->offset_;

    // Converte de graus/s para rad/s
    this->degreesToRadians();
}

void GyroOdometry::getGyroRawOut(int16_t *buffer) {
    this->mpu_.readGyroRaw(buffer);
}

void GyroOdometry::degreesToRadians() {
    const double convFactor = PI / 180.;
    // Multiplica graus/s pelo fator de conversão e obtém rad/s
    for (int i = 0; i < 3; i++) {
        this->angVelocity_[i] = convFactor * this->angVelocity_[i];
    }
}

double GyroOdometry::getGyroOffset() {
    // Caminho do arquivo com os samples de velocidade angular em que o robô está parado
    const char path[] = "test/calibration/data/steady_w.txt";

    // Abre o arquivo para leitura
    std::ifstream file(path);

    // Se não encontrar o arquivo retorna offset 0 por padrão
    if (!file) {
        printf("File not found\n");
        return 0;
    }
    // Se encontrar calcula a média das velocidades angulares nele
    else {
        double offset = 0, acumOffset = 0;
        int numSamples = 0;

        // Lê linha por linha do arquivo
        while (file >> offset) {
            acumOffset += offset;
            numSamples++;
        }

        // Evita divisão por 0 caso o arquivo não tenho nenhum valor com o robô parado
        if (numSamples > 0) 
            offset = acumOffset / (double) numSamples;

        file.close();

        return offset;
    }
}
