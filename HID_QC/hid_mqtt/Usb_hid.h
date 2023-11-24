#ifndef USB_HID
#define USB_HID

#include "EspUsbHostKeybord.h"
#include <Arduino.h>

class Usb_hid : public EspUsbHostKeybord 
{
public:
  void onKey(usb_transfer_t *transfer);
  void print_msg();
  char *send_msg();
  int send_feedback();
};

#endif