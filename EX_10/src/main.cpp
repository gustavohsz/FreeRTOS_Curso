/*********************************************************
* Exemplo que demonstra como enviar valores para uma fila 
* a partir de uma ISR
* Por: Fábio Souza
*********************************************************/

#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"

#define LED 2
#define BT 12

TaskHandle_t xTaskTrataBTHandle;

SemaphoreHandle_t semaforo;

void vTaskTrataBT(void *pvParameters);

void ISR_CallBack(){

    BaseType_t xHighPriorityTaskWoken = pdTRUE;

    xSemaphoreGiveFromISR(semaforo, &xHighPriorityTaskWoken);

    if(xHighPriorityTaskWoken == pdTRUE){
      portYIELD_FROM_ISR();
    }
}

void setup(){

  Serial.begin(9600);
  pinMode(LED, OUTPUT);
  pinMode(BT, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(BT), ISR_CallBack, FALLING);

  semaforo = xSemaphoreCreateBinary(); //CRIAÇÃO DO SEMAFORO EM BINARIO

  if(semaforo == NULL){
    Serial.println("Nao foi possivel criar o semaforo");
    while(1);
  }

  xTaskCreate(vTaskTrataBT, "Task BT", configMINIMAL_STACK_SIZE+1024, NULL, 3, &xTaskTrataBTHandle);

}


void loop(){
  digitalWrite(LED, HIGH);
  vTaskDelay(pdMS_TO_TICKS(100));
  digitalWrite(LED, LOW);
  vTaskDelay(pdMS_TO_TICKS(1000));
}

void vTaskTrataBT(void *pvParameters){

  int contador = 0;

  while (1){
      xSemaphoreTake(semaforo, portMAX_DELAY); //só libera o semáforo quando o botão for pressionado, ou seja, quando huover a ISR
      Serial.println("x: " + String(contador++));
  }
}
