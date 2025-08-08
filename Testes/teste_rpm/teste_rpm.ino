#define HALL_PIN 2 // Pino do sinal do KY-003
unsigned long lastPulseTime = 0;   
float rpmInstant = 0;              
float rpmFiltered = 0;             
const float alpha = 0.3; // Constante que suaviza a variação do RPM     
unsigned long lastDebounceTime = 0; 
const unsigned long debounceDelay = 50; // Serve para impedir que várias medidas muito próximas atuem como ruído e elevem muito o RPM
bool lastHallState = HIGH; // Estado anterior do sensor

const float decayRate = 10; // Quanto o RPM cai por segundo quando não há pulsos
int count = 0;

void setup() {
  Serial.begin(9600);
  pinMode(HALL_PIN, INPUT); // Sem PULLUP
}

void loop() {
  bool hallState = digitalRead(HALL_PIN);

  // Detecta borda de descida (ímã passou pelo sensor)
  if (lastHallState == HIGH && hallState == LOW) {
    count++;
    unsigned long now = millis();

    // Aplica debounce
    if ((now - lastDebounceTime) > debounceDelay) {
      unsigned long nowMicros = micros();
      unsigned long timeDiff = nowMicros - lastPulseTime;

      if (lastPulseTime > 0 && timeDiff > 0) {
        rpmInstant = 60000000.0 / timeDiff;
      }

      lastPulseTime = nowMicros;
      lastDebounceTime = now;
    }
  }
  lastHallState = hallState;

  // Atualiza filtro
  rpmFiltered = (alpha * rpmInstant) + ((1 - alpha) * rpmFiltered);

  // Se não teve pulso por um tempo, reduz valor gradualmente
  if (millis() - (lastDebounceTime) > debounceDelay) {
    if (rpmFiltered > 0) {
      rpmFiltered -= decayRate * (millis() - lastDebounceTime) / 1000.0;
      if (rpmFiltered < 0) rpmFiltered = 0;
    }
  }

  // Exibe a cada 500ms
  static unsigned long lastPrint = 0;
  if (millis() - lastPrint >= 500) {
    lastPrint = millis();
    Serial.print("RPM: ");
    Serial.println(rpmFiltered);
    Serial.print("Count ");
    Serial.println(count);
  }
}