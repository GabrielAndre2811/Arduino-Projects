#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Dados da rede Wi-Fi (Access Point)
const char* ssid = "Quizz";
const char* password = "12345678";

#define MAX_JOGADORES 10
String jogadores[MAX_JOGADORES];
int numJogadores = 0;
int jogadorAtual = 0;


// Servidor
ESP8266WebServer server(80);

// Estrutura da pergunta
struct Pergunta {
  String pergunta;
  String opcoes[4];
  int respostaCorreta;
  String punicao;
};

// Perguntas (adicione mais se quiser)
Pergunta perguntas[] = {
  {"Qual músculo é o maior do corpo humano?", {"Glúteo máximo", "Peitoral", "Quadríceps", "Trapézio"}, 0, "Flexione o braço e mostre o músculo!"},
  {"Qual é o animal terrestre mais rápido do mundo?", {"Leão", "Guepardo", "Tigre", "Águia"}, 1, "Faça uma corrida no lugar por 5 segundos!"},
  {"Qual é a principal função do sistema circulatório?", {"Respirar", "Bombear sangue", "Digestionar", "Produzir hormônios"}, 1, "Imite o som de um coração batendo forte!"},
  {"Qual carro é conhecido como 'O Carrão do Povo'?", {"Fusca", "Corolla", "Gol", "Civic"}, 0, "Imite o ronco de um motor potente!"},
  {"Quem ganhou a Copa do Mundo de 2018?", {"Alemanha", "Brasil", "França", "Argentina"}, 2, "Faça a comemoração de um gol famoso!"},
  
  {"Qual é o componente responsável pela potência do motor?", {"Radiador", "Cilindro", "Injeção eletrônica", "Turbo"}, 3, "Faça barulho de turbo acionado!"},
  {"Quantos ossos tem um corpo humano adulto?", {"206", "210", "201", "215"}, 0, "Bata um osso imaginário com força!"},
  {"Quem é o maior artilheiro da história da Seleção Brasileira?", {"Pelé", "Neymar", "Ronaldo Fenômeno", "Romário"}, 0, "Imite a comemoração do Pelé!"},
  {"Qual é a velocidade máxima aproximada de um guepardo?", {"90 km/h", "110 km/h", "120 km/h", "140 km/h"}, 3, "Corra no lugar o mais rápido que puder!"},
  {"Qual é a função do óleo no motor do carro?", {"Lubrificar", "Refrigerar", "Aumentar potência", "Limpar"}, 0, "Imite o som de uma troca de óleo!"},
  
  {"Qual parte do corpo humano produz insulina?", {"Fígado", "Pâncreas", "Rim", "Coração"}, 1, "Mostre uma expressão de dor rápida!"},
  {"Qual time tem mais títulos da Champions League?", {"Real Madrid", "Barcelona", "Bayern de Munique", "Liverpool"}, 0, "Imite a comemoração de torcida!"},
  {"Qual animal tem a mordida mais forte?", {"Leão", "Jacaré", "Tubarão Branco", "Hipopótamo"}, 2, "Faça um rugido aterrorizante!"},
  {"Qual combustível é mais eficiente para carros esportivos?", {"Gasolina comum", "Etanol", "Gasolina aditivada", "Diesel"}, 2, "Imite o ronco de um carro esportivo!"},
  {"Qual é o maior órgão interno do corpo humano?", {"Fígado", "Coração", "Pulmão", "Rim"}, 0, "Faça um movimento imitando a respiração!"},
  
  {"Quem foi o técnico da seleção brasileira na Copa de 2002?", {"Felipão", "Zagallo", "Tite", "Parreira"}, 3, "Dê uma bronca engraçada!"},
  {"Qual animal é símbolo de força e resistência?", {"Elefante", "Touro", "Urso", "Gorila"}, 1, "Faça uma pose de força!"},
  {"Quantos litros de sangue um adulto tem em média?", {"3-4 litros", "5-6 litros", "7-8 litros", "9-10 litros"}, 1, "Bata no peito mostrando coragem!"},
  {"Qual carro é famoso por ser o mais vendido no mundo?", {"Ford F-150", "Toyota Corolla", "Volkswagen Golf", "Honda Civic"}, 1, "Faça som de motor e acelere!"},
  {"Qual jogador é conhecido como 'Fenômeno'?", {"Ronaldo Nazário", "Romário", "Pelé", "Zico"}, 0, "Imite uma jogada de gol!"},
  
  {"Qual a principal função dos pulmões?", {"Bombear sangue", "Filtrar toxinas", "Trocar gases", "Produzir células"}, 2, "Faça uma respiração profunda!"},
  {"Quem é o maior recordista de gols na NBA?", {"Michael Jordan", "Kobe Bryant", "LeBron James", "Kareem Abdul-Jabbar"}, 3, "Imite uma enterrada!"},
  {"Qual animal é conhecido por ter a memória mais forte?", {"Elefante", "Golfinho", "Papagaio", "Cachorro"}, 0, "Imite um elefante caminhando!"},
  {"Qual parte do carro é responsável pela transmissão da força?", {"Freio", "Embreagem", "Direção", "Radiador"}, 1, "Faça som de câmbio engatando!"},
  {"Qual é o país com mais títulos da Copa América?", {"Argentina", "Brasil", "Uruguai", "Chile"}, 2, "Comemore com o grito do seu time!"},
  
  {"Qual é o nome do osso do braço?", {"Fêmur", "Úmero", "Tíbia", "Cúbito"}, 1, "Mostre um golpe de karatê!"},
  {"Qual jogador brasileiro tem mais títulos da Libertadores?", {"Palmeiras", "Santos", "Grêmio", "São Paulo"}, 0, "Imite um drible famoso!"},
  {"Qual combustível utiliza maior energia por litro?", {"Gasolina", "Etanol", "Diesel", "Gás natural"}, 2, "Faça barulho de motor pesado!"},
  {"Qual animal é símbolo de resistência e paciência?", {"Tartaruga", "Cavalo", "Cachorro", "Águia"}, 0, "Imite a caminhada lenta de uma tartaruga!"},
  {"Qual parte do corpo é responsável pelo equilíbrio?", {"Cerebelo", "Coração", "Pulmão", "Fígado"}, 0, "Tente andar em linha reta com os olhos fechados!"}
};


