#include "HardwareSerial.h"
#include "esp_system.h"
#include "MicMMS.h"
#include "config.h"
MicMMS::MicMMS(const char* ssid, const char* password, const char* mqtt_server, int mqtt_port, const char* mc_no, int slaveId, HardwareSerial& serialPort,const char* ip_address, const char* gateway_address, const char* subnet_mask)
  : wifiClient(), mqttClient(wifiClient),ssid(ssid),password(password),mqtt_server(mqtt_server),mqtt_port(mqtt_port),mc_no(mc_no), slaveId(slaveId), serialPort(serialPort), modbus(slaveId, serialPort, 0) {
  // this->ssid = ssid;
  // this->password = password;
  // this->mqtt_server = mqtt_server;
  // this->mqtt_port = mqtt_port;
  // this->mc_no = mc_no;
  ip.fromString(ip_address);
  gateway.fromString(gateway_address);
  subnet.fromString(subnet_mask);
}

void MicMMS::init() {
  std::vector<std::vector<String>> def_tb;
  pinMode(1, OUTPUT);
  pinMode(2, OUTPUT);

  Serial.begin(115200);
  Serial1.begin(115200);
  WiFi.config(ip, gateway,subnet);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");\
  Serial.println(WiFi.localIP());
  mqttClient.setServer(mqtt_server, mqtt_port);

  digitalWrite(2, HIGH);
  init_heap = esp_get_free_heap_size();
  modbus.start();
}

void MicMMS::reconnect() {
  while (!mqttClient.connected()) {
    Serial.println("Attempting MQTT connection...");
    if (mqttClient.connect("ESP32Client")) {
      Serial.println("Connected to MQTT Broker");
    } else {
      Serial.print("Failed with state ");
      Serial.print(mqttClient.state());
      delay(2000);
    }
  }
}

void MicMMS::publishMessage(char* topic, const char* message) {
  if (!mqttClient.connected()) {
    reconnect();
  }
  mqttClient.publish(topic, message);
}

void MicMMS::run() {
  if (!mqttClient.connected()) {
    reconnect();
    digitalWrite(2, HIGH);
  }
  modbus.poll(got_data, num_got_data);
}

void MicMMS::start() {
  xTaskCreatePinnedToCore(modbus_Task, "Task0", 10000, this, 5, NULL, 0);
  xTaskCreatePinnedToCore(func1_Task, "Task1", 10000, this, 4, NULL, 0);
  xTaskCreatePinnedToCore(func2_Task, "Task2", 10000, this, 3, NULL, 0);
  xTaskCreatePinnedToCore(func3_Task, "Task3", 10000, this, 2, NULL, 0);
  xTaskCreatePinnedToCore(esp_Task, "Task4", 10000, this, 1, NULL, 0);
}
void MicMMS::modbus_Task(void* pvParam) {
  MicMMS* instance = (MicMMS*)pvParam;
  while (1) {
    //record raw data to table
    unsigned long long int start = micros();
    for (int i = 0; i < sizeof(def_tb) / sizeof(def_tb[0]); i++) {
      if (def_tb[i][2] == "0" || def_tb[i][2] == "3" || def_tb[i][2] == "4") {
        def_tb[i][3] = got_data[(def_tb[i][1].toInt()) - 1];
      }
    }
    // clean data yy-->yyyy
    for (int i = 0; i < sizeof(def_tb) / sizeof(def_tb[0]); i++) {
      if (def_tb[i][0] == "yyyy" and def_tb[i][3].length() == 2) {  //select only yy
        def_tb[i][3] = "20" + def_tb[i][3];
      }
    }
    ct_read = micros() - start;
    vTaskDelay(pdMS_TO_TICKS(itr_modbus));
  }
}

