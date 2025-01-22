#include <WiFi.h>
#include <WebSocketsClient.h>
#include <DHT.h>

// Definir o tipo do sensor DHT (DHT11 ou DHT22)
#define DHTPIN 15         // Pino GPIO 15
#define DHTTYPE DHT11     // Ou DHT11
DHT dht(DHTPIN, DHTTYPE);

// Substitua pelos seus dados de rede Wi-Fi
const char *ssid = "NOME_DA_REDE";
const char *password = "SENHA_DA_REDE";

// Endereço do servidor WebSocket
const char *websocket_server = "wss://websocket-production-e602.up.railway.app"; // URL do servidor WebSocket

WebSocketsClient webSocket;

void setup() {
  Serial.begin(115200);

  // Conectar-se à rede Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando ao WiFi...");
  }
  Serial.println("Conectado ao WiFi!");

  // Inicializar o sensor DHT
  dht.begin();

  // Inicializar a conexão WebSocket
  webSocket.begin(websocket_server, 12345);  // Certifique-se de que a porta esteja correta
  webSocket.onEvent(webSocketEvent);
}

void loop() {
  webSocket.loop(); // Manter a comunicação com o servidor

  // Ler os dados do sensor DHT
  float temperature = dht.readTemperature(); // Temperatura em Celsius
  float humidity = dht.readHumidity();      // Umidade relativa

  // Verificar se a leitura falhou
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Falha na leitura do sensor DHT!");
    return;
  }

  // Criar a mensagem com os dados do sensor
  String message = "Temperatura: " + String(temperature) + "C, Umidade: " + String(humidity) + "%";

  // Enviar os dados para o servidor WebSocket
  webSocket.sendTXT(message);
  Serial.println("Mensagem enviada: " + message);

  delay(5000);  // Enviar a cada 5 segundos
}

// Evento WebSocket
void webSocketEvent(WStype_t type, uint8_t *payload, size_t length) {
  switch (type) {
    case WStype_DISCONNECTED:
      Serial.println("Desconectado do servidor");
      break;
    case WStype_CONNECTED:
      Serial.println("Conectado ao servidor WebSocket");
      break;
    case WStype_TEXT:
      Serial.println("Mensagem recebida: " + String((char*)payload));
      break;
  }
}
