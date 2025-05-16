#include <OneWire.h>
#include <DallasTemperature.h>

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
  }
}

void loop() {
  sensor.requestTemperatures();
  if (sensor.isConnected(endereco_temp)) {
    float temperatura = sensor.getTempC(endereco_temp);
    Serial.print("Temperatura = ");
    Serial.println(temperatura, 1);
  } else {
    Serial.println("SENSOR DESCONECTADO DURANTE LEITURA");
  }
  delay(1000);
}
