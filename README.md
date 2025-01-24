# Projeto da Seletiva RobôCIn 2025 - Embarcados
Implementação da comunicação entre uma [MPU-6050](https://invensense.tdk.com/wp-content/uploads/2015/02/MPU-6000-Datasheet1.pdf) e uma [NUCLEO-F767ZI](https://www-st-com.translate.goog/en/evaluation-tools/nucleo-f767zi.html?_x_tr_sl=en&_x_tr_tl=pt&_x_tr_hl=pt&_x_tr_pto=tc) para transmissão de dados e cálculo da odometria do giroscópio usando [PlatformIO](https://platformio.org/) e [MbedOS](https://os.mbed.com/).

### Importante:
O branch *'main'* contém o código usando a minha implementação da biblioteca MPU6050, já o branch *'version-2'* contém o código com as mesmas funcionalidades, mas usando a biblioteca MPU6050 do [firmware SSL](https://github.com/robocin/ssl-firmware/tree/main) disponibilizada pelo RobôCIn.

## Visão Geral do Projeto
O objetivo principal é coletar o output do giroscópio, que corresponde à velocidade angular da MPU-6050, nos registradores do sensor e enviá-los para a NUCLEO-F767ZI. Com os dados na placa, é feita primeiro a conversão de LSB/(grau/s) para grau/s. No entanto, normalmente o dado medido pelo giroscópio possui um offset e é preciso calibrá-lo na placa. Dessa forma, usando o log de medidas fornecido, foi calculado o offset e subtraído do resultado bruto medido, obtendo a velocidade angular calibrada. Após isso, a velocidade angular é convertida para rad/s e ela é usada para obter a variação angular do robô. Como a velocidade angular é calculada a uma frequência fixa, usamos esse intervalo entre os cálculos e a média das velocidades angulares para obter uma variação angular mais precisa.

Para mais detalhes: [Slides da apresentação do projeto](https://www.canva.com/design/DAGcqC27xzI/j8jTnLOlu57EmrF3MJ6ERQ/view)