void MicMMS::func1_Task(void* pvParam) {
  MicMMS* instance = (MicMMS*)pvParam;
  MicMMS* mcNo = (MicMMS*)(pvParam);

  char topic_pub[30];
  strcpy(topic_pub, mcNo->mc_no);
  strcat(topic_pub, topic_pub_1);

  while (1) {
    unsigned long long int start = micros();
    bool change_1 = false;
    StaticJsonDocument<300> json_1;  // size = 30*topic [avg]
    // check data change
    for (int i = 0; i < sizeof(def_tb) / sizeof(def_tb[0]); i++) {
      if (def_tb[i][2] == "3") {
        if (def_tb[i][3] != def_tb[i][4]) {
          change_1 = true;
          break;
        }
      }
    }
    if (change_1 == true) {  // data change !!!
      for (int j = 0; j < sizeof(def_tb) / sizeof(def_tb[0]); j++) {
        if (def_tb[j][2] == "3") {
          json_1[String(def_tb[j][0])] = def_tb[j][3].toFloat();
        }
      }

      for (int k = 0; k < sizeof(def_tb) / sizeof(def_tb[0]); k++) {
        if (def_tb[k][2] == "4") {
          json_1["ball_size"] = (def_tb[k][3] + "." + def_tb[k + 1][3]).toFloat();
          break;
        }
      }

      json_1["rssi"] = (float)WiFi.RSSI();
      String json_topic1;
      serializeJson(json_1, json_topic1);
      // instance->publishMessage(mcNo->mc_no, json_topic1.c_str());
      instance->publishMessage(topic_pub, json_topic1.c_str());
      for (int k = 0; k < sizeof(def_tb) / sizeof(def_tb[0]); k++) {
        if (def_tb[k][2] == "3") {
          def_tb[k][4] = def_tb[k][3];
        }
      }
      ct_fn1 = micros() - start;
      digitalWrite(1, HIGH);
      delay(100);
      digitalWrite(1, LOW);
    }
    vTaskDelay(pdMS_TO_TICKS(itr_fnc_1));
  }
}

