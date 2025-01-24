#ifndef MPU6050
#define MUP6050

#include "mbed.h"

// Slave address da MPU-6050 considerando AD0 = 0
#define MPU6050_ADDRESS 0b1101000 // LSB é determinado pelo nivel lógico do pino AD0


// Número de registradores que geram output do giroscópio
#define GYRO_OUT_NUM 6

// Registradores que definem o sample rate
#define SMPLRT_DIV 0x19
#define CONFIG 0x1A

// Registrador de configurações do giroscópio
#define GYRO_CONFIG 0x1B // 0d27

// Registradores de output
#define GYRO_XOUT_H 0x43 // 0d67
#define GYRO_XOUT_L 0x44 // 0d68
#define GYRO_YOUT_H 0x45 // 0d69
#define GYRO_YOUT_L 0x46 // 0d70
#define GYRO_ZOUT_H 0x47 // 0d71
#define GYRO_ZOUT_L 0x48 // 0d72

// Pinos de I2C1 da nucleo f767zi
#define MASTER_SDA PB_9
#define MASTER_SCL PB_8

// Frequência do I2C para o fast mode (FM) = 400kHz
#define FM_FREQ 400000

// Interface de comunicação do mestre com a MPU6050
class MPU6050 {
public:
    // Construtor recebe os pinos da comunicação I2C
    MPU6050(PinName sda, PinName scl);

    // Inicializa os registradores necessários da MPU6050
    void setup();

    // Lê numBytes registradores de forma sequencial partindo de regAddress e armazenando os dados em buffer. 
    // Para isso o tamanho do buffer deve ser de no mínimo numBytes.
    // Essa função garante que todos os dados serão do mesmo sample.
    void burstRead(char *regAddress, char *buffer, int numBytes);

    // Lê o valor do registrador regAddress e armazena em buffer
    void readReg(char *regAddress, char *buffer);

    // Escreve o valor de buffer no registrador regAddress
    void writeReg(char *regAddress, char *buffer);

    // Escreve os outputs do giroscópio em buffer
    void getGyroOut(short *buffer);

    // Altera a configuração do giroscópio (self-tests e full scale range)
    void setGyroConfig(
        unsigned char xg_st = 0, // Não faz self-test no x
        unsigned char yg_st = 0, // Não faz self-test no y
        unsigned char zg_st = 0, // Não faz self-test no z
        unsigned char fs_sel = 3 // Full scale range = +- 2000 graus/s -> 16.4 LSB/(grau/s)
    );

    // Obtém a configuração atual do registrador para saber o full scale range
    void getGyroConfig(char *buffer);

    // Escreve no registrador SMPLRT_DIV (padrão -> sample rate = 8kHz = gyroscope output rate / (1 + SMPLRT_DIV))
    void setSampleRateDiv(char smplrt_div = 0);

    // Escreve no registrador CONFIG (padrão -> gyroscope output rate = 8kHZ)
    void setConfig(unsigned char ext_sync_set = 0, unsigned char dlpf_cfg = 0);

    // Retorna o offset de leitura do sensor que será usado para calibrá-lo
    double getGyroOffset();

private:
    // Instanciação da comunicação I2C
    I2C i2c_;
};

#endif