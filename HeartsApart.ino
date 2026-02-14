#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <SoftwareSerial.h>

// -----------------------------
// Wi-Fi credentials
// -----------------------------
const char* ssid = "NOAH 1690";
const char* password = "R006x6:6";
const int BAUD = 9600;
// -----------------------------
// HiveMQ Cloud credentials
// -----------------------------
const char* mqtt_server = "makec04-1b773ec2.a02.usw2.aws.hivemq.cloud"; // Example: abc12345.s1.eu.hivemq.cloud
const int mqtt_port = 8883; // TLS port
const char* mqtt_user = "makec04";
const char* mqtt_pass = "makeuoftC04";

// Topics
const char* topic_sub = "test/in";   // messages the server will send to this device
const char* topic_pub = "test/out";  // messages this device publishes

//configure pins (TX, RX)

//double check order
SoftwareSerial stmSerial(D6, D7);

WiFiClientSecure espClient;
PubSubClient client(espClient);

// -----------------------------
// Callback for incoming messages
// -----------------------------
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Received [");
  Serial.print(topic);
  Serial.print("]: ");

  for (unsigned int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

// -----------------------------
// Connect to Wi-Fi
// -----------------------------
void setup_wifi() {
  Serial.print("Connecting to Wi-Fi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWi-Fi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

// -----------------------------
// Reconnect to MQTT if disconnected
// -----------------------------
void reconnect() {
  while (!client.connected()) {
    Serial.print("Connecting to HiveMQ Cloud MQTT...");

    if (client.connect("ESP8266Client", mqtt_user, mqtt_pass)) {
      Serial.println("connected");
      // Subscribe to the topic
      client.subscribe(topic_sub);
      Serial.print("Subscribed to: ");
      Serial.println(topic_sub);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" retry in 5 seconds");
    }
  }
}

// -----------------------------
// Setup
// -----------------------------
void setup() {


  Serial.begin(BAUD);
  stmSerial.begin(BAUD);
  setup_wifi();

  espClient.setInsecure(); // skip certificate verification for testing
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);

  Serial.println("Setup complete, ready to connect to HiveMQ Cloud!");
}

// -----------------------------
// Main loop
// -----------------------------
void loop() {
  if (!client.connected()) {
    reconnect();
  }

  if (stmSerial.available()){
    String hb = stmSerial.readStringUntil('\n');
    hb.trim();

    if (hb.length() > 0) {
      Serial.print("Heartbeat received: ");
      Serial.println(hb);

      //publish to MQTT
      client.publish("sensor/heartbeat", hb.c_str());
    }
  }
  // } else {
  //   String hb = "-1";
  //   Serial.print("No Heartbeat: ");
  //   Serial.println(hb);

  //   client.publish("sensor/heartbeat NULL", hb.c_str());
  // }
  client.loop();

  // Publish a message every 5 seconds
  static unsigned long lastMsg = 0;
  unsigned long now = millis();
  if (now - lastMsg > 5000) {
    lastMsg = now;
    String message = "Hello from ESP8266 at " + String(now / 1000) + "s";
    client.publish(topic_pub, message.c_str());
    Serial.print("Published: ");
    Serial.println(message);
  }
}


