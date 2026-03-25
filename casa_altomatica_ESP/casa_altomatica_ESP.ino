#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>

const char* ssid = "Sala_de_Robotica";
const char* password = "12345678";

WebServer server(80);

int mensagem = 0;
int posicao_varal = 0;

String html_home() {
  String html = "<!DOCTYPE html><html>";
  html += "<head><meta charset='UTF-8'><title>Controle ESP32</title></head>";
  html += "<body style='text-align:center; font-family: Arial;'>";
  html += "<h1>Controle da Casa Automática</h1>";
  html += "<form action='/luzes' method='GET'><button type='submit'>Controle de Luzes</button></form>";
  html += "<form action='/sensores' method='GET'><button type='submit'>Controle de Sensores</button></form>";
  html += "<form action='/perifericos' method='GET'><button type='submit'>Controle de Periféricos</button></form>";
  html += "</body></html>";
  return html;
}

String html_controle_luzes() {
  String html = "<!DOCTYPE html><html>";
  html += "<head><meta charset='UTF-8'><title>Controle Luzes</title></head>";
  html += "<body style='text-align:center; font-family: Arial;'>";
  html += "<h1>Controle de Luzes</h1>";

  html += "<h2>Sala</h2>";
  html += "<form action='/luzes' method='GET'>";
  html += "<button type='submit' name='btnl' value='sala_on'>Ligar</button>";
  html += "<button type='submit' name='btnl' value='sala_off'>Desligar</button>";
  html += "</form>";

  html += "<h2>Quarto</h2>";
  html += "<form action='/luzes' method='GET'>";
  html += "<button type='submit' name='btnl' value='quarto_on'>Ligar</button>";
  html += "<button type='submit' name='btnl' value='quarto_off'>Desligar</button>";
  html += "</form>";

  html += "<h2>Garagem</h2>";
  html += "<form action='/luzes' method='GET'>";
  html += "<button type='submit' name='btnl' value='garagem_on'>Ligar</button>";
  html += "<button type='submit' name='btnl' value='garagem_off'>Desligar</button>";
  html += "</form>";

  html += "<br><a href='/'>Voltar</a>";
  html += "</body></html>";
  return html;
}

String html_controle_sensores() {
  String html = "<!DOCTYPE html><html>";
  html += "<head><meta charset='UTF-8'><title>Controle Sensores</title></head>";
  html += "<body style='text-align:center; font-family: Arial;'>";
  html += "<h1>Controle Sensores</h1>";

  html += "<h2>Sensor de Temperatura</h2>";
  html += "<form action='/sensores' method='GET'>";
  html += "<button type='submit' name='btns' value='temperatura_on'>Ligar</button>";
  html += "<button type='submit' name='btns' value='temperatura_off'>Desligar</button>";
  html += "</form>";

  html += "<h2>Sensor de Chuva</h2>";
  html += "<form action='/sensores' method='GET'>";
  html += "<button type='submit' name='btns' value='chuva_on'>Ligar</button>";
  html += "<button type='submit' name='btns' value='chuva_off'>Desligar</button>";
  html += "</form>";

  html += "<br><a href='/'>Voltar</a>";
  html += "</body></html>";
  return html;
}

String html_controle_perifericos() {
  String html = "<!DOCTYPE html><html>";
  html += "<head><meta charset='UTF-8'><title>Controle Periféricos</title></head>";
  html += "<body style='text-align:center; font-family: Arial;'>";
  html += "<h1>Controle Periféricos</h1>";

  html += "<h2>Porta</h2>";
  html += "<form action='/perifericos' method='GET'>";
  html += "<button type='submit' name='btnp' value='porta_aberta'>Abrir</button>";
  html += "<button type='submit' name='btnp' value='porta_fechada'>Fechar</button>";
  html += "</form>";

  html += "<h2>Varal</h2>";
  html += "<input type='range' name='value' min='0' max='255' value='" + String(posicao_varal) + "' id='varalSlider' oninput='updateValue(this.value)'>";
  html += "<output id='varalOutput'>" + String(posicao_varal) + "</output>";

  html += "<script>";
  html += "function updateValue(val) {";
  html += "  document.getElementById('varalOutput').value = val;";
  html += "  var xhr = new XMLHttpRequest();";
  html += "  xhr.open('GET', '/perifericos?value=' + val, true);";
  html += "  xhr.send();";
  html += "}";
  html += "</script>";

  html += "<br><a href='/'>Voltar</a>";
  html += "</body></html>";
  return html;
}

void home() {
  server.send(200, "text/html", html_home());
}

void controle_luzes() {
  if (server.hasArg("btnl")) {
    String botao = server.arg("btnl");

    if (botao == "sala_on") mensagem = 1;
    else if (botao == "sala_off") mensagem = 2;
    else if (botao == "quarto_on") mensagem = 3;
    else if (botao == "quarto_off") mensagem = 4;
    else if (botao == "garagem_on") mensagem = 5;
    else if (botao == "garagem_off") mensagem = 6;
  }
  server.send(200, "text/html", html_controle_luzes());
}

void controle_sensores() {
  if (server.hasArg("btns")) {
    String botao = server.arg("btns");

    if (botao == "temperatura_on") mensagem = 7;
    else if (botao == "temperatura_off") mensagem = 8;
    else if (botao == "chuva_on") mensagem = 9;
    else if (botao == "chuva_off") mensagem = 10;
  }
  server.send(200, "text/html", html_controle_sensores());
}

void controle_perifericos() {
  if (server.hasArg("btnp")) {
    String botao = server.arg("btnp");

    if (botao == "porta_aberta") mensagem = 11;
    else if (botao == "porta_fechada") mensagem = 12;
  }

  if (server.hasArg("value")) {
    posicao_varal = server.arg("value").toInt();
    mensagem = posicao_varal;
  }

  server.send(200, "text/html", html_controle_perifericos());
}

void setup() {
  Wire.begin();
  Serial.begin(115200);

  WiFi.softAP(ssid, password);
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.softAPIP());

  server.on("/", home);
  server.on("/luzes", controle_luzes);
  server.on("/sensores", controle_sensores);
  server.on("/perifericos", controle_perifericos);

  server.begin();
  Serial.println("Servidor Web iniciado!");
}

void loop() {
  server.handleClient();

  if (mensagem != 0) {
    Wire.beginTransmission(8);
    Wire.write(mensagem);
    Wire.endTransmission();
    mensagem = 0;
  }
}