//--DISPLAY--
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Endereço I2C padrão para displays OLED (0x3C é o mais comum)
#define OLED_ADDR 0x3C

void setup() {
  // Inicializa o display com a alimentação e o endereço I2C
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println(F("Falha ao inicializar o display OLED"));
    while (1); // Se falhar, fica em loop infinito
  }

  // Limpa o display
  display.clearDisplay();

  // Configura o texto
  display.setTextSize(1);      // Tamanho do texto
  display.setTextColor(SSD1306_WHITE);  // Cor do texto
  display.setCursor(0, 0);     // Inicia no canto superior esquerdo

  // Exibe a mensagem no display OLED
  display.println(F("Hello, OLED!"));

  // Atualiza o display para mostrar o texto
  display.display();
}

void loop() {
  // O loop está vazio porque o teste só precisa ser feito uma vez no setup
}
