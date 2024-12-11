#include <FS.h>                 // Biblioteca para manipulação do sistema de arquivos
#include <ESP8266WiFi.h>        // Biblioteca WiFi para ESP8266
#include <ESP8266WebServer.h>   // Biblioteca para servidor web
#include <SD.h>                 // Biblioteca para manipulação do cartão SD
#include <SPI.h>                // Biblioteca SPI para comunicação com o cartão SD
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

// Definição do display OLED (supondo que esteja usando um display I2C)
#define SCREEN_WIDTH 128  // Largura do display
#define SCREEN_HEIGHT 64  // Altura do display
#define OLED_RESET    -1  // Não utilizaremos o pino reset
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Configuração da rede Wi-Fi do Access Point
const char* ssid = "CifrasPrompt"; // Nome da rede (SSID)
const char* password = "123gabriel";      // Senha da rede (mínimo 8 caracteres)

// Configuração do IP fixo
IPAddress local_IP(192, 168, 5, 1);       // Endereço IP fixo
IPAddress gateway(192, 168, 5, 1);        // Gateway
IPAddress subnet(255, 255, 255, 0);       // Máscara de sub-rede

int velocidade = 0;  // Armazena a velocidade atual da rolagem
bool autoScroll = false;  // Variável global que controla o estado de Play/Pause
int potPin = A0;  // Pino analógico conectado ao potenciômetro
int potValue = 0;  // Valor lido do potenciômetro
bool isPotMode = false;  // Flag para verificar o modo (Web ou Pot)

// Criação do servidor web
ESP8266WebServer server(80);

// Função para listar os arquivos no cartão SD
String listFiles(String path) {
  String folderList = "<ul>";  // Lista de pastas
  String fileList = "<ul>";    // Lista de arquivos

  // Adiciona link para voltar à pasta anterior (se não for a raiz)
  if (path != "/") {
    String parentPath = String(path).substring(0, path.lastIndexOf("/"));
    if (parentPath == "") parentPath = "/";  // Caso esteja na raiz
    folderList += "<li><a href=\"/list?path=" + parentPath + "\">Voltar</a></li>";
  }

  // Abre o diretório no SD
  File root = SD.open(path);
  if (!root) {
    return "Erro ao acessar a pasta!";
  }

  // Percorre os arquivos e pastas dentro do diretório
  while (true) {
    File file = root.openNextFile();
    if (!file) break;  // Sai do loop se não há mais arquivos

    String fileName = String(file.name());
    Serial.println(fileName);  // Log para depuração

    if (file.isDirectory()) {
      // Adiciona o link para navegar para a pasta com o ícone de pasta
      folderList += "<li><a href=\"/list?path=" + String(path) + "/" + fileName + "\">📁 " + fileName + "</a></li>";
    } else {
      // Remove espaços extras e verifica se o arquivo tem a extensão .txt
      fileName.trim();  // Remove espaços em branco extras
      if (fileName.endsWith(".txt")) {
        fileList += "<li><a href=\"/file?name=" + String(path) + "/" + fileName + "\">📝 " + fileName + "</a></li>";
      } else {
        fileList += "<li><a href=\"/file?name=" + String(path) + "/" + fileName + "\">" + fileName + "</a></li>";
      }
    }

    file.close();  // Fecha o arquivo após processá-lo
  }

  root.close();  // Fecha o diretório raiz

  folderList += "</ul>";
  fileList += "</ul>";

  // Juntar as pastas e arquivos, com pastas primeiro
  return folderList + fileList;
}


