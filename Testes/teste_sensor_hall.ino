int sensor = 10;
int val;


void setup() {
  // put your setup code here, to run once:
  pinMode(sensor, INPUT);
  digitalWrite(sensor, HIGH); //
  Serial.begin(9600);
  Serial.println("Lendo sensor");
}

void loop() {
  // put your main code here, to run repeatedly:
  val = digitalRead(sensor);
  if (val == LOW) {
    Serial.println("LEU ÍMÃ");
  }
  else {
    Serial.flush();
  }
}
