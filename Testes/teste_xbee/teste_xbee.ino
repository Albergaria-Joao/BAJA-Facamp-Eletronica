#include <SoftwareSerial.h>


// Pinos
SoftwareSerial xbee(2, 3);
const int botaoPin = 6;  
int estadoBotao = 0;

void setup() {
  pinMode(botaoPin, INPUT);  // botão entre pino e GND
  Serial.begin(9600); // Comunicação com o XBee
  xbee.begin(9600);
}

void loop() {
  estadoBotao = digitalRead(botaoPin);

  if (estadoBotao == HIGH) {  // pressionado (puxado para GND)
    Serial.println("B"); // envia para o XBee
    xbee.println("BATATA");
    delay(200);              // evita múltiplos disparos
  }
}
