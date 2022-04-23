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

//sincroniza o loop com vTaskADC após o LED piscar, então fazendo a leitura da entrada ADC
void loop(){
  digitalWrite(LED, !digitalRead(LED));
  Serial.println("Valor do LED: " + String(digitalRead(LED)));
  vTaskDelay(pdMS_TO_TICKS(1500));
  xSemaphoreGive(xSemaphore);
}

void vtaskADC(void *pvParameters){

  int adcValue;

  while (1){
    xSemaphoreTake(xSemaphore, portMAX_DELAY);
    adcValue = analogRead(34);
    Serial.println("Valor do ADC: " + String(adcValue));
    Serial.println(" ");
  }
}