const int total = sizeof(perguntas) / sizeof(Pergunta);
int atual = 0;

const int NUM_PERGUNTAS = sizeof(perguntas) / sizeof(perguntas[0]);  // total de perguntas


int perguntasFeitas[NUM_PERGUNTAS];  // armazena índices de perguntas já feitas
int numPerguntasFeitas = 0;

  int getPerguntaAleatoriaNaoRepetida() {
  if (numPerguntasFeitas >= NUM_PERGUNTAS) {
    // Já fez todas as perguntas, reinicia para poder repetir
    numPerguntasFeitas = 0;
  }
  
  int indice;
  bool repetida;
  do {
    indice = random(NUM_PERGUNTAS);  // função random do Arduino para 0 a NUM_PERGUNTAS-1
    repetida = false;
    for (int i = 0; i < numPerguntasFeitas; i++) {
      if (perguntasFeitas[i] == indice) {
        repetida = true;
        break;
      }
    }
  } while (repetida);
  
  perguntasFeitas[numPerguntasFeitas++] = indice;
  return indice;
}

void setup() {
  Serial.begin(115200);
  randomSeed(analogRead(A0));

  // Inicia display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("Erro no display");
    while (true);
  }

  // Cria rede Wi-Fi
  WiFi.softAP(ssid, password);
  delay(1000);
  IPAddress ip = WiFi.softAPIP();

  // Exibe dados no display
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.setTextColor(SSD1306_WHITE);
  display.println("Quizzes");
  display.println("--------------------");
  display.println("SSID: " + String(ssid));
  display.println("SENHA: " + String(password));
  display.display();

  // Exibe IP destacado
  display.setTextSize(1);
  display.setCursor(10, 48); // centraliza aproximadamente
  display.println(ip.toString());
  display.display();




