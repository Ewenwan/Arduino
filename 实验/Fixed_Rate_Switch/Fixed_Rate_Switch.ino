#define EXTERN_LED 12
#define BUILTIN_LED 13

void setup() {
  // put your setup code here, to run once:
  pinMode(EXTERN_LED, OUTPUT);
  pinMode(BUILTIN_LED, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
    digitalWrite(EXTERN_LED, HIGH);
    delay(800);
    digitalWrite(EXTERN_LED, LOW);
    digitalWrite(BUILTIN_LED, HIGH);
    delay(800);
    digitalWrite(BUILTIN_LED, LOW);
    digitalWrite(EXTERN_LED, HIGH);
    delay(800);
    digitalWrite(EXTERN_LED, LOW);

    delay(1000);

    digitalWrite(EXTERN_LED, HIGH);
    delay(800);
    digitalWrite(EXTERN_LED, LOW);
    digitalWrite(BUILTIN_LED, HIGH);
    delay(800);
    digitalWrite(BUILTIN_LED, LOW);
    digitalWrite(EXTERN_LED, HIGH);
    delay(800);
    digitalWrite(EXTERN_LED, LOW);

    delay(1000);

    digitalWrite(EXTERN_LED, HIGH);
    delay(300);
    digitalWrite(EXTERN_LED, LOW);
    digitalWrite(BUILTIN_LED, HIGH);
    delay(300);
    digitalWrite(BUILTIN_LED, LOW);
    digitalWrite(EXTERN_LED, HIGH);
    delay(300);
    digitalWrite(EXTERN_LED, LOW);
    digitalWrite(BUILTIN_LED, HIGH);
    delay(300);
    
    digitalWrite(BUILTIN_LED, LOW);
    digitalWrite(EXTERN_LED, HIGH);
    delay(500);
    digitalWrite(EXTERN_LED, LOW);
    digitalWrite(BUILTIN_LED, HIGH);
    delay(500);
    digitalWrite(BUILTIN_LED, LOW);
    digitalWrite(EXTERN_LED, HIGH);
    delay(500);
    digitalWrite(EXTERN_LED, LOW);

    delay(1000);
}
