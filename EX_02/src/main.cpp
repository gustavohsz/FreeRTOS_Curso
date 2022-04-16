/*

Exemplo para deletar task
Esse exemplo exibr como deletar uma task

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
  xTaskCreate(vTask2, "TASK2", configMINIMAL_STACK_SIZE+1024, NULL, 2, &task2Handle);
}

void loop() {
  vTaskDelay(1000);
}

void vTask1(void *pvParameters){
  pinMode(LED, OUTPUT);
  while (1)
  {
    digitalWrite(LED, !digitalRead(LED));
    vTaskDelay(pdMS_TO_TICKS(200));
  }
  
}

void vTask2(void *pvParameters){

  int cont = 0;

  while (1)
  {
    Serial.println("Task2: " + String(cont++));

  if(cont>=10){
    if(task1Handle !=NULL){ //verifica se a task já não foi deletada
      Serial.println("Deletando task 1");
      vTaskDelete(task1Handle);
      digitalWrite(LED, LOW);
      task1Handle = NULL; //coloca null para que não entre nessa condição novamente
    }
  }

  if(cont>15){
      Serial.println("Deletando task 1");
      vTaskDelete(NULL); //deleta task 2
  }

    vTaskDelay(pdMS_TO_TICKS(1000));
  }
  
}