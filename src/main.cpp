#include "mbed.h"

DigitalOut led(LED1);
UnbufferedSerial serial(USBTX, USBRX);

int main() {
    serial.baud(9600);
    serial.write("Inicializando...", 17);

    while(true) {
        led = !led;
        serial.write("Led alternado!", 16);
        ThisThread::sleep_for(500ms);
    }
}
