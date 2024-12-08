#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFi.h>
#include <cstdio>
#include <Wire.h>

// biblioteca para comunicação com o SHT40 via I2C
#include <SensirionI2cSht4x.h>

// exigido pela SensirionI2cSht4x, para usar a definição correta de NO_ERROR
#ifdef NO_ERROR
#undef NO_ERROR
#endif
#define NO_ERROR 0

// criar instância para comunicação com o SHT40
SensirionI2cSht4x sht40;

// biblioteca para comunicação com o PZEM
#include <PZEM004Tv30.h>

// criar instância da classe no hardware serial rx=16 tx=17
PZEM004Tv30 pzem(Serial2, 16, 17);

const uint8_t pinout_temperatura = 34;
const uint8_t pinout_umidade = 35;

const char *mqtt_topic_temperatura = "/m4bknr5d/data/sensores/temperatura";
const char *mqtt_topic_umidade = "/m4bknr5d/data/sensores/umidade";
const char *mqtt_topic_corrente = "/m4bknr5d/data/sensores/corrente";
const char *mqtt_topic_tensao = "/m4bknr5d/data/sensores/tensao";

const char *wifi_ssid = "Nome do Wi-Fi";
const char *wifi_senha = "12345678";

const char *mqtt_client_id = "firmware-testes-0001";
const char *mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;

WiFiClient wifi_client;
PubSubClient mqtt_client(wifi_client);

float temperatura;
float umidade;
float corrente;
float tensao;

void setup()
{
  Serial.begin(115200);

  // Inicializar pzem
  pzem.resetEnergy();

  // Inicializar I2C
  Wire.begin();
  sht40.begin(Wire, SHT40_I2C_ADDR_44);
  sht40.softReset();

  pinMode(pinout_temperatura, INPUT);
  pinMode(pinout_umidade, INPUT);

  Serial.print("Conectando ao Wi-Fi...");
  WiFi.begin(wifi_ssid, wifi_senha);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
  }
  Serial.println();
  Serial.println("Conectado!");

  // inicializar MQTT
  mqtt_client.setServer(mqtt_server, mqtt_port);
}

void publish_float(float valor, const char *topico)
{
  char buffer[32];                                 // Allocate enough space for float-to-string conversion
  snprintf(buffer, sizeof(buffer), "%.2f", valor); // Format as a string with two decimal places
  mqtt_client.publish(topico, buffer);             // Publish as a string
}

void loop()
{
  // se temos wi-fi mas não temos mqtt...
  while (wifi_client.connected() && !mqtt_client.connected())
  {
    // conectar ao mqtt
    while (!mqtt_client.connect(mqtt_client_id))
    {
      Serial.println("Fazendo conexão ao MQTT...");
      delay(100);
    }

    Serial.println("Conectado!");
  }

  // fazer coleta dos sensores
  corrente = pzem.current();
  tensao = pzem.voltage();
  int16_t read_error = sht40.measureLowestPrecision(temperatura, umidade);
  if (read_error != NO_ERROR)
  {
    // não conseguimos ler esses dados por algum motivo
    temperatura = NAN;
    umidade = NAN;
  }

  // publicar dados coletados
  publish_float(temperatura, mqtt_topic_temperatura);
  publish_float(umidade, mqtt_topic_umidade);
  publish_float(corrente, mqtt_topic_corrente);
  publish_float(tensao, mqtt_topic_tensao);

  delay(100);
}
