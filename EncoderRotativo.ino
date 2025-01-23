#define CLK 2
#define DT 3

int contador = 0;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 10; // Ajuste o tempo de debounce conforme necessário

void setup() {
  pinMode(CLK, INPUT_PULLUP);
  pinMode(DT, INPUT_PULLUP);
  Serial.begin(9600);

  // Interrupção para detectar mudança no CLK
  attachInterrupt(digitalPinToInterrupt(CLK), rotacao, CHANGE);
}

void loop() {
  // Aqui você pode realizar outras tarefas enquanto a interrupção gerencia a rotação
}

void rotacao() {
  unsigned long currentTime = millis();

  // Verifica se o tempo de debounce já passou
  if ((currentTime - lastDebounceTime) > debounceDelay) {
    int clkAtual = digitalRead(CLK);
    int dtEstado = digitalRead(DT);
    
    if (dtEstado != clkAtual) {
      contador++; // Sentido anti-horário
    } else {
      contador--; // Sentido horário
    }

    Serial.println(contador);
    
    // Atualiza o tempo do último debounce
    lastDebounceTime = currentTime;
  }
}
