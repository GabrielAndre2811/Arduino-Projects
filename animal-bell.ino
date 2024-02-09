#include <Adafruit_Sensor.h> //Biblioteca basica do Adafruit
#include <Wire.h>                     // biblioteca requerida para rodar I2C
#include <SPI.h>                      // biblioteca requerida para rodar I2C
#include <Adafruit_GFX.h> // inclui biblioteca de gráficos Adafruit
#include <Adafruit_SSD1306.h> // inclui driver de exibição OLED Adafruit SSD1306
#include <DHT.h> // Biblioteca DHT Sensor Adafruit

DHT sensor(A0, DHT11); // Biblioteca DHT Sensor Adafruit

float T; //variavel que armazena a temperatura
float U; //variavel que armazena a umidade

#define rele 7

// configuração do display OLED
#define SCREEN_WIDTH 128             // OLED display largura, em pixels (128)
#define SCREEN_HEIGHT 64             // OLED display altura, em pixels (64)

#define OLED_RESET     -1            // pino de RESETE # (OU -1 se procurando Arduino resete pino)

// instancia o objeto display da biblioteca Adfruit
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET); 

void setup (){

  
   delay(1000); // espere um segundo
 
   display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // inicializa o display OLED SSD1306 com endereço I2C 0x3C
   display.clearDisplay(); // limpa o buffer de exibição.
 
   display.setTextSize(1); //Define tamanho do texto
   display.setTextColor(WHITE, BLACK); //Define as cores do texto
   display.setCursor(0, 0); //define a posição do texto
   display.print("CARREGANDO"); //texto
  //ponto
   display.setTextSize(1); //tamanho do texto "ponto"
   display.setTextColor(WHITE, BLACK);//cor do texto "ponto"
   display.setCursor(70, 0);//posição do texto "ponto"
   display.print("."); //texto "ponto"
  //ponto
   display.setTextSize(1);
   display.setTextColor(WHITE, BLACK);
   display.setCursor(80, 0);
   display.print(".");
   //ponto
   display.setTextSize(1);
   display.setTextColor(WHITE, BLACK);
   display.setCursor(90, 0);
   display.print(".");

   display.display();
   delay(2000); // espere um segundo

  Serial.begin(9600);

  sensor.begin(); //inicia sensor de tewmperatura e pressão

  pinMode(rele,OUTPUT); //define o rele como saida
}

void loop(){  
  /* Limpa o display ao iniciar. */
   display.clearDisplay();
 
  T = float(sensor.readTemperature());
  U = float(sensor.readHumidity());

  delay(200);

  Serial.println("Temperatura:  ");
  Serial.print(T); //IMPRIME NA SERIAL O VALOR DE UMIDADE MEDIDO
  Serial.println("C°");
  Serial.println("Umidade:  ");
  Serial.print(U); //IMPRIME NA SERIAL O VALOR DE UMIDADE MEDIDO
  Serial.println("%");
 
 

  if(T <= 28){ //se T for menor que 28 graus desliga o rele
   
    digitalWrite(rele, LOW);
  }

  else { //se nao liga
    digitalWrite(rele, HIGH);
  }   

  //LIMPA O DISPLAY
  display.clearDisplay();
  // Escreve texto preto com fundo branco
  display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
  // Posição para escrever mensagem
  display.setCursor(0,0);
    // Aumenta tamanho da letra 1x
  display.setTextSize(1);
  display.print(F("Temperatura:"));
   
  // Aumenta tamanho da letra 2x
  display.setTextSize(2);
  // Escreve texto branco
  display.setTextColor(WHITE);
  // Posição para escrever temperatura
  display.setCursor(0,12);
  display.println(T); //escreve a variavel T que contem o valor da temperatura
  display.setCursor(70,12);
  display.print("C");
  display.display();

 // Escreve texto preto com fundo branco
  display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
  // Posição para escrever mensagem
  display.setCursor(0,30);
    // Aumenta tamanho da letra 1x
  display.setTextSize(1);
  display.print(F("Umidade:"));
 // Aumenta tamanho da letra 2x
  display.setTextSize(2);
  // Escreve texto branco
  display.setTextColor(SSD1306_WHITE);
  // Posição para escrever temperatura
  display.setCursor(0,42);
  display.println(U); //escreve a variavel U que contem o valor da umidade
  display.setCursor(70,42);
  display.print("%");
  display.display();
 
    }