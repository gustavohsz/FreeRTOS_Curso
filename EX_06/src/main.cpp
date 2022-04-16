/*

Exemplo que mostra a quantidade de memória consumida na STACK

*/


#include <Arduino.h>

// Bibliotecas FreeRTOS
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define LED 2
#define LED_2 4

//Variáveis para armazenamento do handle das tasks
TaskHandle_t task1Handle = NULL;
TaskHandle_t task2Handle = NULL;
TaskHandle_t task3Handle = NULL;

//Protótipos das tasks
void vTaskBlink(void *pvParameters);
void vTask2(void *pvParameters);

int valor = 500;

void setup() {
  Serial.begin(9600);
  
  xTaskCreatePinnedToCore(vTaskBlink, "TASK1", configMINIMAL_STACK_SIZE, (void*)LED, 1, &task1Handle, APP_CPU_NUM); // app nucleo 1
  xTaskCreatePinnedToCore(vTask2, "TASK2", configMINIMAL_STACK_SIZE+1024, (void*)valor, 2, &task2Handle, PRO_CPU_NUM);// pro núcleo 0
  xTaskCreatePinnedToCore(vTaskBlink, "TASK3", configMINIMAL_STACK_SIZE, (void*)LED_2, 1, &task3Handle, APP_CPU_NUM);
}

void loop() {
  vTaskDelay(1000);
}

void vTaskBlink(void *pvParameters){

  UBaseType_t uxHighWaterMark;

  int pin = (int)pvParameters;
  pinMode(pin, OUTPUT);
  while (1)
  {
    digitalWrite(pin, !digitalRead(pin));
    vTaskDelay(pdMS_TO_TICKS(200));

    uxHighWaterMark = uxTaskGetStackHighWaterMark(NULL);
    Serial.print(pcTaskGetTaskName(NULL));
    Serial.print(" : ");
    Serial.println(uxHighWaterMark);
  }
  
}

void vTask2(void *pvParameters){

  UBaseType_t uxHighWaterMark;

  int cont = (int)pvParameters;

  while (1)
  {
    /*Serial.println("Task2: " + String(cont++));*/
    vTaskDelay(pdMS_TO_TICKS(1000));

    uxHighWaterMark = uxTaskGetStackHighWaterMark(NULL);
    Serial.print(pcTaskGetTaskName(NULL));
    Serial.print(" : ");
    Serial.println(uxHighWaterMark);
  }
  
}