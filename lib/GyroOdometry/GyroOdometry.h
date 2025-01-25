#ifndef GYROODOMETRY_H
#define GYROODOMETRY_H

#include <MPU6050.h>

// Usado na conversão de graus para radianos
#define PI 3.14159265358979323846

// Intervalo de tempo em que os dados do giroscópio serão levados para o mestre
#define FETCH_TIME 5ms

// Garante que o ângulo seja 0 para baixas variações (complementar à calibração de offset)
#define GYRO_TOLERANCE 9.64e-05 // Valor obtido por testes em 'test/tolerance'

// Pinos de I2C1 da nucleo f767zi
#define MASTER_SDA PB_9
#define MASTER_SCL PB_8

// Classe que implementa os métodos de odometria apenas do giroscópio
class GyroOdometry {
public:
    // Construtor seta os pinos SDA e SCL como sendo os do mestre (nucleo f767zi) por padrão 
    GyroOdometry(PinName sda = MASTER_SDA, PinName scl = MASTER_SCL);

    // Inicialização dos atributos necessários (Retorna se a mpu está funcionando)
    bool setup();

    // Atualiza os sample e calcula o valor com base neles
    void update();
    
    // Retorna o valor da velocidade angular no eixo z em rad/s
    // Recomendável usar apenas após o update (coleta o sample mais recente)
    double getAngularVelocityZ();

    // Retorna o valor da variação angular em rad
    // Recomendável usar apenas após o update (coleta o sample mais recente)
    double getAngularVariation();

private:
    // Instanciação da MPU6050, que se comunicará com a Nucleo f767zi
    MPU6050 mpu_;

    // Offset de leitura do giroscópio
    double offset_;

    // Armazena as velocidades angulares do último sample
    double angVelocity_[3];

    // Armazena a variação angular do último sample
    double angVariation_;

    // Calcula a velocidade angular em rad/s e armazena em angVelocity_
    void updateAngularVelocity();

    // Obtém o output bruto do giroscópio e armazena em buffer
    void getGyroRawOut(int16_t *buffer);

    // Converte a velocidade angular de graus/s para rad/s (sobrescreve o valor do array)
    void degreesToRadians();

    // Calcula o offset do giroscópio
    double getGyroOffset();
};

#endif