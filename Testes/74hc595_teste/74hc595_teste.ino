// Pinos conectados ao 74HC595
#define DATA_PIN 2   // DS (pino 14 do 74HC595)
#define LATCH_PIN 4  // STCP (pino 12 do 74HC595)
#define CLOCK_PIN 3 // SHCP (pino 11 do 74HC595)

void setup() {
  pinMode(DATA_PIN, OUTPUT);
  pinMode(LATCH_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
}

void loop() {
  // Acende LEDs um por um
  for (int i = 0; i < 8; i++) {
    byte leds = 1 << i; // Liga apenas um LED por vez
    enviarDados(leds);
    delay(200);
  }

  byte led = 1 << 4;
  enviarDados(led);
  delay(900);
  // Acende todos
  enviarDados(0xFF);
  delay(500);

  // Apaga todos
  enviarDados(0x00);
  delay(500);
}

// Função para enviar dados ao 74HC595
void enviarDados(byte data) {
  digitalWrite(LATCH_PIN, LOW);
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, data);
  digitalWrite(LATCH_PIN, HIGH);
}