#include <OneWire.h> // É preciso instalar essas bibliotecas
#include <DallasTemperature.h> // É preciso instalar essas bibliotecas

const int PINO_ONEWIRE = 6;
OneWire oneWire(PINO_ONEWIRE);
DallasTemperature sensor(&oneWire);
DeviceAddress endereco_temp;

int azul = 9;//Atribui o valor 9 a variável azul
int verde = 10;//Atribui o valor 10 a variável verde
int vermelho = 12;//Atribui o valor 12 a variável vermelho


void setup(){
  pinMode(azul, OUTPUT);//Define a variável azul como saída
  pinMode(verde, OUTPUT);//Define a variável verde como saída
  pinMode (vermelho, OUTPUT);//Define a variável vermelho como saída
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(9600);
  Serial.println("Medindo Temperatura");
  sensor.begin();

  if (!sensor.getAddress(endereco_temp, 0)) {
    Serial.println("SENSOR NAO CONECTADO");
    while (1); // Stop the program
  }
}

void loop(){
  digitalWrite(vermelho, LOW);
  digitalWrite(verde, LOW);

  sensor.requestTemperatures();
  long reading = sensor.getTempC(endereco_temp);  
  Serial.print("Temperatura = ");
  Serial.println(reading, 1);
  delay(500);
 
 // converting that reading to voltage, for 3.3v arduino use 3.3
  float voltage = reading * 5.0;
  voltage /= 1024.0; 
 
 // print out the voltage
  Serial.print(voltage); Serial.println(" volts");
  
  if(reading < 30){
    digitalWrite(verde, HIGH);//Função para acionamento na cor verde
    digitalWrite(azul, LOW);
    digitalWrite(vermelho, LOW);
  }else if(reading < 35){
    digitalWrite(verde, HIGH);//Coloca verde em nível alto
  	digitalWrite(azul, LOW);//Função para acionamento na cor amarela
    digitalWrite(vermelho, HIGH);
  }else{
    digitalWrite(verde, LOW);
    digitalWrite(azul, LOW);
    digitalWrite(vermelho, HIGH);//Função para acionamento na cor vermelha
  }
  
  delay(1000); 
}