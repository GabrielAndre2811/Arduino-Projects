/* Inclui as bibliotecas para o controle do display */
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <Ultrasonic.h>

Adafruit_SSD1306 display(128, 64, &Wire); /* Cria o objeto de controle para o display OLED.  */

/* Define os pinos do sensor ultrassônico.   */
#define pino_trigger A1
#define pino_echo A0

Ultrasonic ultrasonic(pino_trigger, pino_echo); /* Cria o objeto de controle para o untrassônico.  */

float medida; //variavel que armazena a medida

////////////////////////////////CONFIGURAÇÕES////////////////////////////////////////////////

void setup() {
  /* Inicia o display OLED, através de seu endereço físico 0X3C.   */
display.begin(SSD1306_SWITCHCAPVCC, 0X3C);
}        

//////////////////////////////////FUNÇÃO PRINCIPAL/////////////////////////////////////////////
void loop() {
  
medida = ultrasonic.read(); /* Faz a leitura da distância e armazena na variável medida. */

estrutura();
mostrarCM();

if (medida >= 100){ /* Se maior que 100 ele repete toda a estrutura mas o valor é dividido por 100 e a unidade de medida é metro                */
estrutura();
mostrarM(); 
}

display.display(); /* Atualiza as informações do display.           */
delay(100); /* Pequeno atraso para a leitura da medida.      */
}

///////////////////////////////////FUNÇÃO///IMAGENS DE ENFEITE///////////////////////////////////////////////////////////////////////

void estrutura(){ //estrutura com circulo e escritas

display.clearDisplay(); /* Limpa o Display  */

display.setCursor(8, 0); /* Posiciona o cursor na coordenada informada.   */
display.setTextSize(1);  /* Define o tamanho da fonte: 1                */ 
display.setTextColor(WHITE); /* Define a cor da fonte: Claro (WHITE) - Escuro (BLACK).*/
display.print("-- TRENA DIGITAL --"); /* Imprime a palavra -- TRENA DIGITAL --.        */

/* Desenha um retângulo sem preenchimento, com origem na coordenada*/ /* (0, 12), com dimensões 128x40 pixels, raios dos cantos arredondados */
/* 10 pixels na cor clara. */
display.drawRoundRect(0, 12, 128, 40, 10, WHITE);

  
display.setCursor(20, 55); /* Posiciona o cursor na coordenada informada.   */ 
display.print("Rob"); /* Imprime a palavra Rob.                        */
display.write(162);  /* Imprime a letra ó.                            */
display.print("tica Paran"); /* Imprime a palavra tica.                       */
display.write(160); /* Imprime a letra á.                            */

}

//////////////////////////////////////FUNÇÃO//QUE///MOSTRA///EM///CENTIMETROS///////////////////////////////////////////////////////////////////////

void mostrarCM(){
display.setTextSize(2);  /* Define o tamanho da fonte: 2.                 */ 
display.setCursor(25, 25);  /* Posiciona o cursor na coordenada informada. */  
display.print(medida); /* Imprime a medida.                           */   
display.setCursor(90, 25); /* Posiciona o cursor na coordenada informada. */
display.print("cm");  /* Imprime a unidade de medida.                */
}

////////////////////////////////////FUNÇÃO//QUE///MOSTRA///EM///METROS///////////////////////////////////////////////////////////////////////

void mostrarM(){
display.setTextSize(2); /* Define o tamanho da fonte: 2.                 */
display.setCursor(25, 25);  /* Posiciona o cursor na coordenada informada. */   
display.print(medida/100); /* Imprime a medida formatando em metro.                           */ 
display.setCursor(90, 25); /* Posiciona o cursor na coordenada informada. */
display.print("m");  /* Imprime a unidade de medida.                */
}