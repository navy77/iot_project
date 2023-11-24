#ifndef CONFIG_H
#define CONFIG_H

// #### WIFI Config  #### //
const char* ssid = "MIC_IIOT_2.4GHz";  //MIC_IIOT_2.4GHz MIC_IIOT
const char* password = "mic@admin";

// #### MQTT Config  #### //
const char* mqtt_server = "192.168.1.5";  //192.168.1.16
const char* topic_pub_1 = "simple_1";
const char* topic_sub_1 = "simple_2";

// #### USB param Config  #### //
char* value_msg;
char result_msg;
bool ready_msg = true;
int count_rcv = 0;
unsigned long time_trigger = 0;
long prv_time = 0;

#endif