// Página principal
void handleRoot() {
  String page = "<!DOCTYPE html>";
  page += "<html><head><title>Prompt Cifra</title>";
  page += "<meta charset=\"UTF-8\">"; // Suporte a caracteres especiais
  page += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";

  // Estilo
  page += "<style>";
  page += "body { font-family: Arial, sans-serif; margin: 0; background: #e0e0e0; color: #333; }"; // Fundo cinza claro
  page += "header { background: #4CAF50; color: white; padding: 15px; text-align: center; font-size: 24px; }"; // Cabeçalho verde
  page += ".content { padding: 20px; }"; // Conteúdo principal com espaçamento
  page += "ul { list-style-type: none; padding: 0; }"; // Remove a bolinha da lista
  page += "li { margin-bottom: 10px; }"; // Espaçamento entre os itens da lista
  page += "a { text-decoration: none; color: #333; font-size: 16px; padding: 10px; display: block; border-radius: 5px; background: #f9f9f9; }"; // Links com fundo
  page += "a:active { background: #4CAF50; color: white; }"; // Feedback visual ao tocar no link
  page += "button { padding: 10px 20px; background: #4CAF50; color: white; border: none; border-radius: 5px; cursor: pointer; font-size: 16px; width: 100%; max-width: 200px; margin: 0 auto; display: block; }"; // Botão centralizado
  page += "button:active { background: #3e8e41; }"; // Feedback no botão ao ser pressionado
  page += ".file-list { margin-top: 20px; }"; // Espaçamento acima da lista de arquivos
  page += "</style>";

  page += "</head><body>";
  page += "<header>PROMPT CIFRA &#x1F3B8</header>"; // Cabeçalho moderno

  // Verifica se há um caminho na URL e lista os arquivos da pasta
  String path = "/";
  if (server.hasArg("path")) {
    path = server.arg("path");
  }

  page += "<div class=\"content\">";

 

  // Lista os arquivos e pastas
  page += "<div class=\"file-list\">";
  page += listFiles(path); // Insere a lista de arquivos/pastas
  page += "</div>";

  page += "</div>"; // Fecha a div content
  page += "</body></html>";

  server.send(200, "text/html", page);

}


