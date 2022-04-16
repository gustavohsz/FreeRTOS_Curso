/*

Exemplo para criação de tasks
Esse exemplo exibr como criar tarefas no FreeRTOS

*/


#include <Arduino.h>

// Bibliotecas FreeRTOS
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define LED 2

//Variáveis para armazenamento do handle das tasks
TaskHandle_t task1Handle = NULL;
TaskHandle_t task2Handle = NULL;

//Protótipos das tasks
void vTask1(void *pvParameters);
void vTask2(void *pvParameters);

void setup() {
  Serial.begin(115200);
  
  
  xTaskCreate(vTask1, "TASK1", configMINIMAL_STACK_SIZE, NULL, 1, &task1Handle);
  xTaskCreate(vTask2, "TASK2", configMINIMAL_STACK_SIZE+1024, NULL, 2, &task1Handle);
}

void loop() {
  vTaskDelay(1000);
}

void vTask1(void *pvParameters){
  pinMode(LED, OUTPUT);
  while (1)
  {
    digitalWrite(LED, !digitalRead(LED));
    vTaskDelay(pdMS_TO_TICKS(1200));
  }
  
}

void vTask2(void *pvParameters){

  int cont = 0;

  while (1)
  {
    Serial.println("Task2: " + String(cont++));
    vTaskDelay(pdMS_TO_TICKS(1200));
  }
  
}