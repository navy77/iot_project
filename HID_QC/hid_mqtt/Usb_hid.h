#ifndef USB_HID
#define USB_HID

#include "EspUsbHostKeybord.h"
#include <Arduino.h>

class Usb_hid : public EspUsbHostKeybord 
{
public:
  void onKey(usb_transfer_t *transfer);
};

#endif