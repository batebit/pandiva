int buttonPins[] = {
  22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51};
int piezoPins[] = {
  A10,A11,A12,A13,A14,A15};

void setup(){
  for(int i = 0; i < 30; i++){
    pinMode(buttonPins[i], INPUT_PULLUP);
  }
  Serial.begin(115200);
}

void loop(){

  int value1 = analogRead(A10);
  int value2 = analogRead(A11);
  int value3 = analogRead(A12);
  int value4 = analogRead(A13);
  int value5 = analogRead(A14);
  int value6 = analogRead(A15);
  int soma = value1 + value2 + value3 + value4 + value5 + value6;
  int velocity = map(soma, 0, 4000, 65, 127);
  velocity = constrain(velocity, 65, 127);

  char divisor = '\t';
  //  for(int i = 0; i < 6; i++){
  //    Serial.print(analogRead(piezoPins[i]));
  //    Serial.print(divisor);
  //  }
  //  for(int i = 0; i < 30; i++){
  //    int estadoBotao = digitalRead(buttonPins[i]);
  //    Serial.print(estadoBotao);
  //    Serial.print(divisor);
  //  }
  Serial.print(soma);
  Serial.print(divisor);
    Serial.print(velocity);
  Serial.print(divisor);
  Serial.println(" => FIM");
  //Serial.println("\t");
  delay(10); 
}




