//-- Sensor ultrassonico
//Incluindo bibliotecas
#include "Ultrasonic.h"
#include "Servo.h"
#include "musica.h"

//Criando objeto ultrasonic e definindo as portas digitais

int trig = 12;
int echo = 13;
float distancia1 = 0;
float distancia2 = 0;
int duracao;
 int distanciaObstaculo = 30; //distância para o robô parar e recalcular o melhor caminho

Ultrasonic SensorUltrassonico(trig, echo);

long Microsegundos = 0;// Variável para armazenar o valor do tempo da reflexão do som refletido pelo objeto fornecido pela biblioteca do sensor
float DistanciaemCM = 0;// Variável para armazenar o valor da distância a ser convertido por uma função da própria bilbioteca do sensor
int TempoGirar = 200;//esse é o tempo para o robô girar em 45º com uma bateria de 9v.

//---Motores Ponte H
//motor direito
#define MotorDireito1 3
#define MotorDireito2 7
int velocidadedireito = 5;

//motoresquerdo
#define MotorEsquerdo1 8        
#define MotorEsquerdo2 9
int velocidadeesquerdo = 6;
//leds
#define led1 2
#define led2 4
int Pinservo = 11;

//buzzer
int buzzerPin = 10;

//objeto da classe servo
Servo servo;


