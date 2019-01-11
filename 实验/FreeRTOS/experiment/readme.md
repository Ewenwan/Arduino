FreeRTOS Experiment
在给定盒子所包含的器件范围内，利用freertos构建一个小综合实验（至少包含三个实时任务）

说明：

使用器材及相应pin口：

```
#define Button 8
#define LED_Extern 9
#define Buzzer 12
#define LED_Buildin 13
#define Light_Sensor A0
#define TH_Sensor A3
#define Rotation A5

pinMode(Button,INPUT);
pinMode(LED_Extern,OUTPUT);
pinMode(Buzzer,OUTPUT);
pinMode(LED_Buildin,OUTPUT);
pinMode(Light_Sensor,INPUT);
pinMode(TH_Sensor,INPUT);
pinMode(Rotation,INPUT);
```

用来确保串口互斥访问的信号量及其使用：

```
// Declare a mutex Semaphore Handle which we will use to manage the Serial Port.
// It will be used to ensure only only one Task is accessing this resource at any time.
SemaphoreHandle_t xSerialSemaphore;

// Semaphores are useful to stop a Task proceeding, where it should be paused to wait,
// because it is sharing a resource, such as the Serial port.
// Semaphores should only be used whilst the scheduler is running, but we can set it up here.
if ( xSerialSemaphore == NULL )  
// Check to confirm that the Serial Semaphore has not already been created.
{
xSerialSemaphore = xSemaphoreCreateMutex(); 
// Create a mutex semaphore we will use to manage the Serial Port
if ( ( xSerialSemaphore ) != NULL )
	xSemaphoreGive( ( xSerialSemaphore ) );  
// Make the Serial Port available for use, by "Giving" the Semaphore.
}
```

实验中包含五个实时任务：

```
// define five Tasks
void TaskBuzzerAlarm( void *pvParameters );//按钮控制蜂鸣器
void TaskLEDControl( void *pvParameters );//旋位器控制LED灯
void TaskLEDBlink( void *pvParameters );//板载LED灯闪烁
void TaskLightMonitor( void *pvParameters );//光线传感器监控亮度
void TaskTHMonitor( void *pvParameters );//温湿度传感器监控温湿度
```

任务实体：

```
void TaskBuzzerAlarm( void *pvParameters __attribute__((unused)) )  // This is a Task.
{
  /*
    DigitalRead
    Reads a digital input on pin 8 that has a pushbutton attached to it, prints the result to the serial monitor
  */

  for (;;) // A Task shall never return or exit.
  {
    // read the input pin:
    int buttonState = digitalRead(Button);

    if(buttonState==LOW){
    digitalWrite(Buzzer,LOW);
    }
    else{
     digitalWrite(Buzzer,HIGH);
    }

    vTaskDelay( 500 / portTICK_PERIOD_MS );  // half second delay in between reads for stability
  }
}

void TaskLEDControl(void *pvParameters)  // This is a task.
{
  (void) pvParameters;

/*
  LED Controlled By Rotation Sensor
*/
  for (;;) // A Task shall never return or exit.
  {
    int rota=analogRead(Rotation);
    analogWrite(LED_Extern,map(rota,0,1023,0,255));
    vTaskDelay( 500 / portTICK_PERIOD_MS ); // wait for half second
  }
}

void TaskLEDBlink(void *pvParameters)  // This is a task.
{
  (void) pvParameters;

/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
*/
  for (;;) // A Task shall never return or exit.
  {
    digitalWrite(LED_Buildin, HIGH);   // turn the LED on (HIGH is the voltage level)
    vTaskDelay( 1000 / portTICK_PERIOD_MS ); // wait for one second
    digitalWrite(LED_Buildin, LOW);    // turn the LED off by making the voltage LOW
    vTaskDelay( 1000 / portTICK_PERIOD_MS ); // wait for one second
  }
}

void TaskLightMonitor( void *pvParameters __attribute__((unused)) )  // This is a Task.
{
  /*
    AnalogRead
    Reads a analog input on pin A0 that has a Light Sensor attached to it, prints the result to the serial monitor
  */

  for (;;) // A Task shall never return or exit.
  {
    // read the input pin:
    int LightValue = analogRead(Light_Sensor);

    // See if we can obtain or "Take" the Serial Semaphore.
    // If the semaphore is not available, wait 5 ticks of the Scheduler to see if it becomes free.
    if ( xSemaphoreTake( xSerialSemaphore, ( TickType_t ) 5 ) == pdTRUE )
    {
      // We were able to obtain or "Take" the semaphore and can now access the shared resource.
      // We want to have the Serial Port for us alone, as it takes some time to print,
      // so we don't want it getting stolen during the middle of a conversion.
      // print out the value of the light sensor:
      Serial.print("Light Sensor(0~1023):");
      Serial.println(LightValue);

      xSemaphoreGive( xSerialSemaphore ); // Now free or "Give" the Serial Port for others.
    }

    vTaskDelay( 5000 / portTICK_PERIOD_MS );  // 5 second delay in between reads for stability
  }
}

void TaskTHMonitor( void *pvParameters __attribute__((unused)) )  // This is a Task.
{
  /*
    AnalogRead
    Reads a analog input on pin A0 that has a Temporature&Humidity Sensor attached to it, prints the result to the serial monitor
  */

  for (;;) // A Task shall never return or exit.
  {
    // read the input pin:
    int THValue = analogRead(TH_Sensor);

    // See if we can obtain or "Take" the Serial Semaphore.
    // If the semaphore is not available, wait 5 ticks of the Scheduler to see if it becomes free.
    if ( xSemaphoreTake( xSerialSemaphore, ( TickType_t ) 5 ) == pdTRUE )
    {
      // We were able to obtain or "Take" the semaphore and can now access the shared resource.
      // We want to have the Serial Port for us alone, as it takes some time to print,
      // so we don't want it getting stolen during the middle of a conversion.
      // print out the value of the THlk liyhb kmyhh kmmi sensor:
      Serial.print("Temporature&Humidity Sensor(0~1023):");
      Serial.println(THValue);

      xSemaphoreGive( xSerialSemaphore ); // Now free or "Give" the Serial Port for others.
    }

    vTaskDelay( 5000 / portTICK_PERIOD_MS );  // 5 second delay in between reads for stability
  }
}
```

PS:演示视频中依次展示五个实时任务