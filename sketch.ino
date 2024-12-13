#include <WiFi.h>
#include <HTTPClient.h>

#define led_verde 2 // Pino utilizado para controle do led verda
#define led_vermelho 40 // Pino utilizado para controle do led vermelho
#define led_amarelo 9 // Pino utilizado para controle do led azul

const int buttonPin = 18;  // pino do botão
int buttonState = 0;  // estado do botão

const int ldrPin = 4;  // pino do botão
int threshold = 600; // limite para modo noturno

void setup() {

  // Configuração inicial dos pinos para controle dos leds como OUTPUTs (saídas) do ESP32
  pinMode(led_amarelo,OUTPUT);
  pinMode(led_verde,OUTPUT);
  pinMode(led_vermelho,OUTPUT);

  // Inicialização das entradas
  pinMode(buttonPin, INPUT); // inicializa pino do botão como input

  digitalWrite(led_amarelo, LOW);
  digitalWrite(led_verde, LOW);
  digitalWrite(led_vermelho, LOW);
  
  Serial.begin(9600); // Configuração para debug por interface serial entre ESP e computador com baud rate de 9600

  Serial.print("Conectando-se ao Wi-Fi");
  WiFi.begin("Wokwi-GUEST", "", 6);
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
  Serial.println(" Conectado!");


  if(WiFi.status() == WL_CONNECTED){ // Se o ESP32 estiver conectado à Internet
    HTTPClient http;

    String serverPath = "http://www.google.com.br/"; // Endpoint da requisição HTTP

    http.begin(serverPath.c_str());

    int httpResponseCode = http.GET(); // Código do Resultado da Requisição HTTP

    if (httpResponseCode>0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      String payload = http.getString();
      Serial.println(payload);
      }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
      }
      http.end();
    }

  else {
    Serial.println("WiFi Desconectado");
  }

}

void loop() {
  int ldrstatus=analogRead(ldrPin);

  while (ldrstatus<=threshold){
    Serial.print("Modo noturno");
    Serial.println(ldrstatus);

    digitalWrite(led_amarelo, HIGH); 
    delay(1000);
    digitalWrite(led_amarelo, LOW);
    delay(1000);
  }
  
    Serial.print("Modo diurno");
    Serial.println(ldrstatus);

        //Funcionamento normal das fases do farol
        digitalWrite(led_verde, HIGH);
        digitalWrite(led_amarelo, LOW);
        digitalWrite(led_vermelho, LOW);
        delay(3000);
        Serial.println("Semáforo verde: 3 segundos");

        digitalWrite(led_verde, LOW);
        digitalWrite(led_amarelo, HIGH);
        digitalWrite(led_vermelho, LOW);
        delay(2000);
        Serial.println("Semáforo amarelo: 2 segundos");

        digitalWrite(led_verde, LOW);
        digitalWrite(led_amarelo, LOW);
        digitalWrite(led_vermelho, HIGH);
        delay(5000);
        Serial.println("Semáforo vermelho: 5 segundos");
        // Verifica estado do botão
        buttonState = digitalRead(buttonPin);
        if (buttonState == HIGH) {
          Serial.println("Botão pressionado!");
          digitalWrite(led_amarelo, LOW);
          digitalWrite(led_verde, HIGH);
          digitalWrite(led_vermelho, LOW);
          delay(1000);
        } else {
          Serial.println("Botão não pressionado!");
        }
}