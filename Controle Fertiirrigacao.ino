#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

#define RELE_PIN 2       

const char *ssid = "AGRINHO-ROBOTICA";
const char *password = "12345678";

ESP8266WebServer server(80);

bool releState = false;
int valor_analogico = 0;


void setup() {
  
  pinMode(RELE_PIN, OUTPUT);
  
  Serial.begin(115200); 

  digitalWrite(RELE_PIN, LOW);  // Inicialmente, mantenha o relé desligado
 
  WiFi.softAP(ssid, password);
  server.on("/", handleRoot);
  server.on("/ligar", handleLigar);
  server.on("/desligar", handleDesligar);
  server.onNotFound(handleNotFound);
  server.begin();
}

void loop() {

  server.handleClient();

}


void handleRoot() {

  String dadosRecebidos = Serial.readString();
  valor_analogico = dadosRecebidos.toInt();
  int Porcento = map(valor_analogico, 1023, 0, 0, 100);
  
  String page = buildPage(Porcento); 
  server.send(200, "text/html", page);
}

void handleLigar() {
  if (!releState) {  
    digitalWrite(RELE_PIN, HIGH);  
    releState = true; 
  }
  server.sendHeader("Location", String("/"), true);
  delay(100); 
  server.send(302, "text/plain", "");
 
}

void handleDesligar() {
  if (releState) {  
    digitalWrite(RELE_PIN, LOW); 
    releState = !releState;
    
  }
  server.sendHeader("Location", String("/"), true);
  delay(100); 
  server.send(302, "text/plain", "");
 
}

void handleNotFound() {
  server.send(404, "text/plain", "Página não encontrada");
}

String buildPage(int hum) {
  String page = "<!DOCTYPE html><html><head>";
  page += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">";
  page += "<title>Monitoramento Online e Irriga&ccedil;&atilde;o</title>";
  page += "<style>";
  page += "body { font-family: Arial, sans-serif; background-color: #f2f2f2; }";
  page += ".container { max-width: 600px; margin: 0 auto; padding: 20px; background-color: #fff; border-radius: 10px; box-shadow: 0 0 10px rgba(0,0,0,0.1); }";
  page += ".button { display: inline-block; background-color: #4CAF50; border: none; color: white; padding: 10px 20px; text-align: center; text-decoration: none; font-size: 16px; margin: 4px 2px; cursor: pointer; border-radius: 8px; }";
  page += ".button:hover { background-color: #45a049; }";
  page += "</style>";
  page += "</head><body>";
  page += "<div class=\"container\">";
  page += "<h1>Monitoramento Online</h1>";
  page += "<h1>&#127793;  Fertirriga&ccedil;&atilde;o &#x1F4A7</h1>";
  page += "<p>Umidade do solo: " + String(hum) + "%</p>";
  // Adiciona botões para ligar e desligar o relé
  page += "<p><a href=\"/ligar\" class=\"button\">Ligar Bomba</a></p>";
  page += "<p><a href=\"/desligar\" class=\"button\">Desligar Bomba</a></p>";
  // Exibe o estado atual do relé
  page += "<p>Estado da Bomba: " + String(releState ? "Ligado" : "Desligado") + "</p>";
  page += "</div>";
  page += "<script>";
  page += "setTimeout(function() {";
  page += "  window.location.reload(true);";  // Recarrega a página a cada 1 segundos
  page += "}, 1000);";  // Intervalo de 1000 milissegundos (1 segundos)
  page += "</script>";
  page += "</body></html>";
  return page;
}
