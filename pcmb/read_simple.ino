#include <HardwareSerial.h>
int count = 0;
bool flag_1 = true;
//Define max digit for reading
const int8_t max_len = 18; 
char data_list[9];
// String data;

void setup() {
 Serial1.begin(9600,SERIAL_7E1,18,17);
 Serial.begin(9600);
}

void loop() {
  if(Serial1.available() > 0) {
    convert_data();
  }
}

void convert_data(){
    char inByte = Serial1.read();
    if(inByte !='\r' and inByte !='\n' and inByte != ' '){
        data_list[count] = inByte;
        // Serial.println(data_list[count]);
        count++;
    }else if(inByte =='\r') {

      Serial.println(data_list);
      count = 0;
    }
}

