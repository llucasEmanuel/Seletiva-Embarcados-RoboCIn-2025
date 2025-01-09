#include <MPU6050.h>

void setup();
void loop();

void getAngularVelocity(short *data);
void calibrateGyroOut();

// Instanciação da MPU-6050, que se comunicará com a Nucleo f767zi
MPU6050 mpu;

int main() {
    setup();
    loop();
}

void setup() {

}

void loop() {
    while (true) {

    }
}

// Remover o ruído da leitura
void calibrateGyroOut() {

}

void getAngularVelocity(short *data) {
    // Precisa descobrir a sensibilidade antes
    // Ela é definida pelo FS_SEL do reg GYRO_CONFIG (Precisa da função de getGyroConfig antes)
}
