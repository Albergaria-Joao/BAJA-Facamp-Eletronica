#include <math.h>

int pinoSensor = 6;   // Pino digital onde o sensor está conectado
int ultimoEstadoSensor = HIGH; 
int pulsoCount= 0;

long ultimoTempo = 0;
long intervalo = 1000;

const float circunferencia = M_PI * 0.2675;

void setup() {
  pinMode(pinoSensor, INPUT);    // Define o pino do sensor como entrada
  Serial.begin(9600);            // Inicializa a comunicação serial para monitorar o estado
}

void loop() {
  int estadoSensor = digitalRead(pinoSensor);  // Lê o estado do sensor
  if (ultimoEstadoSensor == HIGH && estadoSensor == LOW){
    pulsoCount++;
  }
  ultimoEstadoSensor = estadoSensor;

  if(millis()-ultimoTempo >= intervalo){
    float vel = (pulsoCount * circunferencia)/(intervalo/1000);
    float vel_kmh = vel * 3.6;
    int vel_int = vel_kmh;
    //Serial.print("Velocidade: ");
    Serial.println(vel_kmh);
    ultimoTempo = millis();
    pulsoCount = 0;
  }  

}

