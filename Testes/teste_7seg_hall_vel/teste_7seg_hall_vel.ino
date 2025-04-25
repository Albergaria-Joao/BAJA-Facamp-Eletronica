#include <math.h>
// Instalar biblioteca SevSeg

int unidades[] = {31, 33, 35, 37, 39, 41, 43};
int dezenas[] = {30, 32, 34, 36, 38, 40, 42};
const int l = 1, d = 0;
byte digito[10][7] = { 
 { l,l,l,l,l,l,d },  //DIGITO 0
 { d,l,l,d,d,d,d },  //DIGITO 1
 { l,l,d,l,l,d,l },  //DIGITO 2
 { l,l,l,l,d,d,l },  //DIGITO 3
 { d,l,l,d,d,l,l },  //DIGITO 4
 { l,d,l,l,d,l,l },  //DIGITO 5
 { l,d,l,l,l,l,l },  //DIGITO 6
 { l,l,l,d,d,d,d },  //DIGITO 7
 { l,l,l,l,l,l,l },  //DIGITO 8
 { l,l,l,d,d,l,l }  //DIGITO 9
};

const int SEGMENTOS = 7;
const int NUMEROS = 10;

int pinoSensor = 7;   // Pino digital onde o sensor está conectado
int ultimoEstadoSensor = HIGH; 
int pulsoCount= 0;

long ultimoTempo = 0;
long intervalo = 1000;

const float circunferencia = M_PI * 0.2675;

void setup() {
  pinMode(pinoSensor, INPUT);    // Define o pino do sensor como entrada
  Serial.begin(9600);            // Inicializa a comunicação serial para monitorar o estado
    
  for(int i = 0; i < SEGMENTOS; i++) {
       pinMode(dezenas[i], OUTPUT);
       pinMode(unidades[i], OUTPUT);
    }
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
    Serial.println(vel_kmh);
    ultimoTempo = millis();
    acende(unidades, vel_int % 10);
    acende(dezenas, vel_int / 10);
    pulsoCount = 0;
  }  
}

void acende(int* pino, int dig) {
    for(int i = 0; i < SEGMENTOS; i++) {
       digitalWrite(pino[i], digito[dig][i]);
    }S
}