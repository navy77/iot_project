#include "config.h"
#include "ModbusRtu.h"
#include <vector>
#include <WiFi.h>
#include <PubSubClient.h>

Modbus slave(1, Serial1, 0);
WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char *topic, byte *payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP32";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // client.subscribe(topic_sub_1);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(2000);
    }
  }
}

void setup() {
  std::vector<std::vector<String>> def_tb;
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1884);
  client.setCallback(callback);
  xTaskCreatePinnedToCore(modbus_Task, "Task1", 1000, NULL, 0, &Read_modbus, 0);
  xTaskCreatePinnedToCore(func1_Task, "Task2", 1000, NULL, 1, &Func_1, 0);
  xTaskCreatePinnedToCore(func2_Task, "Task3", 1000, NULL, 2, &Func_2, 0);
  xTaskCreatePinnedToCore(func3_Task, "Task4", 1000, NULL, 3, &Func_3, 0);

  Serial1.begin(9600);  // modbus port
  slave.start();        // start modbus
}

void get_modbus() {
  for (int i = 0; i < sizeof(def_tb) / sizeof(def_tb[0]); i++) {
    def_tb[i][3] = got_data[(def_tb[i][1].toInt())];
  }
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }

  //poll data
  slave.poll(got_data, 5);

  // Serial.print(def_tb[0][3]);
  // Serial.print(":");
  // Serial.print(def_tb[1][3]);
  // Serial.print(":");
  // Serial.println(def_tb[2][3]);
}

void modbus_Task(void *pvParam) {
  while (1) {
    for (int i = 0; i < sizeof(def_tb) / sizeof(def_tb[0]); i++) {
      def_tb[i][3] = got_data[(def_tb[i][1].toInt())];
    }
    vTaskDelay(pdMS_TO_TICKS(100));  //delay 200 ms
  }
}

void func1_Task(void *pvParam) {  // type data -->1
  while (1) {
    // ready = true;

    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

void func2_Task(void *pvParam) {
  while (1) {
    // client.publish(topic_pub_1, msg1);
    vTaskDelay(pdMS_TO_TICKS(3000));
  }
}

void func3_Task(void *pvParam) {
  while (1) {
    // if (ready == true) {
      float float_data = 1.2345;
      char char_value[6];
      dtostrf(float_data, 5, 4, char_value);
      client.publish(topic_pub_1, char_value);
    // }

    vTaskDelay(pdMS_TO_TICKS(4000));
  }
}