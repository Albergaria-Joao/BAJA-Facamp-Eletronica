#define HALL_PIN 2

volatile unsigned long lastPulseMicros = 0;
volatile float rpmInstant = 0;
volatile unsigned long pulseCount = 0;

float rpmFiltered = 0;
const float alpha = 0.2;
const float decayRate = 50; // RPM por segundo

unsigned long lastUpdateMillis = 0;

void setup() {
  Serial.begin(9600);
  pinMode(HALL_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(HALL_PIN), pulseDetected, RISING);
}

void loop() {
  unsigned long currentMillis = millis();

  // Criar cópia atômica das variáveis usadas na ISR
  noInterrupts();
  float rpmCopy = rpmInstant;
  unsigned long lastPulseCopy = lastPulseMicros;
  unsigned long pulseCountCopy = pulseCount;
  interrupts();

  // Atualiza filtro exponencial
  rpmFiltered = (alpha * rpmCopy) + ((1 - alpha) * rpmFiltered);

  // Se não teve pulso recente, aplica decaimento
  // Considera que 100 ms sem pulso já é inatividade
  if ((currentMillis - (lastPulseCopy / 1000)) > 100) {
    // tempo decorrido desde a última atualização do filtro
    float deltaTime = (currentMillis - lastUpdateMillis) / 1000.0;

    if (rpmFiltered > 0) {
      rpmFiltered -= decayRate * deltaTime;
      if (rpmFiltered < 0) rpmFiltered = 0;
    }
  }

  // Atualiza tempo da última atualização do filtro
  lastUpdateMillis = currentMillis;

  // Imprime a cada 500 ms
  static unsigned long lastPrintMillis = 0;
  if (currentMillis - lastPrintMillis >= 500) {
    lastPrintMillis = currentMillis;
    Serial.print("RPM: ");
    Serial.print(rpmFiltered);
    Serial.print(" | Pulsos: ");
    Serial.println(pulseCountCopy);
  }
}

void pulseDetected() {
  unsigned long nowMicros = micros();
  unsigned long diff = nowMicros - lastPulseMicros;

  if (lastPulseMicros > 0 && diff > 0) {
    rpmInstant = 60000000.0 / diff;
  }

  lastPulseMicros = nowMicros;
  pulseCount++;
}