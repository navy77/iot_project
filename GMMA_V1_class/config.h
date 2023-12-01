#include "WString.h"
#ifndef CONFIG_H
#define CONFIG_H

// #### WIFI Config  #### //
const char* ssid = "MIC_IIOT_2.4GHz";  //MIC_IIOT_2.4GHz MIC_IIOT
const char* password = "mic@admin";

// #### MQTT Config  #### //

const char* mqtt_server = "192.168.1.2";  //192.168.1.16
const char* topic_pub_1 = "simple_1";
const char* topic_pub_2 = "simple_2";
const char* topic_pub_3 = "simple_3";
const char* topic_sub_1 = "sub_1";

// #### Modbus Address Config  #### //
const uint8_t itr_modbus = 100;  // ms
const uint8_t itr_fnc_1 = 5000;  // ms
const uint8_t itr_fnc_2 = 2000;  // ms
const uint8_t itr_fnc_3 = 2000;  // ms
const uint8_t num_got_data = 100;
uint16_t got_data[num_got_data];

String data_tb[][5]{
  { "ttl_a", "90", "3", "", "" },
  { "ttl_b", "91", "3", "", "" },
  { "ttl_c", "92", "3", "", "" },
  { "yeild_a", "93", "3", "", "" },
  { "yeild_b", "94", "3", "", "" },
  { "yeild_c", "95", "3", "", "" },
  { "tray_1", "96", "3", "", "" },
  { "tray_2", "97", "3", "", "" },
  { "tray_3", "98", "3", "", "" },
  { "tray_main", "99", "3", "", "" }
};
String alarm_tb[][5]{
  { "1", "32", "2", "", "" },   //alarm1
  { "2", "33", "2", "", "" },   //alarm2
  { "3", "34", "2", "", "" },   //alarm3
  { "4", "35", "2", "", "" },   //alarm4
  { "5", "36", "2", "", "" },   //alarm5
  { "6", "37", "2", "", "" },   //alarm6
  { "7", "38", "2", "", "" },   //alarm7
  { "8", "39", "2", "", "" },   //alarm8
  { "9", "40", "2", "", "" },   //alarm9
  { "40", "71", "2", "", "" },  //alarm40
};
String def_tb[][4] = {
  // name||address||type||value
  // type for separate detail of data
  { "1", "1", "1", "" },    //run
  { "2", "2", "1", "" },    //stop
  { "3", "3", "1", "" },    //alarm
  { "4", "4", "1", "" },    //adjust
  { "5", "5", "1", "" },    //setup
  { "6", "6", "1", "" },    //ch_tool
  { "7", "7", "1", "" },    //warm
  { "8", "8", "1", "" },    //other
  { "20", "20", "1", "" },  //last status
  { "1", "32", "2", "" },   //alarm1
  { "2", "33", "2", "" },   //alarm2
  { "3", "34", "2", "" },   //alarm3
  { "4", "35", "2", "" },   //alarm4
  { "5", "36", "2", "" },   //alarm5
  { "6", "37", "2", "" },   //alarm6
  { "7", "38", "2", "" },   //alarm7
  { "8", "39", "2", "" },   //alarm8
  { "9", "40", "2", "" },   //alarm9
  { "40", "71", "2", "" },  //alarm40
  { "yyyy", "80", "0", "" },
  { "mm", "81", "0", "" },
  { "dd", "82", "0", "" },
  { "hh", "83", "0", "" },
  { "min", "84", "0", "" },
  { "sec", "85", "0", "" },
  { "ttl_a", "90", "3", "" },
  { "ttl_b", "91", "3", "" },
  { "ttl_c", "92", "3", "" },
  { "yeild_a", "93", "3", "" },
  { "yeild_b", "94", "3", "" },
  { "yeild_c", "95", "3", "" },
  { "tray_1", "96", "3", "" },
  { "tray_2", "97", "3", "" },
  { "tray_3", "98", "3", "" },
  { "tray_main", "99", "3", "" }
};


// #### Freertos  Config  #### //
TaskHandle_t Read_modbus = NULL;
TaskHandle_t Func_1 = NULL;  //data
TaskHandle_t Func_2 = NULL;  // status
TaskHandle_t Func_3 = NULL;  // alarm

#endif