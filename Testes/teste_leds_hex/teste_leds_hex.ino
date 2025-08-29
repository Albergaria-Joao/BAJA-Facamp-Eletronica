// Pinos do 74HC595
#define DATA_PIN 2   // DS
#define LATCH_PIN 4  // STCP
#define CLOCK_PIN 3 // SHCP

// Pino do LED independente
#define LED_EXTRA 7

int rpm;

void setup() {
  pinMode(DATA_PIN, OUTPUT);
  pinMode(LATCH_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(LED_EXTRA, OUTPUT);
}

void loop() {
  // Sequência para 9 LEDs (0 a 7 no 74HC595, 8 no pino extra)
  for (int i = 10; i >= 0; i--) {
    rpm = i;
  
    if (rpm <= 1) {
      enviarDados(0x01);
      digitalWrite(LED_EXTRA, LOW);
    }
    else if (rpm <= 2) {
      enviarDados(0x03);
      digitalWrite(LED_EXTRA, LOW);
    }
    else if (rpm <= 3) {
      enviarDados(0x07);
      digitalWrite(LED_EXTRA, LOW);
    }
    else if (rpm <= 4) {
      enviarDados(0x0F);
      digitalWrite(LED_EXTRA, LOW);
    }
    else if (rpm <= 5) {
      enviarDados(0x1F);
      digitalWrite(LED_EXTRA, LOW);
    }
    else if (rpm <= 6) {
      enviarDados(0x3F);
      digitalWrite(LED_EXTRA, LOW);
    }
    else if (rpm <= 7) {
      enviarDados(0x7F);
      digitalWrite(LED_EXTRA, LOW);
    }
    else if (rpm <= 8) {
      enviarDados(0xFF);
      digitalWrite(LED_EXTRA, LOW);
    }
    else {
      enviarDados(0xFF);
      digitalWrite(LED_EXTRA, HIGH);
    }
    delay(300);
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