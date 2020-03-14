// testing code
#include <WiFi.h>
#include <PubSubClient.h>
// Connection info.
const char* ssid = "TheHunter";
const char* password =  "Ramon2121";
const char* mqttServer = "test.mosquitto.org";
const int mqttPort = 8081;
const char* clientID = "rafdyamestira";
const char* channelName = "rafdytester/1";
long lastMsg = 0;
char msg[50];
int value = 0;
WiFiClient MQTTclient;

PubSubClient client(MQTTclient);

void callback(char* topic, byte* payload, unsigned int length) {
  String payload_buff;
  for (int i = 0; i < length; i++) {
    payload_buff = payload_buff + String((char)payload[i]);
  }
  Serial.println(payload_buff); // Print out messages.
}
long lastReconnectAttempt = 0;
boolean reconnect() {
  if (client.connect(clientID)) {
    client.subscribe(channelName); // Subscribe to channel.
  }
  return client.connected();
}
void setup() {
  Serial.begin(115200);
  Serial.println("Attempting to connect...");
  WiFi.begin(ssid, password); // Connect to WiFi.
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  client.setServer(mqttServer, mqttPort); // Connect to PubNub.
  client.setCallback(callback);
  lastReconnectAttempt = 0;
}
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 1) {
    lastMsg = now;
    ++value;
//    snprintf (msg, 50, "hello world #%ld", value);
//    Serial.print("Publish message: ");
    Serial.println(String(value).c_str());
    client.publish(channelName,String(value).c_str());
  }
}
