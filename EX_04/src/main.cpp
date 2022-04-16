/*

Exemplo para passagem de parametros na criação da task

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
  
  
  xTaskCreate(vTaskBlink, "TASK1", configMINIMAL_STACK_SIZE, (void*)LED, 1, &task1Handle);
  xTaskCreate(vTask2, "TASK2", configMINIMAL_STACK_SIZE+1024, (void*)valor, 2, &task2Handle);
  xTaskCreate(vTaskBlink, "TASK3", configMINIMAL_STACK_SIZE, (void*)LED_2, 1, &task3Handle);
}

void loop() {
  vTaskDelay(1000);
}

void vTaskBlink(void *pvParameters){

  int pin = (int)pvParameters;
  pinMode(pin, OUTPUT);
  while (1)
  {
    digitalWrite(pin, !digitalRead(pin));
    vTaskDelay(pdMS_TO_TICKS(200));
  }
  
}

void vTask2(void *pvParameters){

  int cont = (int)pvParameters;

  while (1)
  {
    Serial.println("Task2: " + String(cont++));
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
  
}