// Página para exibir o conteúdo do arquivo com melhorias
void handleFile() {
  if (!server.hasArg("name")) {
    server.send(400, "text/plain", "Falta o parâmetro 'name'");
    return;
  }

  String fileName = server.arg("name");
  File file = SD.open(fileName, FILE_READ);

  if (!file) {
    server.send(404, "text/plain", "Arquivo não encontrado");
    return;
  }

  // Lê o conteúdo do arquivo
  String fileContent = "";
  while (file.available()) {
    fileContent += (char)file.read();
  }
  file.close();

    // Atualiza o display com o nome do arquivo selecionado
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("IP: ");
  display.print(WiFi.softAPIP());  // Exibe novamente o IP no topo
  display.setCursor(0, 16);  // Desce um pouco
  display.print("Cifra: ");
  display.print(fileName);  // Exibe o nome do arquivo na tela
  display.display();  // Atualiza a tela

  // Exibe a velocidade atual no display
  display.setCursor(0, 32);  // Desce um pouco mais
  display.print("Velocidade: ");
  display.print(velocidade);  // Exibe a variável de velocidade
  display.display();  // Atualiza a tela

String page = "<html><head><title>";
page += fileName;
page += "</title>";
page += "<meta charset=\"UTF-8\">"; // Adiciona codificação UTF-8
page += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1, maximum-scale=3, user-scalable=yes\">"; // Permite zoom
page += "<style>";
page += "body { font-family: Arial; margin: 20px; line-height: 1.5; zoom: 1; background-color: white; color: black; transition: background-color 0.3s, color 0.3s; }"; // Estilo padrão
page += "#content { max-width: 600px; margin: 0 auto; white-space: pre-wrap; font-size: 16px; zoom: 1; }"; // Permite zoom no conteúdo
page += "#controls { position: fixed; bottom: 10px; right: 10px; background: rgba(0, 0, 0, 0.7); padding: 10px; border-radius: 10px; color: white; text-align: center; width: 150px; transform: none; }"; // Garante que o controle de velocidade não muda com o zoom
page += "#slider { width: 80%; margin: 5px 0; }";
page += "#playPause { margin-top: 10px; padding: 5px; background: #4CAF50; border: none; border-radius: 5px; color: white; cursor: pointer; font-size: 12px; }";
page += "#playPause.paused { background: #f44336; }";

// Estilo do botão de alternância de modo noturno (sol/lua)
page += "#themeSwitch { position: fixed; top: 10px; right: 10px; width: 30px; height: 30px; background-color: #ffcc00; border-radius: 50%; cursor: pointer; display: flex; justify-content: center; align-items: center; transition: background-color 0.3s; }"; // Sol
page += "#themeSwitch.nightMode { background-color: #333; }"; // Lua
page += "#themeSwitch i { color: white; font-size: 18px; }"; // Ícone do sol/lua
page += "#modeSwitch { display: inline-block; position: relative; width: 60px; height: 30px; background-color: #ccc; border-radius: 50px; cursor: pointer; }";
page += "#modeSwitch .ball { position: absolute; top: 3px; left: 3px; width: 24px; height: 24px; background-color: #fff; border-radius: 50%; transition: left 0.3s; }"; // A bola de slide
page += "#modeSwitch.active { background-color: #4CAF50; }"; // Quando ativado, fundo verde
page += "#modeSwitch.active .ball { left: 33px; }"; // Quando ativado, move a bola para a direita
page += "</style>";

page += "<script>";
page += "let speed = " + String(velocidade) + ";";  // Passa a velocidade inicial para o JavaScript
page += "let autoScroll = false;"; // Inicialmente, a rolagem está pausada
page += "let scrollInterval;"; // Intervalo para controle da rolagem
page += "let isPotMode = false;"; // Inicialmente, o modo Pot está desativado
page += "let isNightMode = false;"; // Inicialmente, o modo noturno está desativado

// Função para alternar entre modo noturno e diurno
page += "function toggleTheme() {";
page += "  isNightMode = !isNightMode;";
page += "  const body = document.body;";
page += "  const themeSwitch = document.getElementById('themeSwitch');";
page += "  const themeIcon = document.getElementById('themeIcon');"; // Pegando o ícone
page += "  if (isNightMode) {";
page += "    body.style.backgroundColor = '#333';"; // Fundo escuro
page += "    body.style.color = '#fff';"; // Texto claro
page += "    themeSwitch.classList.add('nightMode');"; // Altera o ícone para lua
page += "    themeIcon.innerHTML = '🌙';"; // Troca o ícone para lua
page += "  } else {";
page += "    body.style.backgroundColor = '#fff';"; // Fundo claro
page += "    body.style.color = '#000';"; // Texto escuro
page += "    themeSwitch.classList.remove('nightMode');"; // Altera o ícone para sol
page += "    themeIcon.innerHTML = '☀️';"; // Troca o ícone para sol
page += "  }";
page += "}";

// Atualiza a velocidade dependendo do modo
page += "function updateSpeed(value) {";
page += "  speed = value;";
page += "  document.getElementById('speedDisplay').innerText = value;";
page += "  document.getElementById('slider').value = value;";
page += "  fetch(`/setSpeed?value=${value}`, { method: 'GET' })";  // Corrigido a interpolação do string
page += "    .then(response => response.text())";
page += "    .then(data => { console.log(data); });";
page += "  updateDisplay();";
page += "  fetch('/updateOLED', { method: 'GET' })"; // Chama a função de atualização do OLED
page += "    .then(response => response.text())";
page += "    .then(data => { console.log('OLED atualizado:', data); });";
page += "}"; 

// Alterna entre os modos Pot e Web
page += "function toggleMode() {";
page += "  isPotMode = !isPotMode;"; // Alterna o estado entre os modos
page += "  const modeText = isPotMode ? 'Modo Pot' : 'Modo Web';";
page += "  document.getElementById('modeText').innerText = modeText;";
page += "  const slider = document.getElementById('slider');";
page += "  slider.disabled = isPotMode;"; // Desativa o slider no modo Pot
page += "  if (!isPotMode) {";
page += "    slider.value = speed;"; // Reseta o slider para a velocidade atual no modo Web
page += "  }";
page += "  const modeSwitch = document.getElementById('modeSwitch');";
page += "  modeSwitch.classList.toggle('active');"; // Atualiza a aparência do botão";
page += "  const mode = isPotMode ? 'Pot' : 'Web';"; // Define o modo atual";
page += "  fetch(`/setMode?mode=${mode}`, { method: 'GET' })"; // Envia o modo selecionado ao servidor
page += "    .then(response => response.text())";
page += "    .then(data => {";
page += "      console.log('Modo alterado no servidor:', data);";
page += "      fetch('/updateOLED?message=${mode}', { method: 'GET' })"; // Atualiza o OLED com o modo atual
page += "        .then(response => response.text())";
page += "        .then(oledData => { console.log('OLED atualizado:', oledData); })";
page += "        .catch(err => console.error('Erro ao atualizar o OLED:', err));";
page += "    })";
page += "    .catch(err => console.error('Erro ao alterar modo no servidor:', err));";
page += "}";

// Função de rolagem
page += "function toggleScroll() {";
page += "  autoScroll = !autoScroll;";
page += "  const btn = document.getElementById('playPause');";
page += "  btn.classList.toggle('paused');";
page += "  btn.innerText = autoScroll ? 'Pause' : 'Play';";

page += "  if (autoScroll) {";
page += "    startScrolling();";  // Inicia a rolagem
page += "  } else {";
page += "    stopScrolling();";  // Para a rolagem
page += "  }";
page += "  fetch(`/toggleScroll?state=${autoScroll ? 'play' : 'pause'}`);";  // Corrigido a interpolação do string
page += "  updateDisplay();";
page += "  fetch('/updateOLED', { method: 'GET' })"; // Chama a função de atualização do OLED
page += "    .then(response => response.text())";
page += "    .then(data => { console.log('OLED atualizado:', data); });";
page += "}";

// Inicia a rolagem automática com a velocidade definida
page += "function startScrolling() {";
page += "  if (scrollInterval) clearInterval(scrollInterval);";
page += "  scrollInterval = setInterval(function() {";
page += "    window.scrollBy(0, speed / 2);"; // Ajuste a rolagem pela velocidade
page += "  }, 50);"; // Ajuste o intervalo para controlar a velocidade de rolagem
page += "}";

// Para a rolagem automática
page += "function stopScrolling() {";
page += "  if (scrollInterval) clearInterval(scrollInterval);"; // Para a rolagem
page += "}";

// Chama a função de leitura do potenciômetro a cada 500ms quando o modo Pot estiver ativo
page += "setInterval(function() {";
page += "  if (isPotMode) {";
page += "    fetch('/getPotValue')";  // Faz a requisição para obter o valor do potenciômetro";
page += "      .then(response => response.text())";
page += "      .then(data => {";
page += "        speed = data;";  // Atualiza a velocidade com o valor do potenciômetro";
page += "        document.getElementById('speedDisplay').innerText = speed;";
page += "        document.getElementById('slider').value = speed;";
page += "        updateDisplay();";
page += "        fetch('/updateOLED', { method: 'GET' })"; // Chama a função de atualização do OLED
page += "          .then(response => response.text())";
page += "          .then(data => { console.log('OLED atualizado:', data); });";
page += "      });";
page += "  }";
page += "}, 500);"; // A cada 500ms, atualiza a rolagem no modo Pot

page += "</script>";

page += "</head><body>";
page += "<div id=\"content\">";
page += fileContent; // Insere o conteúdo do arquivo
page += "</div>";

page += "<div id=\"themeSwitch\" onclick=\"toggleTheme()\"><i id=\"themeIcon\">☀️</i></div>"; // Ícone de sol/lua no topo
page += "<div id=\"controls\">";
page += "Velocidade: <span id=\"speedDisplay\">" + String(velocidade) + "</span>";  // Atualiza a velocidade no display
page += "<div id=\"modeSwitch\" onclick=\"toggleMode()\"><div class=\"ball\"></div></div>";  // Botão de alternância de modo
page += "<span id=\"modeText\">Modo Web</span>";  // Texto para indicar o modo ativo
page += "<input id=\"slider\" type=\"range\" min=\"1\" max=\"10\" value=\"" + String(velocidade) + "\" onchange=\"updateSpeed(this.value)\">";  // Slider para ajustar a velocidade
page += "<button id=\"playPause\" onclick=\"toggleScroll()\">Play</button>";  // Botão de Play/Pause
page += "</div>";

page += "</body></html>";

  server.send(200, "text/html", page);
}

