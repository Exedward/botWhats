//Funcionando
#include <WiFiManager.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <MediaMovel.h>
#include <Edu_Ultrassonico.h>

WiFiClient client;
HTTPClient http;
WiFiManager wm;
Ultrassonico  ultra(D1, D2); //Echo, Trigger
MM  mms(10);

const char* ssid = "TP LINK";
const char* password = "morais1212";
bool statusDetect = 0;
unsigned long cm = 10, timeIntervalUltra = 0;
float distancia = 0;

String apiKey = "3466759";
String whats = "558597498597";
String messageToSend = "Botão+1+Acionado.";

void setup(){
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, 1);
  bool res;
  res = wm.autoConnect("Edu_Whats","edu12345678");
  if(!res){
    Serial.println("Falha ao conectar");
  } 
  else{
    Serial.println("Conectado! :)");
  }
}
void loop(){
  if(millis() - timeIntervalUltra >= 60){
    timeIntervalUltra = millis();
    cm = ultra.centimetrosUltra();
    distancia = mms.calculaMMS(cm);
    Serial.print("Distancia: ");
    Serial.println(distancia);
  }
  if(distancia <= 5) digitalWrite(LED_BUILTIN, 0);
  else digitalWrite(LED_BUILTIN, 1);
  if((distancia > 5) && statusDetect){
    statusDetect = 0;
  } 
  if((distancia <= 5) && !statusDetect){
    statusDetect = 1;
    if(WiFi.status() == WL_CONNECTED){
      String requestUrl = "http://api.callmebot.com/whatsapp.php?phone="+whats+"&text="+messageToSend+"&apikey="+apiKey; 
      http.begin(client, requestUrl);
      
      int httpResponseCode = http.GET();

      if(httpResponseCode == HTTP_CODE_OK){
        String payload = http.getString();
        Serial.print("Code: ");
        Serial.println(httpResponseCode);
        Serial.print("Response Payload: ");
        Serial.println(payload);
      }
      else{
        Serial.print("Erro, code: ");
        Serial.println(httpResponseCode);
      }
      http.end();
    } 
    else{
      Serial.println("WiFi Desconectado");
    }
  }
}