/* Programa: Utilizando Joystick Shield */
/* Inclui a biblioteca de controle do servo */
#include <Servo.h>
/* Cria os objetos para controlar os servos */
Servo servo1;
Servo servo2;
/* Define os pinos de dados dos servos */
int Servo1 = A2;
int Servo2 = A3;
/* Define os pinos dos botões */
int Botao_A = 2;
int Botao_B = 3;
int Botao_C = 4;
int Botao_D = 5;
int Botao_E = 6;
int Botao_F = 7;
int Botao_K = 8;
/* Define os pinos dos LEDs C ao F */
int LED_C = 9;
int LED_D = 10;
int LED_E = 11;
int LED_F = 12;
/* Define o pino do buzzer */
int Buzzer = 13;
/* Define os pinos dos potenciômetros */
int Pot_X = A0;
int Pot_Y = A1;
/* Define os pinos dos LEDs A e B. Usaremos essas portas analógicas para
simular portas digitais devido à limitação de pinos deste projeto. */
int LED_A = A4;
int LED_B = A5;
void setup() {
/* Endereça os objetos servos aos pinos de dados definidos */
servo1.attach(Servo1);
servo2.attach(Servo2);
/* Loop para definir os pinos 2 ao 8 como entradas e ativa os resistores
internos do Arduino */
for (int i = 2; i <= 8; i++) {
 pinMode(i, INPUT_PULLUP);
}
/* Loop para definir os pinos 9 ao 13 como saídas */
for (int i = 9; i <= 13; i++) {
 pinMode(i, OUTPUT);
}
/* Define os pinos analógicos do LED_A e LED_B como saída digital */
pinMode(LED_A, OUTPUT);
pinMode(LED_B, OUTPUT);
}
void loop() {
/* Comandos para posicionar os servos de acordo com as leituras dos dados
dos potenciômetros */
servo1.write(map(analogRead(Pot_X), 0, 1023, 0, 180));
servo2.write(map(analogRead(Pot_Y), 0, 1023, 0, 180));
/* Enquanto o Botão_A estiver pressionado, mantenha o LED_A ligado. */
while (!digitalRead(Botao_A)) {
 digitalWrite(LED_A, HIGH);
}
/* Enquanto o Botão_B estiver pressionado, mantenha o LED_B ligado. */
while (!digitalRead(Botao_B)) {
 digitalWrite(LED_B, HIGH);
}
/* Enquanto o Botão_C estiver pressionado, mantenha o LED_C ligado. */
while (!digitalRead(Botao_C)) {
 digitalWrite(LED_C, HIGH);
}
/* Enquanto o Botão_D estiver pressionado, mantenha o LED_D ligado. */
while (!digitalRead(Botao_D)) {
 digitalWrite(LED_D, HIGH);
}
/* Enquanto o Botão_E estiver pressionado, mantenha o LED_E ligado. */
while (!digitalRead(Botao_E)) {
 digitalWrite(LED_E, HIGH);
}
/* Enquanto o Botão_F estiver pressionado, mantenha o LED_F ligado. */
while (!digitalRead(Botao_F)) {
 digitalWrite(LED_F, HIGH);
}
/* Enquanto o Botão_K estiver pressionado, emita som através do Buzzer. */
while (!digitalRead(Botao_K)) {
 tone(Buzzer, 800);
}
/* Se nenhum botão estiver sendo pressionado, chame a função para desligar
todos os LEDs e desligue o Buzzer. */
Off_LEDs();
noTone(Buzzer);
}
/* Função responsável por desligar todos os LEDs. */
void Off_LEDs() {
for (int i = 9; i <= 12; i++) {
 digitalWrite(i, LOW);
}
digitalWrite(LED_A, LOW);
digitalWrite(LED_B, LOW);
}