#include "MPU6050.h"

MPU6050::MPU6050(PinName sda, PinName scl) : i2c_(sda, scl) {
    // Comunicação com os regs deve ser feita a 400kHz
    this->i2c_.frequency(FM_FREQ);
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
    // Os registradores guardam o valor com complemento a 2, mas o sinal só muda o sentido nesse caso
    short xOut = (gyroOut[0] << 8) | gyroOut[1];
    short yOut = (gyroOut[2] << 8) | gyroOut[3];
    short zOut = (gyroOut[4] << 8) | gyroOut[5];

    // Cada elemento do buffer é uma componente das medidas do giroscópio
    buffer[0] = xOut;
    buffer[1] = yOut;
    buffer[2] = zOut;
}

void MPU6050::setGyroConfig(unsigned char fs_sel) {
    // Não seta valores fora do intervalo do fs_sel
    if (fs_sel > 4) return;

    // Endereço de 8 bits da MPU para passar no I2C
    char mpuAddress8bit = MPU6050_ADDRESS << 1;

    // Registrador onde ocorrerá escrita    
    char regAddress = GYRO_CONFIG;

    // PRECISA ESCREVER O BYTE TODO NO REGISTRADOR (Vai precisar dos bits de self-test)
}
