#ifndef CONFIG_H
#define CONFIG_H

// #### WIFI Config  #### //

// #### MQTT Config  #### //

// const char* mqtt_server = "192.168.1.111"; 
 char* topic_pub_1 = "/data";  // ball/insp/A001/data
 char* topic_pub_2 = "/status";
 char* topic_pub_3 = "/alarm";
 char* topic_esp_health = "/esp_health";
//  char* topic_sub_1 = "sub_1";

// #### Modbus Address Config  #### //
const uint16_t itr_modbus = 100;  // ms
const uint16_t itr_fnc_1 = 1000;  // ms
const uint16_t itr_fnc_2 = 1000;  // ms
const uint16_t itr_fnc_3 = 1000;  // ms
const uint16_t itr_esp = 20000;  // ms

const uint8_t num_got_data = 110;
uint16_t got_data[num_got_data];
float init_heap;
String status;
String prv_status;
String alarm_;
String prv_alarm;
uint16_t prv_time = 0;
uint16_t heap_cnt1,heap_cnt2,heap_cnt3;
uint16_t ct_fn1,ct_fn2,ct_fn3,ct_read;
uint16_t ct_read_cnt,ct_fn1_cnt,ct_fn2_cnt,ct_fn3_cnt;
uint16_t ct_read_ = 220;
uint16_t ct_fn1_ = 3500;
uint16_t ct_fn2_ = 1800;
uint16_t ct_fn3_ = 2300;

String def_tb[][5] = {
  // name||address||type||value||prv_value
  // type for separate detail of data
  { "s1", "1", "1", "", "" },    //run
  { "s2", "2", "1", "", "" },    //stop
  { "s3", "3", "1", "", "" },    //alarm
  { "s4", "4", "1", "", "" },    //adjust
  { "s5", "5", "1", "", "" },    //setup
  { "s6", "6", "1", "", "" },   //ch_tool
  { "s7", "7", "1", "", "" },   //warm
  { "s8", "8", "1", "", "" },   //other
  { "s20", "20", "1", "", "" },  //last status
  { "a1", "32", "2", "", "" },   //alarm1
  { "a2", "33", "2", "", "" },   //alarm2
  { "a3", "34", "2", "", "" },   //alarm3
  { "a4", "35", "2", "", "" },   //alarm4
  { "a5", "36", "2", "", "" },   //alarm5
  { "a6", "37", "2", "", "" },   //alarm6
  { "a7", "38", "2", "", "" },   //alarm7
  { "a8", "39", "2", "", "" },   //alarm8
  { "a9", "40", "2", "", "" },   //alarm9
  { "a40", "71", "2", "", "" },  //alarm40
  { "yyyy", "80", "0", "", "" },
  { "mm", "81", "0", "", "" },
  { "dd", "82", "0", "", "" },
  { "hh", "83", "0", "", "" },
  { "min", "84", "0", "", "" },
  { "sec", "85", "0", "", "" },
  { "ttl_sh", "90", "3", "", "" },
  { "ttl_a", "91", "3", "", "" },
  { "ttl_b", "92", "3", "", "" },
  { "ttl_c", "93", "3", "", "" },
  { "yeild_a", "94", "3", "", "" },
  { "yeild_b", "95", "3", "", "" },
  { "yeild_c", "96", "3", "", "" },
  { "tray_1", "97", "3", "", "" },
  { "tray_2", "98", "3", "", "" },
  { "tray_3", "99", "3", "", "" },
  { "tray_main", "100", "3", "", "" },
  { "ball_OK", "101", "4", "", "" },
  { "ball_OK_", "102", "4", "", "" },
  { "ball_NG", "103", "4", "", "" },
  { "ball_NG_", "104", "4", "", "" },
};

#endif