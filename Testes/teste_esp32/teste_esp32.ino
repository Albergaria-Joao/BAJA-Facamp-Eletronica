#include "heltec.h"
#include "DHTesp.h"

#define BAND 915E6

DHTesp dht;

void sendPacket(float temperature, float humidity)

void setup() {
  // put your setup code here, to run once:
  pinMode(LED, OUTPUT);
  Serial.begin(9600);
  Heltec.begin(true, true, true, true, BAND);
  Heltec.display->init();
  Heltec.display->flipScreenVertically();
  Heltec.display->setFont(ArialMT_Plain_10);
  Heltec.display->clear();
  Heltec.display->drawString(33, 5, "INICIADO");
  Heltec.display->drawString(10, 30, "TCC SÁVIO!");
  Heltec.display->display();
  delay(5000);
  dht.setup(17, DHTesp::DHT11);:
}

void loop() {
  // put your main code here, to run repeatedly:

}
