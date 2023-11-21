#ifndef CONFIG_H
#define CONFIG_H

// #### WIFI Config  #### //
const char* ssid = "MIC_IIOT_2.4GHz";  //MIC_IIOT_2.4GHz MIC_IIOT
const char* password = "mic@admin";

// #### MQTT Config  #### //

const char* mqtt_server = "192.168.1.10";  //192.168.1.16
const char* topic_pub_1 = "simple_1";
const char* topic_pub_2 = "simple_2";
const char* topic_pub_3 = "simple_3";
const char* topic_sub_1 = "sub_1";
const char* msg1 = "a";
const char* msg2 = "b";
// #### Modbus Address Config  #### //
uint16_t got_data[5];
String def_tb[][4] = {  //name-address-type-value
  { "part", "1", "1", "" },
  { "cycle", "2", "2", "" },
  { "alarm", "5", "3", "" }
};

// #### Freertos  Config  #### //
TaskHandle_t Read_modbus = NULL;
TaskHandle_t Func_1 = NULL;
TaskHandle_t Func_2 = NULL;
TaskHandle_t Func_3 = NULL;

bool ready = false;

// char def_tb1[5][2] = {
//   { 'AAA', '4' }, { 'BBB', '5' }, { 'CCC', '6' }, { 'DDD', '7' }, { 'EEE', '8' }  // name,address
// };

// String def_tb2[5][2] = {
//   { "4", "AAA" }, { "5", "BBB" }, { "6", "CCC" }, { "7", "DDD" }, { "8", "EEE" }  // name,address
// };

// String def_tb3[5][2] = {
//   { "AAA", "4" }, { "BBB", "5" }, { "CCC", "6" }, { "DDD", "7" }, { "EEE", "8" }  // name,address
// };

#endif