#include "MicMMS.h"
MicMMS aaa("MIC_Iot", "Micdev@2024", "192.168.0.160", 1884, 1, Serial1);

void setup() {
  aaa.init();
  aaa.start();
}
  
void loop() {
  aaa.run();
}