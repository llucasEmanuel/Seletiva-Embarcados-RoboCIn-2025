#ifndef GYROODOMETRY_H
#define GYROODOMETRY_H

#include "MPU6050.h"

// Usado na conversão de graus para radianos
#define PI 3.14159265358979323846

// Frequência em que os dados do giroscópio serão coletados
#define GYRO_FREQ 10ms

// Classe que implementa os métodos de odometria apenas do giroscópio
class GyroOdometry {
public:
    // Construtor seta os pinos SDA e SCL como sendo os do mestre (nucleo f767zi) por padrão 
    GyroOdometry(PinName sda = MASTER_SDA, PinName scl = MASTER_SCL);

    // Inicialização dos atributos da classe
    void setup();
    
    // Calcula a velocidade angular em rad/s e armazena em angVelocity
    void getAngularVelocity(double *angVelocity);

    // Retorna o valor da gyroFlag_
    bool getGyroFlag();
private:
    // Instanciação da MPU-6050, que se comunicará com a Nucleo f767zi
    MPU6050 mpu_;

    // Ticker que coordena a coleta dos dados a uma frequência fixa
    Ticker gyroTicker_;

    // Flag que indica quando os dados do giroscópio podem ser coletados
    bool gyroFlag_;

    // Obtém o valor da sensibilidade do giroscópio
    double getGyroSensitivity();

    // Obtém o output bruto do giroscópio e armazena em buffer
    void getGyroRawOut(short *buffer);

    // Remover o offset da leitura do sensor (deve ser feito a cada leitura) 
    // Valor do offset vai ser calculado em outra função
    // Sobrescreve o valor de gyroOut pelo valor calibrado
    void calibrateGyroOut(short *gyroOut);

    // Obtém o outputs processados e livres de ruído do giroscópio e armazena em bufer
    void getGyroCorrectedOut(short *buffer);

    // Converte a velocidade angular de graus/s para rad/s
    void degreesToRadians(double *angVelocity);

    // Método da interrupção que muda a gyroFlag_ e
    void changeFlag();
};

#endif