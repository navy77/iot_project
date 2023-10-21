String myArray1[5][2] = {
  {"1a","A"},
  {"2a","B"},
  {"3a","C"},
  {"4a","D"},
  {"5a","E"},
};

char myArray2[5][2] = {
  {10,'A'},
  {20,'B'},
  {30,'C'},
  {40,'D'},
  {50,'E'},
};
int x,y;
String data_1;
//String target = "3a";
String target1;
char data_2;
char target2;

void setup() {
  Serial.begin(115200);
}

void loop() {
//  target1 = "4a";
//  fn1();
  target2 = 30;
  fn2();
}

void fn1(){
    for(int i=0;i< sizeof(myArray1)/sizeof(myArray1[0]);i++){
      if(myArray1[i][0] == target1){
         data_1 = myArray1[i][1];
      }
  }
  Serial.println(data_1);
}
void fn2(){
    for(int i=0;i< sizeof(myArray2)/sizeof(myArray2[0]);i++){
      if(myArray2[i][0] == target2){
         data_2 = myArray2[i][1];
      }
  }
  Serial.println(data_2);
}
