#include <Arduino.h>

#include <HCSR04.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"

#include <HCSR04.h>

#define LED 32

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

  xTaskCreate(vTask1,"Task1",configMINIMAL_STACK_SIZE+1024,NULL,1,&xTask1Handle); //task para medir a distancia / nucleo 1
  xTaskCreate(vTask2,"Task2",configMINIMAL_STACK_SIZE+1024,NULL,1,&xTask2Handle); // task para imprimir a distancia / nucleo 0
  xTaskCreate(vTask3,"TASK3", configMINIMAL_STACK_SIZE+1024,NULL,1,&xTask3Handle); //task para piscar o led / nucleo 0

}

void loop() {}

void vTask1(void *pvParameters){

  while (1)
  {
    
    double distancia = distanciaSensor.measureDistanceCm(); // calcula a distancia
    xQueueSend(xFila, &distancia,portMAX_DELAY); //Envia a distancia para a fila

    vTaskDelay(pdMS_TO_TICKS(100)); //tempo de amostragem 100ms
  
  }
}

void vTask2(void *pvParameters){

  double valorRecebido = 0;

  while (1)
  {
    if(xQueueReceive(xFila,&valorRecebido,pdMS_TO_TICKS(1000))==pdTRUE){
      Serial.println("Distancia: " + String(valorRecebido) + " cm");
      if(valorRecebido>20){
        digitalWrite(LED, LOW);
        vTaskSuspend(xTask3Handle);
        }else{
        vTaskResume(xTask3Handle);
        }
    }
    else{
      Serial.println("TIMEOUT");
    }
  }
}

void vTask3(void *pvParameters){

  double valorRecebido = 0;

  while (1)
  {
    if(xQueueReceive(xFila,&valorRecebido,pdMS_TO_TICKS(3000))==pdTRUE){
      Serial.println("Distancia: " + String(valorRecebido) + " cm");
      if(valorRecebido >10 && valorRecebido<20){
        digitalWrite(LED, !digitalRead(LED));
        vTaskDelay(pdMS_TO_TICKS(200));
      }
      else if(valorRecebido <10){
        digitalWrite(LED, !digitalRead(LED));
        vTaskDelay(pdMS_TO_TICKS(30));
      }

    }
    else{
      Serial.println("TIMEOUT");
    }
  }
}

