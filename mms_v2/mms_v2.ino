#include "MicMMS.h"
MicMMS aaa("Pwty_2.4G", "1234abcd", "192.168.1.69", 1883,"A01", 1, Serial1,"192.168.1.100","192.168.1.1","255.255.255.0");
// MicMMS aaa("MIC_Iot", "Micdev@2024", "192.168.0.160", 1883,"A01", 1, Serial1,"192.168.0.100","192.168.0.1","255.255.255.0");
// MicMMS aaa("MIC_IIOT", "mic@admin", "192.168.1.104", 1883,"A01", 1, Serial1,"192.168.1.106","192.168.1.1","255.255.255.0");

void setup() {
  aaa.init();
  aaa.start();
}
  
void loop() {
  aaa.run();
}