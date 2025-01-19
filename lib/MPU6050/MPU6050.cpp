#include "MPU6050.h"
#include <fstream>

MPU6050::MPU6050(PinName sda, PinName scl) : i2c_(sda, scl) {
    // Comunicação com os regs deve ser feita a 400kHz (fonte: documentação)
    this->i2c_.frequency(FM_FREQ);
}

void MPU6050::setup() {
    // Configura a sensibilidade correta do sensor
    this->setGyroConfig();
}

void MPU6050::burstRead(char *regAddress, char *buffer, int numBytes) {
    // Endereço de 8 bits da MPU para passar no I2C
    char mpuAddress8bit = MPU6050_ADDRESS << 1;

    // Solicita escrita no registrador (não libera o barramento ainda)
    this->i2c_.write(mpuAddress8bit, regAddress, 1, true);

    // Lê os dados de registradores consecutivos e os armazena em buffer
    this->i2c_.read(mpuAddress8bit, buffer, numBytes);
}

void MPU6050::readReg(char *regAddress, char *buffer) {
    // Faz um burstRead de 1 byte
    this->burstRead(regAddress, buffer, 1);
}

void MPU6050::writeReg(char *regAddress, char *buffer) {
    // Endereço de 8 bits da MPU para passar no I2C
    char mpuAddress8bit = MPU6050_ADDRESS << 1;

    // Solicita escrita no registrador (não libera o barramento ainda)
    this->i2c_.write(mpuAddress8bit, regAddress, 1, true);

    // Escreve o dado de buffer no registrador regAddress
    this->i2c_.write(mpuAddress8bit, buffer, 1);
}

void MPU6050::getGyroOut(short *buffer) {
    // Registrador base
    char regAddress = GYRO_XOUT_H; 

    // Cada elemento corresponde a um output de registrador
    char gyroOut[GYRO_OUT_NUM];

    // Lê os dados dos 6 registradores de output do giroscópio
    this->burstRead(&regAddress, gyroOut, GYRO_OUT_NUM);

    // Usa os regs H e L para montar o valor de 16 bits
    short xOut = (gyroOut[0] << 8) | gyroOut[1];
    short yOut = (gyroOut[2] << 8) | gyroOut[3];
    short zOut = (gyroOut[4] << 8) | gyroOut[5];

    // Cada elemento do buffer é uma componente das medidas do giroscópio
    buffer[0] = xOut;
    buffer[1] = yOut;
    buffer[2] = zOut;
}

void MPU6050::setGyroConfig(unsigned char xg_st, unsigned char yg_st, unsigned char zg_st, unsigned char fs_sel) {
    // Valores disponíveis para fs_sel = {0, 1, 2, 3} e bits de self-test = {0, 1}
    if (xg_st > 1 || yg_st > 1 || zg_st > 1 || fs_sel > 3) return;

    // Desloca os valores para condizer ao formato do byte esperado pelo registrador
    // | xg_st | yg_st | zg_st | fs_sel[2] | reservado[3] |
    xg_st = xg_st << 7;
    yg_st = yg_st << 6;
    zg_st = zg_st << 5;
    fs_sel = fs_sel << 3;

    // Endereço do registrador de configuração
    char regAddress = GYRO_CONFIG;

    // Monta o byte que será escrito no registrador
    char byte = xg_st | yg_st | zg_st | fs_sel;

    // Escreve no GYRO_CONFIG
    this->writeReg(&regAddress, &byte);
}

void MPU6050::getGyroConfig(char *buffer) {
    // Endereço do registrador de configuração
    char regAddress = GYRO_CONFIG;

    // Lê o byte do GYRO_CONFIG
    this->readReg(&regAddress, buffer);
}

double MPU6050::getGyroOffset() {
    // Caminho do arquivo com os samples de velocidade angular em que o robô está parado
    const char path[] = "test/calibration/steady_w.txt";

    // Abre o arquivo para leitura
    std::ifstream file(path);

    // Se não encontrar o arquivo retorna offset 0 por padrão
    if (!file) return 0;
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


