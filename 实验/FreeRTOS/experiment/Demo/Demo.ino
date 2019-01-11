#include <Arduino_FreeRTOS.h>
#include <semphr.h>  // add the FreeRTOS functions for Semaphores (or Flags).

#define Button 8
#define LED_Extern 9
#define Buzzer 12
#define LED_Buildin 13
#define Light_Sensor A0
#define TH_Sensor A3
#define Rotation A5

// Declare a mutex Semaphore Handle which we will use to manage the Serial Port.
// It will be used to ensure only one Task is accessing this resource at any time.
SemaphoreHandle_t xSerialSemaphore;

// define five Tasks
void TaskBuzzerAlarm( void *pvParameters );
void TaskLEDControl( void *pvParameters );
void TaskLEDBlink( void *pvParameters );
void TaskLightMonitor( void *pvParameters );
void TaskTHMonitor( void *pvParameters );

void setup() {
  // put your setup code here, to run once:
  pinMode(Button,INPUT);
  pinMode(LED_Extern,OUTPUT);
  pinMode(Buzzer,OUTPUT);
  pinMode(LED_Buildin,OUTPUT);
  pinMode(Light_Sensor,INPUT);
  pinMode(TH_Sensor,INPUT);
  pinMode(Rotation,INPUT);

  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB, on LEONARDO, MICRO, YUN, and other 32u4 based boards.
  }

  // Semaphores are useful to stop a Task proceeding, where it should be paused to wait,
  // because it is sharing a resource, such as the Serial port.
  // Semaphores should only be used whilst the scheduler is running, but we can set it up here.
  if ( xSerialSemaphore == NULL )  // Check to confirm that the Serial Semaphore has not already been created.
  {
    xSerialSemaphore = xSemaphoreCreateMutex();  // Create a mutex semaphore we will use to manage the Serial Port
    if ( ( xSerialSemaphore ) != NULL )
      xSemaphoreGive( ( xSerialSemaphore ) );  // Make the Serial Port available for use, by "Giving" the Semaphore.
  }

  // Now set up two tasks to run independently.
  xTaskCreate(
    TaskBuzzerAlarm
    ,  (const portCHAR *)"BuzzerAlarm"   // A name just for humans
    ,  128  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  3  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL );

  xTaskCreate(
    TaskLEDControl
    ,  (const portCHAR *) "LEDControl"
    ,  128  // Stack size
    ,  NULL
    ,  2  // Priority
    ,  NULL );

  xTaskCreate(
    TaskLEDBlink
    ,  (const portCHAR *) "LEDBlink"
    ,  128  // Stack size
    ,  NULL
    ,  0  // Priority
    ,  NULL );

  xTaskCreate(
    TaskLightMonitor
    ,  (const portCHAR *) "LightMonitor"
    ,  128  // Stack size
    ,  NULL
    ,  1  // Priority
    ,  NULL );

  xTaskCreate(
    TaskTHMonitor
    ,  (const portCHAR *) "THMonitor"
    ,  128  // Stack size
    ,  NULL
    ,  1  // Priority
    ,  NULL );
  // Now the task scheduler, which takes over control of scheduling individual tasks, is automatically started.
}

void loop() {
  // put your main code here, to run repeatedly:

  // Empty. Things are done in Tasks.
  
}

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
