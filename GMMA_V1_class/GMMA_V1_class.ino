#include "config.h"
#include "ModbusRtu.h"
#include <vector>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

Modbus slave(1, Serial1, 0);
WiFiClient espClient;
PubSubClient client(espClient);
String status;
String prv_status;

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
  std::vector<std::vector<String>> data_tb;
  std::vector<std::vector<String>> alarm_tb;

  Serial.begin(115200);
  // setup_wifi();
  // client.setServer(mqtt_server, 1884);
  // client.setCallback(callback);
  // Setup freertos
  xTaskCreatePinnedToCore(modbus_Task, "Task1", 8000, NULL, 1, &Read_modbus, 0);
  xTaskCreatePinnedToCore(func1_Task, "Task2", 10000, NULL, 1, &Func_1, 0);
  xTaskCreatePinnedToCore(func2_Task, "Task3", 10000, NULL, 1, &Func_2, 0);
  xTaskCreatePinnedToCore(func3_Task, "Task4", 10000, NULL, 1, &Func_3, 0);

  Serial1.begin(115200);  // modbus port
  // Serial.println(esp_get_free_heap_size());
  delay(1000);

  slave.start();  // start modbus
}

void loop() {
  // if (!client.connected()) {
  //   reconnect();
  // }
}

void modbus_Task(void *pvParam) {  // cytime = 0.2-1 ms
  while (1) {
    String yyyy;
    String mm;
    String dd;
    String hh;
    String min;
    String sec;
    String dt;

    slave.poll(got_data, num_got_data);
    //This loop ct = 0.2 ms
    //record raw data to table
    for (int i = 0; i < sizeof(def_tb) / sizeof(def_tb[0]); i++) {
      if (def_tb[i][2] == "0" || def_tb[i][2] == "3") {
        def_tb[i][3] = got_data[(def_tb[i][1].toInt())];
      }
    }
    // clean data yy-->yyyy
    for (int i = 0; i < sizeof(def_tb) / sizeof(def_tb[0]); i++) {

      if (def_tb[i][0] == "yyyy" and def_tb[i][3].length() == 2) {  //select only yy
        def_tb[i][3] = "20" + def_tb[i][3];                         // and def_tb[i][3].length() == 2
      }
    }
    // clean data d,m,min,h,sec
    for (int i = 0; i < sizeof(def_tb) / sizeof(def_tb[0]); i++) {
      if (def_tb[i][2] == "0" and def_tb[i][3].length() == 1) {
        def_tb[i][3] = "0" + def_tb[i][3];
      }
    }

    vTaskDelay(pdMS_TO_TICKS(100));
  }
}

void func1_Task(void *pvParam) {  // type data -->1
  while (1) {
    int j = 0;
    bool change_ = false;
    StaticJsonDocument<300> json_1;  // size = 30*topic [avg]
    // update data --> data_tb
    for (int i = 0; i < sizeof(def_tb) / sizeof(def_tb[0]); i++) {
      if (def_tb[i][2] == "3") {
        data_tb[j][3] = def_tb[i][3];
        j++;
      }
    }
    // check data change
    for (int i = 0; i < sizeof(data_tb) / sizeof(data_tb[0]); i++) {
      if (data_tb[i][3] != data_tb[i][4]) {
        change_ = true;
        break;
      }
    }

    if (change_ == true) {  // data change !!!
      for (int i = 0; i < sizeof(def_tb) / sizeof(def_tb[0]); i++) {
        if (def_tb[i][2] == "3") {
          json_1[String(def_tb[i][0])] = def_tb[i][3].toFloat();
        }
      }

      json_1["rssi"] = (float)WiFi.RSSI();
      String json_topic1;
      serializeJson(json_1, json_topic1);
      // client.publish(topic_pub_1, json_topic1.c_str());

      // update data --> pdata_tb
      for (int i = 0; i < sizeof(data_tb) / sizeof(data_tb[0]); i++) {
        data_tb[i][4] = data_tb[i][3];
      }
    }

    vTaskDelay(pdMS_TO_TICKS(2000));
  }
}

void func2_Task(void *pvParam) {  // status
  while (1) {

    bool ready_2 = false;
    bool check_data_2[2] = { false, false };
    double bit_data_2[2];
    uint8_t coil = 0;

    //Convert data
    for (int i = 0; i < 2; i++) {
      if (got_data[i] >= 0) {
        bit_data_2[i] = log2(got_data[i]);
      }
    }
    //check correct data
    for (int i = 0; i < 2; i++) {
      if (bit_data_2[i] == round(bit_data_2[i])) {
        check_data_2[i] = true;
      } else {
        check_data_2[i] = false;
      }
    }
    //get coil
    if (check_data_2[0] == true and check_data_2[1] == true) {
      if (bit_data_2[0] > 0) {
        coil = bit_data_2[0];
        ready_2 = true;
      } else {
        coil = bit_data_2[1] + 16;
        ready_2 = true;
      }
    }
    if (coil > 0 and ready_2 == true) {
      StaticJsonDocument<300> json_2;  // size = 30*topic [avg]
      for (int i = 0; i < sizeof(def_tb) / sizeof(def_tb[0]); i++) {
        if (def_tb[i][1].toInt() == coil) {
          status = def_tb[i][0].toInt();
          json_2["mc_status"] = status;
        }
      }
      // Get datetime
      for (int j = 0; j < sizeof(def_tb) / sizeof(def_tb[0]); j++) {
        if (def_tb[j][2] == "0") {                              // 0--> d_time
          json_2[String(def_tb[j][0])] = def_tb[j][3].toInt();  //yyyy-MM-dd HH:mm:ss
        }
      }
      // publish data
      if (status != prv_status) {
        String json_topic2;
        serializeJson(json_2, json_topic2);
        // client.publish(topic_pub_2, json_topic2.c_str());
        ready_2 = false;
        prv_status = status;
      }
    }
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

void func3_Task(void *pvParam) {
  while (1) {
    bool ready_3 = false;
    bool check_data_3[2] = { false, false };
    double bit_data_3[2];

    //Convert data
    for (int i = 2; i < 5; i++) {
      if (got_data[i] >= 0) {
        bit_data_3[i] = log2(got_data[i]);
      }
    }


    vTaskDelay(pdMS_TO_TICKS(100));
  }
}