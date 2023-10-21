#include "EspUsbHostKeybord.h"

class MyEspUsbHostKeybord : public EspUsbHostKeybord {
  public:

    char hidTable[13][2] = {
      {86, '-'}, {87, '+'}, {89, '1'}, {90, '2'}, {91, '3'}, {92, '4'}, {93, '5'}, {94, '6'}, {95, '7'}, {96, '8'}, {97, '9'}, {98, '0'}, {99, '.'},
      {4,'A'} ,{5,'B'}, {6,'C'}, {7,'D'}, {8,'E'}, {9,'F'}, {10,'G'}, {11,'H'}, {12,'I'}, {13,'J'}, {14,'K'}, {15,'L'}, {16,'M'}, {17,'N'}, {18,'O'},
      {19,'P'}, {20,'Q'}, {21,'R'}, {22,'S'}, {23,'T'}, {24,'U'}, {25,'V'}, {26,'W'}, {27,'X'}, {28,'Y'}, {29,'Z'}     
      };
    char data_list[10];
    int count = 0;
    int digit = 3;
    
    void onKey(usb_transfer_t *transfer) {
      
      uint8_t *const p = transfer->data_buffer;
//      Serial.println(p[2]);

      if (p[2] != 0) {
        if (count<digit+2) {
          for (int i = 0; i < sizeof(hidTable) / sizeof(hidTable[0]); i++) {
            if (hidTable[i][0] == p[2]) {
              data_list[count] = hidTable[i][1];
//              Serial.print(data_list[count]);
//              Serial.println(count);
              count++;
              break;
            }
          }
        }else{
          Serial.println(data_list);
          count = 0;
        }
      }
    }
};


MyEspUsbHostKeybord usbHost;

void setup() {
  Serial.begin(115200);
  usbHost.begin();
}

void loop() {
  usbHost.task();

}
