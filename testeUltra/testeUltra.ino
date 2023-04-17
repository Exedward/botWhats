//Funcionando
#include <MediaMovel.h>
#include <Edu_Ultrassonico.h>

Ultrassonico  ultra(D1, D2);
MM  mms(10);
unsigned long cm = 0, timeIntervalUltra = 0;
float distancia = 0;

void setup(){
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, 1);
}

void loop(){
  if(millis() - timeIntervalUltra >= 60){
    timeIntervalUltra = millis();
    cm = ultra.centimetrosUltra();
    distancia = mms.calculaMMS(cm);
    if(distancia < 5){
    digitalWrite(LED_BUILTIN, 0);
    }
    else{
      digitalWrite(LED_BUILTIN, 1);
    }
    Serial.print("Distancia: ");
    Serial.println(distancia);
  }
}