# Projeto de Case ESP32

Projeto para medir temperatura, umidade, corrente e tensão de um sistema, utilizando um sensor SHT40 e um sensor PZEM, conectados a um microcontrolador ESP32, que enviará esses dados para um servidor MQTT, que foi desenvolvido em Python.

## Especificações do hardware

- Microcontrolador ESP32
- Sensor SHT40
- Sensor PZEM004Tv30

## Requisitos do software

- Ambiente de desenvolvimento para ESP32
- Biblioteca MQTT para ESP32
- Biblioteca Paho MQTT para Python
- Python

# Instruções para uso

- Instalar as bibliotecas necessárias para o ESP32 e Python
- Configurar as conexões no código do ESP32 e do Python
- Inicializar os sensores no código do ESP32
- Inicializar os arquivos CSV e os escritores no código do Python
- Conectar o ESP32 ao Wi-Fi e ao broker MQTT
- Iniciar o servidor do Python

## Funcionamento do projeto

- O microcontrolador ESP32 lê os dados dos sensores SHT40 e PZEM.
- O ESP32 publica esses dados no servidor MQTT.
- O servidor MQTT, desenvolvido em Python, recebe as mensagens e as grava em arquivos CSV.
- O servidor também recebe os dados dos sensores, caso existam outros dispositivos conectados, e os grava em arquivos CSV também.

## Observações importantes

- Certifique-se de que o microcontrolador esteja configurado corretamente.
- Certifique-se de que o Wi-Fi esteja funcionando corretamente e que o broker MQTT esteja acessível.
- Certifique-se de que o servidor Python esteja rodando corretamente e que os arquivos CSV estejam sendo escritos corretamente.

# Autoria

Guilherme Lazaro de Oliveira
