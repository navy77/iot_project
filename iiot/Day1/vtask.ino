const TickType_t xDelay2000ms = pdMS_TO_TICKS(2000);   
TaskHandle_t Task1 = NULL;    
TaskHandle_t Task2 = NULL;
TaskHandle_t Task3 = NULL;

void setup() {

  Serial.begin(115200);  
  delay(1000);  
  xTaskCreatePinnedToCore(func1_Task,"Task1",1000,NULL,0,&Task1,0);
  xTaskCreatePinnedToCore(func2_Task,"Task2",1000,NULL,1,&Task2,0);
  xTaskCreatePinnedToCore(func3_Task,"Task3",1000,NULL,2,&Task3,0);
  
}

void loop() {

}
void func1_Task(void *pvParam){
  while(1){
      Serial.println(String("hello from Task1"));
      vTaskDelay(xDelay2000ms);  
    }    
  }
void func2_Task(void *pvParam){
    while(1){      
      Serial.println(String("hello from Task2"));       
      vTaskDelay(xDelay2000ms);
    }    
  }
void func3_Task(void *pvParam){
  while(1){
      Serial.println(String("hello from Task3"));
      vTaskDelay(xDelay2000ms);  
    }    
  }  