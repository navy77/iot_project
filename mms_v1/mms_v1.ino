#include "MicMMS.h"

MicMMS aaa("your_ssid", "your_password", "192.168.1.69", 1884,"A02", 1, Serial1,"192.168.1.100","192.168.1.1","255.255.255.0");
// xxx(ssid,password,mqttBroker,mqttPort,slaveID,modbusPort,FixIP,gateway,subnet);
void setup() {
  aaa.init();
  aaa.start();

}
  
void loop() {
  aaa.run();
}
