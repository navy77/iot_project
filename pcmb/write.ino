#include <HardwareSerial.h>
int data[] = {43,32,32,49,51,46,49,52,50,50,53,32,32,32,32,13,10,10};

int state = 0;
int last_state = 0;

void setup() {
  Serial1.begin(9600,SERIAL_7E1,18,17);
  Serial.begin(9600);
  pinMode(0, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  int state = digitalRead(0);
  
  if (state != last_state){
    if (state == LOW){
      send_data();
      // Serial.println("OK");
      digitalWrite(LED_BUILTIN, HIGH);
    }
    else {
      digitalWrite(LED_BUILTIN, LOW);
    }
    delay(50);
  }
  last_state = state;
}
void send_data(){
  data[3]=random(48,57);data[4]=random(48,57);
  data[6]=random(48,57);data[7]=random(48,57);data[8]=random(48,57);data[9]=random(48,57);data[10]=random(48,57);

  for(int i = 0;i<18;i++){
    Serial1.write(data[i]);
  }
  
}
