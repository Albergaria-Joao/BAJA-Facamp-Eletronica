#include <OneWire.h> // É preciso instalar essas bibliotecas
#include <DallasTemperature.h> // É preciso instalar essas bibliotecas
#include <SoftwareSerial.h>


#define DATA_PIN 5   // DS
#define LATCH_PIN 6  // STCP
#define CLOCK_PIN 7
#define LED_EXTRA 13

#define HALL_PIN 11 // Pino do sinal do KY-003

const int PINO_ONEWIRE = 8;
OneWire oneWire(PINO_ONEWIRE);
DallasTemperature sensor(&oneWire);
DeviceAddress endereco_temp;
SoftwareSerial xbee(2, 3);

int azul = 9;//Atribui o valor 9 a variável azul
int verde = 10;//Atribui o valor 10 a variável verde
int vermelho = 12;//Atribui o valor 12 a variável vermelho


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

void setup(){
  pinMode(azul, OUTPUT);//Define a variável azul como saída
  pinMode(verde, OUTPUT);//Define a variável verde como saída
  pinMode(vermelho, OUTPUT);//Define a variável vermelho como saída
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(DATA_PIN, OUTPUT);
  pinMode(LATCH_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(LED_EXTRA, OUTPUT);

  Serial.begin(9600);
  pinMode(HALL_PIN, INPUT);
  //Serial.println("Medindo Temperatura");
  sensor.begin();

  xbee.begin(9600);

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
  //Serial.print("Temperatura = ");
  Serial.println(reading, 1);
  
  xbee.print("Temperatura = ");
  xbee.println(reading, 1);

  //delay(50);
 
 // converting that reading to voltage, for 3.3v arduino use 3.3
  float voltage = reading * 5.0;
  voltage /= 1024.0; 
 
 // print out the voltage
  //Serial.print(voltage); Serial.println(" volts");
  
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
    //    Serial.print("RPM: ");
    Serial.println(rpmFiltered);
    acender(rpmFiltered);

    xbee.print("RPM = ");
    xbee.println(rpmFiltered);
    //Serial.print("Count ");
    //Serial.println(count);
  
  delay(1000); 
  }
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
