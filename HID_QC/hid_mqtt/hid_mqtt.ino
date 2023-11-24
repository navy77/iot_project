#include "Usb_hid.h"
#include <WiFi.h>
#include <PubSubClient.h>
#include "config.h"

WiFiClient espClient;
PubSubClient client(espClient);
Usb_hid usbHost;

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

void callback(char* topic, byte* payload, unsigned int length) {
  result_msg = char(payload[0]);
  if (result_msg =='1') {  //OK
    for (int i = 0; i < 3; i++) {
      neopixelWrite(RGB_BUILTIN, 0, 255, 0);  // Green
      delay(100);
      neopixelWrite(RGB_BUILTIN, 0, 0, 0);
      delay(100);
    }
    ready_msg = true;
  } else {
    for (int i = 0; i < 3; i++) {
      neopixelWrite(RGB_BUILTIN, 255, 0, 0);  // Red
      delay(100);
      neopixelWrite(RGB_BUILTIN, 0, 0, 0);
      delay(100);
    }
    ready_msg = true;
  }
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
      client.subscribe(topic_sub_1);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void publish_data() {
  char* rcv_data = usbHost.send_msg();
  int rcv_event = usbHost.send_feedback();
  if (rcv_event > 0 and ready_msg == true) {
    count_rcv++;
    if (count_rcv == 1) {  //send 1st data
      Serial.println(rcv_data);
      client.publish(topic_pub_1, rcv_data, true);
      value_msg = rcv_data;
      ready_msg = false;
    }
  } else {
    count_rcv = 0;
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1884);
  client.setCallback(callback);
  neopixelWrite(RGB_BUILTIN, 255, 0, 0);  // Red
  delay(1000);
  usbHost.begin();
  delay(1000);
  neopixelWrite(RGB_BUILTIN, 0, 0, 255);  // Blue
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  // client.subscribe(topic_sub_1);
  usbHost.task();
  // Serial.println(ready_msg);
  publish_data();
  // sub data
  // client.subscribe(topic_sub_1);
}
