#include "HardwareSerial.h"
#include "pgmspace.h"
#include "Usb_hid.h"

char hidTable[59][2] = {
  { 30, '1' }, { 31, '2' }, { 32, '3' }, { 33, '4' }, { 34, '5' }, { 35, '6' }, { 36, '7' }, { 37, '8' }, { 38, '9' }, { 39, '0' }, { 86, '-' }, { 87, '+' },
   { 89, '1' }, { 90, '2' }, { 91, '3' }, { 92, '4' }, { 93, '5' }, { 94, '6' }, { 95, '7' }, { 96, '8' }, { 97, '9' }, { 98, '0' }, { 99, '.' }, 
   { 4, 'A' }, { 5, 'B' }, { 6, 'C' }, { 7, 'D' }, { 8, 'E' }, { 9, 'F' }, { 10, 'G' }, { 11, 'H' }, { 12, 'I' }, { 13, 'J' }, { 14, 'K' }, { 15, 'L' }, { 16, 'M' }, { 17, 'N' }, { 18, 'O' }, { 19, 'P' }, { 20, 'Q' }, { 21, 'R' }, { 22, 'S' }, { 23, 'T' }, { 24, 'U' }, { 25, 'V' }, { 26, 'W' }, { 27, 'X' }, { 28, 'Y' }, { 29, 'Z' }
};
char data_list[20];
char *rcv_msg;
int feedback_msg = 0;
int count = 0;
const int digit = 3;  // 7-->rfid  ,4-->mitutoyo
int digit_ = 0;
bool rcv = true;

void Usb_hid::onKey(usb_transfer_t *transfer) {
  feedback_msg = 0;
  uint8_t *const p = transfer->data_buffer;

  if (p[2] != 0) {
    // Serial.println(p[2]);
    if (p[2] != 40) {  // 43=Enter key
      for (int i = 0; i < sizeof(hidTable) / sizeof(hidTable[0]); i++) {
        if (hidTable[i][0] == p[2]) {
          data_list[count] = hidTable[i][1];
          count++;
          break;
        }
      }
    } else {
      rcv_msg = data_list;
      feedback_msg = 1;
      send_msg();
      send_feedback();
      // print_msg();
      count = 0;                              // reset
      neopixelWrite(RGB_BUILTIN, 0, 255, 0);  // green
      delay(100);
      neopixelWrite(RGB_BUILTIN, 0, 0, 0);
    }
  }
}

void Usb_hid::print_msg() {
  Serial.println(rcv_msg);
}

char *Usb_hid::send_msg() {
  return rcv_msg;
}

int Usb_hid::send_feedback() {
  return feedback_msg;
}
