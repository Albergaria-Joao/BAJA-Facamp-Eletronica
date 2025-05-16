#include <OneWire.h> // É preciso instalar essas bibliotecas
#include <DallasTemperature.h> // É preciso instalar essas bibliotecas

const int PINO_ONEWIRE = 12;
OneWire oneWire(PINO_ONEWIRE);
DallasTemperature sensor(&oneWire);
DeviceAddress endereco_temp;

void setup() {
  Serial.begin(9600);
  Serial.println("Medindo Temperatura");
  sensor.begin();

  if (!sensor.getAddress(endereco_temp, 0)) {
    Serial.println("SENSOR NAO CONECTADO");
    while (1); // Stop the program
  }
}


void loop() {
  sensor.requestTemperatures();
  Serial.print("Temperatura = ");
  Serial.println(sensor.getTempC(endereco_temp), 1);
  delay(1000);
}

