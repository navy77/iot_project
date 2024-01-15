#include "ModbusRtu.h"
Modbus slave(1, Serial1, 0);
const int max_data =100;
uint16_t got_data[max_data];

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);

  slave.start();

}

void loop() {
  slave.poll(got_data, max_data);
  for(int i=0;i<=max_data;i++){
    Serial.print(got_data[i]);
    Serial.print(":");
  }
  Serial.println();

delay(100);
}