void MicMMS::func2_Task(void* pvParam) {
  MicMMS* instance = (MicMMS*)pvParam;
  MicMMS* mcNo = (MicMMS*)(pvParam);

  char topic_pub[30];
  strcpy(topic_pub, mcNo->mc_no);
  strcat(topic_pub, topic_pub_2);
  while (1) {
    unsigned long long int start = micros();
    bool ready_state = false;
    bool check_data[2] = { false, false };
    double bit_data[2];
    uint8_t coil_no = 0;
    //Convert & check data
    for (int i = 0; i < 2; i++) {
      if (got_data[i] >= 0) {
        bit_data[i] = log2(got_data[i]);
        if (bit_data[i] == round(bit_data[i])) {
          check_data[i] = true;
        } else {
          check_data[i] = false;
        }
      }
    }
    if (bit_data[0] >= 0 and bit_data[1] >= 0) {
      check_data[0] = false;
      check_data[1] = false;
    }

    //get coil_no
    for (int i = 0; i < 2; i++) {
      if (bit_data[i] >= 0 and check_data[i] == true) {
        coil_no = (i * 16) + bit_data[i];
        ready_state = true;
      }
    }

    if (coil_no > 0 and ready_state == true) {
      StaticJsonDocument<300> json_2;  // size = 30*topic [avg]
      for (int i = 0; i < sizeof(def_tb) / sizeof(def_tb[0]); i++) {
        if (def_tb[i][1].toInt() == coil_no) {
          status = def_tb[i][0].toInt();
          json_2["mc_status"] = status.toInt();
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
        instance->publishMessage(topic_pub, json_topic2.c_str());
        ready_state = false;
        prv_status = status;
        ct_fn2 = micros() - start;
        digitalWrite(1, HIGH);
        delay(100);
        digitalWrite(1, LOW);
      }
    }
    vTaskDelay(pdMS_TO_TICKS(itr_fnc_2));
  }
}

void MicMMS::func3_Task(void* pvParam) {
  MicMMS* instance = (MicMMS*)pvParam;
  MicMMS* mcNo = (MicMMS*)(pvParam);

  char topic_pub[30];
  strcpy(topic_pub, mcNo->mc_no);
  strcat(topic_pub, topic_pub_3);
  while (1) {
    unsigned long long int start = micros();
    bool ready_state = false;
    bool check_data[3] = { false, false, false };
    double bit_data[3];
    uint8_t coil_no = 0;
    //Convert & check data
    for (int i = 2; i < 5; i++) {
      if (got_data[i] >= 0) {
        bit_data[i] = log2(got_data[i]);
        if (bit_data[i] == round(bit_data[i])) {
          check_data[i] = true;
        } else {
          check_data[i] = false;
        }
      }
    }

    if (bit_data[0] >= 0 and bit_data[1] >= 0 and bit_data[2] >= 0) {
      check_data[0] = false;
      check_data[1] = false;
      check_data[3] = false;
    }
    //get coil_no
    for (int i = 2; i < 5; i++) {
      if (bit_data[i] >= 0 and check_data[i] == true) {
        coil_no = (i * 16) + bit_data[i];
        ready_state = true;
      }
    }

    if (coil_no > 0 and ready_state == true) {
      StaticJsonDocument<300> json_3;  // size = 30*topic [avg]
      for (int i = 0; i < sizeof(def_tb) / sizeof(def_tb[0]); i++) {
        if (def_tb[i][1].toInt() == coil_no) {
          alarm_ = def_tb[i][0];
          json_3["alarm_status"] = alarm_;
        }
      }
      // Get datetime
      for (int j = 0; j < sizeof(def_tb) / sizeof(def_tb[0]); j++) {
        if (def_tb[j][2] == "0") {                              // 0--> d_time
          json_3[String(def_tb[j][0])] = def_tb[j][3].toInt();  //yyyy-MM-dd HH:mm:ss
        }
      }
      // publish data
      if (alarm_ != prv_alarm) {
        String json_topic3;
        serializeJson(json_3, json_topic3);
        instance->publishMessage(topic_pub, json_topic3.c_str());
        ready_state = false;
        prv_alarm = alarm_;
        ct_fn3 = micros() - start;
        digitalWrite(1, HIGH);
        delay(100);
        digitalWrite(1, LOW);
      }
    }
    vTaskDelay(pdMS_TO_TICKS(itr_fnc_3));
  }
}

void MicMMS::esp_Task(void* pvParam) {  //ESP status
  MicMMS* instance = (MicMMS*)pvParam;
  MicMMS* mcNo = (MicMMS*)(pvParam);

  char topic_pub[30];
  strcpy(topic_pub, mcNo->mc_no);
  strcat(topic_pub, topic_esp_health);
  while (1) {
    StaticJsonDocument<200> json_4;
    String json_topic4;
    float use_heap = (1 - (esp_get_free_heap_size() / init_heap)) * 100;
    // check heap
    if (use_heap >= 5.0 && use_heap <= 10.0) {
      heap_cnt1++;
    } else if (use_heap > 10.0 && use_heap <= 20.0) {
      heap_cnt2++;
    } else if (use_heap > 20.0) {
      heap_cnt3++;
    }
    // check cpu
    // Serial.println(ct_read);
    float read_over = ((ct_read / ct_read_) - 1) * 100;
    // Serial.println(read_over);
    if (read_over > 80) {
      ct_read_cnt++;
    }
    float fnc1_over = ((ct_fn1 / ct_fn1_) - 1) * 100;
    if (fnc1_over > 80) {
      ct_fn1_cnt++;
    }
    float fnc2_over = ((ct_fn2 / ct_fn2_) - 1) * 100;
    if (fnc2_over > 80) {
      ct_fn2_cnt++;
    }
    float fnc3_over = ((ct_fn3 / ct_fn3_) - 1) * 100;
    if (fnc3_over > 80) {
      ct_fn3_cnt++;
    }

    if (millis() - prv_time >= 12 * 60 * 60 * 1000) {  // 12hr
      json_4["mem_use"] = use_heap;
      json_4["mem_cnt1"] = heap_cnt1;
      json_4["mem_cnt2"] = heap_cnt2;
      json_4["mem_cnt3"] = heap_cnt3;
      json_4["cpu_fn0"] = ct_read_cnt;
      json_4["cpu_fn1"] = ct_fn1_cnt;
      json_4["cpu_fn2"] = ct_fn2_cnt;
      json_4["cpu_fn3"] = ct_fn3_cnt;

      serializeJson(json_4, json_topic4);
      instance->publishMessage(topic_pub, json_topic4.c_str());
      prv_time = millis();
      heap_cnt1 = 0;
      heap_cnt2 = 0;
      heap_cnt3 = 0;
      ct_read_cnt = 0;
      ct_fn1_cnt = 0;
      ct_fn2_cnt = 0;
      ct_fn3_cnt = 0;
    }

    vTaskDelay(pdMS_TO_TICKS(itr_esp));
  }
}
