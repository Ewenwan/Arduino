#define BUTTON 2
#define zero_state 0
#define one_state 1

int pbIn = 0;                  // 定义中断引脚为0，也就是D2引脚
char cmd;
volatile unsigned long stick = 0;
volatile unsigned long duration;
volatile unsigned long t1;
volatile unsigned long t2;
volatile unsigned int state_bt;
volatile unsigned int count=0;
volatile unsigned int state_fsm=zero_state;
volatile char input_bit;

void setup()
{                
  // 监视中断输入引脚的变化
  pinMode(BUTTON,INPUT);
  attachInterrupt(pbIn, stateChange, CHANGE);
  Serial.begin(9600);
}
 
void loop()                     
{
  if(Serial.available()){
    cmd=Serial.read();
    if(cmd=='c'){
      Serial.println();
      Serial.print("The num of one's sections:");
      Serial.println(count);
    }
  }else{
    delay(10);
  }
}
 
void stateChange()
{
  state_bt = digitalRead(BUTTON);
  if(stick==0){
    stick = millis();
    switch(state_bt){
      case LOW:
        t1 = millis();
        break;
      case HIGH:
        t2 = millis();
        duration = t2-t1;
        if(duration < 500)
         {
          input_bit='0';
          Serial.print(input_bit);
         }
        else
        {
          input_bit='1';
          Serial.print(input_bit);
        }
        break;
    } 
    switch(state_fsm){
      case zero_state:
        if(input_bit=='1'){
           state_fsm=one_state;
           count++;
        }
        break;
      case one_state:
        if(input_bit=='0'){
           state_fsm=zero_state;
        }
        break;
    }
  }
  else if(millis() - stick>3){
    stick =millis();
    switch(state_bt){
      case LOW:
        t1 = millis();
        break;
      case HIGH:
        t2 = millis();
        duration = t2-t1;
        if(duration < 500)
         {
          input_bit='0';
          Serial.print(input_bit);
         }
        else
        {
          input_bit='1';
          Serial.print(input_bit);
        }
        break;
    }  
    switch(state_fsm){
      case zero_state:
        if(input_bit=='1'){
           state_fsm=one_state;
           count++;
        }
        break;
      case one_state:
        if(input_bit=='0'){
           state_fsm=zero_state;
        }
        break;
    }
  }
}
