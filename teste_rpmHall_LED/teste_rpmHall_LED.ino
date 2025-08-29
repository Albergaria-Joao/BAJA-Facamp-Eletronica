// Pinos do 74HC595
#define DATA_PIN 2   // DS
#define LATCH_PIN 4  // STCP
#define CLOCK_PIN 3 // SHCP

// Pino do LED independente
#define LED_EXTRA 7

#define HALL_PIN 8 // Pino do sinal do KY-003

int rpm;


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
  pinMode(DATA_PIN, OUTPUT);
  pinMode(LATCH_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(LED_EXTRA, OUTPUT);
  
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
    acender(rpmFiltered);
    Serial.print("Count ");
    Serial.println(count);
  }
  



  // for (int i = 0; i < 9; i++) {
  //   if (i < 8) { 
  //     // Acende LED pelo 74HC595
  //     enviarDados(1 << i);
  //     digitalWrite(LED_EXTRA, LOW);
  //   } else { 
  //     // Acende LED extra
  //     enviarDados(0x00);
  //     digitalWrite(LED_EXTRA, HIGH);
  //   }
  //   delay(200);
  // }

  // // Acende todos
  // enviarDados(0xFF);
  // digitalWrite(LED_EXTRA, HIGH);
  // delay(500);

  // // Apaga todos
  // enviarDados(0x00);
  // digitalWrite(LED_EXTRA, LOW);
  // delay(500);
}

void enviarDados(byte data) {
  digitalWrite(LATCH_PIN, LOW);
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, data);
  digitalWrite(LATCH_PIN, HIGH);
}


void acender(unsigned long rpm){
    if (rpm <= 20) {
      enviarDados(0x01);
      digitalWrite(LED_EXTRA, LOW);
    }
    else if (rpm <= 40) {
      enviarDados(0x03);
      digitalWrite(LED_EXTRA, LOW);
    }
    else if (rpm <= 80) {
      enviarDados(0x07);
      digitalWrite(LED_EXTRA, LOW);
    }
    else if (rpm <= 160) {
      enviarDados(0x0F);
      digitalWrite(LED_EXTRA, LOW);
    }
    else if (rpm <= 180) {
      enviarDados(0x1F);
      digitalWrite(LED_EXTRA, LOW);
    }
    else if (rpm <= 220) {
      enviarDados(0x3F);
      digitalWrite(LED_EXTRA, LOW);
    }
    else if (rpm <=260) {
      enviarDados(0x7F);
      digitalWrite(LED_EXTRA, LOW);
    }
    else if (rpm <=300) {
      enviarDados(0xFF);
      digitalWrite(LED_EXTRA, LOW);
    }
    else {
      enviarDados(0xFF);
      digitalWrite(LED_EXTRA, HIGH);
    }
    delay(300);
}













