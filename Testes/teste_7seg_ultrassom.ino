// Instalar biblioteca SevSeg

int unidades[] = {30, 31, 32, 33, 34, 35, 36};
int dezenas[] = {37, 38, 39, 40, 41, 42, 43};
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


int PinTrigger = 10; 
int PinEcho = 9; 
float TempoEcho = 0; 
const float velocidadeSom_mpus = 0.000340;

//#define BUZZER 10
const float limiteMovimento = 0.05;

float distanciaAnterior = 0;  
const int intervaloLeitura = 500; 



void setup() {
    pinMode(PinTrigger, OUTPUT); 
    digitalWrite(PinTrigger, LOW); 
    pinMode(PinEcho, INPUT);
    //pinMode(BUZZER, OUTPUT);
    Serial.begin(9600);
    delay(100); 
    
  for(int i = 0; i < SEGMENTOS; i++) {
       pinMode(dezenas[i], OUTPUT);
       pinMode(unidades[i], OUTPUT);
    }
}

void loop() {
  DisparaPulsoUltrassonico(); 
  TempoEcho = pulseIn(PinEcho, HIGH);
  float distanciaAtual = CalculaDistancia(TempoEcho); 

    
  Serial.print("Distancia em centimetros: ");
  Serial.println(distanciaAtual);
    

  // Contagem  
  // for(int k = 0; k < NUMEROS; k++) {
  //      acende(dezenas, k);
  //      for(int i = 0; i < NUMEROS; i++) {
  //         acende(unidades, i);
  //         delay(500);
  //      } 
  //   }

  // Medida de distância
  int intDist = int(distanciaAtual);

  acende(unidades, intDist % 10);
  acende(dezenas, int(intDist/10));

  // float delta = abs(distanciaAtual -  distanciaAnterior);

  // float velocidade = delta/(intervaloLeitura/1000);
  // int intVel = int(velocidade);


  // acende(unidades, intVel % 10);
  // acende(dezenas, int(intVel/10));


  distanciaAnterior = distanciaAtual;

  delay(intervaloLeitura);
}

void DisparaPulsoUltrassonico() {
    digitalWrite(PinTrigger, HIGH); 
    delayMicroseconds(10); 
    digitalWrite(PinTrigger, LOW); 
}

float CalculaDistancia(float tempo_us) {
    return ((tempo_us * velocidadeSom_mpus) / 2)*100; 
}

void acende(int* pino, int dig) {
    for(int i = 0; i < SEGMENTOS; i++) {
       digitalWrite(pino[i], digito[dig][i]);
    }
}
