#include <LoRa.h>

// TRANSMISSOR

void setup() {
  Serial.begin(9600);
  LoRa.begin(915E6); // Frequência LoRa (verifique a correta para o Brasil)
}

void loop() {
  // Arduino pseudo-código
  int rpm = lerSensorRPM();
  float temp = lerTemperatura();
  int velocidade = lerVelocidade();

  String dados = String(rpm) + "," + String(temp) + "," + String(velocidade);
  Serial.println(dados); // Envia via Serial para o ESP32

  if (Serial.available()) {
    String dados = Serial.readStringUntil('\n');
    LoRa.beginPacket();
    LoRa.print(dados);
    LoRa.endPacket();
  }
}


// RECEPTOR


#include <LoRa.h>

void setup() {
  Serial.begin(9600);
  LoRa.begin(915E6);
}

void loop() {
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    String dados = "";
    while (LoRa.available()) {
      dados += (char)LoRa.read();
    }
    Serial.println("Recebido: " + dados);
    // Aqui você pode processar e exibir os dados
  }
}
