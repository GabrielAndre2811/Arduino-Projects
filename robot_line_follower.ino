/* Inclui a Biblioteca para controle da ponte H. */
#include <L298NX2.h>

#define CorLinha 1 /* Linha PRETA */
boolean detectou = 1;
boolean naodetectou = 0;

#define ENA 10  // ENA precisa estar em uma porta PWM
#define IN1 9
#define IN2 8
#define IN3 7
#define IN4 6
#define ENB 5  // ENB precisa estar em uma porta PWM

 /* Criamos um objeto de controle dos motores.           */
L298NX2 motores(ENA, IN1, IN2, ENB, IN3, IN4);

/* Define os pinos 2 e 3 para os sensores IR            */
#define Pino_Sensor_Direita 2
#define Pino_Sensor_Esquerda 3

//MOTOR A  = ESQUERDA
//MOTOR B =  DIREITA

void setup() {
  pinMode(Pino_Sensor_Direita, INPUT);
  pinMode(Pino_Sensor_Esquerda, INPUT);

  /* Inicia com os motores parados. */
  motores.stop();
}

void loop() {
  /* Sensores realizam a leitura                        */
  int Sensor_Direita = digitalRead(Pino_Sensor_Direita);
  int Sensor_Esquerda = digitalRead(Pino_Sensor_Esquerda);

  /* Nenhum sensor detectou a linha                     */
  if (Sensor_Esquerda == naodetectou && Sensor_Direita == naodetectou) {
 /* Define a velocidade dos motores */
    motores.setSpeed(120); 
/* Robô move para frente           */
    motores.forward(); 
  }

  /* Sensor da direita detectou a linha */
  if (Sensor_Esquerda == naodetectou && Sensor_Direita == detectou) {
/* Define a velocidade dos motores */
    motores.setSpeed(120); 
/* Robô vira à direita                */
    motores.backwardB();
    motores.forwardA();      
  }

/* Sensor da esquerda detectou a linha                      */
  if (Sensor_Esquerda == detectou && Sensor_Direita == naodetectou) {
/* Define a velocidade dos motores */
    motores.setSpeed(120); 
 /* Robô vira à esquerda               */
    motores.forwardB();
    motores.backwardA();
  }

  /* Os dois sensores detectaram a linha                      */
  if (Sensor_Esquerda == detectou && Sensor_Direita == detectou) {
/* Robô permanece parado                  */
    motores.stop(); 
  }
}