// Configuração inicial
void setup() {
  Serial.begin(115200);
  pinMode(potPin, INPUT);  // Configura o pino analógico para leitura
 
  // Configurar o ESP8266 como Access Point
  Serial.println("Configurando Access Point...");
  WiFi.softAP(ssid, password);
    // Configura o IP fixo no modo AP
  WiFi.softAPConfig(local_IP, gateway, subnet);

  IPAddress ip = WiFi.softAPIP();
  Serial.print("Access Point iniciado. IP: ");
  Serial.println(ip);

  // Inicia o display OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("Falha ao inicializar o display OLED"));
    while (1);
  }

  display.clearDisplay();  // Limpa a tela

  // Exibe a mensagem de inicialização
  display.setTextSize(1);      // Tamanho do texto
  display.setTextColor(SSD1306_WHITE);  // Cor do texto
  display.setCursor(0, 0);     // Define a posição inicial do texto
  display.println(F("Carregando..."));

  // Exibe a barra de progresso
  int progressBarHeight = SCREEN_HEIGHT - 10;  // Posição para a barra de progresso (um pouco acima do fundo)
  int barWidth = SCREEN_WIDTH - 2;              // Largura da barra de progresso
  display.drawRect(1, progressBarHeight, barWidth, 10, SSD1306_WHITE); // Desenha a borda da barra

  display.display();  // Atualiza a tela

  // Preenche a barra de progresso em etapas
  for (int i = 0; i <= barWidth; i++) {
    display.fillRect(2, progressBarHeight + 1, i, 8, SSD1306_WHITE);  // Preenche a barra progressivamente
    display.display();           // Atualiza a tela com o progresso
    delay(10);                   // Aguarda um curto intervalo para animar o progresso
  }

  delay(500);  // Espera meio segundo para garantir que a barra de progresso esteja completa

