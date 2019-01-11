#define EXTERN_LED 12
#define BUILTIN_LED 13

void setup() {
  // put your setup code here, to run once:
  pinMode(EXTERN_LED, OUTPUT);
  pinMode(BUILTIN_LED, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  int choose = random(0,10);
  if(choose%2==0){
    digitalWrite(EXTERN_LED, HIGH);
    delay(1000);
    digitalWrite(EXTERN_LED, LOW);
    delay(1000);
    }
  else{
    digitalWrite(BUILTIN_LED, HIGH);
    delay(1000);
    digitalWrite(BUILTIN_LED, LOW);
    delay(1000);
    }
}
