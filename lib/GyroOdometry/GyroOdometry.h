#ifndef GYROODOMETRY_H
#define GYROODOMETRY_H

#include "MPU6050.h"

// Usado na conversão de graus para radianos
#define PI 3.14159265358979323846

// Intervalo de tempo em que os dados do giroscópio serão coletados
#define FETCH_TIME 5ms

// Classe que implementa os métodos de odometria apenas do giroscópio
class GyroOdometry {
public:
    // Construtor seta os pinos SDA e SCL como sendo os do mestre (nucleo f767zi) por padrão 
    GyroOdometry(PinName sda = MASTER_SDA, PinName scl = MASTER_SCL);

    // Inicialização dos atributos da classe
    void setup();
    
    // Calcula a velocidade angular em rad/s e armazena em angVelocity
    void getAngularVelocity(double *angVelocity);

private:
    // Instanciação da MPU-6050, que se comunicará com a Nucleo f767zi
    MPU6050 mpu_;

    // Ticker que coordena a coleta dos dados a uma frequência fixa
    Ticker gyroTicker_;

    // Offset de leitura do giroscópio
    double offset_;

    // Fator usado na conversão para velocidade angular
    double sensitivity_;

    // Obtém o valor da sensibilidade do giroscópio
    double getGyroSensitivity();

    // Obtém o output bruto do giroscópio e armazena em buffer
    void getGyroRawOut(short *buffer);

    // Converte a velocidade angular de graus/s para rad/s
    void degreesToRadians(double *angVelocity);
};

#endif