#include "Usb_hid.h"
Usb_hid usbHost;

void setup() {

  Serial.begin(115200);

  neopixelWrite(RGB_BUILTIN, 255, 0, 0);  // Red
  delay(2000);
  usbHost.begin();

  neopixelWrite(RGB_BUILTIN, 0, 0, 255);  // Blue
  delay(2000);
}

void loop() {
  usbHost.task();
}