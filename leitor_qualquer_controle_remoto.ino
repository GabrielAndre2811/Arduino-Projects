#include <IRremote.h>
#include <PushButton.h>

/* Define o pino do Arduino que receberá os dados do receptor IR. */
int Pino_receptor = 11;
int ledalerta = 6;
#define pinBot1 4
#define pinLedIr 3  // Pino do LED IR

PushButton botaoLeitura(pinBot1);

/* Cria um objeto IRsend para enviar sinais IR. */
IRsend irsend(pinLedIr);

  float armazenavalor;


/* Defina o código hexadecimal que você deseja enviar. */
unsigned long codigoHexadecimal = 0x57E3E817;  // tem que ser o que é lido ppelo sensor quando enviado pelo emissor
/* Variáveis para controle de debounce */
unsigned long lastReceiveTime = 0;
const unsigned long debounceDelay = 500;  // Tempo para debouncing em milissegundos

void setup()
{
    /* Inicializa a comunicação serial na velocidade 9600 bauds. */
    Serial.begin(9600);
    pinMode(ledalerta, OUTPUT);
    pinMode(pinLedIr, OUTPUT);
    
    /* Inicializa o receptor IR no pino definido. */
    IrReceiver.begin(Pino_receptor);
    
    /* Mostra, no monitor serial, as instruções a serem seguidas. */
    Serial.println("Pressione qualquer tecla do controle remoto.");
    Serial.println("Informações detalhadas serão exibidas no monitor serial.");
    Serial.println("");
}

void loop()
{
    digitalWrite(ledalerta, LOW);

   armazenavalor = IrReceiver.decodedIRData.decodedRawData;

    /* Atualiza o estado do botão. */
    botaoLeitura.button_loop();
    
    /* Verifica se um código IR foi recebido. */
    if (IrReceiver.decode())
    {
        unsigned long currentTime = millis();
        digitalWrite(ledalerta, HIGH);
        
        /* Verifica se o tempo decorrido desde a última leitura é maior que o tempo de debounce. */
        if (currentTime - lastReceiveTime > debounceDelay)
        {
            /* Atualiza o tempo da última leitura. */
            lastReceiveTime = currentTime;
            
            /* Exibe o código hexadecimal da tecla pressionada. */
            Serial.print("Código hexadecimal da tecla pressionada: 0x");
            Serial.println(IrReceiver.decodedIRData.decodedRawData, HEX);
            
            /* Exibe o tipo de protocolo IR usado. */
            Serial.print("Tipo de protocolo: ");
            Serial.println(IrReceiver.decodedIRData.protocol);
            
            /* Exibe o valor do comando. */
            Serial.print("Valor do comando: ");
            Serial.println(IrReceiver.decodedIRData.command, HEX);
            
            /* Exibe a quantidade de pulsos no sinal. */
            Serial.print("Quantidade de pulsos: ");
            Serial.println(IrReceiver.decodedIRData.rawlen);

            Serial.println("-----------------------------------------");
        }
        
        /* Lê o próximo valor. */
        IrReceiver.resume();
        
        /* Pequena pausa para evitar a sobrecarga do monitor serial. */
        delay(100);
    }

    /* Verifica se o botão foi pressionado e envia o sinal IR quando pressionado. */
    if (botaoLeitura.pressed()) {
        Serial.println("Botão pressionado. Enviando sinal IR...");
        irsend.sendNEC(codigoHexadecimal, 32); // Envia o código hexadecimal
        Serial.print("Sinal IR enviado: 0x");
        Serial.println(codigoHexadecimal, HEX);
        Serial.println("Se esse código for diferente do controle, substitua esse pelo do controle ");
        Serial.println("-----------------------------------------");
        delay(1000);  // Aguarda um segundo para evitar múltiplos envios rápidos
    }
  
}
