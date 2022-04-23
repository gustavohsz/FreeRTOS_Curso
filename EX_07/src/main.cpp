/*

Exemplo que demonstra como criar uma fila, adicionar e ler
dados da fila através de duas tasks

*/

#include <Arduino.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#define LED 2

QueueHandle_t xFila;

TaskHandle_t xTask1Handle, xTask2Handle;

void vTask1(void *pvParameters);
void vTask2(void *pvParameters);

void setup() {

  BaseType_t xReturned;

  Serial.begin(9600);
  pinMode(LED, OUTPUT);

  xFila = xQueueCreate(5, sizeof(int)); // a fila pode conter até 5 elementos para variaveis do tipo int

  //verifica se a fila foi criada
  if(xFila == NULL){
    Serial.println("Nao foi possivel criar a fila");
    while(1);
  }

  xReturned = xTaskCreate(vTask1, "Task1", configMINIMAL_STACK_SIZE + 1024, NULL, 1, &xTask1Handle);

  //verifica se a task foi criada, por exemplo, em casos de falta de memoria
  if(xReturned == pdFAIL){
    Serial.println("Nao foi possivel criar a task");
    while(1);
  }

  xReturned = xTaskCreate(vTask2, "Task2", configMINIMAL_STACK_SIZE + 1024, NULL, 1, &xTask2Handle);

  //verifica se a task foi criada, por exemplo, em casos de falta de memoria
  if(xReturned == pdFAIL){
    Serial.println("Nao foi possivel criar a task");
    while(1);
  }
}

void loop() {
  digitalWrite(LED, !digitalRead(LED));
  vTaskDelay(pdMS_TO_TICKS(1000));
}

void vTask1(void *pvParameters){

  int count = 0;

  while (1)
  {
    if(count <10){
        xQueueSend(xFila, &count, portMAX_DELAY); //portMAX_DELAY aguarda indefinidamente até a fila estar liberada, caso esteja cheia
        count++;
    }
    else{
      count = 0;
      vTaskDelay(pdMS_TO_TICKS(5000));
    }
    vTaskDelay(pdMS_TO_TICKS(500));
  }
}


void vTask2(void *pvParameters){
  
  int valorRecebido = 0;

  while (1)
  {
    if(xQueueReceive(xFila, &valorRecebido, pdMS_TO_TICKS(1000))==pdTRUE){ //timeout de até 1 segundo
      Serial.println("Valor recebido: " + String(valorRecebido));
    }else{
      Serial.println("TIMEOUT"); //entra em timeout quando o programa estiver resetando count para 0, pois levará 5 segundos 
    }
  }
}
