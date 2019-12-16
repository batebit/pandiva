int buttonPins[] = {
  22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52};
  int piezoPins[] = {A10,A11,A12,A13,A14,A15};

void setup(){
  for(int i = 0; i < 31; i++){
    pinMode(buttonPins[i], INPUT_PULLUP);
  }
  Serial.begin(115200);
}

void loop(){
  char divisor = ';';
//  for(int i = 0; i < 6; i++){
//    Serial.print(analogRead(piezoPins[i]));
//    Serial.print(divisor);
//  }
  for(int i = 0; i < 31; i++){
    int estadoBotao = digitalRead(buttonPins[i]);
    Serial.print(estadoBotao);
    Serial.print(divisor);
  }
  Serial.println(" => FIM");
  //Serial.println("\t");
  delay(10); 
}



