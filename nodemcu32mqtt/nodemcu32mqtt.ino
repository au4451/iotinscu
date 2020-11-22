#include <WiFi.h>
#include <PubSubClient.h>

#define pinMqttStatusLED  BUILTIN_LED

// Update these with publishCounts suitable for your network.
const char* ssid = "[your ssid]";
const char* password = "[your password]";

const char* mqtt_server = "mqtt.eclipse.org";
const char* mqtt_id = "[your unique id]";
const char* mqtt_publish_topic = "[your topic]";
const int   mqtt_qos = 1;  //0：at most once 1：at least once 2：exactly once
const bool  mqtt_retain = true;

WiFiClient espWiFiClient;
PubSubClient mqttClient(espWiFiClient);
long lastMsgMillis = 0;
char msg[50];
int publishCount = 0, reConnectCount=0;

void setup() {
  pinMode(pinMqttStatusLED, OUTPUT);   
  Serial.begin(115200);

  setup_wifi();
  mqttClient.setServer(mqtt_server, 1883);
  //mqttClient.setCallback(callback);  
  digitalWrite(pinMqttStatusLED, LOW);
  while (!mqttClient.connected()) { // Loop until we're connected
      mqttConnect();   
  }
}

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
 
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }  
  Serial.print("     reConnectCount = ");
  Serial.println(reConnectCount);
}

void mqttConnect() {  
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    boolean connectOK = mqttClient.connect(mqtt_id /*, mqtt_publish_topic, mqtt_qos, mqtt_retain*/);
    if (connectOK) {  //deviceID
      Serial.println("mqtt broker connected");
      digitalWrite(pinMqttStatusLED, HIGH);
      // Once connected, publish an announcement...
      //mqttClient.publish(mqtt_publish_topic, "esp32 connected");
      // ... and resubscribe
      //mqttClient.subscribe(mqtt_subscribe_topic, mqtt_qos);
      //publishCount = 0; //reset count for test
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.print("  reConnectCount = ");
      Serial.println(reConnectCount);
      digitalWrite(pinMqttStatusLED, LOW);
      //Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
}

void loop() {
  if (!mqttClient.connected()) {
    digitalWrite(pinMqttStatusLED, LOW);
    mqttConnect();
    reConnectCount++;
  } else {
    //mqttClient.loop();
    long now = millis();
    if (now - lastMsgMillis > 2000) {
      lastMsgMillis = now;
      ++publishCount;
      snprintf (msg, 75, "%ld", publishCount);
      Serial.print("Publish message: ");
      Serial.println(msg);
      mqttClient.publish(mqtt_publish_topic, msg);
    }
  }
}