void setup() {
  //Definições de entrada e saída
  pinMode(MotorDireito1, OUTPUT);
  pinMode(MotorDireito2, OUTPUT);
  pinMode(velocidadedireito, OUTPUT);
  pinMode(MotorEsquerdo1, OUTPUT);
  pinMode(MotorEsquerdo2, OUTPUT);
  pinMode(velocidadeesquerdo, OUTPUT);
  pinMode (trig, OUTPUT);
  pinMode (echo,INPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  randomSeed(analogRead(0));
  servo.attach(Pinservo);

  Serial.begin(9600);// Inicia a comunicação seria com velocidade de 9600 bits por segundo
   aleatorio();
  //inicia com um movimento e um som aleatório
 servo.write(120);
 delay(500);
  servo.write(60);
 delay(500);
 servo.write(90);
 aleatorio();
  delay(2000);// Vamos dar um tempo de 4 segundos aqui o suficiente para ligar o carrinho e colocar no chao
}

void loop() {
pensar();
}


 ////////////////////////////////////////////// COMEÇA AQUI AS FUNÇÕES ESPECIFICAS /////////////////////////////////////////////



 // Função para chamar outras funções e definir o que o robô fará  
  void pensar(){ 
   reposicionaServoSonar(); //Coloca o servo para olhar a frente    
   int distancia = lerSonar(); // Ler o sensor de distância  
   Serial.print("distancia em cm: "); 
   Serial.println(distancia);   // Exibe no serial monitor 
   if (distancia > distanciaObstaculo) {  // Se a distância for maior que 20 cm  
     frente(); //robô anda para frente  
     digitalWrite(led1, HIGH); //liga os leds
    digitalWrite(led2, HIGH); 
   }else{   
     freio();  //para o robô  
     movcabeca();
     paratras(); //da ré  
     pensar();    
   }   

  }



 void frente(){

       //Ande para frente
    
    // Motor lado esquerdo para frente
    digitalWrite(MotorEsquerdo1, HIGH);
    digitalWrite(MotorEsquerdo2, LOW);
    analogWrite(velocidadeesquerdo, 120);
    // Motor lado direito para frente
    digitalWrite(MotorDireito1, HIGH);
    digitalWrite(MotorDireito2, LOW);
    analogWrite(velocidadedireito, 120);
   

 }

void freio(){

    digitalWrite(MotorEsquerdo1, LOW);
    digitalWrite(MotorEsquerdo2, LOW);
    digitalWrite(MotorDireito1, LOW);
    digitalWrite(MotorDireito2, LOW);
}


void paratras(){

   //Andar para trás

    digitalWrite(MotorEsquerdo1, LOW);
    digitalWrite(MotorEsquerdo2, HIGH);
    analogWrite(velocidadedireito, 100);
    digitalWrite(MotorDireito1, LOW);
    digitalWrite(MotorDireito2, HIGH);
    analogWrite(velocidadedireito, 100);
    
    delay(800);// Vá para trás durante 1/2 segundo 
    freio();
    posicionaCarroMelhorCaminho(); //calcula o melhor caminho  
    
    
  
}
 void direita(){
 
  // Motor lado esquerdo para frente
    digitalWrite(MotorEsquerdo1, HIGH);
    digitalWrite(MotorEsquerdo2, LOW);
    analogWrite(velocidadeesquerdo, 100);
    // Motor lado direito para trás
    digitalWrite(MotorDireito1, LOW);
    digitalWrite(MotorDireito2, HIGH);
    analogWrite(velocidadedireito, 100);

    delay(200); //para o lado direito durante 200 milisegundos
   

 }
 void esquerda(){
 
   // Motor lado esquerdo para frente
    digitalWrite(MotorEsquerdo1, LOW);
    digitalWrite(MotorEsquerdo2, HIGH);
    analogWrite(velocidadeesquerdo, 100);
    // Motor lado direito para trás
    digitalWrite(MotorDireito1, HIGH);
    digitalWrite(MotorDireito2, LOW);
    analogWrite(velocidadedireito, 100);

    delay(200); //para o lado direito durante 200 milisegundos
   

 }


   // Função para ler e calcular a distância do sensor ultrassônico    
  int lerSonar(){    
   digitalWrite(trig, LOW); //não envia som
   delayMicroseconds(2);
   digitalWrite(trig,HIGH); //envia som 
   delayMicroseconds(10);
   digitalWrite(trig,LOW); //não envia o som e espera o retorno do som enviado
   duracao = pulseIn(echo,HIGH); //Captura a duração em tempo do retorno do som.
   DistanciaemCM = duracao/56; //Calcula a distância
   delay(30);  
   return DistanciaemCM;  // Retorna a distância  
  }   

  // Função para colocar o carrinho na melhor distância, isto é, girá-lo para a melhor distância    
  void posicionaCarroMelhorCaminho(){    
   char melhorDist = calculaMelhorDistancia();     
   Serial.print("melhor Distancia em cm: ");  
   Serial.println(melhorDist);  
   if (melhorDist == 'c'){    
     pensar();    
   }else if (melhorDist == 'd'){    
     direita();    
   }else if (melhorDist == 'e'){    
     esquerda();     
   }else{    
     paratras();    
   }    
   reposicionaServoSonar();    
  }   

  // Função para captar as distâncias lidas e calcular a melhor distância. Acesse: Seu Robô https://SeuRobo.com.br/    
  char calculaMelhorDistancia(){    
   int esquerda = calcularDistanciaEsquerda();    
   int centro = calcularDistanciaCentro();    
   int direita = calcularDistanciaDireita();    
   reposicionaServoSonar();    
   int maiorDistancia = 0;   
   char melhorDistancia = '0';     
     
   if (centro > direita && centro > esquerda){    
     melhorDistancia = 'c';    
     maiorDistancia = centro;    
   }else   
   if (direita > centro && direita > esquerda){    
     melhorDistancia = 'd';    
     maiorDistancia = direita;    
   }else  
   if (esquerda > centro && esquerda > direita){    
     melhorDistancia = 'e';    
     maiorDistancia = esquerda;    
   }    
   if (maiorDistancia <= distanciaObstaculo) { //distância limite para parar o robô   
     //paratras();    
     posicionaCarroMelhorCaminho();    
   }    
   reposicionaServoSonar();  
   return melhorDistancia;    
  }    
  // Função para deixar o sensor "olho" do robô no centro    
  void reposicionaServoSonar(){    
   servo.write(90);   
   delay(200);   
  }    

//FUNÇÃO DESATIVADA
 /* void radar(){ 

   for(int pos=15;pos<=165;pos+=50){
     //CARRO PARADO
    digitalWrite(MotorEsquerdo1, LOW);
    digitalWrite(MotorEsquerdo2, LOW);
    digitalWrite(MotorDireito1, LOW);
    digitalWrite(MotorDireito2, LOW);
   //SERVO MEXE COM A FUNÇÃO FOR
  servo.write(pos);

  //SENSOR ULTRASONICO SALVA INSTANCIA
  delay(260);
  digitalWrite(trig, LOW);
  delay(0005);
  digitalWrite(trig, HIGH);
  delay(0010);
  digitalWrite(trig, LOW);
  
   distancia1 = pulseIn (echo, HIGH);
   distancia1 = distancia1/38;
   }

   delay(2000); //espera 2 segundos

 for(int pos=165;pos>=15;pos-=50){
   //CARRO PARADO
    digitalWrite(MotorEsquerdo1, LOW);
    digitalWrite(MotorEsquerdo2, LOW);
    digitalWrite(MotorDireito1, LOW);
    digitalWrite(MotorDireito2, LOW);

   //SERVO MEXE COM A FUNÇÃO FOR
  servo.write(pos);

  //SENSOR ULTRASONICO SALVA INSTANCIA
  delay(260);
  digitalWrite(trig, LOW);
  delay(0005);
  digitalWrite(trig, HIGH);
  delay(0010);
  digitalWrite(trig, LOW);
  
   distancia2 = pulseIn (echo, HIGH);
   distancia2 = distancia2/38;
      
   }

   delay(2000);//espera mais 2 segundos

   servo.write(90); //volta a posição
   
    //COMPARAÇÃO DAS DISTANCIAS PARA TOMADA DE DECISÃO

   if(distancia1 > distancia2){
    //direita();
    // Motor lado esquerdo para frente
    digitalWrite(MotorEsquerdo1, HIGH);
    digitalWrite(MotorEsquerdo2, LOW);
    
    // Motor lado direito para trás
     digitalWrite(MotorDireito1, LOW);
     digitalWrite(MotorDireito2, HIGH);
    
   // delay(250); //para o lado direito durante 200 milisegundos
   } else {

     //esquerda();
      //Motor lado esquerdo para TRÁS
    digitalWrite(MotorEsquerdo1, LOW);
    digitalWrite(MotorEsquerdo2, HIGH);
    
    // Motor lado direito para frente
   digitalWrite(MotorDireito1, HIGH);
   digitalWrite(MotorDireito2, LOW);
    
    delay(250); //para o lado esquerdo durante 200 milisegundos

   }

 }*/

 // Função para calcular a distância do centro    
  int calcularDistanciaCentro(){    
   servo.write(90);    
   delay(20);   
   int leituraDoSonar = lerSonar();  // Ler sensor de distância  
   delay(500);   
   leituraDoSonar = lerSonar();   
   delay(500);   
   Serial.print("Distancia do Centro: "); // Exibe no serial  
   Serial.println(leituraDoSonar);   
   return leituraDoSonar;       // Retorna a distância  
  }    

   // Função para calcular a distância da direita    
  int calcularDistanciaDireita(){    
   servo.write(15);   
   delay(200);  
   int leituraDoSonar = lerSonar();   
   delay(500);   
   leituraDoSonar = lerSonar();   
   delay(500);   
   Serial.print("Distancia da Direita: ");  
   Serial.println(leituraDoSonar);   
   return leituraDoSonar;    
  }    
    
  // Função para calcular a distância da esquerda    
  int calcularDistanciaEsquerda(){    
   servo.write(165);   
   delay(200);  
   int leituraDoSonar = lerSonar();   
   delay(500);   
   leituraDoSonar = lerSonar();   
   delay(500);   
   Serial.print("Distancia Esquerda: ");  
   Serial.println(leituraDoSonar);   
   return leituraDoSonar;    
  }    

 void movcabeca(){
//movimento da cabeça quando ve um objeto e som do R2D2
 servo.write(140);
 delay(260);
 servo.write(40);
 delay(260);
 servo.write(140);
 delay(260);
 servo.write(40);
 delay(260);
 servo.write(140);
 delay(260);
 servo.write(90);
 delay(50);
 r2D2();
}


// Emite o Som Aleatório
void tone (int buzzerPin, float noteFrequency, long noteDuration)
{    
  int x;
  
  float microsecondsPerWave = 1000000/noteFrequency;
  float millisecondsPerCycle = 1000/(microsecondsPerWave * 2);
  float loopTime = noteDuration * millisecondsPerCycle;
  for (x=0;x<loopTime;x++)   
          {   
              digitalWrite(buzzerPin,HIGH); 
              digitalWrite(led2, LOW);
              delayMicroseconds(microsecondsPerWave); 
              digitalWrite(buzzerPin,LOW); 
              delayMicroseconds(microsecondsPerWave); 
          } 
}     

void r2D2(){ //SOM DE PARADA R2D2
          tone(buzzerPin, note_A7,100); //A 
          tone(buzzerPin, note_G7,100); //G 
          tone(buzzerPin, note_E7,100); //E 
          tone(buzzerPin, note_C7,100); //C
          tone(buzzerPin, note_D7,100); //D 
          tone(buzzerPin, note_B7,100); //B 
          tone(buzzerPin, note_F7,100); //F 
          tone(buzzerPin, note_C8,100); //C 
          tone(buzzerPin, note_A7,100); //A 
          tone(buzzerPin, note_G7,100); //G 
          tone(buzzerPin, note_E7,100); //E 
          tone(buzzerPin, note_C7,100); //C
          tone(buzzerPin, note_D7,100); //D 
          tone(buzzerPin, note_B7,100); //B 
          tone(buzzerPin, note_F7,100); //F 
          tone(buzzerPin, note_C8,100); //C 
          digitalWrite(led2, HIGH);
}

void phrase1() { //FRASE ALEATÓRIA 1
    
    int k = random(1000,2000);
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    for (int i = 0; i <=  random(100,2000); i++){
        
        tone(buzzerPin, k+(-i*2));          
        delay(random(.9,2));             
    } 
    digitalWrite(led1, LOW);   
    digitalWrite(led2, LOW); 
    for (int i = 0; i <= random(100,1000); i++){
        
        tone(buzzerPin, k + (i * 10));          
        delay(random(.9,2));             
    } 
}

void phrase2() { //FRASE ALEATÓRIA 2
    
    int k = random(1000,2000);
    digitalWrite(led1, HIGH);  
    digitalWrite(led2, HIGH); 
    for (int i = 0; i <= random(100,2000); i++){
        
        tone(buzzerPin, k+(i*2));          
        delay(random(.9,2));             
    } 
    digitalWrite(led1, LOW);   
    digitalWrite(led2, LOW);  
    for (int i = 0; i <= random(100,1000); i++){
        
        tone(buzzerPin, k + (-i * 10));          
        delay(random(.9,2));             
    } 
}


void aleatorio() { //FUNÇÃO PARA DECIDIR ALEATÓRIAMENTE O SOM AO LIGAR
int K = 2000;
    switch (random(1,7)) {
        
       case 1:phrase1(); break;
       case 2:phrase2(); break;
        //case 3:phrase1(); phrase2(); break;
        //case 4:phrase1(); phrase2(); phrase1();break;
        //case 5:phrase1(); phrase2(); phrase1(); phrase2(); phrase1();break;
        //case 6:phrase2(); phrase1(); phrase2(); break;
    }
    for (int i = 0; i <= random(3, 9); i++){
        
        digitalWrite(led1, HIGH);  
        digitalWrite(led2, HIGH);  
        tone(buzzerPin, K + random(-1700, 2000));          
        delay(random(70, 170));  
        digitalWrite(led1, LOW); 
        digitalWrite(led2, LOW);            
        noTone(buzzerPin);         
        delay(random(0, 30));             
    } 
    noTone(buzzerPin);         
    delay(random(2000, 4000));    


}