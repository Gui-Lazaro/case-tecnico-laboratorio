import paho.mqtt.client as mqtt
import csv
import os
from datetime import datetime

BROKER = "broker.hivemq.com"
PORT = 1883
TOPICOS = {
    "/m4bknr5d/data/sensores/temperatura": "temperatura.csv",
    "/m4bknr5d/data/sensores/umidade": "umidade.csv",
    "/m4bknr5d/data/sensores/corrente": "corrente.csv",
    "/m4bknr5d/data/sensores/tensao": "tensao.csv",
}

arquivos_abertos = {}
escritores_csv = {}


def inicializar_arquivos_csv():
    for topico, arquivo in TOPICOS.items():
        if not os.path.exists(arquivo):
            with open(arquivo, mode="w", newline="", encoding="utf-8") as f:
                writer = csv.writer(f)
                writer.writerow(["timestamp", "valor"])
        file_handle = open(arquivo, mode="a", newline="", encoding="utf-8")
        arquivos_abertos[arquivo] = file_handle
        escritores_csv[arquivo] = csv.writer(file_handle)


def escrever_csv(arquivo, valor):
    timestamp = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
    escritores_csv[arquivo].writerow([timestamp, valor])
    arquivos_abertos[arquivo].flush()


def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("Conectado ao broker MQTT com sucesso!")
        for topico in TOPICOS.keys():
            client.subscribe(topico)
            print(f"Inscrito no tópico: {topico}")
    else:
        print(f"Falha na conexão. Código de retorno: {rc}")


def on_message(client, userdata, msg):
    valor = msg.payload.decode("utf-8")
    topico = msg.topic
    print(f"Mensagem recebida no tópico {topico}: {valor}")
    if topico in TOPICOS:
        escrever_csv(TOPICOS[topico], valor)


inicializar_arquivos_csv()


cliente = mqtt.Client()


cliente.on_connect = on_connect
cliente.on_message = on_message


print("Conectando ao broker MQTT...")
cliente.connect(BROKER, PORT)

try:
    print("Aguardando mensagens...")
    cliente.loop_forever()
except KeyboardInterrupt:
    print("\nEncerrando o programa...")


for arquivo, handle in arquivos_abertos.items():
    handle.close()

print("Arquivos CSV fechados. Programa encerrado.")
