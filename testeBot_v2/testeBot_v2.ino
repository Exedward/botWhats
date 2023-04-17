#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

WiFiClient client;
HTTPClient http;

#define   PIN_BT    D2

const char* ssid = "TP LINK";
const char* password = "morais1212";
bool statusBt = 0;
unsigned long lastTime = 0;

String apiKey = "3466759";
String whats = "558597498597";
String messageToSend = "Botão+1+Acionado.";

void setup(){
  Serial.begin(115200);
  pinMode(PIN_BT, INPUT_PULLUP);
  setup_wifi();
}
void loop(){
  if(!digitalRead(PIN_BT) && !statusBt){
    lastTime = millis();
    statusBt = 1;
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
  if(statusBt) if(millis() - lastTime >= 1000) statusBt = 0;
}

void setup_wifi(){
  delay(10);

  Serial.println();
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