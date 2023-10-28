#include <WiFi.h>
#include <PubSubClient.h>
#include <HardwareSerial.h>

//Define max digit for reading
int count = 0;
int count_split = 0;
const int8_t max_len = 30; 
char data_list[max_len];
char data_list_1[10];
char data_list_2[10];
char *split_data;
String data_1,data_2;

const char* ssid = "MIC_IIOT"; // MIC_IIOT  MIC_IIOT_2.4GHz
const char* password =  "mic@admin"; //mic@admin
const char* mqttServer = "192.168.1.103"; //192.168.1.103
const int mqttPort = 1884; 

IPAddress local_IP(192, 168, 1, 111);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial1.begin(9600,SERIAL_7E1,18,17); // hid
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
  client.setServer(mqttServer, mqttPort);
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
    if (client.connect("ESP32Client")) {
      Serial.println("connected");
    } else {
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
    }
  }
}
 
void reconnect(){
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
 
    if (client.connect("ESP32Client")) {
 
      Serial.println("connected");
 
    } else {
 
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
    }
  }
}

void loop() {
  if(!client.connected()){
    reconnect();
  }
  client.loop();
  if(Serial1.available() > 0){
    convert_data();
  }
  
}

void convert_data(){
    char inByte = Serial1.read();
     if (inByte != '\r'){
        data_list[count] = inByte;
        count++;
    }else {
      split_data = strtok(data_list,",");
      while (split_data!=NULL) {
              if(count_split <2){
            switch (count_split) {
            case 0:
              data_1 =  split_data;
              count_split++;
              break;
            case 1:
              data_2 = split_data;
              count_split++;
              break;
            }
        }
        else{
          data_1.toCharArray(data_list_1,10);
          data_2.toCharArray(data_list_2,10);
          // Serial.println(data_1);
          // Serial.println(data_2);
          count_split=0;
          client.publish("Keyence_1", data_list_1);
          client.publish("Keyence_2", data_list_2);
        }
      split_data = strtok(NULL,",");
      }
      count = 0;
      }
}

