/*
 * O código a baixo permite que um led seja controlado tanto pelo botão,
 * quanto pelo browser.
 * É dado um feedback ON ou OFF quando o botão é pressionado
 */

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include "homePage.h"

#define NOME "Valdirene"  // O nome da rede utilizada
#define SENHA "wifi@@@@"  //credenciais da rede
#define LED 2            //GPIO 2 terá um led 
#define BOTAO 0           //GPIO 0 terá um botão

//Definindo a porta do nosso webserver
ESP8266WebServer server(80);
void setup() {
  Serial.begin(115200);
  
  //definindo inputs e outputs
  pinMode(LED, OUTPUT);
  pinMode(BOTAO, INPUT);
  
  digitalWrite(LED, LOW);

  //=========== Configurações do Wifi =======================
  
  //Rece NOME e a SENHA que definimos
  WiFi.begin(NOME, SENHA);
  
    //Feedback do status da conexão
  Serial.println("");
  Serial.print("Conectando");
  while(WiFi.status() != WL_CONNECTED){
  delay(500);
  Serial.print(".");
  }
   //Configuração do IP fixo, deve ser congigurado com base na rede utilizada
  IPAddress ip(10, 0, 0, 125);
  IPAddress gateway(10, 0, 0, 1);
  IPAddress subnet(255, 255, 255, 0);
  Serial.print("Configurando IP fixo para: ");
  Serial.println(ip);

   
  //enviamos todas as informações que configuramos
  WiFi.config(ip, gateway, subnet);
    //Feedback
  Serial.println("");
  Serial.println("Connectado");
  Serial.print ("IP: ");
  Serial.println(WiFi.localIP());

  //=================== Funções das requisições HTTP ====================
   
  server.on("/home", HTTP_GET, pgInicial);
  server.on("/acender", HTTP_GET, acenderLed);
  server.on("/apagar", HTTP_GET, apagarLed);
  server.on("/pressionado", HTTP_GET, getPressionado);
  
  server.onNotFound(onNotFound);

  //Iniciando o servidor
  server.begin();
  Serial.println("O servidor HTTP foi iniciado");
}

//================================== LOOP =================================

void loop() {
  // Responsável por gerênciar as requisições,
  //chamando a função equivalente para cada requisição
  server.handleClient();
}

//Funções das requisições 
void onNotFound()
{
  server.send(404, "text/plain", "Not Found");    
}

//=========== Funções do LED================

//A função basicamente confere em que nível lógico está o led
//e inverte, ou seja, se ligado -> desliga e se desligado -> liga
void acenderLed()
{
  digitalWrite(LED, HIGH);
}
void apagarLed(){
  digitalWrite(LED, LOW);
}



//============ Funções do Botão ===================
 
void getPressionado(){
  int p = digitalRead(BOTAO);
  delay(200);
  
  String json = "{\"pressionado\":"+String(p)+"}";

  server.send(200, "application/json", json);
}

//==============Função da página Inicial=============

void pgInicial(){
  server.send(200, "text/html", homePage);
}