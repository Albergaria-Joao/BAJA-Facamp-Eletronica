#define BUTTON_PIN 7 // Botão no pino 2
unsigned long lastPulseTime = 0;   
float rpmInstant = 0;              
float rpmFiltered = 0;             
const float alpha = 0.2;           
unsigned long lastDebounceTime = 0; 
const unsigned long debounceDelay = 50; 
bool lastButtonState = HIGH;

const float decayRate = 10; // Quanto o RPM cai por segundo quando não há pulsos
int count = 0;


void setup() {
  Serial.begin(9600);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void loop() {
  bool buttonState = digitalRead(BUTTON_PIN);

  // Detecta borda de descida (pressionamento)
  if (lastButtonState == HIGH && buttonState == LOW) {
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
  lastButtonState = buttonState;

  // Atualiza filtro
  rpmFiltered = (alpha * rpmInstant) + ((1 - alpha) * rpmFiltered);

  // Se não teve pulso por um tempo, reduz valor gradualmente
  if (millis() - (lastDebounceTime) > debounceDelay) {
    if (rpmFiltered > 0) {
      rpmFiltered -= decayRate * (millis() - lastDebounceTime) / 1000.0;
      if (rpmFiltered < 0) rpmFiltered = 0;
    }
  }

  // Exibe a cada 200ms
  static unsigned long lastPrint = 0;
  if (millis() - lastPrint >= 200) {
    lastPrint = millis();
    Serial.print("RPM: ");
    Serial.println(rpmFiltered);
    Serial.print("Count ");
    Serial.println(count);
  }
}