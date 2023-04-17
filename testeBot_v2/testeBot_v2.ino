//Funcionando
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <MediaMovel.h>
#include <Edu_Ultrassonico.h>

WiFiClient client;
HTTPClient http;
Ultrassonico  ultra(D1, D2); //Echo, Trigger
MM  mms(10);

const char* ssid = "TP LINK";
const char* password = "morais1212";
bool statusDetect = 0;
unsigned long lastTime = 0, cm = 10, timeIntervalUltra = 0;
float distancia = 0;

String apiKey = "3466759";
String whats = "558597498597";
String messageToSend = "Botão+1+Acionado.";

void setup(){
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  setup_wifi();
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
    lastTime = millis();
    
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

void setup_wifi(){
  Serial.print("Conectando-se a ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }

  Serial.print("WiFi Conectado! - Endereço IP: ");
  Serial.println(WiFi.localIP());
}