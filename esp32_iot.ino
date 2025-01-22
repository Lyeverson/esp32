#include <WiFi.h>
#include <WebSocketsClient.h>

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

  // Inicializar a conexão WebSocket
  webSocket.begin(websocket_server, 12345);  // Certifique-se de que a porta esteja correta
  webSocket.onEvent(webSocketEvent);
}

void loop() {
  webSocket.loop(); // Manter a comunicação com o servidor
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
      // Exibir os dados recebidos (temperatura e umidade)
      String receivedData = String((char*)payload);
      Serial.println("Dados recebidos: " + receivedData);
      break;
  }
}
