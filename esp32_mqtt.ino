#include <WiFi.h>
#include <PubSubClient.h>

#define WIFISSID "<WIFI SSID>"
#define PASSWORD "<Clave WiFi>"
#define TOPIC "esp32/data"

char mqttBroker[]  = "<IP MQTT BROKER>";
char payload[100];

WiFiClient esp32client;
PubSubClient client(esp32client);

void setup_wifi() {
  // Conectarse al WiFi
  Serial.println();
  Serial.print("Conectando a ");
  Serial.println(WIFISSID);

  WiFi.begin(WIFISSID, PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi conectado");
  Serial.println("IP: ");
  Serial.println(WiFi.localIP());
}


void callback(char* topic, byte* payload, unsigned int length) {
  char p[length + 1];
  memcpy(p, payload, length);
  p[length] = NULL;
  String message(p);
  Serial.println(message);
}

void reconnect() {
  // Intentar hasta conectarse
  while (!client.connected()) {
    Serial.print("Conectandose al broker MQTT...");
    // Intento de conección
    if (client.connect("ESP32Client")) {
      Serial.println("conectado");
      // Subscripción
      client.subscribe(TOPIC);
    } else {
      Serial.print("error, rc=");
      Serial.print(client.state());
      Serial.println(" intentar en 5 segundos...");
      // Esperar 5 segundos
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);

  Serial.println("Test MQTT");

  setup_wifi();
  client.setServer(mqttBroker, 1883);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  
  char serial_byte;
  if(Serial.available()>0)
  {
    //Leer el puerto serial
    serial_byte = Serial.read();
    if(isalnum(serial_byte)){
      //Publicar el byte leido
      sprintf(payload, "%c", serial_byte);
      client.publish(TOPIC, payload);
    }
  }
}