// Clear the buffer.
  display.clearDisplay();
  display.display();  // Atualiza a tela
  delay(2000);        // Aguarda 2 segundos antes de limpar a tela

  // Exibe o IP na tela OLED
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("IP: ");
  display.print(ip);  // Exibe o IP na parte superior da tela
  display.display();  // Atualiza a tela



  // Inicializar o cartão SD
  if (!SD.begin(D8)) { // D8 é o pino CS
    Serial.println("Falha ao inicializar o cartão SD");
    return;
  }
  Serial.println("Cartão SD inicializado");

  // Configuração de rotas
  server.on("/", HTTP_GET, handleRoot);  // Página inicial
  server.on("/list", HTTP_GET, handleRoot);  // Página de navegação de pastas
  server.on("/", handleRoot);
  server.on("/file", handleFile);

  // Configuração das rotas do servidor
server.on("/updateOLED", HTTP_GET, []() {
  updateOLED();
  // Atualiza o display com as informações de velocidade e estado de scroll
  server.send(200, "text/plain", "Display OLED atualizado");
});

server.on("/toggleScroll", HTTP_GET, []() {
  
  if (server.hasArg("state")) {
    String state = server.arg("state");
    if (state == "play") {
      autoScroll = true;
      updateOLED();
    } else {
      autoScroll = false;
      updateOLED();
    }
  }
  server.send(200, "text/plain", "Estado do scroll alterado");
});

// Configurações da rota
server.on("/setMode", HTTP_GET, []() {
  // Obtendo o parâmetro "mode" da requisição
  
  String mode = server.arg("mode");
  if (mode == "Web") {
    isPotMode = false;  // Altera para modo Web
  } else if (mode == "Pot") {
    isPotMode = true;  // Altera para modo Pot
  }

  updateOLED();  // Chama a função de atualização do display
  // Respondendo com o modo alterado
  server.send(200, "text/plain", "Modo alterado para: " + mode);
});

// Rota para obter o valor do potenciômetro
server.on("/getPotValue", HTTP_GET, []() {
  int potValue = analogRead(A0); // Exemplo de leitura de um potenciômetro
  int velocidade = map(potValue, 0, 1023, 0, 10); // Mapeando o valor do potenciômetro
  updateOLED();  // Atualiza o display com o novo valor de velocidade
  server.send(200, "text/plain", String(velocidade));  // Envia o valor da velocidade
});

// Rota para atualizar a velocidade
server.on("/setSpeed", HTTP_GET, []() {
  if (server.hasArg("value")) {
    velocidade = server.arg("value").toInt();  // Atualiza a variável de velocidade
    updateOLED();  // Chama a função de atualização do display
    server.send(200, "text/plain", "Velocidade atualizada para " + String(velocidade));
  } else {
    server.send(400, "text/plain", "Valor de velocidade não fornecido");
  }
});


  // Inicializa o servidor
  server.begin();
  Serial.println("Servidor iniciado");
}
void updateOLED() {
  String estado = autoScroll ? "Started" : "Paused";  // Estado de scroll
  String modo = isPotMode ? "Pot" : "Web";  // Atualiza o modo corretamente

  display.clearDisplay();
  
  // Exibe o IP
  display.setCursor(0, 0);
  display.print("IP: ");
  display.print(WiFi.softAPIP());
  
  // Exibe a velocidade
  display.setCursor(0, 16);
  display.print("Velocidade: ");
  display.print(velocidade);
  
  // Exibe o estado do scroll
  display.setCursor(0, 32);
  display.print("Estado: ");
  display.print(estado);

  // Exibe o modo atual
  display.setCursor(0, 48);
  display.print("Modo: ");
  display.print(modo);
  
  display.display();  // Atualiza a tela com as novas informações
}

void loop() {


  server.handleClient();

  
}