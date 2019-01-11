#define LED 6        //A0
#define Light 15      //A1
#define Rotation 16   //A2

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(LED,OUTPUT); 
  pinMode(Light,INPUT);
  pinMode(Rotation,INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  int rota=analogRead(Rotation);
  //电位器读数为0时由环境控制亮度
  if(rota!=0){
    Serial.println("LED controlled by rotation!");
    Serial.println(rota);
    analogWrite(LED,map(rota,0,1023,0,255));
    delay(500); 
    }
  else{
    Serial.println("LED effected by environment!");
    int light=analogRead(Light);
    //666,921是根据自己台灯亮度取的接近数值
    analogWrite(LED,map(light,200,1200,0,255));
    delay(500); 
    }
}
