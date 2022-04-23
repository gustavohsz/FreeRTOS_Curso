#include <Arduino.h>

#include <HCSR04.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"

#include <HCSR04.h>

#define LED 2

int triggerPin = 13;
int echoPin = 12;
UltraSonicDistanceSensor distanciaSensor(triggerPin, echoPin);

QueueHandle_t xFila;

TaskHandle_t xTask1Handle,xTask2Handle, xTask3Handle;

void vTask1(void *pvParameters);
void vTask2(void *pvParameters);
void vTask3(void *pvParameters);

void setup() {

  Serial.begin(9600);
  pinMode(LED,OUTPUT);

  xFila = xQueueCreate(5,sizeof(double)); 

  xTaskCreate(vTask1,"Task1",configMINIMAL_STACK_SIZE+1024,NULL,2,&xTask1Handle); //task para medir a distancia

  xTaskCreate(vTask2,"Task2",configMINIMAL_STACK_SIZE+1024,NULL,1,&xTask2Handle); // task para imprimir a distancia

}

void loop() {}

void vTask1(void *pvParameters){

  while (1)
  {
    
    double distancia = distanciaSensor.measureDistanceCm();
    xQueueSend(xFila, &distancia,portMAX_DELAY);

    vTaskDelay(pdMS_TO_TICKS(100));
  
  }
}

void vTask2(void *pvParameters){

  double valorRecebido = 0;

  while (1)
  {
    if(xQueueReceive(xFila,&valorRecebido,pdMS_TO_TICKS(1000))==pdTRUE){
      Serial.println("Distancia: " + String(valorRecebido) + " cm");
    }
    else{
      Serial.println("TIMEOUT");
    }
     if(valorRecebido >10 && valorRecebido<20){
        digitalWrite(LED, !digitalRead(LED));
        vTaskDelay(pdMS_TO_TICKS(200));
      }
      else if(valorRecebido <10){
        digitalWrite(LED, !digitalRead(LED));
        vTaskDelay(pdMS_TO_TICKS(30));
      }
      else if(valorRecebido>50){
        digitalWrite(LED, LOW);
      }
  }
}

