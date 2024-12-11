#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SD.h>

// Defina as configurações do display OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
#define OLED_ADDR     0x3C  // Endereço I2C do display OLED (0x3C é o mais comum)

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Defina o pino do CS para o cartão SD
#define CS_PIN D8  // D8 no NodeMCU (ajuste conforme necessário)

// Pino do potenciômetro
#define POT_PIN A0

// Variáveis para navegação
int currentPage = 0;
int linesPerPage = 5;  // Total de linhas que podem ser exibidas (1 linha para a página + 4 linhas de conteúdo)
int contentLines = 4;  // Número de linhas para exibir o conteúdo do arquivo
int totalLines = 0;
File file;

void setup() {
  Serial.begin(115200);

  // Inicializa o display OLED com a alimentação e o endereço I2C
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println(F("Falha ao inicializar o display OLED"));
    while (1); // Se falhar, fica em loop infinito
  }

  // Limpa o display
  display.clearDisplay();

  // Inicializa o cartão SD
  if (!SD.begin(CS_PIN)) {
    Serial.println("Erro ao inicializar o cartão SD!");
    return;
  }
  Serial.println("Cartão SD inicializado com sucesso!");

  // Abre o arquivo de texto
  file = SD.open("ARQUIVO.txt");
  if (!file) {
    Serial.println("Erro ao abrir o arquivo!");
    while (1);
  }

  // Conta o número de linhas no arquivo
  totalLines = 0;
  while (file.available()) {
    String line = file.readStringUntil('\n');
    totalLines++;
  }
  file.seek(0);  // Reseta o ponteiro do arquivo para o início
}

void loop() {
  // Lê o valor do potenciômetro
  int potValue = analogRead(POT_PIN);
  Serial.print("Potenciômetro: ");
  Serial.println(potValue);

  // Mapeia o valor do potenciômetro para selecionar a página (mapeando para páginas disponíveis)
  int newPage = map(potValue, 0, 1023, 0, (totalLines / contentLines));  // Divide o total de linhas em páginas
  if (newPage != currentPage) {
    currentPage = newPage;

    // Exibe a página no display OLED
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);

    // Exibe a primeira linha de paginação (ex: "Página 1")
    display.print("Pagina ");
    display.println(currentPage + 1);  // Exibe "Pagina 1", "Pagina 2", etc.

    // Pula uma linha antes de exibir o conteúdo
    display.setCursor(0, 10);  // Muda para a segunda linha para começar a exibir o conteúdo

    // Reposiciona o ponteiro do arquivo para a linha da página
    int startLine = currentPage * contentLines;
    file.seek(0);
    for (int i = 0; i < startLine; i++) {
      file.readStringUntil('\n');  // Ignora as linhas até chegar à linha da página atual
    }

    // Exibe as linhas de conteúdo da página no display (a partir da linha 2 até a 5)
    for (int i = 0; i < contentLines && file.available(); i++) {
      String line = file.readStringUntil('\n');
      display.setCursor(0, (i + 2) * 10);  // Posiciona a linha a partir da segunda linha do display
      display.println(line);  // Exibe a linha no display
    }

    display.display();
  }

  delay(100);  // Delay para evitar leitura rápida demais
}
