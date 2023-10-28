#include <WiFi.h>
#include <PubSubClient.h>
#include <HardwareSerial.h>

//Define max digit for reading
int count = 0;
const int8_t max_len = 18; 
char data_list[max_len];

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
     if(inByte !='\r' and inByte !='\n' and inByte != ' '){
        data_list[count] = inByte;
        count++;
    }else if(inByte =='\r') {
      client.publish("Heidanhiin", data_list);
      count = 0;
      }
}


