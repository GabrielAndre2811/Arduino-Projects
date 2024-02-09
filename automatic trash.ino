#include <Servo.h>

int distancia = 0;

////int posicaoInicial = 0;


long readUltrasonicDistance(int triggerPin, int echoPin)
{
  pinMode(triggerPin, OUTPUT);  // LIMPA TRIGGER
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  // ENVIO DO TRIGGER
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  pinMode(echoPin, INPUT);
  // LEITURA DO ECHO
  return pulseIn(echoPin, HIGH);
}

Servo servo_2;

//definindo uma posição inicial fora do loop para controle de algo se aproximou
// a posição inicial é definida como a distância medida na hora em que liga o sensor.
// foi colocado -2, porque a leitura do sensor varia 1cm e o motor girava sem mexer no sensor.
int posicaoInicial = 0.01723 * readUltrasonicDistance(13, 12) -2;


void setup()
{
  servo_2.attach(2);
  Serial.begin(9600);

}

void loop()
{
  

  distancia = 0.01723 * readUltrasonicDistance(13, 12);
  Serial.println(distancia);
  if (distancia < posicaoInicial) {
    // se a distancia medida for menor que a inicial, significa que algo se aproximou
    // então o motor gira em 90 graus.
    servo_2.write(90);
  } else {
    // se não for menor nada se aproximou, então o motor fica em 0 graus
    // se ja estiver em 90 graus volta para 0.
    servo_2.write(0);
  }
  delay(10); // DELAY POR CUSA DO DEBOUNCING