server.on("/", []() {
  String html = "<!DOCTYPE html><html><head><meta charset='UTF-8'>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  html += "<title>Cadastro de Jogadores</title>";
  html += "<style>";
  html += "body { font-family: 'Segoe UI', sans-serif; background: #f7f9fc; margin: 0; padding: 0; }";
  html += ".container { max-width: 500px; margin: 30px auto; background: white; padding: 20px; border-radius: 12px; box-shadow: 0 4px 10px rgba(0,0,0,0.1); }";
  html += "h2 { text-align: center; color: #333; }";
  html += "form { display: flex; justify-content: center; margin-bottom: 20px; }";
  html += "input { flex: 1; padding: 10px; font-size: 16px; border-radius: 8px; border: 1px solid #ccc; }";
  html += "button { margin-left: 10px; padding: 10px 20px; font-size: 16px; background: #28a745; color: white; border: none; border-radius: 8px; cursor: pointer; }";
  html += "button:hover { background: #218838; }";
  html += "ul { list-style: none; padding: 0; }";
  html += "li { background: #f1f1f1; margin: 8px 0; padding: 10px 14px; border-radius: 8px; display: flex; justify-content: space-between; align-items: center; }";
  html += ".remove { background: #dc3545; border: none; color: white; padding: 6px 10px; border-radius: 6px; cursor: pointer; }";
  html += ".remove:hover { background: #c82333; }";
  html += ".start { display: block; margin: 20px auto; padding: 12px 20px; font-size: 16px; background: #007bff; color: white; border: none; border-radius: 8px; text-decoration: none; text-align: center; }";
  html += ".start:hover { background: #0069d9; }";
  html += "</style></head><body>";
  
  html += "<div class='container'>";
  html += "<h2>👥 Cadastro de Jogadores</h2>";
  html += "<form action='/add' method='get'>";
  html += "<input type='text' name='nome' placeholder='Digite o nome' required />";
  html += "<button type='submit'>➕</button>";
  html += "</form>";

  html += "<ul>";
  for (int i = 0; i < numJogadores; i++) {
    html += "<li>" + jogadores[i] + "<a href='/remove?index=" + String(i) + "'><button class='remove'>Remover</button></a></li>";
  }
  html += "</ul>";

  if (numJogadores > 0) {
    html += "<a href='/jogar' class='start'>🎮 Iniciar Jogo</a>";
  }

  html += "</div></body></html>";
  server.send(200, "text/html", html);
});



server.on("/add", []() {
  if (server.hasArg("nome") && numJogadores < MAX_JOGADORES) {
    jogadores[numJogadores++] = server.arg("nome");
  }
  server.sendHeader("Location", "/");
  server.send(303);
});

server.on("/remove", []() {
  if (server.hasArg("index")) {
    int index = server.arg("index").toInt();
    if (index >= 0 && index < numJogadores) {
      // Remove o jogador do array, movendo os demais para preencher o espaço
      for (int i = index; i < numJogadores - 1; i++) {
        jogadores[i] = jogadores[i + 1];
      }
      numJogadores--;
    }
  }
  server.sendHeader("Location", "/");
  server.send(303); // Redireciona para a tela principal após remoção
});


 server.on("/jogar", []() {
  if (numJogadores == 0) {
    server.sendHeader("Location", "/");
    server.send(303);
    return;
  }



  String html = "<!DOCTYPE html><html><head><meta charset='UTF-8'>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  html += "<title>Roleta dos Jogadores</title>";
  html += "<style>";
  html += "body { font-family: sans-serif; display: flex; flex-direction: column; align-items: center; justify-content: center; background: #f9f9f9; height: 100vh; margin: 0; }";
  html += "#canvas { border: 6px solid #333; border-radius: 50%; background: white; }";
  html += "#start { margin-top: 20px; padding: 12px 24px; font-size: 18px; background: #007bff; color: white; border: none; border-radius: 8px; cursor: pointer; }";
  html += "#start:hover { background: #0056b3; }";
  html += "</style></head><body>";
  html += "<a href='/' style='position: absolute; top: 20px; left: 20px; text-decoration: none; font-size: 28px;'>🏠</a>";
  html += "<canvas id='canvas' width='300' height='300'></canvas>";
  html += "<button id='start'>🎯 Girar Roleta</button>";

  // SCRIPT DA ROLETA
  html += "<script>";
  html += "const nomes = [";
  for (int i = 0; i < numJogadores; i++) {
    html += "'" + jogadores[i] + "'";
    if (i < numJogadores - 1) html += ",";
  }
  html += "];";

  html += R"(
const canvas = document.getElementById('canvas');
const ctx = canvas.getContext('2d');
const startBtn = document.getElementById('start');
const total = nomes.length;
const angle = 2 * Math.PI / total;
let startAngle = 0;
let spinning = false;

function drawWheel() {
  ctx.clearRect(0, 0, canvas.width, canvas.height);

  for (let i = 0; i < total; i++) {
    const ang = startAngle + i * angle;
    ctx.beginPath();
    ctx.fillStyle = i % 2 === 0 ? '#ffcc00' : '#ff6666';
    ctx.moveTo(150, 150);
    ctx.arc(150, 150, 140, ang, ang + angle);
    ctx.lineTo(150, 150);
    ctx.fill();

    ctx.save();
    ctx.translate(150, 150);
    ctx.rotate(ang + angle / 2);
    ctx.textAlign = "right";
    ctx.fillStyle = "#000";
    ctx.font = "bold 14px sans-serif";
    ctx.fillText(nomes[i], 130, 5);
    ctx.restore();
  }

  

ctx.beginPath();
ctx.fillStyle = "#000";
ctx.moveTo(150, 10); // ponta da seta (centro inferior)
ctx.lineTo(140, -10);
ctx.lineTo(160, -10);
ctx.fill();

}

let spinAngle = 0;
let spinVelocity = 0;

function spin() {
  if (spinning) return;
  spinning = true;
  spinVelocity = Math.random() * 0.3 + 0.3;

  const spinInterval = setInterval(() => {
    spinAngle += spinVelocity;
    spinVelocity *= 0.97;
    startAngle = spinAngle;

    drawWheel();

    if (spinVelocity < 0.002) {
      clearInterval(spinInterval);
      
      // Ângulo da seta no topo (em radianos = 3 * PI / 2)
      const pointerAngle = (spinAngle + Math.PI / 2) % (2 * Math.PI); // seta embaixo (90°)
      const winnerIndex = Math.floor((total - (pointerAngle / angle)) % total);
      const jogador = nomes[winnerIndex];
      window.location.href = "/pergunta?j=" + encodeURIComponent(jogador);
    }
  }, 20);
}

drawWheel();
startBtn.onclick = spin;
)";


  html += "</script></body></html>";
  server.send(200, "text/html", html);
});


server.on("/pergunta", []() {
  if (!server.hasArg("j")) {
    server.sendHeader("Location", "/");
    server.send(303);
    return;
  }

  String nomeJogador = server.arg("j");
  int indicePergunta = getPerguntaAleatoriaNaoRepetida();
  Pergunta p = perguntas[indicePergunta];


  String html = "<!DOCTYPE html><html><head><meta charset='UTF-8'>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  html += "<title>Pergunta</title>";
  html += "<style>";
  html += "body { font-family: sans-serif; background: #e6f7ff; text-align: center; padding: 20px; }";
  html += ".jogador { font-size: 20px; margin-bottom: 10px; color: #444; }";
  html += "h2 { color: #222; font-size: 24px; }";
  html += "button { width: 90%; max-width: 300px; padding: 15px; margin: 12px auto; font-size: 18px; border-radius: 10px; border: none; background-color: #4CAF50; color: white; display: block; }";
  html += "button:hover { background-color: #45a049; }";
  html += "</style></head><body>";

  html += "<div class='jogador'>👤 Jogador: <strong>" + nomeJogador + "</strong></div>";
  html += "<h2>&#127919; " + p.pergunta + "</h2>";
  for (int i = 0; i < 4; i++) {
   html += "<a href='/responder?resp=" + String(i) + "&j=" + nomeJogador + "&p=" + String(indicePergunta) + "'><button>" +
        String(char('A' + i)) + ") " + p.opcoes[i] + "</button></a>";
  }

  html += "</body></html>";
  server.send(200, "text/html", html);
});



server.on("/responder", []() {
  if (server.hasArg("resp") && server.hasArg("p")) {
    int resp = server.arg("resp").toInt();
    int indicePergunta = server.arg("p").toInt();
    Pergunta p = perguntas[indicePergunta]; 
    String nomeJogador = server.arg("j");


    String html = "<!DOCTYPE html><html><head><meta charset='UTF-8'>";
    html += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
    html += "<title>Resultado</title>";
    html += "<style>";
    html += "body { font-family: sans-serif; background: #e8f5e9; text-align: center; padding: 20px; }";
    html += "h2 { font-size: 26px; color: #222; }";
    html += "p { font-size: 18px; color: #444; }";
    html += "button { padding: 15px 25px; font-size: 18px; background: #2196F3; color: white; border: none; border-radius: 10px; margin-top: 20px; }";
    html += "button:hover { background: #0b7dda; }";
    html += "</style></head><body>";

    html += "<h2>👤 " + nomeJogador + "</h2>";

    if (resp == p.respostaCorreta) {
      html += "<h2>✅ Acertou!</h2>";
      html += "<p>Mandou bem na pergunta: <strong>" + p.pergunta + "</strong></p>";
    } else {
      html += "<h2>❌ Errou!</h2>";
      html += "<p>A resposta correta era: <strong>" + p.opcoes[p.respostaCorreta] + "</strong></p>";
      html += "<p><strong>Punição para " + nomeJogador + ":</strong><br><em>" + p.punicao + "</em></p>";
    }

    html += "<a href='/jogar'><button>➡️ Próxima Rodada</button></a>";
    html += "</body></html>";

    server.send(200, "text/html", html);

    atual++;
    if (atual >= total) atual = 0;
  }
});


  server.begin();
}

void loop() {
  server.handleClient();
}
